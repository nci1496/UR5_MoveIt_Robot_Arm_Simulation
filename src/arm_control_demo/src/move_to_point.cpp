#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "move_to_point");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");

    geometry_msgs::Pose target_pose;

    target_pose.orientation.w = 1.0;

    target_pose.position.x = 0.40;
    target_pose.position.y = 0.20;
    target_pose.position.z = 0.40;

    move_group.setPoseTarget(target_pose);

    bool success =
        (move_group.move() ==
         moveit::core::MoveItErrorCode::SUCCESS);

    if(success)
        ROS_INFO("Move Success!");
    else
        ROS_WARN("Move Failed!");

    ros::shutdown();

    return 0;
}