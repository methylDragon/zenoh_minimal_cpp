#include <unistd.h>

#include <mutex>
#include <unordered_map>
#include <string>
#include <chrono>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::unordered_map<std::string, size_t> topic_map;
std::string key_expr("/test");

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
        topic_map[std::string(key_expr)] = zn_declare_resource(s, key_expr.c_str());
      }
    } else {
      topic_map[std::string(key_expr)] = zn_declare_resource(s, key_expr.c_str());
    }

    for (int i = 0; i < 3; i++)
    {
      for (std::pair<std::string, size_t> element : topic_map) {
        std::string msg = "hello world from " + element.first + " " + std::to_string(i);

        zn_write_wrid(s,
                      element.second,
                      msg.c_str(),
                      msg.length());
        printf("PUBLISHED TO %s (%ld): %s\n", element.first.c_str(), element.second, msg.c_str());
      }
      sleep(3);
    }
}
