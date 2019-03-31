
#include <sever.h>
using namespace std;

char inputBuffer[256] = {};
char message[]="hi";
int port=8700;
bool runFlag;
int forClientSockfd = 0;
int sockfd = 0;       //sockfd套接字
int recvbytes=0;                          //判断是否断开连接
struct sockaddr_in serverInfo,clientInfo; // clientInfo连接实体地址
socklen_t addrlen = sizeof(clientInfo);

//u_char mes=1;

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
            if(runFlag)
            {
                buffer_pool_.push_back(*container_ptr);
            }
            usleep(100);
        }
    }
}

RecvContainer *Receive()
{
    RecvContainer *recv_container=recv_container_ptr_;
    memset(inputBuffer, 0, sizeof(inputBuffer));
    recvbytes = recv(forClientSockfd, inputBuffer, sizeof(inputBuffer), 0);
    /**
     * 处理粘包
     */
    if (inputBuffer[0] == inputBuffer[1] == inputBuffer[4] == 'F' || inputBuffer[5] == 'E') {
        inputBuffer[6] = '\0';
    }
    /**
     * 异常情况退出
     */
    if(recvbytes==0||recvbytes==-1){
        runFlag=false;
        //return 0;
    }
    /**
     * 接口
     */
    u_char mes=0;
    switch (inputBuffer[3]){
        case 'U':
            mes=1;
            break;
        case 'D':
            mes=2;
            break;
        case 'L':
            mes=3;
            break;
        case 'R':
            mes=4;
            break;
        case 'S':
            mes=5;
            break;
        default:
            mes=0;
    }
    //printf("%d\n", sizeof(RecvContainer));
    //printf("Get:%s\n", inputBuffer);
   // printf("%c\n",mes);
    printf("recv_container_ptr_=%d\n", recv_container->direct_cmd);
    recv_container->direct_cmd=mes;
    return recv_container;

}


bool Take(RecvContainer *recv_container)
{
    //printf("run Take\n");
    //RecvContainer *recv_container=new RecvContainer();
    if(!buffer_pool_.size())return false;
    else{
        //printf("run Take\n");
        printf("\nbuffer_pool_.size=%d\n",buffer_pool_.size());
        *recv_container=buffer_pool_.front();
        printf("run Take%d\n",recv_container->direct_cmd);
        buffer_pool_.erase(buffer_pool_.begin());
        return true;
    }

}








