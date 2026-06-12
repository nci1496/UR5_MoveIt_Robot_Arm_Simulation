#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <visualization_msgs/Marker.h>//绘制轨迹

int main(int argc, char** argv)
{
    ros::init(argc, argv, "six_points");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");

    //绘制轨迹
    ros::NodeHandle nh;
    ros::Publisher marker_pub = nh.advertise<visualization_msgs::Marker>(
        "trajectory_marker", 1, true);

    visualization_msgs::Marker line_strip;
    line_strip.header.frame_id = "base_link";
    line_strip.type = visualization_msgs::Marker::POINTS;
    line_strip.scale.x = 0.05;
    line_strip.scale.y = 0.05;
    line_strip.color.r = 1.0;
    line_strip.color.g = 0.0;
    line_strip.color.b = 0.0;
    line_strip.color.a = 1.0;

    geometry_msgs::Pose target_pose[6];

    for(int i=0; i<6; i++)
    {
        target_pose[i].orientation.w = 1.0;
    }

    target_pose[0].position.x = 0.40;
    target_pose[0].position.y = 0.20;
    target_pose[0].position.z = 0.40;


    target_pose[1].position.x = 0.40;
    target_pose[1].position.y = -0.20;
    target_pose[1].position.z = 0.40;


    target_pose[2].position.x = 0.55;
    target_pose[2].position.y = 0.00;
    target_pose[2].position.z = 0.30;


    target_pose[3].position.x = 0.30;
    target_pose[3].position.y = -0.40;
    target_pose[3].position.z = 0.20;

    target_pose[4].position.x = 0.40;
    target_pose[4].position.y = 0.20;
    target_pose[4].position.z = -0.40;

    target_pose[5].position.x = -0.40;
    target_pose[5].position.y = 0.20;
    target_pose[5].position.z = 0.40;

    for(int i = 0; i < 6; i++) {
        move_group.setPoseTarget(target_pose[i]);

        //绘制路径点(红色大圆点)
        geometry_msgs::Point p;
        p.x = target_pose[i].position.x;
        p.y = target_pose[i].position.y;
        p.z = target_pose[i].position.z;
        line_strip.points.push_back(p);

        bool success =
            (move_group.move() ==
             moveit::core::MoveItErrorCode::SUCCESS);

        if(success)
            ROS_INFO("Move to point %d Success!", i+1);
        else
            ROS_WARN("Move to point %d Failed!", i+1);
    }

    //显示六个路径点
    marker_pub.publish(line_strip);

    ros::shutdown();

    return 0;
}