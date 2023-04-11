#include "switch.h"
#include <iostream>

class MySwitch : public SwitchBase {
   public:
    /*
     InitSwitch函数用于进行交换机状态的初始化，其中包含的参数意义如下：
     numPorts表示此交换机的端口的数量，保证numPorts大于0
     注意：与Lab3一致，我们约定端口号从1开始进行编号，特别地，每个交换机的1号端口均与Controller相链接。
    */
    void InitSwitch(int numPorts) override {
        portsNum = numPorts;
    }
    /*
      ProcessFrame函数负责交换机接收到的帧进行处理与转发，其中包含了两个参数：

      inPort表示收到的帧的入端口号

      framePrt是一个指针，表示收到的帧，其中帧的格式满足3.1中规定的格式

      ProcessFrame的返回值表示的是这个帧应当被转发到的端口号，特别地

      返回的端口号为-1，表示此帧应当被丢弃。
      返回的端口号为0，表示此帧应当被广播到除了入端口外的其他端口上。
      注意：

      1.请不要在ProcessFrame函数内部对framePtr进行释放。

      2.交换机在处理完来自控制器的AGING帧后，应当直接丢弃此帧。即对于来自controller的控制帧，你的交换机的ProcessFrame函数应当返回-1。
    */
    int cmp_mac(mac_addr_t a, mac_addr_t b) {
        for (int i = 0; i < 6; i++) {
            if (a[i] != b[i]) {
                return 0;
            }
        }
        return 1;
    }

    void aging() {
        for (int i = 0; i < mac_table.size(); i++) {
            mac_table[i].time--;
            if (mac_table[i].time == 0) {
                mac_table.erase(mac_table.begin() + i);
            }
        }
    }

    int ProcessFrame(int inPort, char* framePtr) override {
        //mac_table_printer();
        int ret = 0;
        int src_flag = 0;
        ether_header_t* eth = (ether_header_t*)framePtr;

        for (int i = 0; i < mac_table.size(); i++) {
            if (cmp_mac(mac_table[i].mac_addr, eth->ether_src)) {
                mac_table[i].time = ETHER_MAC_AGING_THRESHOLD;
                src_flag = 1;
                break;
            }
        }
        if (src_flag == 0) {
            mac_table_entry entry;
            for(int i = 0; i < 6; i++){
                entry.mac_addr[i] = eth->ether_src[i];
            }
            entry.port = inPort;
            entry.time = ETHER_MAC_AGING_THRESHOLD;
            mac_table.push_back(entry);
        }

        if (eth->ether_type == ETHER_CONTROL_TYPE) {
            aging();
            ret = -1;
        }
        else if(eth->ether_type == ETHER_DATA_TYPE) {
            int flag = 0;
            for (int i = 0; i < mac_table.size(); i++) {
                if (cmp_mac(mac_table[i].mac_addr, eth->ether_dest)) {
                    flag = 1;
                    mac_table[i].time = ETHER_MAC_AGING_THRESHOLD;
                    if(mac_table[i].port != inPort){
                        // if(mac_table[i].port == 0){
                        //     std::cout<<"What the hell is going on ????????????????";
                        // }
                        std::cout<<"#####################################port: "<<mac_table[i].port<<std::endl;
                        ret = mac_table[i].port;
                    }
                    else{
                        ret = -1;
                    }
                }
            }
            if (flag == 0) {
                // mac_table_entry entry;
                // for(int i = 0; i < 6; i++){
                //     entry.mac_addr[i] = eth->ether_src[i];
                // }
                // entry.port = inPort;
                // entry.time = ETHER_MAC_AGING_THRESHOLD;
                // mac_table.push_back(entry);
                ret = 0;
            }

        }
        mac_table_printer();
        return ret;
    }
    void mac_table_printer() {
    std::cout << "--------------------------mac_table_printer--------------------------" << std::endl;
    for (int i = 0; i < mac_table.size(); i++) {
      printf("mac_addr: %02x:%02x:%02x:%02x:%02x:%02x, port: %d, time: %d\n",
             mac_table[i].mac_addr[0], mac_table[i].mac_addr[1],
             mac_table[i].mac_addr[2], mac_table[i].mac_addr[3],
             mac_table[i].mac_addr[4], mac_table[i].mac_addr[5],
             mac_table[i].port, mac_table[i].time);
    }
    std::cout << "--------------------------mac_table_printer--------------------------" << std::endl;
  }
};

SwitchBase* CreateSwitchObject() {
    // TODO : Your code.
    // return nullptr;
    return new MySwitch();
}