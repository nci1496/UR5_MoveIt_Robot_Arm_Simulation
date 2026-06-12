#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/RobotTrajectory.h>
#include <vector>
#include<cmath>

#include <visualization_msgs/Marker.h>//绘制轨迹

int main(int argc, char** argv)
{
    ros::init(argc, argv, "eight_path");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface 
        move_group("manipulator");
    
    //绘制轨迹
    ros::NodeHandle nh;

    ros::Publisher marker_pub =
        nh.advertise<visualization_msgs::Marker>(
            "trajectory_marker",
            1,
            true);

    visualization_msgs::Marker line_strip;

    line_strip.header.frame_id = "base_link";
            
    line_strip.header.stamp = ros::Time::now();
            
    line_strip.ns = "trajectory";
            
    line_strip.id = 0;
            
    line_strip.type =
        visualization_msgs::Marker::LINE_STRIP;
            
    line_strip.action =
        visualization_msgs::Marker::ADD;
    
    line_strip.scale.x = 0.01;    
    line_strip.color.r = 1.0;
    line_strip.color.g = 0.0;
    line_strip.color.b = 0.0;
    line_strip.color.a = 1.0;

    //记录起点
    geometry_msgs::Pose start_pose =
        move_group.getCurrentPose().pose;

    std::vector<geometry_msgs::Pose> waypoints;

    //椭圆参数,做世界原点附近椭圆
    double a=0.40;
    double b=0.30;

    for(double t=0;t<=2*M_PI;t+=0.1)
{
    double x,y,z;
    x=0.4;
    y=b*sin(2*t);
    z=a*cos(t);
    

    geometry_msgs::Pose pose;
    pose.position.x=x;
    pose.position.y=y;
    pose.position.z=z;
    pose.orientation.w=start_pose.orientation.w;//保持起始末端姿态

    waypoints.push_back(pose);

    //绘制点
    geometry_msgs::Point p;
    p.x = pose.position.x;
    p.y = pose.position.y;
    p.z = pose.position.z;

    line_strip.points.push_back(p);

}

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