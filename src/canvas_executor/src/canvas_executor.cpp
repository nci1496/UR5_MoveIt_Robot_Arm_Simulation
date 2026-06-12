#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <moveit/move_group_interface/move_group_interface.h>
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
        nh_.param("execution_delay", execution_delay_, 0.5);
        nh_.param("publish_marker", publish_marker_, true);

        // Publisher for trajectory marker
        marker_pub_ = nh_.advertise<visualization_msgs::Marker>(
            "trajectory_marker", 10, true);

        // Subscriber for canvas trajectory (absolute path)
        trajectory_sub_ = nh_.subscribe(
            "/canvas_trajectory", 10,
            &CanvasExecutor::trajectoryCallback, this);

        ROS_INFO("Canvas Executor initialized!");
        ROS_INFO("Subscribing to topic: canvas_trajectory");
        ROS_INFO("Publishing markers to: trajectory_marker");

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

        // Execute trajectory
        executeTrajectory(msg);
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

    void executeTrajectory(const geometry_msgs::PoseArray::ConstPtr& trajectory)
    {
        if (trajectory->poses.empty()) return;

        // Get current orientation as reference
        geometry_msgs::Pose current_pose = move_group_.getCurrentPose().pose;

        int success_count = 0;
        int total_count = trajectory->poses.size();

        ROS_INFO("Starting trajectory execution with %d points...", total_count);

        for (size_t i = 0; i < trajectory->poses.size(); ++i)
        {
            if (g_shutdown) {
                ROS_WARN("Shutdown requested, stopping trajectory!");
                break;
            }

            const auto& target = trajectory->poses[i];

            // Create pose target
            geometry_msgs::Pose target_pose;
            target_pose.position = target.position;
            // Keep orientation from current pose for consistency
            target_pose.orientation = current_pose.orientation;

            move_group_.setPoseTarget(target_pose);

            // Plan and execute
            bool success = (move_group_.move() == moveit::core::MoveItErrorCode::SUCCESS);

            if (success) {
                success_count++;
                if (i % 20 == 0 || i == trajectory->poses.size() - 1) {
                    ROS_INFO("Progress: %zu/%zu points executed",
                             i + 1, trajectory->poses.size());
                }
            } else {
                ROS_WARN("Failed at point %zu: (%.3f, %.3f, %.3f)",
                         i, target.position.x, target.position.y, target.position.z);
            }

            // Small delay between points
            ros::Duration(execution_delay_).sleep();
        }

        ROS_INFO("Trajectory execution complete! Success rate: %d/%d (%.1f%%)",
                 success_count, total_count,
                 100.0 * success_count / total_count);
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
