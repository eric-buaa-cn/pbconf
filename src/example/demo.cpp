#include <iostream>
#include <pbconf/pbconf.h>

#include "demo.pb.h"

int main() {
    demo::ConfMessage msg;
    pbconf::PbConf conf;
    if (!conf.SetFilename("conf/application.yml").Load(msg)) {
        std::cerr << "Fail load conf" << std::endl;
        std::cerr << conf.ErrorMessage() << std::endl;
        return -1;
    }

    std::cout << "i32=" << msg.i32() << std::endl;
    std::cout << "ui32=" << msg.ui32() << std::endl;
    std::cout << "i64=" << msg.i64() << std::endl;
    std::cout << "ui64=" << msg.ui64() << std::endl;
    return 0;
}
