#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}


int main() {
  ZNSession *s = zn_open(PEER, 0, 0);
  if (s == 0) {
    printf("Error creating session!\n");
    exit(-1);
  }
  sleep(1);
  std::string data = "Hello from C++";
  std::string key = "/zenoh/test";
  zn_write(s, key.c_str(), data.c_str(), data.length());
  zn_close(s);
}
