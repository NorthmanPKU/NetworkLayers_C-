#ifndef FUNCS_H
#define FUNCS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <vector>
#include "rtp.h"
#include "util.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <iomanip>
#define MAGIC_NUMBER_LENGTH 6
#define MAX_HEAD_SIZE 12
#define MAX_FILE_SIZE 100*1024*1024+1
#define MAX_DATA_SIZE 1472
#define DATA 2


RTP_header make_head(uint8_t _type, uint16_t _length, uint32_t _seq_num, const char *message = NULL);
void headprinter(RTP_header head);
rtp_packet_t make_packet(uint8_t _type, uint16_t _length, uint32_t _seq_num, const char *message=NULL);
bool judge_head(rtp_packet_t packet);
int udpdatasend(int sock_fd, const char *message, int sendlen, int seq_num, struct sockaddr_in receiver_addr, bool resend = 0);
int mymax(int a,int b);
int mymin(int a,int b);
char* makedata(const char *message, int len);




#endif