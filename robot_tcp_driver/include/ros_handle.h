//
// Created by ray on 19-3-31.
//

#ifndef AGV_TCP_DRIVER_ROS_HANDLE_H
#define AGV_TCP_DRIVER_ROS_HANDLE_H

#include <ros/ros.h>
#include <thread>
#include <tcp_define.h>
#include <geometry_msgs/Twist.h>

bool HandleInit(ros::NodeHandle ros_nh);
void handle_spin();
#endif //AGV_TCP_DRIVER_ROS_HANDLE_H
