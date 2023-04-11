#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <iomanip>
#define MAGIC_NUMBER_LENGTH 6
#define HEAD_SIZE 12
#define MAX_FILE_SIZE 1024*1024+1

struct pattern{
    char m_protocol[MAGIC_NUMBER_LENGTH]; /* protocol magic number (6 bytes) */
    uint8_t m_type;                          /* type (1 byte) */
    uint8_t m_status;                      /* status (1 byte) */
    uint32_t m_length;                    /* length (4 bytes) in Big endian*/
} __attribute__ ((packed));


ssize_t safesend(int __fd, const char *__buf, size_t __n, int __flags=0){
    //const void *data=__buf;
    size_t ret = 0;
    while (ret < __n) {
        size_t b = send(__fd, __buf + ret, __n - ret, __flags);
        if (b == 0) printf("safesend: socket Closed \n"); // 当连接断开
        if (b < 0) printf("Error ?"); // 这里可能发生了一些意料之外的情况
        if(b<=0) {
            return b;
        }
        ret += b; // 成功将b个byte塞进了缓冲区
        std::cout<<"ret=="<<ret<<std::endl;
    }
    std::cout<<"Now I'm going back."<<std::endl;
    return ret;
}

ssize_t saferecv(int __fd, char *__buf, size_t __n, bool showret=0, int __flags=0){
    //const void *data=__buf;
    size_t ret = 0;
    std::cout<<"saferecv: length = "<<__n<<std::endl;
    while (ret < __n) {
        size_t b = recv(__fd, __buf + ret, __n - ret, __flags);
        if (b == 0) printf("saferecv: socket Closed \n"); // 当连接断开
        if (b < 0) printf("Error ?"); // 这里可能发生了一些意料之外的情况
        if(b<=0) {
            return b;
            }
        ret += b; // 成功将b个byte塞进了缓冲区
        if(showret) std::cout<<"ret="<<ret<<std::endl;
    }
    return ret;
} 

std::vector<std::string> parse(){
    std::string line;
    getline(std::cin, line);
    /** 解析字符串 */
    int len = line.size(), i=0;

    std::string tmp;
    std::vector<std::string> tmp_vc;
    while(i < line.size()){
        if(line[i] == ' '){
            i++;
            continue;
        }
        int pos = line.find(' ', i);    // 获取下一个空格的位置
        tmp = line.substr(i, pos-i);    // 截取字符串
        tmp_vc.push_back(tmp);
        i = pos;
    }
    return tmp_vc;
}

void headPrinter(pattern pat);

pattern headReceiver(int sock){
    char buffer[HEAD_SIZE];
    memset(buffer, 0, HEAD_SIZE);
    pattern recvHead;
    memset(&recvHead,0,sizeof(recvHead));
    //std::cout<<"---------------This is before getting the value-----------------"<<std::endl;
    //headPrinter(recvHead);
    //recv(sock, buffer, sizeof(buffer), 0 );
    //int sign=recv(sock, &recvHead, sizeof(recvHead), 0 );
    int sign=saferecv(sock, buffer, HEAD_SIZE, 0 );
    if(sign==0){
        std::cout<<"headReceiver: Socket closed."<<std::endl;
        recvHead.m_status=0xEE;
        
        return recvHead;
        }
    //std::cout<<"sign="<<sign<<std::endl;
    memcpy(&recvHead, buffer, sizeof(buffer) );
    std::cout<<"---------------This is just after getting the value-----------------"<<std::endl;
    headPrinter(recvHead);

    return recvHead;
}

void headSender(int sock, uint8_t type, uint8_t status, uint32_t length=12, int __flags=0){
    //char prot[6]={'\xe3','m','y','f','t','p'};
    std::cout<<"Sending......................................"<<std::endl;
    pattern head={{'\xe3','m','y','f','t','p'}, type, status, htonl(length)};
    //std::cout<<head.m_length<<head.m_status<<head.m_protocol<<head.m_status<<std::endl;
    headPrinter(head);
    //printf("size of packed_str=%d\n",(int)sizeof(struct pattern));
    int sign=safesend(sock, (char*)&head, sizeof(head), __flags);
    printf("type: %x , status: %x ,length: %x",head.m_type,head.m_status,head.m_length);
    if(sign==0){
        pattern recvHead;
        std::cout<<"headReceiver: Socket closed."<<std::endl;
        recvHead.m_status=0xEE;

    }
    //send(sock, (char *)&head, sizeof(head), __flags);
    std::cout<<"Sent......................................"<<std::endl;
}

void headPrinter(pattern pat){
    char tem[7];
    strncpy(tem, pat.m_protocol,6);
    tem[6]='\0';
    std::cout<<"protocol: "<<tem<<std::endl;
    std::cout<<"type: "<<std::hex<<uint16_t(pat.m_type)<<std::endl;
    std::cout<<"status: "<<std::dec<<int(pat.m_status)<<std::endl;
    std::cout<<"length: "<<ntohl(pat.m_length)<<std::endl;
}


// int recvfile(int _sock, const char* fname) {
//     int len = strlen(fname);
//     headSender(_sock, 0xA7,0, 12 + len + 1);
//     safesend(_sock, fname, len+1);
//     pattern reply = headReceiver(_sock);

//     char* buf = new char[MAX_FILE_SIZE];
//     //int recvlen=saferecv(_sock, buf, MAX_FILE_SIZE);
//     int recvlen = send(_sock, buf, MAX_FILE_SIZE, 0);
//     //FILE *fq=fopen("get.txt", "ab");
//     FILE *fq=fopen(fname, "ab");
//     fwrite(buf, 1, recvlen, fq);
//     fclose(fq);

//     return 1;
// }
int recvfile(int _sock, const char* fname) {
    int len = strlen(fname);

    pattern filedatahead = headReceiver(_sock);
    int filelen=htonl(filedatahead.m_length)-HEAD_SIZE;
    char* buf = new char[filelen + 1];
    int recvlen=saferecv(_sock, buf, filelen);
    //FILE *fq=fopen("get.txt", "ab");
    FILE *fq=fopen(fname, "ab");
    fwrite(buf, 1, recvlen, fq);
    fclose(fq);
    
    return 1;
}

int sendfile(const char* filename, int conn){
    std::cout<<"sendfile: filename="<<filename<<std::endl;
    FILE *fp=fopen(filename, "rb");
    char buffer[MAX_FILE_SIZE];
    bzero(buffer, sizeof(buffer));
    int len=0;
    
    while(!feof(fp)){

        int temlen=fread(buffer+len, 1, sizeof(buffer+len), fp);
        len+=temlen;
        std::cout<<"sendfile: len="<<len<<std::endl;
        
        // if(len != write(conn, buffer, len)){
        //     printf("write.\n");
        //     break;
        // }
    }
    headSender(conn, 0xFF, 0, HEAD_SIZE+len);
    safesend(conn, buffer, len);
    std::cout<<"##########File sended.##############"<<std::endl;
    fclose(fp);
    return 1;
}
