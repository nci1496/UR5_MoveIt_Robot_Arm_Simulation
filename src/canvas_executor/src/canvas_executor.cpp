#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_state/robot_state.h>
#include <visualization_msgs/Marker.h>
#include <signal.h>

bool g_shutdown = false;

void signalHandler(int sig)
{
    g_shutdown = true;
}

class CanvasExecutor
{
public:
    CanvasExecutor()
        : nh_("~")
        , move_group_("manipulator")
    {
        // Get parameters
        nh_.param("execution_delay", execution_delay_, 0.1);
        nh_.param("publish_marker", publish_marker_, true);
        nh_.param("max_velocity_scaling", max_velocity_scaling_, 0.5);
        nh_.param("cartesian_step", cartesian_step_, 0.01);
        nh_.param("jump_threshold", jump_threshold_, 0.0);

        // Publisher for trajectory marker
        marker_pub_ = nh_.advertise<visualization_msgs::Marker>(
            "trajectory_marker", 10, true);

        // Subscriber for canvas trajectory (absolute path)
        trajectory_sub_ = nh_.subscribe(
            "/canvas_trajectory", 10,
            &CanvasExecutor::trajectoryCallback, this);

        // Timer to refresh velocity parameter from GUI
        velocity_param_timer_ = nh_.createTimer(
            ros::Duration(0.5),
            &CanvasExecutor::refreshVelocityParam, this);

        ROS_INFO("Canvas Executor initialized!");
        ROS_INFO("Subscribing to topic: /canvas_trajectory");
        ROS_INFO("Publishing markers to: trajectory_marker");
        ROS_INFO("Max velocity scaling: %.2f", max_velocity_scaling_);

        // Initialize marker
        initMarker();
    }

    void initMarker()
    {
        marker_.header.frame_id = "base_link";
        marker_.header.stamp = ros::Time::now();
        marker_.ns = "canvas_trajectory";
        marker_.id = 0;
        marker_.type = visualization_msgs::Marker::POINTS;
        marker_.action = visualization_msgs::Marker::ADD;
        marker_.scale.x = 0.02;
        marker_.scale.y = 0.02;
        marker_.color.r = 1.0;
        marker_.color.g = 0.0;
        marker_.color.b = 0.0;
        marker_.color.a = 1.0;
    }

    void trajectoryCallback(const geometry_msgs::PoseArray::ConstPtr& msg)
    {
        if (msg->poses.empty()) {
            ROS_WARN("Received empty trajectory!");
            return;
        }

        ROS_INFO("Received trajectory with %zu points", msg->poses.size());

        // Publish marker visualization
        if (publish_marker_) {
            publishTrajectoryMarker(msg);
        }

        // Execute trajectory using Cartesian path
        executeCartesianTrajectory(msg);
    }

    void publishTrajectoryMarker(const geometry_msgs::PoseArray::ConstPtr& trajectory)
    {
        visualization_msgs::Marker marker;
        marker.header.frame_id = "base_link";
        marker.header.stamp = ros::Time::now();
        marker.ns = "canvas_trajectory";
        marker.id = 0;
        marker.type = visualization_msgs::Marker::POINTS;
        marker.action = visualization_msgs::Marker::ADD;
        marker.scale.x = 0.02;
        marker.scale.y = 0.02;
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;

        for (const auto& pose : trajectory->poses) {
            geometry_msgs::Point p;
            p.x = pose.position.x;
            p.y = pose.position.y;
            p.z = pose.position.z;
            marker.points.push_back(p);
        }

        marker_pub_.publish(marker);
    }

    void executeCartesianTrajectory(const geometry_msgs::PoseArray::ConstPtr& trajectory)
    {
        if (trajectory->poses.empty()) return;

        ROS_INFO("Starting Cartesian trajectory execution with %zu points...",
                 trajectory->poses.size());

        // Get current pose as starting point
        geometry_msgs::Pose start_pose = move_group_.getCurrentPose().pose;

        // Build waypoints from trajectory
        std::vector<geometry_msgs::Pose> waypoints;
        waypoints.reserve(trajectory->poses.size());

        for (const auto& pose_msg : trajectory->poses) {
            geometry_msgs::Pose waypoint = pose_msg;
            // Use a consistent orientation (from start pose)
            waypoint.orientation = start_pose.orientation;
            waypoints.push_back(waypoint);
        }

        // Compute Cartesian path
        moveit_msgs::RobotTrajectory trajectory_msg;
        double fraction = 0.0;
        int attempts = 0;
        const int max_attempts = 3;

        while (fraction < 0.9 && attempts < max_attempts) {
            fraction = move_group_.computeCartesianPath(
                waypoints,
                cartesian_step_,      // eef_step
                jump_threshold_,       // jump_threshold (0 = disabled)
                trajectory_msg,
                true);                // avoid_collisions

            attempts++;
            ROS_INFO("Cartesian path computed: %.1f%% (attempt %d/%d)",
                     fraction * 100, attempts, max_attempts);

            if (fraction < 0.9) {
                ROS_WARN("Path fraction %.1f%% < 90%%, retrying...", fraction * 100);
                ros::Duration(0.5).sleep();
            }
        }

        if (fraction < 0.5) {
            ROS_ERROR("Failed to compute sufficient Cartesian path (%.1f%%)", fraction * 100);
            return;
        }

        // Execute the trajectory
        move_group_.execute(trajectory_msg);

        ROS_INFO("Cartesian trajectory execution complete! Path coverage: %.1f%%", fraction * 100);
    }

    void publishMarkerOnce()
    {
        visualization_msgs::Marker marker;
        marker.header.frame_id = "base_link";
        marker.header.stamp = ros::Time::now();
        marker.ns = "canvas_trajectory";
        marker.id = 1;
        marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = 0;
        marker.pose.position.y = 0.3;
        marker.pose.position.z = 0.6;
        marker.scale.z = 0.05;
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;
        marker.text = "Canvas Trajectory Executor Running...";

        marker_pub_.publish(marker);
    }

    void refreshVelocityParam(const ros::TimerEvent&)
    {
        double gui_velocity;
        if (nh_.getParam("max_velocity_scaling", gui_velocity)) {
            if (gui_velocity != current_velocity_) {
                current_velocity_ = gui_velocity;
                move_group_.setMaxVelocityScalingFactor(current_velocity_);
                ROS_INFO("Velocity scaling updated to: %.2f", current_velocity_);
            }
        }
    }

    void run()
    {
        ros::Rate rate(1); // 1 Hz
        while (nh_.ok() && !g_shutdown) {
            publishMarkerOnce();
            ros::spinOnce();
            rate.sleep();
        }
    }

private:
    ros::NodeHandle nh_;
    moveit::planning_interface::MoveGroupInterface move_group_;

    ros::Subscriber trajectory_sub_;
    ros::Publisher marker_pub_;

    double execution_delay_;
    bool publish_marker_;
    double max_velocity_scaling_;
    double cartesian_step_;
    double jump_threshold_;
    double current_velocity_;
    ros::Timer velocity_param_timer_;

    visualization_msgs::Marker marker_;
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "canvas_executor", ros::init_options::NoSigintHandler);

    // Setup signal handler
    signal(SIGINT, signalHandler);

    ROS_INFO("=== Canvas Trajectory Executor ===");
    ROS_INFO("Waiting for trajectory messages on /canvas_trajectory...");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    CanvasExecutor executor;
    executor.run();

    ros::shutdown();
    return 0;
}
