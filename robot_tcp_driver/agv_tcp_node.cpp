//
// Created by ray on 19-3-31.
//
#include <sever.h>
#include <ros_handle.h>
int main(int argc, char **argv) {
    ros::init(argc, argv, "sever");
    ros::NodeHandle nh;
    if(!SeverInit()) return 1;
    if(!HandleInit(nh))return 1;
    ros::Rate main_loop_rate(1000);
    while(ros::ok()){
        handle_spin();
        ros::spinOnce();
        main_loop_rate.sleep();
    }
}