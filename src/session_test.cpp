#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

int main(int argc, char** argv) {
  ZNSession *s = zn_open(CLIENT, "0", 0);
  if (s == 0) {
    printf("Error creating session!\n");
    exit(-1);
  }
  sleep(1);
  const char *data = "Hello from C++";
  const char *key = "/zenoh/test";
  zn_write(s, key, data, strlen(data));
  zn_close(s);
}
