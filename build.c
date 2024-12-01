#define COSTRUISCI_IMPL
#include "costruisci.h"

int main(int argc, char **argv) {
        struct cos_cmd cmd = {0};

        cos_cmd_append(&cmd, "gcc", "-o", "hello", "src/hello.c", NULL);

        cos_cmd_run(&cmd);
        return 0;
}
