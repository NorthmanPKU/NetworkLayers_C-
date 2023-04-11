#include "sender_def.h"
#include "util.h"
#include "rtp.h"
#include "funcs.h"
#include <iostream>
#include <fstream>

#include <stdio.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <unistd.h>   
#include <errno.h>   
#include <string.h>   
#include <stdlib.h>   
#include <vector>
#include <memory.h>

#define START 0
#define END 1
#define DATA 2
#define ACK 3
#define MAX_HEAD_SIZE 11
#define MAX_DATA_SIZE 1461
#define MAX_PACKET_SIZE 1472
#define MAX_DATA_WAITTIME 100
//#define MAX_WINDOW_EXIST_TIME 100
#define MAX_WINDOW_EXIST_TIME 100000
//#define ALLOW_HEADPRINTER
//#define ALLOW_ACKWAITERPRINTER

using namespace std;
int sock_fd;
uint32_t windowsize = 0;
//char recerverip[33];
uint16_t recerverport = 0;
struct sockaddr_in receiver_addr;
uint32_t seq_num=0;

struct ACKwaiter{
    uint32_t seq_num;
    int data_start;
    int data_size;
    ACKwaiter(uint32_t _seq_num, int _data_start, int _data_size){
        seq_num = _seq_num;
        data_start = _data_start;
        data_size = _data_size;
    }
};

void ACKwaiterprinter(ACKwaiter* ackwaiter){
    cout<<"ACKwaiter: seq_num="<<ackwaiter->seq_num<<", data_start="<<ackwaiter->data_start<<", data_size="<<ackwaiter->data_size<<endl;
}   


int initSender(const char *receiver_ip, uint16_t receiver_port, uint32_t window_size)
{
    //LOG_MSG("#######################initSender#######################");
    cout<<"#######################Enter initSender#######################"<<endl;
    
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    //int opt=1;
    //setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
    if (sock_fd < 0)
    {
        perror("socket");
        return -1;
    }
    // cout<<"#######################get socket#######################"<<endl;
    // struct sockaddr_in sender_addr;
    // memset(&sender_addr, 0, sizeof(sender_addr));
    // cout<<"#######################memset#######################"<<endl;
    // sender_addr.sin_family = AF_INET;
    // cout<<"#######################sinfam#######################"<<endl;
    // sender_addr.sin_port = htons(12336);
    // cout<<"#######################sinport#######################"<<endl;
    // sender_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // cout<<"#######################saddr#######################"<<endl;
    
    // cout<<"#######################bind#######################"<<endl;
    // if(bind(sock_fd, (struct sockaddr *)&sender_addr, sizeof(sender_addr)) < 0)
    // {
    //     perror("bind error:");
    //     return -1;
    // }
    memset(&receiver_addr, 0, sizeof(receiver_addr));   
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(receiver_port);
    receiver_addr.sin_addr.s_addr = inet_addr(receiver_ip);

    uint32_t randseqnum=rand();
    cout<<"#######################randseqnum#######################"<<randseqnum<<endl;
    rtp_header_t starthead = make_head(START, 0, randseqnum);
    #ifdef ALLOW_HEADPRINTER    
    headprinter(starthead);
    #endif
    rtp_packet_t startpacket;
    startpacket.rtp = starthead;
    memset(startpacket.payload, 0, sizeof(startpacket.payload));
    startpacket.rtp.checksum = compute_checksum(&startpacket, MAX_HEAD_SIZE+startpacket.rtp.length);
    cout<<"#######################checksum#######################"<<startpacket.rtp.checksum<<endl;
    #ifdef ALLOW_HEADPRINTER
    headprinter(startpacket.rtp);
    #endif
    cout<<"#######################Begin Send initSender#######################"<<endl;
    int sendlen=sendto(sock_fd, &startpacket, sizeof(rtp_header_t)+strlen(startpacket.payload), 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
    if(sendlen<0)
    {
        perror("sendto error:");
        return -1;
    }
    cout<<"#######################Send Success#######################"<<sendlen<<endl;
    char recvbuf[MAX_PACKET_SIZE];
    memset(recvbuf, 0, sizeof(recvbuf));
    cout<<"#######################Begin recv#######################"<<endl;
    int recvlen=recvfrom(sock_fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&receiver_addr, (socklen_t *)sizeof(receiver_addr));
    cout<<"#######################Recv#######################"<<endl;
    rtp_packet_t recvpacket = *(rtp_packet_t *)recvbuf;
    if(recvpacket.rtp.type==ACK)
    {
        cout<<"#######################Recv Start#######################"<<endl;
        if(recvpacket.rtp.seq_num==randseqnum)
        {
            cout<<"#######################Recv Start Success#######################"<<endl;
            windowsize=window_size;
            recerverport=receiver_port;
            //strcpy(recerverip, receiver_ip);
            return 0;
        }
        else
        {
            cout<<"#######################Recv Start Failed#######################"<<endl;
            return -1;
        }
    }
    else
    {
        cout<<"#######################Recv Start Failed#######################"<<endl;
        return -1;
    }
    // if(recvpacket.
    // {
    //     cout<<"#######################initSender success#######################"<<endl;
    //     windowsize = window_size;
    //     //strcpy(recerverip, receiver_ip);
    //     recerverport = receiver_port;
    //     return 0;
    // }
    // else
    // {
    //     cout<<"#######################initSender failed#######################"<<endl;
    //     return -1;
    // }
}
// typedef struct __attribute__ ((__packed__)) rtp_header_t {
//     uint8_t type;       // 0: START; 1: END; 2: DATA; 3: ACK
//     uint16_t length;    // Length of data; 0 for ACK, START and END packets
//     uint32_t seq_num;
//     uint32_t checksum;  // 32-bit CRC
// } rtp_header_t_t;

//V0.3
int sendMessage(const char *message)
{   
    cout<<"#######################Enter sendMessage#######################"<<endl;
    //vector<ACKwaiter> sendlist;
    vector<int> sendlenlist;
    int curwaitnum=0;
    int base=0;
    seq_num=0;
    //open file and read content
    
    FILE *fp=fopen(message, "rb");
    cout<<"#######################open file success#######################"<<endl;
    char* filebuf = new char[MAX_FILE_SIZE];
    cout<<"#######################memset filebuf#######################"<<endl;
    bzero(filebuf, sizeof(filebuf));
    int len=0;
    
    cout<<"#######################read file#######################"<<endl;
    while(!feof(fp)){
        int temlen=fread(filebuf+len, 1, sizeof(filebuf+len), fp);
        len+=temlen;
    }
    cout<<"#######################read file success#######################"<<endl;
    int filelen = len;
    if(filelen<0)
    {
        perror("read file error:");
        return -1;
    }

    int mlen=filelen;


    int totalnum = filelen/MAX_DATA_SIZE;
    //int mlen = strlen(message);
    //char* filebuf = (char*)message;

    int seq_num_len=0;
    int base_len=0;
    //clock_t windowstart=clock();
    uint64_t windowstart=now_us();
    cout<<"#######################Begin Send#######################"<<endl;
    cout<<"windowsize: "<<windowsize<<endl;
    int prerecvackseqnum = 0;
    while(base_len<mlen)// || curwaitnum>0)
    {
        // if(seq_num > 18900){
        //     cout<<"seq_num_len:"<<seq_num_len<<endl;
        //     cout<<"curwaitnum:"<<curwaitnum<<endl;
        //     cout<<"mlen:"<<mlen<<endl;
        // }
        // if(curwaitnum != sendlist.size()){
        //     cout<<"WTF???????? curwaitnum:"<<curwaitnum<<" "<<"sendlist.size():"<<sendlist.size()<<endl;
        // }
        //clock_t curtime=clock();
        uint64_t curtime=now_us();
        //超时重传判断
        if(curtime-windowstart>=MAX_WINDOW_EXIST_TIME)
        {

            seq_num = base;
            seq_num_len = base_len;
            windowstart=now_us();
        }
        //如果还能发包就发包
        else if(seq_num_len<mlen && seq_num - base + 1 <= windowsize)
        {
            //cout<<"#######################Send Packet#######################"<<endl;
            int sendlen=mymin(mlen-seq_num_len, MAX_DATA_SIZE);    
            int udpsend = udpdatasend(sock_fd, filebuf+seq_num_len, sendlen, seq_num, receiver_addr);    
            if(udpsend<0)
            {
                perror("sendto error:");
                return -1;
            }
            sendlenlist.push_back(sendlen);
            curwaitnum++;
            seq_num_len+=sendlen;
            seq_num++;
        }

        char recvbuf[MAX_PACKET_SIZE];
        memset(recvbuf, 0, sizeof(recvbuf));
        int len = MAX_PACKET_SIZE;
        if(recvfrom(sock_fd, recvbuf, sizeof(recvbuf), MSG_DONTWAIT, (struct sockaddr *)&receiver_addr, (socklen_t *)&len)>0)
        //while(recvfrom(sock_fd, recvbuf, sizeof(recvbuf), MSG_DONTWAIT, (struct sockaddr *)&receiver_addr, (socklen_t*)&len)>0)
        {
            //cout<<"#######################收到ACK#######################"<<endl;
            //break;
            //rtp_header_t *recvhead=(rtp_header_t *)recvbuf;
            rtp_header_t *recvhead=&(((rtp_packet_t *)recvbuf)->rtp);
            #ifdef ALLOW_HEADPRINTER
            cout<<"--------------------------recv ACK--------------------------"<<endl;
            headprinter(*recvhead);
            cout<<"--------------------------recv ACK--------------------------"<<endl;
            #endif
            if(recvhead->type==ACK && recvhead->seq_num > base)
            {
                int packetsnewack = recvhead->seq_num - base;
                base = recvhead->seq_num;
                for(int i=0; i<packetsnewack; i++)
                {
                    base_len+=sendlenlist.front();
                    curwaitnum--;
                }
                sendlenlist.erase(sendlenlist.begin(), sendlenlist.begin()+packetsnewack);
                // if(recvhead->seq_num==prerecvackseqnum)
                // {
                //     continue;
                // }
                // if(recvhead->seq_num==14)
                // {
                //     cout<<"recv ACK 13!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                //     for(int i=0;i<curwaitnum;i++)
                //     {
                //         cout<<sendlist[i].seq_num<<","; 
                //     }
                // }
                //int i=0;
                // for(;i<curwaitnum;i++)
                // {
                //     if(sendlist[i].seq_num==recvhead->seq_num-1)
                //     {
                //         prerecvackseqnum = recvhead->seq_num;
                //         break;
                //     }
                // }
                // if(i<curwaitnum)
                // {
                //     sendlist.erase(sendlist.begin()+i);
                //     curwaitnum--;
                //     //windowstart=clock();
                //     if(i==0)
                //         windowstart=now_us();
                //     //base=(base+1)%windowsize;
                // }
                // if(recvhead->seq_num==14){
                //     cout<<"after erase!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
                //     for(int i=0;i<curwaitnum;i++)
                //     {
                //         cout<<sendlist[i].seq_num<<","; 
                //     }
                // }
            }
            // if(seq_num > 18900){
            //     cout<<"seq_num:"<<seq_num<<endl;
            // }
            //memset(recvbuf, 0, sizeof(recvbuf));
        }
       
    }
    //结束发送END
    cout<<"#######################Send Over#######################"<<endl;
    //terminateSender();
    rtp_packet_t endpacket = make_packet(END, 0, seq_num);
    headprinter(endpacket.rtp);
    int sendlen=sendto(sock_fd, &endpacket, MAX_HEAD_SIZE, 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
    cout<<"sendlen:"<<sendlen<<endl;
    
    return 0;
}



void terminateSender()
{
    
    close(sock_fd);
}
/**
 * @brief 用于发送数据(优化版本的RTP)
 * @param message 要发送的文件名
 * @return -1表示发送失败，0表示发送成功
 **/
int sendMessageOpt(const char* message){
    return 0;
}

