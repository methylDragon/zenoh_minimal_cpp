#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <mutex>
#include <unordered_map>
#include <string>
#include <chrono>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::mutex sub_callback_mutex;
std::unordered_map<std::string, size_t> topic_map;

void sub_callback(const zn_sample * sample) {
    std::lock_guard<std::mutex> guard(sub_callback_mutex);

    printf(">>> Received %d bytes on %.*s: '%.*s'",
           sample->value.len,
           sample->key.len,
           sample->key.val,
           sample->value.len,
           sample->value.val);
    if (strncmp((char*)sample->value.val + sample->value.len - 5,
                sample->key.val + sample->key.len - 5,
                5) != 0) {
      printf("\t<---- Mismatched topics\n");
    } else {
      printf("\n");
    }
}

int main(int argc, char** argv) {
    char *key_expr = "/test";

    ZNSession *s = zn_open(PEER, 0, 0);

    if (s == 0) {
        printf("Error creating session!\n");
        exit(-1);
    }

    sleep(1);

    std::string whoami(argv[1]);
    if (argc > 2)
    {
      for (int i = 2; i < argc; i++) {
        key_expr = argv[i];
        topic_map[std::string(key_expr)] = zn_declare_resource(s, key_expr);
        zn_declare_subscriber(s, key_expr, zn_subinfo_default(), sub_callback);

        printf("Subscription expression to %s (%ld)\n", key_expr, topic_map[std::string(key_expr)]);
      }
    } else {
      topic_map[std::string(key_expr)] = zn_declare_resource(s, key_expr);
      zn_declare_subscriber(s, key_expr, zn_subinfo_default(), sub_callback);

      printf("Subscription expression to %s (%ld)\n", key_expr, topic_map[std::string(key_expr)]);
    }
    printf("\n");

    for (auto i = 0; i < 3; ++i) {
      for (std::pair<std::string, size_t> element : topic_map) {
        std::string msg = "Message #" + std::to_string(i) + " from " + whoami + " to topic " + element.first;

        zn_write_wrid(s, element.second, msg.c_str(), strlen(msg.c_str()));
        printf("<<< Published %ld bytes to %s: '%s'\n", msg.length(), element.first.c_str(), msg.c_str());
      }
      sleep(3);
      printf("\n");
    }

    // char ch;
    // read(0, &ch, 1);
}
