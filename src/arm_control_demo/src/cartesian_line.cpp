#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/RobotTrajectory.h>
#include <visualization_msgs/Marker.h>//绘制轨迹
#include <vector>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "cartesian_line");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");

    //绘制轨迹
    ros::NodeHandle nh;
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>(
        "trajectory_marker", 1, true);

    //起点绿色Marker
    visualization_msgs::Marker start_marker;
    start_marker.header.frame_id = "base_link";
    start_marker.type = visualization_msgs::Marker::SPHERE;
    start_marker.action = visualization_msgs::Marker::ADD;
    start_marker.scale.x = 0.05;
    start_marker.scale.y = 0.05;
    start_marker.scale.z = 0.05;
    start_marker.color.g = 1.0;
    start_marker.color.a = 1.0;

    //终点蓝色Marker
    visualization_msgs::Marker end_marker;
    end_marker.header.frame_id = "base_link";
    end_marker.type = visualization_msgs::Marker::SPHERE;
    end_marker.action = visualization_msgs::Marker::ADD;
    end_marker.scale.x = 0.05;
    end_marker.scale.y = 0.05;
    end_marker.scale.z = 0.05;
    end_marker.color.b = 1.0;
    end_marker.color.a = 1.0;

    //轨迹红线
    visualization_msgs::Marker line_strip;
    line_strip.header.frame_id = "base_link";
    line_strip.type = visualization_msgs::Marker::LINE_STRIP;
    line_strip.scale.x = 0.01;
    line_strip.color.r = 1.0;
    line_strip.color.a = 1.0;

    //读起点位置
    geometry_msgs::Pose start_pose = move_group.getCurrentPose().pose;

    geometry_msgs::Pose target_pose = start_pose;

    //目标线段终点
    target_pose.position.y += 0.20;
    target_pose.position.x -= 0.20;

    std::vector<geometry_msgs::Pose> waypoints;

    waypoints.push_back(start_pose);
    waypoints.push_back(target_pose);

    //添加起点和终点到Marker
    geometry_msgs::Point p1, p2;
    p1.x = start_pose.position.x; p1.y = start_pose.position.y; p1.z = start_pose.position.z;
    p2.x = target_pose.position.x; p2.y = target_pose.position.y; p2.z = target_pose.position.z;
    line_strip.points.push_back(p1);
    line_strip.points.push_back(p2);

    start_marker.pose = start_pose;
    end_marker.pose = target_pose;

    moveit_msgs::RobotTrajectory trajectory;

    //0.01插值
    double fraction =
        move_group.computeCartesianPath(
            waypoints,
            0.01,
            0.0,
            trajectory);

    //fraction==1,成功
    ROS_INFO("Fraction = %.2f", fraction);

    //成功率>95%，执行
    if(fraction > 0.95)
    {
        moveit::planning_interface::MoveGroupInterface::Plan plan;
        plan.trajectory_ = trajectory;

        //显示起点(绿)、终点(蓝)、轨迹(红)
        marker_pub.publish(start_marker);
        marker_pub.publish(end_marker);
        marker_pub.publish(line_strip);
        ros::Duration(1.0).sleep();

        move_group.execute(plan);
    }
    else
    {
        ROS_WARN("Cartesian Path Failed");
    }
    ros::shutdown();

    return 0;
}