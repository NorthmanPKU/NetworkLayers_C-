#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include <arpa/inet.h>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "funcs.h"

#define MAGIC_NUMBER_LENGTH 6
#define HEAD_SIZE 12

using namespace std;
bool connsign = 0;
int sock;

int download(int _sock, char* fname) {
    int len = strlen(fname);
    headSender(_sock, 0xA7, 12 + len + 1, 0);
    pattern reply = headReceiver(_sock);
    if (reply.m_status == 0) {
        cout << "Error. Maybe the file doesn't exist. " << reply.m_status
             << endl;
        return 0;
    } else if (reply.m_status == 1) {
        pattern filedatahead = headReceiver(_sock);
        char* buf = new[filedatahead.m_length + 1];
        saferecv(_sock, filedatahead.m_length - HEAD_SIZE);
    }
}

int listfiles(int _sock) {
    cout << "###############listfiles###############" << endl;
    headSender(_sock, 0xA5, 0, 12);
    pattern list_reply = headReceiver(_sock);
    int length = htons(list_reply.m_length) - HEAD_SIZE;
    char* buf = new char[length]{0};
    saferecv(_sock, buf, length, 1);
    cout << buf;
}

char authentication(int sock, char* usrn_psw) {
    // cout<<__LINE__<<": I'm in authentication"<<endl;
    headSender(sock, 0xA3, 0, 12 + strlen(usrn_psw));
    safesend(sock, usrn_psw, strlen(usrn_psw));
    // cout<<__LINE__<<": I'm after safesend."<<endl;
    pattern reply = headReceiver(sock);
    return reply.m_status;
}

int myConnect(int sock_useless, vector<std::string> ins) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    cout << "sock=" << sock << endl;
    if (sock == -1) {
        std::cout << "Error: socket" << endl;
        return 0;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    cout << "ins.size()=" << ins.size() << endl;
    if (ins.size() < 3) {
        serverAddr.sin_port = htons(12123);
        inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    } else {
        serverAddr.sin_port = htons(atoi(ins[2].c_str()));
        inet_pton(AF_INET, ins[1].c_str(), &serverAddr.sin_addr);
    }
    int conrepl =
        connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (conrepl < 0) {
        std::cout << "Error: connect with " << conrepl << std::endl;
        return 0;
    }
    // headSender(sock,0xA1,0,12);
    headSender(sock, 0xA1, 0, 12);
    pattern ans = headReceiver(sock);
    cout << "This is what I got." << endl;
    headPrinter(ans);
    if (ans.m_status == 1) {
        cout << "Connected successfully." << endl;
    } else {
        cout << "That's weird. I received a head whose status osn't 1, which "
                "shouldn't happen at all."
             << endl;
    }
    connsign = 1;
}

int main(int argc, char** argv) {
    cout << "Client starting..." << endl;
    // socket
    // int sock = socket(AF_INET, SOCK_STREAM, 0);
    // cout<<"sock="<<sock<<endl;
    // if (sock == -1) {
    //     std::cout << "Error: socket" << endl;
    //     return 0;
    // }
    cout << "Client started successfully." << endl;
    // connect
    vector<std::string> instruction;

    while (1) {
        cout << "myFTP> ";
        instruction = parse();
        // dealing(sock,instruction)
        if (instruction[0] == "open") {
            myConnect(sock, instruction);

        } else if (instruction[0] == "close") {
            close(sock);
            connsign = 0;
        } else if (connsign == 0 && instruction[0] != "quit") {
            cout << "Please open first before other operations." << endl;
            continue;
        } else if (instruction[0] == "quit") {
            // safesend(sock, "exit", 5, 0);
            headSender(sock, 0xAB, 0);
            headReceiver(sock);
            close(sock);
            cout << "Closing..." << endl;
            return 0;
        } else if (instruction[0] == "auth") {
            //"create a new char* with exictent string"
            char* usrn_psw =
                new char[instruction[1].length() + instruction[2].length() + 2];
            strcat(usrn_psw, instruction[1].c_str());
            strcat(usrn_psw, " ");
            strcat(usrn_psw, instruction[2].c_str());
            
            cout << "Gonna send " << usrn_psw
                 << " with length=" << strlen(usrn_psw) << endl;

            int reply = authentication(sock, usrn_psw);
            if (reply == 0) {
                cout << "Username or password is wrong. Please try again later."
                     << endl;
                close(sock);
                connsign = 0;
            } else {
                cout << "Login successfully!" << endl;
            }
        } else if (instruction[0] == "ls") {
            listfiles(sock);
        } else {
            // cout<<"I got "<<instruction[0]<<" "<<instruction[1]<<"
            // "<<instruction[2]<<endl;
            cout << "C'mon, don't kid me." << endl;
        }
    }

    return 0;

    char data[255];
    char buf[255];
    while (true) {
        std::cin >> data;
        int l = strlen(data);
        // size_t ret = 0;
        // while (ret < len) {
        //     size_t b = send(sock, data + ret, len - ret, 0);
        //     if (b == 0) printf("socket Closed"); // 当连接断开
        //     if (b < 0) printf("Error ?"); // 这里可能发生了一些意料之外的情况
        //     ret += b; // 成功将b个byte塞进了缓冲区
        // }
        safesend(sock, data, l, 0);
        // send(sock, data, strlen(data), 0);
        if (strcmp(data, "exit") == 0) {
            std::cout << "...disconnect" << std::endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
        int len = recv(sock, buf, sizeof(buf), 0);
        buf[len] = '\0';
        std::cout << buf << std::endl;
    }
    close(sock);
    return 0;

    // accept

    return 0;
}