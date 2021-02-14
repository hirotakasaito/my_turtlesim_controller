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

void MyTurtlesimController::go_turn()
{
    ROS_INFO_STREAM(current_pose.theta);
    geometry_msgs::Twist cmd_vel;
    for(int i = 0;i<16;i++)
    {
        cmd_vel.angular.z = 1;
    }
    pub_cmd_vel.publish(cmd_vel);


}

void MyTurtlesimController::go_straight()
{
    ROS_INFO_STREAM(current_pose);
    ROS_INFO_STREAM(ang);
    ROS_INFO_STREAM(turn_count);
    geometry_msgs::Twist cmd_vel;

    if(turn >= 40)
    {
        ang = turn_ang[turn_count]*M_PI/2 - current_pose.theta;
        if(ang <= 0.01)
        {
            turn = 0;
            ang = 0;
            turn_count++;
            if(turn_count == 4)
            {
                 turn_count = 0;
            }
        }
        else
        {
            cmd_vel.angular.z = 0.1;
        }
    }
    else
    {
        cmd_vel.linear.x = 1;
        turn +=1;
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
