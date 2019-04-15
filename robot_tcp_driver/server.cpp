
#include <sever.h>
#include <fstream>

using namespace std;

int picLen=0;

int num=0;
int i=0;
char inputBuffer[256] = {};
int port=8700;
bool runFlag;
int forClientSockfd = 0;
int sockfd = 0;       //sockfd套接字
int recvbytes=0;                          //判断是否断开连接
int map_flag=0;
struct sockaddr_in serverInfo,clientInfo; // clientInfo连接实体地址
socklen_t addrlen = sizeof(clientInfo);

void server();
RecvContainer *Receive();

std::vector<RecvContainer> buffer_pool_;

RecvContainer *recv_container_ptr_;


bool SeverInit()
{
    /**
 * 1.创建套接字
 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /**
     * 2.绑定
     */
    bzero(&serverInfo, sizeof(serverInfo));
    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(port);
    bind(sockfd, (struct sockaddr *) &serverInfo, sizeof(serverInfo));
    /**
     * 3.监听
     */
    listen(sockfd,5);

    recv_container_ptr_=new RecvContainer();

    thread t1(server);
    t1.detach();



    return true;

}

void server() {

    while (1) {
        forClientSockfd = accept(sockfd, (struct sockaddr *) &clientInfo, &addrlen);
        runFlag=true;
        while(runFlag) {
            RecvContainer *container_ptr=Receive();
            if(container_ptr)
            {
                buffer_pool_.push_back(*container_ptr);
            }
            usleep(100);
        }
        usleep(10000);
    }

}

RecvContainer *Receive()
{
    //read the file
    //ROS_INFO("run Receive");
    char ctl_mes[4]={};
    RecvContainer *recv_container=recv_container_ptr_;
    memset(ctl_mes, 0, sizeof(ctl_mes));
    recvbytes = recv(forClientSockfd, ctl_mes, sizeof(ctl_mes), 0);


    /**
     * 异常情况退出
     */
    if(recvbytes==0||recvbytes==-1){
        ROS_INFO("Receive error !!!");
        runFlag=false;
        return nullptr;
        //return 0;
    }

    if(ctl_mes[0]=='A'&&ctl_mes[1]=='A') {
        ROS_INFO("Receive req for map send!");
        map_flag = 1;
        return nullptr;
    }
    else if(ctl_mes[0]=='C'&&ctl_mes[1]=='T') {


        recv_container->direct_cmd_v_angle=ctl_mes[2];
        recv_container->direct_cmd_v_line=ctl_mes[3];
        //ROS_INFO("Receive direct_cmd_v_line: %d  direct_cmd_v_angle: %d",\
        recv_container->direct_cmd_v_line,recv_container->direct_cmd_v_angle );
    }
        return recv_container;

}



bool Take(RecvContainer *recv_container)
{
    if(!buffer_pool_.size())return false;
    else{
        *recv_container=buffer_pool_.front();
        buffer_pool_.erase(buffer_pool_.begin());
        return true;
    }
}

bool Send(MapMessage &send_map_message)
{
    ROS_INFO("run send");
  if(map_flag==1)
  {
        ROS_INFO("map_flag==1");
        send(forClientSockfd, send_map_message.data_ptr, send_map_message.map_head.width.at* \
        send_map_message.map_head.height.at + sizeof(MapHead), 0);
        ROS_INFO("Map send success!!");
      return true;
  }
    return false;
}







