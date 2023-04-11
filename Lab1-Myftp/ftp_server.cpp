#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include <arpa/inet.h>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "funcs.h"
#define MAX_FILE_SIZE 1024*1024+1

using namespace std;
bool login = 0;
struct User {
    string username;
    string password;
    bool stat;
} user = {"user", "123123", 0};


bool file_exist(const char* rfile) {
    // ifstream f(fileName);
    // return f.good();
    
    cout<<"Gonna seek <<"<<rfile<<"."<<endl;
    //if (FILE *file = fopen(fileName, "r")) {
    if (FILE *file = fopen(rfile, "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    } 
}

bool server_auth(User u, char* recv) {
    char* usrn_psw = new char[u.username.length() + u.password.length() + 2];
    strcat(usrn_psw, u.username.c_str());
    strcat(usrn_psw, " ");
    strcat(usrn_psw, u.password.c_str());
    if (strcmp(recv, usrn_psw) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {
    char* ip;
    char* port;
    char defaultip[] = "127.0.0.1";
    char defaultport[] = "12123";
    if (argc < 2) {
        ip = defaultip;
        port = defaultport;
    } else {
        ip = argv[1];
        port = argv[2];
    }
    cout << "Client starting..." << endl;
    // socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        std::cout << "Error: socket" << endl;
        return 0;
    }

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    cout << "ip:port" << ip << " " << port << endl;
    addr.sin_port = htons(atoi(port));
    cout << port << " " << addr.sin_port << endl;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        cout << "Error: bind " << listenfd << endl;
        return 0;
    }
    // listen
    if (listen(listenfd, 128) == -1) {
        std::cout << "Error: listen" << std::endl;
        return 0;
    }

    // accept
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (true) {
        std::cout << "...listening" << std::endl;
        conn = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (conn < 0) {
            std::cout << "Error: accept" << std::endl;
            continue;
        }
        cout<<1<<endl;
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":"
                  << ntohl(clientAddr.sin_port) << std::endl;
        pattern tem = headReceiver(conn);
        cout << "This is what I got." << endl;
        headPrinter(tem);
        headSender(conn, 0xA2, 1, 12);

        vector<std::string> instruction;
        char buf[1024];
        while (true) {
            cout << "begin loop" << endl;
            memset(buf, 0, sizeof(buf));
            pattern curHead = headReceiver(conn);
            cout<<"curHead.m_status == "<<std::hex << uint16_t(curHead.m_status)
                 << endl
                 << std::dec;
                 
            if(curHead.m_status==0xEE){
                cout<<"Main gets the close and break."<<endl;
                break;
            }
            cout << "curHead.mtype == " << std::hex << uint16_t(curHead.m_type)
                 << endl
                 << std::dec;
            if (curHead.m_type == 0xA3) {
                // cout<<"I got a 0xA3"<<endl;
                int length = ntohl(curHead.m_length);
                cout << "length = " << length << endl;
                recv(conn, buf, length - HEAD_SIZE, 0);
                cout << "buf : " << buf << endl;
                bool auth = server_auth(user, buf);
                if (auth) {
                    cout << "User login successfully." << endl;
                    headSender(conn, 0xA4, 1);
                    login = 1;
                } else {
                    cout << "User login failed." << endl;
                    headSender(conn, 0xA4, 0);
                    break;
                }
            } else if (curHead.m_type == 0xA5) {
                char buf[2049] = {0};
                FILE* fp;
                fp = popen("ls", "r");

                int nread = fread(buf, 1, 2049, fp);
                buf[nread]='\0';
                cout << "##############ls = " << buf << endl;
                headSender(conn, 0xA6, 0, HEAD_SIZE + nread+1);
                safesend(conn, buf, nread+1);
            } else if (curHead.m_type == 0xA7) {
                int namelen = ntohl(curHead.m_length);
                char* filename = new char[namelen - HEAD_SIZE];
                saferecv(conn, filename, namelen - HEAD_SIZE, 0);
                cout << "########Client request to download file : " << filename
                     << endl;
                char rfile[3+strlen(filename)]="./";
                strcat(rfile, filename);
                bool sign = file_exist(rfile);
                if (sign == 0) {
                    cout<<"Return 0, file dosen't exists."<<endl;
                    headSender(conn, 0xA8, 0, HEAD_SIZE);
                } else {
                    cout<<sign<<" Begin to send."<<endl;
                    headSender(conn, 0xA8, 1);
                    sendfile(rfile, conn);
                }

            }else if(curHead.m_type == 0xA9){
                int namelen = ntohl(curHead.m_length);
                char* filename = new char[namelen - HEAD_SIZE];
                saferecv(conn, filename, namelen - HEAD_SIZE, 0);
                cout << "########Client request to upload file : " << filename
                     << endl;
                headSender(conn, 0xAA, 1);
                recvfile(conn, filename);
            }

            else if (curHead.m_type == 0xAB) {
                headSender(conn, 0xAC, 0, 12);
                break;
            }
            cout << "end loop" << endl;

        }

        close(conn);
    }
    close(listenfd);
    return 0;
}