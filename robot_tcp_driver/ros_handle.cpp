//
// Created by ray on 19-3-31.
//

#include <ros_handle.h>

using namespace std;
void handle_spin();
void mapCallback(const nav_msgs::OccupancyGridConstPtr& map);
ros::Publisher ros_cmd_vel_pub;
ros::Subscriber ros_map_sub;
int8_t data_ptr[1024*1024 + sizeof(MapHead)];
enum DirectCmd direct_cmd_;
bool HandleInit(ros::NodeHandle ros_nh)
{

    ros_cmd_vel_pub = ros_nh.advertise<geometry_msgs::Twist>("cmd_vel", 30);
    ros_map_sub = ros_nh.subscribe("map",1,&mapCallback);
    ROS_INFO("run HandleInit");
   // thread t2(handle_spin);
  //  t2.detach();
    return true;

}


void mapCallback(const nav_msgs::OccupancyGridConstPtr& map) {
    ROS_INFO("Received a %d X %d map @ %.3f m/pix",
             map->info.width,
             map->info.height,
             map->info.resolution);

    if (map->info.width <= 1024 && map->info.height) {
        MapMessage send_map_message;
        send_map_message.map_head.width.at= map->info.width;
        send_map_message.map_head.height.at= map->info.height;
        data_ptr[0] = send_map_message.map_head.width.tab[3];
        data_ptr[1] = send_map_message.map_head.width.tab[2];
        data_ptr[2] = send_map_message.map_head.width.tab[1];
        data_ptr[3] = send_map_message.map_head.width.tab[0];
        data_ptr[4] = send_map_message.map_head.height.tab[3];
        data_ptr[5] = send_map_message.map_head.height.tab[2];
        data_ptr[6] = send_map_message.map_head.height.tab[1];
        data_ptr[7] = send_map_message.map_head.height.tab[0];

        for (unsigned int y = 0; y < map->info.height; y++) {
            for (unsigned int x = 0; x < map->info.width; x++) {
                unsigned int i = x + y * map->info.width;

                    data_ptr[sizeof(MapHead) + i] = map->data[i];
            }
        }
        send_map_message.data_ptr = data_ptr;
        Send(send_map_message);
    }

    else ROS_ERROR("map is out of size!");

}

void directCmdCtl(RecvContainer* recv)
{
    static geometry_msgs::Twist cmd_vel_;
    cmd_vel_.linear.x = (recv->direct_cmd_v_line * MAX_CMD_V_LINE) / 128.0 ;
    cmd_vel_.angular.z = -(recv->direct_cmd_v_angle * MAX_CMD_V_ANGLE) /128.0 ;
    //ROS_INFO("Receive cmd_vel_.linear.x: %f  cmd_vel_.angular.x: %f",cmd_vel_.linear.x,cmd_vel_.angular.x );
    cmd_vel_.linear.y=0.0;
    cmd_vel_.linear.z=0.0;
    cmd_vel_.angular.x=0.0;
    cmd_vel_.angular.y=0.0;

    ros_cmd_vel_pub.publish(cmd_vel_);
}

void handle_spin()
{
    RecvContainer *recv_container=new RecvContainer();

    if(Take(recv_container))
    {
        directCmdCtl(recv_container);
    }
    usleep(1000);
}
