#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <visualization_msgs/Marker.h>//绘制轨迹

//工作空间
//y=0.55,x=-0.4到0.3，z=0.2到0.55
//用于验证工作空间里是否都可达

int main(int argc, char** argv)
{
    ros::init(argc, argv, "workspace_test");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");


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
        visualization_msgs::Marker::POINTS;
                
    line_strip.action =
        visualization_msgs::Marker::ADD;
        
    line_strip.scale.x = 0.03;
    line_strip.scale.y = 0.03;

    line_strip.color.r = 1.0;
    line_strip.color.g = 0.0;
    line_strip.color.b = 0.0;
    line_strip.color.a = 1.0;

    geometry_msgs::Pose start_pose =
        move_group.getCurrentPose().pose;

    geometry_msgs::Pose target_pose[9];

    target_pose[0].position.x = 0.30;
    target_pose[0].position.z = 0.55;

    target_pose[1].position.x = -0.05;
    target_pose[1].position.z = 0.55;

    target_pose[2].position.x = -0.40;
    target_pose[2].position.z = 0.55;

    target_pose[3].position.x = 0.30;
    target_pose[3].position.z = 0.40;

    target_pose[4].position.x = -0.05;
    target_pose[4].position.z = 0.40;

    target_pose[5].position.x = -0.40;
    target_pose[5].position.z = 0.40;

    target_pose[6].position.x = 0.30;
    target_pose[6].position.z = 0.20;

    target_pose[7].position.x = -0.05;
    target_pose[7].position.z = 0.20;

    target_pose[8].position.x = -0.40;
    target_pose[8].position.z = 0.20;

    for(int i=0; i<9; i++)
    {
        target_pose[i].orientation.w = start_pose.orientation.w;
        target_pose[i].position.y=0.55;

        geometry_msgs::Point p;

    p.x = target_pose[i].position.x;
    p.y = target_pose[i].position.y;
    p.z = target_pose[i].position.z;

    line_strip.points.push_back(p);
    }

    marker_pub.publish(line_strip);
    ros::Duration(1.0).sleep();

for(int i=0; i<9; i++)
{
    move_group.setPoseTarget(target_pose[i]);

    moveit::planning_interface::MoveGroupInterface::Plan plan;
    

    // bool success =
    // (
    //     move_group.plan(plan)
    //     ==
    //     moveit::core::MoveItErrorCode::SUCCESS
    // );

    bool success = (move_group.move() == moveit::core::MoveItErrorCode::SUCCESS);

    if(success)
        ROS_INFO("Plan to point %d Success!", i+1);
    else
        ROS_WARN("Plan to point %d Failed!", i+1);

}

ros::shutdown();

return 0;
}