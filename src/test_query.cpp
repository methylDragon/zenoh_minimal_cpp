#include <unistd.h>
#include <string>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::string key_expr("/test");
std::string predicate("");


void query_callback(const zn_source_info *info, const zn_sample *sample) {
    printf(">> Received:\n\t (%.*s, %.*s)\n",
        sample->key.len, sample->key.val,
        sample->value.len, sample->value.val);
}

int main(int argc, char** argv) {
    ZNSubscriber *sub = 0;

    if (argc > 1) {
        key_expr = argv[1];
    }
    if (argc > 2) {
        predicate = argv[2];
    }
    printf("Query expression to %s:%s\n", key_expr.c_str(), predicate.c_str());

    ZNSession *s = zn_open(PEER, 0, 0);

    if (s == 0) {
        printf("Error creating session!\n");
        exit(-1);
    }

    zn_query(s,
             key_expr.c_str(),
             predicate.c_str(),
             zn_query_target_default(),
             zn_query_consolidation_default(),
             query_callback);
    sleep(1);
}
