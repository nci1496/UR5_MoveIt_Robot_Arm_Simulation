#include <ros/ros.h>

#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "get_pose");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");

    geometry_msgs::Pose pose =
        move_group.getCurrentPose().pose;

    ROS_INFO("x = %f", pose.position.x);
    ROS_INFO("y = %f", pose.position.y);
    ROS_INFO("z = %f", pose.position.z);

    ROS_INFO("w = %f", pose.orientation.w);

    ros::shutdown();

    return 0;
}
