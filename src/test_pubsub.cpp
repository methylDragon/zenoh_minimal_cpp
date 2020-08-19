#include <unistd.h>

#include <mutex>
#include <unordered_map>
#include <cstring>
#include <string>
#include <chrono>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::mutex sub_callback_mutex;
std::unordered_map<std::string, size_t> topic_map;
std::string key_expr("/test");

void sub_callback(const zn_sample * sample) {
    std::lock_guard<std::mutex> guard(sub_callback_mutex);

    printf(">>> Received %d bytes on %.*s: '%.*s'",
           sample->value.len,
           sample->key.len,
           sample->key.val,
           sample->value.len,
           sample->value.val);
    if (strncmp(strrchr((char *)sample->value.val, ' ') + 1,
                sample->key.val,
                sample->key.len) != 0) {
      printf("\t<---- Mismatched topics\n");
    } else {
      printf("\n");
    }
}

int main(int argc, char** argv) {
    ZNSession *s = zn_open(PEER, 0, 0);

    if (s == 0) {
        printf("Error creating session!\n");
        exit(-1);
    }

    if (argc == 1) {
      printf("Usage: ./test_pubsub PREFIX [any number of Zenoh topics to publish to]\n");
      printf("\nNotes:");
      printf("\nDefaults to publishing to /test if no topics are specified\n");
      printf("\nAlso, do NOT specify spaces in the topic names!\n");
      exit(-1);
    }

    sleep(1);

    std::string prefix(argv[1]);
    if (argc > 2)
    {
      for (int i = 2; i < argc; i++) {
        key_expr = argv[i];
        topic_map[key_expr] = zn_declare_resource(s, key_expr.c_str());
        zn_declare_subscriber(s, key_expr.c_str(), zn_subinfo_default(), sub_callback);

        printf("Subscription expression to %s (%ld)\n", key_expr.c_str(), topic_map[std::string(key_expr)]);
      }
    } else {
      topic_map[key_expr] = zn_declare_resource(s, key_expr.c_str());
      zn_declare_subscriber(s, key_expr.c_str(), zn_subinfo_default(), sub_callback);

      printf("Subscription expression to %s (%ld)\n", key_expr.c_str(), topic_map[std::string(key_expr)]);
    }
    printf("\n");

    for (auto i = 0; i < 3; ++i) {
      for (std::pair<std::string, size_t> element : topic_map) {
        std::string msg = "Message #" + std::to_string(i) + " from " + prefix + " to topic " + element.first;

        zn_write_wrid(s, element.second, msg.c_str(), strlen(msg.c_str()));
        printf("<<< Published %ld bytes to %s: '%s'\n", msg.length(), element.first.c_str(), msg.c_str());
      }
      sleep(3);
      printf("\n");
    }
}
