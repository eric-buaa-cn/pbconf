#include <iostream>
#include <pbconf/pbconf.h>

#include "demo.pb.h"

int main() {
    demo::ConfMessage msg;
    if (!pbconf::PbConf().Load(msg)) {
        std::cerr << "Fail load conf" << std::endl;
        return -1;
    }

    std::cout << msg.nthread() << std::endl;
    return 0;
}
