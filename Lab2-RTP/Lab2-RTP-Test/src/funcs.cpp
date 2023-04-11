
// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cerrno>
// #include <cstring>
// #include <vector>
#include "rtp.h"
#include "util.h"

// #include <sys/socket.h>
// #include <sys/types.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <getopt.h>
// #include <iomanip>
#include "funcs.h"
#define MAGIC_NUMBER_LENGTH 6
#define MAX_HEAD_SIZE 11
#define MAX_FILE_SIZE 1024*1024+1
#define MAX_DATA_SIZE 1472
#define DATA 2
// typedef struct __attribute__ ((__packed__)) rtp_header_t {
//     uint8_t type;       // 0: START; 1: END; 2: DATA; 3: ACK
//     uint16_t length;    // Length of data; 0 for ACK, START and END packets
//     uint32_t seq_num;
//     uint32_t checksum;  // 32-bit CRC
// } rtp_header_t_t;

rtp_header_t make_head(uint8_t _type, uint16_t _length, uint32_t _seq_num, const char *message)
{
    rtp_header_t head;
    head.type = _type;
    head.length = _length;
    head.seq_num = _seq_num;
    head.checksum = 0;

    return head;
}
// rtp_header_t make_head(uint8_t _type, uint16_t _length, uint32_t _seq_num, const char *message)
// {
//     rtp_header_t head;
//     head.type = _type;
//     head.length = _length;
//     head.seq_num = _seq_num;
//     if(message != NULL && _length != 0)//data
//         head.checksum = compute_checksum(message, _length);
//     else if(_length == 0){//¹¦ÄÜÍ·
//         head.checksum = 0;
//         //head.checksum = compute_checksum((char *)&head, MAX_HEAD_SIZE);
//     }
//     else
//         head.checksum = 0;
//     return head;
// }

void headprinter(rtp_header_t head)
{
    std::cout << "type: " << (int)head.type << std::endl;
    std::cout << "length: " << head.length << std::endl;
    std::cout << "seq_num: " << head.seq_num << std::endl;
    std::cout << "checksum: " << head.checksum << std::endl;
}

void packetprinter(rtp_packet_t packet)
{
    headprinter(packet.rtp);
    std::cout << "data: " << packet.payload << std::endl;
    //std::
}

rtp_packet_t make_packet(uint8_t _type, uint16_t _length, uint32_t _seq_num, const char *message)
{
    rtp_packet_t packet;
    packet.rtp = make_head(_type, _length, _seq_num, message); 
    if(message) memcpy(packet.payload, message, _length);
    packet.rtp.checksum = compute_checksum((char *)&packet, _length + MAX_HEAD_SIZE);
    return packet;
}
// char* make_packet(rtp_header_t head, const char *message, int len)
// {
//     char *packet = (char *)malloc(len+sizeof(head));
//     memcpy(packet, &head, sizeof(head));
//     memcpy(packet+sizeof(head), message, len);
//     return packet;
// }

bool judge_head(rtp_packet_t packet)
{
    uint32_t ori_checksum = packet.rtp.checksum;
    rtp_packet_t tempacket;
    memset(&tempacket, 0, sizeof(tempacket));
    memccpy(&tempacket, &packet, sizeof(packet), sizeof(packet));
    //std::cout<<"--------judge_head cout---------------------------------------------------------------"<<std::endl;
    tempacket.rtp.checksum = 0;
   //std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$tempacket.rtp$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ " << std::endl;
    //headprinter(tempacket.rtp);
    //std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$packet.rtp$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ " << std::endl;
    //headprinter(packet.rtp);
    //std::cout<<"packet.strlen(payload): "<<strlen(packet.payload)<<std::endl;
    //for(int i = 0; i < 20; i++)
    //   std::cout<<packet.payload[i];
    //std::cout<<std::endl;
    uint32_t checksum = compute_checksum((char *)&tempacket, tempacket.rtp.length + MAX_HEAD_SIZE);
    //std::cout<<"packet.checksum: "<<packet.rtp.checksum<<std::endl;
    packet.rtp.checksum = 0;
    uint32_t checksum_on_oripacket = compute_checksum((char *)&packet, packet.rtp.length + MAX_HEAD_SIZE);
    //std::cout<<"tem checksum: "<<checksum<<std::endl;
    //std::cout<<"checksum_on_oripacket: "<<checksum_on_oripacket<<std::endl;
    //std::cout<<"checksum + packet.checksum: "<<checksum + packet.rtp.checksum<<std::endl;
    //std::cout<<"--------judge_head cout---------------------------------------------------------------"<<std::endl;
    if(ori_checksum == checksum_on_oripacket){
            //std::cout << "ori_checksum == packet.rtp.checksum" << std::endl;
            return true;
        }
    else
        return false;
}

int udpdatasend(int sock_fd, const char *message, int sendlen, int seq_num, struct sockaddr_in receiver_addr, bool resend)
{

    rtp_packet_t sendpacket = make_packet(DATA, sendlen, seq_num, message);

    if(resend){
        std::cout<<"--------------------resend------------------------"<<std::endl;
        headprinter(sendpacket.rtp);
        std::cout<<"--------------------resend------------------------"<<std::endl;
    }
    int sendlen3 = sendto(sock_fd, &sendpacket, sendlen + MAX_HEAD_SIZE, 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
    //sendpacket, sizeof(sendpacket), 0, (struct sockaddr *)&receiver_addr, sizeof(receiver_addr));
    if (sendlen3 < 0)
    {
        perror("data sendto error:");
        return -1;
    }
   //packetprinter(sendpacket);
    return sendlen3 - 11 ;
}

    

int mymax(int a,int b)
{
    return a>b?a:b;
}

int mymin(int a,int b)
{
    return a<b?a:b;
}

char* makedata(const char *message, int len)
{
    char *data = (char *)malloc(len);
    memcpy(data, message, len);
    return data;
}


