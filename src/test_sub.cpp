#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mutex>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::mutex sub_callback_mutex;

void sub_callback(const zn_sample * sample) {
    std::lock_guard<std::mutex> guard(sub_callback_mutex);

    printf("\n>> Received on: %.*s, %d\n( ",
           sample->key.len,
           sample->key.val,
           sample->value.len);

    for (int i = 0 ; i < sample->value.len; i++) {
      printf("%02x ",*(sample->value.val + i));
    }

    printf(")\n");
}

int main(int argc, char** argv) {
    char *key_expr = "/test";

    ZNSession *s = zn_open(PEER, 0, 0);

    if (s == 0) {
        printf("Error creating session!\n");
        exit(-1);
    }

    sleep(1);

    if (argc > 1)
    {
      for (int i = 1; i < argc; i++) {
        key_expr = argv[i];
        zn_declare_subscriber(s, key_expr, zn_subinfo_default(), sub_callback);
        printf("Subscription expression to %s\n", key_expr);
      }
    } else {
      zn_declare_subscriber(s, key_expr, zn_subinfo_default(), sub_callback);
      printf("Subscription expression to %s\n", key_expr);
    }

    // int count = 0;
    // while (count < 10)
    // {
    //   count++;
    //   printf("%d", count);
    //   sleep(1);
    // }

    char ch;
    read(0, &ch, 1);
}
