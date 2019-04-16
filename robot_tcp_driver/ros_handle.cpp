//
// Created by ray on 19-3-31.
//

#include <ros_handle.h>

using namespace std;
void handle_spin();
void mapCallback(const nav_msgs::OccupancyGridConstPtr& map);
ros::Publisher ros_cmd_vel_pub;
ros::Subscriber ros_map_sub;
int8_t map_data_buff[1024*1024 + sizeof(MapHead)];
float max_v_line=MAX_CMD_V_LINE;
float max_v_angle=MAX_CMD_V_ANGLE;
int8_t req_map_state = 0;

bool HandleInit(ros::NodeHandle ros_nh)
{
    ros_cmd_vel_pub = ros_nh.advertise<geometry_msgs::Twist>("cmd_vel", 30);
    ros_map_sub = ros_nh.subscribe("map",1,&mapCallback);
    ROS_INFO("run HandleInit");
    return true;
}

void CmdSend(const int8_t id,const int8_t len,void *data_ptr)
{
    CmdMessage send_message;
    send_message.head.id = id;
    send_message.head.len = len;
    send_message.data_ptr = data_ptr;
    Send(send_message);
}

void mapCallback(const nav_msgs::OccupancyGridConstPtr& map) {
    ROS_INFO("Received a %d X %d map @ %.3f m/pix",
             map->info.width,
             map->info.height,
             map->info.resolution);
    if(req_map_state==0) {//req map state
        ROS_INFO("req_map_state is false");
        return;
    }
    if (map->info.width <= 1024 && map->info.height) {
        MapMessage *send_map_message=new MapMessage();
        send_map_message->map_head.width.at= map->info.width;
        send_map_message->map_head.height.at= map->info.height;
        map_data_buff[0] = send_map_message->map_head.width.tab[3];
        map_data_buff[1] = send_map_message->map_head.width.tab[2];
        map_data_buff[2] = send_map_message->map_head.width.tab[1];
        map_data_buff[3] = send_map_message->map_head.width.tab[0];
        map_data_buff[4] = send_map_message->map_head.height.tab[3];
        map_data_buff[5] = send_map_message->map_head.height.tab[2];
        map_data_buff[6] = send_map_message->map_head.height.tab[1];
        map_data_buff[7] = send_map_message->map_head.height.tab[0];

        for (unsigned int y = 0; y < map->info.height; y++) {
            for (unsigned int x = 0; x < map->info.width; x++) {
                unsigned int i = x + y * map->info.width;

                map_data_buff[sizeof(MapHead) + i] = map->data[i];
            }
        }
        send_map_message->data_ptr = map_data_buff;
        CmdSend(CMD_SEND_MAP,0,send_map_message);
    }
    else ROS_ERROR("map is out of size!");
}


void CmdCtlReqMap(const ReqMap *value){
    req_map_state=value->req_value;
    ROS_INFO("Receive req_map_state: %d",req_map_state );
}

void CmdCtlSetAgvSpeed(const SetAgvSpeed *value){
    static int time=0;
    geometry_msgs::Twist cmd_vel_;
    cmd_vel_.linear.x = (value->v_line * max_v_line) / 128.0 ;
    cmd_vel_.angular.z = -(value->v_angle * max_v_angle) /128.0 ;
    cmd_vel_.linear.y=0.0;
    cmd_vel_.linear.z=0.0;
    cmd_vel_.angular.x=0.0;
    cmd_vel_.angular.y=0.0;
    ros_cmd_vel_pub.publish(cmd_vel_);

    time++;
    if(time==25) {
        time=0;
        ROS_INFO("Receive cmd_vel_.linear.x: %f  cmd_vel_.angular.z: %f", cmd_vel_.linear.x, cmd_vel_.angular.z);
    }
}
void CmdCtlSetSpeedRange(const SetSpeedRange *value){
    if((value->max_v_line/10.0>=2*MAX_CMD_V_LINE)||(value->max_v_angle/10.0>=2*MAX_CMD_V_ANGLE)){
        ROS_INFO("Set Speed Range is error!");
        return;
    }
    max_v_line=value->max_v_line/10.0;
    max_v_angle=value->max_v_angle/10.0;
    ROS_INFO("Receive max_v_line: %d  max_v_angle: %d",value->max_v_line,value->max_v_angle );
    ROS_INFO("Receive max_v_line: %f  max_v_angle: %f",max_v_line,max_v_angle );
}

void CmdProcess(const CmdMessage* recv)
{

    switch (recv->head.id)
    {
        case CMD_REQ_MAP:{
            ReqMap *req_map=new ReqMap();
            memcpy(req_map, recv->data, sizeof(ReqMap));
            CmdCtlReqMap(req_map);
        }break;
        case CMD_SET_AGV_SPEED: {
            SetAgvSpeed *set_agv_speed = new SetAgvSpeed();
            memcpy(set_agv_speed, recv->data, sizeof(SetAgvSpeed));
            CmdCtlSetAgvSpeed(set_agv_speed);
        }break;
        case CMD_SET_SPEED_RANGE:{
            SetSpeedRange *set_speed_range=new SetSpeedRange();
            memcpy(set_speed_range, recv->data, sizeof(SetSpeedRange));
            CmdCtlSetSpeedRange(set_speed_range);
        }break;

    }
}

void handle_spin()
{
    CmdMessage *recv_container=new CmdMessage();

    if(Take(recv_container))
    {
        CmdProcess(recv_container); //cmd process
    }
    //usleep(1000);
}
