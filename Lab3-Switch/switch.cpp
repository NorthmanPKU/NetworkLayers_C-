#include "switch.h"
#include <iostream>

class MySwitch : public SwitchBase {
   public:
    /*
     InitSwitch�������ڽ��н�����״̬�ĳ�ʼ�������а����Ĳ����������£�
     numPorts��ʾ�˽������Ķ˿ڵ���������֤numPorts����0
     ע�⣺��Lab3һ�£�����Լ���˿ںŴ�1��ʼ���б�ţ��ر�أ�ÿ����������1�Ŷ˿ھ���Controller�����ӡ�
    */
    void InitSwitch(int numPorts) override {
        portsNum = numPorts;
    }
    /*
      ProcessFrame�������𽻻������յ���֡���д�����ת�������а���������������

      inPort��ʾ�յ���֡����˿ں�

      framePrt��һ��ָ�룬��ʾ�յ���֡������֡�ĸ�ʽ����3.1�й涨�ĸ�ʽ

      ProcessFrame�ķ���ֵ��ʾ�������֡Ӧ����ת�����Ķ˿ںţ��ر��

      ���صĶ˿ں�Ϊ-1����ʾ��֡Ӧ����������
      ���صĶ˿ں�Ϊ0����ʾ��֡Ӧ�����㲥��������˿���������˿��ϡ�
      ע�⣺

      1.�벻Ҫ��ProcessFrame�����ڲ���framePtr�����ͷš�

      2.�������ڴ��������Կ�������AGING֡��Ӧ��ֱ�Ӷ�����֡������������controller�Ŀ���֡����Ľ�������ProcessFrame����Ӧ������-1��
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