#ifndef MY_TURTLESIM_CONTROLLER_H
#define MY_TURTLESIM_CONTROLLER_H

#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

class MyTurtlesimController
{
public:
    MyTurtlesimController();
    void process();

private:
    void pose_callback(const turtlesim::Pose::ConstPtr &);
    void go_straight();
    void go_square();
    void go_turn();

    int hz;
    int turn;
    float ang;

    ros::Publisher pub_cmd_vel;
    ros::Subscriber sub_pose;
    ros::NodeHandle nh;
    ros::NodeHandle private_nh;
    turtlesim::Pose current_pose;


};


#endif
