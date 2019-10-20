#! /bin/sh
slam=1
navigation=2
export TURTLEBOT3_MODEL=burger
export ROS_MASTER_URI=http://localhost:11311
export ROS_HOSTNAME=localhost

source /opt/ros/kinetic/setup.sh
source /home/ray/ROS/turtlebot3_ws/devel/setup.sh
source /home/ray/ROS/cartographer_ws/install_isolated/setup.sh
source /home/ray/ROS/catkin_pionner/devel/setup.sh
source /home/ray/ROS/tcpip_map_driver/devel/setup.sh

if [ "$1" = "$navigation" ];then
echo "start navigation"
roslaunch /home/ray//ROS/turtlebot3_ws/src/turtlebot3/turtlebot3_navigation/launch/xmuagv_navigation.launch
else 
echo "start slam"
roslaunch /home/ray//ROS/turtlebot3_ws/src/turtlebot3/turtlebot3_slam/launch/xmuagv_slam.launch
fi


