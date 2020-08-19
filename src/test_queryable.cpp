#include <unistd.h>
#include <string>

extern "C" {
  #include "zenoh/zenoh-ffi.h"
}

std::string key_expr("/test_query");
std::string value("Hello from C++");

void replier(ZNQuery *query) {
    const zn_string *res = zn_query_res_name(query);
    const zn_string *pred = zn_query_predicate(query);

    printf("Received query: %.*s:%.*s\n", res->len, res->val, pred->len, pred->val);

    zn_send_reply(query, key_expr.c_str(), (const unsigned char *)value.c_str(), value.length());
}

int main(int argc, char** argv) {
    ZNQueryable *q = 0;
    ZNSession *s = zn_open(PEER, 0, 0);

    if (s == 0) {
        printf("Error creating session!\n");
        exit(-1);
    }

    sleep(1);

    if (argc > 1) {
        key_expr = argv[1];
    }
    printf("Subscription expression to %s\n", key_expr.c_str());

    q = zn_declare_queryable(s, key_expr.c_str(), EVAL, replier);
    if (q == 0) {
        printf("Unable to register queryable\n");
        return -1;
    }
    char ch;
    printf("Press a key to terminate...\n");
    scanf("%c", &ch);

    zn_undeclare_queryable(q);
}
