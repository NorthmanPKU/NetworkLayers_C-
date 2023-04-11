#include "receiver_def.h"
#include <iostream>
#include <fstream>
#include "util.h"
#include "rtp.h"
#include "funcs.h"

#include <stdio.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <unistd.h>   
#include <errno.h>   
#include <string.h>   
#include <stdlib.h>   

#define ALLOW_HEADPRINTER

using namespace std;

#define START 0
#define END 1
#define DATA 2
#define ACK 3
#define MAX_HEAD_SIZE 11
#define MAX_DATA_SIZE 1461
#define MAX_DATA_WAITTIME 100
#define MAX_WINDOW_EXIST_TIME 100

//#define ALLOW_COUT_1



uint32_t recvwindowsize = 0;
/**
 * @brief 开启receiver并在所有IP的port端口监听等待连接
 * 
 * @param port receiver监听的port
 * @param window_size window大小
 * @return -1表示连接失败，0表示连接成功
 */
int recvsock_fd = 0;
struct sockaddr_in sender_addr;   
socklen_t addrlen = sizeof(sender_addr);


int initReceiver(uint16_t port, uint32_t window_size){
    cout<<"---------------------initReceiver---------------------"<<endl;
    recvwindowsize = window_size;
    recvsock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    int opt=1;
    setsockopt(recvsock_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
    if (recvsock_fd < 0)
    {
        perror("socket");
        return -1;
    }
    recvwindowsize = window_size;
    
    memset(&sender_addr, 0, sizeof(sender_addr));
    sender_addr.sin_family = AF_INET;
    sender_addr.sin_port = htons(port);
    sender_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t len = sizeof(sender_addr);
    if (bind(recvsock_fd, (struct sockaddr *)&sender_addr, sizeof(sender_addr)) < 0)
    {
        perror("bind");
        return -1;
    }
    char* startbuff = (char*)malloc(MAX_HEAD_SIZE);
    memset(startbuff, 0, MAX_HEAD_SIZE);
    cout<<"---------------------waiting for recv---------------------"<<endl;
    int startlen = recvfrom(recvsock_fd, startbuff, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, &len);
    if(startlen < 0){
        perror("recvfrom");
        return -1;
    }
    RTP_header* starthead = (RTP_header*)startbuff;
    headprinter(*starthead);
    int seq_num = starthead->seq_num;
    RTP_header ackhead = make_head(ACK, 0, seq_num);
    rtp_packet_t ackpacket = make_packet(ACK, 0, seq_num, NULL);
    int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, len);
    cout<<"---------------------initrecv success---------------------"<<endl;

    return 0;
}

/**
 * @brief 用于接收数据并在接收完后断开RTP连接
 * @param filename 用于接收数据的文件名
 * @return >0表示接收完成后到数据的字节数 -1表示出现其他错误
 */
int recvMessage(char* filename)
{
    cout<<"---------------------recvMessage---------------------"<<endl;
    FILE* fp = fopen(filename, "wb");
    if(fp == NULL){
        perror("fopen");
        return -1;
    }
    vector<rtp_packet_t> recvpackets;
    int next_expect_seq_num = 0;
    int recvlen = 0;
    rtp_packet_t* pbuff = (rtp_packet_t*)malloc(sizeof(rtp_packet_t));
    while(recvlen = recvfrom(recvsock_fd, pbuff, MAX_HEAD_SIZE + MAX_DATA_SIZE, 0, (struct sockaddr*)&sender_addr, &addrlen)){
        if(recvlen < 0){
            perror("recvfrom");
            return -1;
        }
        RTP_header* head = &(pbuff->rtp);
        if(!judge_head(*pbuff)){
            //cout<<"---------------------Checksum error---------------------"<<endl;
            //return -1;
        }
        else if(head->seq_num > next_expect_seq_num + recvwindowsize){}
        //headprinter(*head);
        else if(head->type == END){
            cout<<"---------------------recv end---------------------"<<endl;
            if(head->seq_num == next_expect_seq_num){
                RTP_header ackhead = make_head(ACK, 0, head->seq_num);
                rtp_packet_t ackpacket = make_packet(ACK, 0, head->seq_num, NULL);
                int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
                if(acklen < 0){
                    perror("sendto");
                    return -1;
                }
                break;
            }

        }
        else if(head->type == DATA){
            #ifdef ALLOW_COUT_1
            cout<<"---------------------recv data---------------------"<<endl;
            #endif
            if(head->seq_num == next_expect_seq_num){
                #ifdef ALLOW_COUT_1
                cout<<"---------------------recv correct data---------------------"<<endl;
                #endif
                fwrite(pbuff->payload, 1, head->length, fp);
                next_expect_seq_num++;
                #ifdef ALLOW_COUT_1
                cout<<"Lets see whether more packets could be written!"<<endl;
                #endif
                while(recvpackets.size()>0 && recvpackets[0].rtp.seq_num == next_expect_seq_num)
                {
                    fwrite(recvpackets[0].payload, 1, recvpackets[0].rtp.length, fp);
                    recvpackets.erase(recvpackets.begin());
                    next_expect_seq_num++;
                }
                
                rtp_packet_t ackpacket = make_packet(ACK, 0, next_expect_seq_num, NULL);
                int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
                if(acklen < 0){
                    perror("sendto");
                    return -1;
                }
            }
            else if(head->seq_num > next_expect_seq_num){
                #ifdef ALLOW_COUT_1
                cout<<"---------------------recv later data---------------------"<<endl;
                #endif
                bool hasinserted = false;
                for(int i = 0; i < recvpackets.size(); i++){
                    if(recvpackets[i].rtp.seq_num == head->seq_num){
                        hasinserted = true;
                        break;
                    }
                    else if(recvpackets[i].rtp.seq_num > head->seq_num){
                        recvpackets.insert(recvpackets.begin() + i, *pbuff);
                        hasinserted = true;
                        break;
                    }
                }
                if(!hasinserted){
                    recvpackets.push_back(*pbuff);
                }
                rtp_packet_t ackpacket = make_packet(ACK, 0, next_expect_seq_num, NULL);
                int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
                if(acklen < 0){
                    perror("sendto");
                    return -1;
                }
            }
        }
        memset(pbuff, 0, sizeof(rtp_packet_t));
    }
    terminateReceiver();
    fclose(fp);
    return 1;
}

/**
 * @brief 用于接收数据失败时断开RTP连接以及关闭UDP socket
 */
void terminateReceiver(){

    close(recvsock_fd);
    return;
} 


/**
 * @brief 用于接收数据并在接收完后断开RTP连接(优化版本的RTP)
 * @param filename 用于接收数据的文件名
 * @return >0表示接收完成后到数据的字节数 -1表示出现其他错误
 */
int recvMessageOpt(char* filename)
{
    cout<<"---------------------recvMessage---------------------"<<endl;
    FILE* fp = fopen(filename, "wb");
    if(fp == NULL){
        perror("fopen");
        return -1;
    }
    vector<rtp_packet_t> recvpackets;
    int next_expect_seq_num = 0;
    int recvlen = 0;
    rtp_packet_t* pbuff = (rtp_packet_t*)malloc(sizeof(rtp_packet_t));
    while(recvlen = recvfrom(recvsock_fd, pbuff, MAX_HEAD_SIZE + MAX_DATA_SIZE, 0, (struct sockaddr*)&sender_addr, &addrlen)){
        if(recvlen < 0){
            perror("recvfrom");
            return -1;
        }
        RTP_header* head = &(pbuff->rtp);
        if(!judge_head(*pbuff)){
            //cout<<"---------------------Checksum error---------------------"<<endl;
            //return -1;
        }
        else if(head->seq_num >= next_expect_seq_num + recvwindowsize){}
        //headprinter(*head);
        else if(head->type == END){
            cout<<"---------------------recv end---------------------"<<endl;
        
            if(head->seq_num == next_expect_seq_num){
                RTP_header ackhead = make_head(ACK, 0, head->seq_num);
                rtp_packet_t ackpacket = make_packet(ACK, 0, head->seq_num, NULL);
                int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
                if(acklen < 0){
                    perror("sendto");
                    return -1;
                }
                break;
            }
            // else{
            //     RTP_header ackhead = make_head(ACK, 0, next_expect_seq_num);
            //     rtp_packet_t ackpacket = make_packet(ACK, 0, next_expect_seq_num, NULL);
            //     int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
            //     if(acklen < 0){
            //         perror("sendto");
            //         return -1;
            //     }
            // }
            // cout<<"---------------------recvMessage success---------------------"<<endl;
            // rtp_header_t ackhead = make_head(ACK, 0, head->seq_num);
            // return head->seq_num;
        }
        else if(head->type == DATA){
            #ifdef ALLOW_COUT_1
            cout<<"---------------------recv data---------------------"<<endl;
            cout<< "seq_num: " << head->seq_num << endl;
            cout<<"next_expect_seq_num: "<<next_expect_seq_num<<endl;
            #endif
            if(head->seq_num == next_expect_seq_num){
                #ifdef ALLOW_COUT_1
                cout<<"---------------------recv correct data---------------------"<<endl;
                #endif
                fwrite(pbuff->payload, 1, head->length, fp);
                next_expect_seq_num++;
                #ifdef ALLOW_COUT_1
                cout<<"Lets see whether more packets could be written!"<<endl;
                #endif
                while(recvpackets.size()>0 && recvpackets[0].rtp.seq_num == next_expect_seq_num)
                {
                    fwrite(recvpackets[0].payload, 1, recvpackets[0].rtp.length, fp);
                    recvpackets.erase(recvpackets.begin());
                    next_expect_seq_num++;
                }
                
                rtp_packet_t ackpacket = make_packet(ACK, 0, head->seq_num, NULL);
                //cout<<"send seq_num: "<<ackpacket.rtp.seq_num<<endl;
                int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
                if(acklen < 0){
                    perror("sendto");
                    return -1;
                }
            }
            else if(head->seq_num > next_expect_seq_num){
                #ifdef ALLOW_COUT_1
                cout<<"---------------------recv later data---------------------"<<endl;
                #endif
                bool hasinserted = false;
                for(int i = 0; i < recvpackets.size(); i++){
                    if(recvpackets[i].rtp.seq_num == head->seq_num){
                        hasinserted = true;
                        break;
                    }
                    else if(recvpackets[i].rtp.seq_num > head->seq_num){
                        recvpackets.insert(recvpackets.begin() + i, *pbuff);
                        hasinserted = true;
                        break;
                    }
                }
                if(!hasinserted){
                    recvpackets.push_back(*pbuff);
                }
                rtp_packet_t ackpacket = make_packet(ACK, 0, head ->seq_num, NULL);
                //cout<<"send seq_num: "<<ackpacket.rtp.seq_num<<endl;
                int acklen = sendto(recvsock_fd, (char *)&ackpacket, MAX_HEAD_SIZE, 0, (struct sockaddr*)&sender_addr, addrlen);
                if(acklen < 0){
                    perror("sendto");
                    return -1;
                }
            }
        }
        memset(pbuff, 0, sizeof(rtp_packet_t));
    }
    terminateReceiver();
    fclose(fp);
    return 1;
}