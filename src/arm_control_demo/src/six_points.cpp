#include <ros/ros.h>
#include <moveit/move_group_interface/move_group_interface.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "six_points");

    ros::AsyncSpinner spinner(1);
    spinner.start();

    moveit::planning_interface::MoveGroupInterface move_group("manipulator");

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


    target_pose[3].position.x = 0.40;
    target_pose[3].position.y = -0.20;
    target_pose[3].position.z = 0.40;

    target_pose[4].position.x = 0.40;
    target_pose[4].position.y = 0.20;
    target_pose[4].position.z = -0.40;

    target_pose[5].position.x = -0.40;
    target_pose[5].position.y = 0.20;
    target_pose[5].position.z = 0.40;

for(int i=0; i<6; i++)
{
    move_group.setPoseTarget(target_pose[i]);

    bool success =
        (move_group.move() ==
         moveit::core::MoveItErrorCode::SUCCESS);

    if(success)
        ROS_INFO("Move to point %d Success!", i+1);
    else
        ROS_WARN("Move to point %d Failed!", i+1);

    }

    ros::shutdown();

    return 0;
}