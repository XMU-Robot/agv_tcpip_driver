//
// Created by ray on 19-3-31.
//

#include <ros_handle.h>

using namespace std;
void handle_spin();

ros::Publisher ros_cmd_vel_pub;
enum DirectCmd direct_cmd_;
bool HandleInit(ros::NodeHandle ros_nh)
{

    ros_cmd_vel_pub = ros_nh.advertise<geometry_msgs::Twist>("cmd_vel", 30);
    printf("run HandleInit\n");

   // thread t2(handle_spin);
  //  t2.detach();
    return true;

}




void handle_spin()
{
    //printf("run handle_spin\n");
    static geometry_msgs::Twist cmd_vel_;
    RecvContainer *recv_container=new RecvContainer();
    if(Take(recv_container))
    {


        printf("run handle_spin  %d\n",recv_container->direct_cmd);
        switch ((enum DirectCmd) recv_container->direct_cmd)
        {
            case UP:
                cmd_vel_.linear.x = 0.5;
                cmd_vel_.angular.z = 0.0;
                break;
            case DOWN:
                cmd_vel_.linear.x = -0.5;
                cmd_vel_.angular.z = 0.0;
                break;
            case LEFT:
                cmd_vel_.linear.x = 0.0;
                cmd_vel_.angular.z = 0.5;
                break;
            case RIGHT:
                cmd_vel_.linear.x = 0.0;
                cmd_vel_.angular.z = -0.5;
                break;
            case STOP:
                cmd_vel_.linear.x = 0.0;
                cmd_vel_.angular.z = 0.0;
            default:
                cmd_vel_.linear.x = 0.0;
                cmd_vel_.angular.z = 0.0;
        }
        cmd_vel_.linear.y=0.0;
        cmd_vel_.linear.z=0.0;
        cmd_vel_.angular.x=0.0;
        cmd_vel_.angular.y=0.0;

    }
    ros_cmd_vel_pub.publish(cmd_vel_);
    usleep(200);
}
