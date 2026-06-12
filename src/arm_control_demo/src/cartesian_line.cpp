#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/RobotTrajectory.h>
#include <vector>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "cartesian_line");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");

    //读起点位置
    geometry_msgs::Pose start_pose = move_group.getCurrentPose().pose;

    geometry_msgs::Pose target_pose = start_pose;

    //目标线段终点
    target_pose.position.y += 0.20;
    target_pose.position.x -= 0.20;

    std::vector<geometry_msgs::Pose> waypoints;

    waypoints.push_back(start_pose);
    waypoints.push_back(target_pose);

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
    
        move_group.execute(plan);
    }
    else
    {
        ROS_WARN("Cartesian Path Failed");
    }
    ros::shutdown();

    return 0;
}