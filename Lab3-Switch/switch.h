#ifndef COMPNET_LAB4_SRC_SWITCH_H
#define COMPNET_LAB4_SRC_SWITCH_H

#include "types.h"
#include <vector>

class SwitchBase {
 public:
 struct mac_table_entry {
    mac_addr_t mac_addr;
    int port;
    int time;
  };

  void mac_table_printer();
  int portsNum;
  std::vector<mac_table_entry> mac_table;
 
  SwitchBase() = default;
  ~SwitchBase() = default;

  virtual void InitSwitch(int numPorts) = 0;
  virtual int ProcessFrame(int inPort, char* framePtr) = 0;
};

extern SwitchBase* CreateSwitchObject();

// TODO : Implement your switch class.

#endif  // ! COMPNET_LAB4_SRC_SWITCH_H
