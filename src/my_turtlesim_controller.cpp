#include "my_turtlesim_controller/my_turtlesim_controller.h"
#include <stdio.h>
MyTurtlesimController::MyTurtlesimController():private_nh("~")
{
    private_nh.param("hz",hz,{10});

    sub_pose = nh.subscribe("/turtle1/pose",10,&MyTurtlesimController::pose_callback,this);
    pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",1);
}

void MyTurtlesimController::pose_callback(const turtlesim::Pose::ConstPtr &msg)
{
    current_pose = *msg;

}


void MyTurtlesimController::go_straight()
{
    ROS_INFO_STREAM(distance);
    geometry_msgs::Twist cmd_vel;

    if( distance >= goal)
    {
        switch(turn_count)
        {
            case 1:
             ang = fabs(current_pose.theta) - 0.66*M_PI;
             break;
            case 2:
             ang = fabs(current_pose.theta);
             break;
            case 0:
             ang = 0.67*M_PI - fabs(current_pose.theta);
             break;
            default:
             break;
        }
        if(ang <= 0.01)
        {
            turn = 0;
            ang = 0;
            turn_count++;
            if(turn_count == 3)
            {
                 turn_count = 0;
            }
            init_pose_x = current_pose.x;
            init_pose_y = current_pose.y;
            distance = 0.0;
        }
        else
        {
            cmd_vel.angular.z = 0.1;
        }
    }
    else
    {
        cmd_vel.linear.x = 1.0;
        if(init_pose_x == 0.0)
        {
            init_pose_x = current_pose.x;
            init_pose_y = current_pose.y;
        }
        distance = sqrt((current_pose.x - init_pose_x) * (current_pose.x - init_pose_x) + (current_pose.y - init_pose_y) * (current_pose.y - init_pose_y));
    }
    pub_cmd_vel.publish(cmd_vel);
}



void MyTurtlesimController::process()
{
    ros::Rate loop_rate(hz);

    while(ros::ok())
    {
        go_straight();
        ros::spinOnce();
        loop_rate.sleep();
    }

}






int main(int argc,char** argv)
{
    ros::init(argc,argv,"my_turtlesim_controller");
    MyTurtlesimController my_turtlesim_controller;
    my_turtlesim_controller.process();
    return 0;
}
