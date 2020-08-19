#include <unistd.h>
#include <string>
#include <mutex>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::mutex sub_callback_mutex;
std::string key_expr("/test");

void sub_callback(const zn_sample * sample) {
    std::lock_guard<std::mutex> guard(sub_callback_mutex);

    printf(">>> Received %d bytes on %.*s: '%.*s'\n( ",
           sample->value.len,
           sample->key.len,
           sample->key.val,
           sample->value.len,
           sample->value.val);
    for (unsigned int i = 0; i < sample->value.len; i++) {
      printf("%02x ", *(sample->value.val + i));
    }
    printf(")\n");
}

int main(int argc, char** argv) {
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
        zn_declare_subscriber(s, key_expr.c_str(), zn_subinfo_default(), sub_callback);
        printf("Subscription expression to %s\n", key_expr.c_str());
      }
    } else {
      zn_declare_subscriber(s, key_expr.c_str(), zn_subinfo_default(), sub_callback);
      printf("Subscription expression to %s\n", key_expr.c_str());
    }

    char ch;
    read(0, &ch, 1);
}
