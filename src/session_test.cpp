#include <unistd.h>
#include <string>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::string data("Hello from C++");
std::string key("/zenoh/test");

int main(int argc, char** argv) {
  ZNSession *s = zn_open(PEER, 0, 0);
  if (s == 0) {
    printf("Error creating session!\n");
    exit(-1);
  }

  if (argc > 1) {
    key = argv[1];
  }

  sleep(1);

  zn_write(s, key.c_str(), data.c_str(), data.length());
  zn_close(s);
}
