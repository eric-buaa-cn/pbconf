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
    std::cout << "btrue=" << msg.btrue() << std::endl;
    std::cout << "bfalse=" << msg.bfalse() << std::endl;
    std::cout << "float=" << msg.f() << std::endl;
    std::cout << "double=" << msg.d() << std::endl;
    std::cout << "gender=" << demo::Gender_Name(msg.g()) << std::endl;
    std::cout << "string=" << msg.s() << std::endl;

    std::cout << "i32s:";
    for (int i = 0; i < msg.i32s_size(); ++i) {
        std::cout << " " << msg.i32s(i);
    }
    std::cout << std::endl;

    std::cout << "i64s:";
    for (int i = 0; i < msg.i64s_size(); ++i) {
        std::cout << " " << msg.i64s(i);
    }
    std::cout << std::endl;

    std::cout << "ui32s:";
    for (int i = 0; i < msg.ui32s_size(); ++i) {
        std::cout << " " << msg.ui32s(i);
    }
    std::cout << std::endl;

    std::cout << "ui64s:";
    for (int i = 0; i < msg.ui64s_size(); ++i) {
        std::cout << " " << msg.ui64s(i);
    }
    std::cout << std::endl;

    std::cout << "btrues:";
    for (int i = 0; i < msg.btrues_size(); ++i) {
        std::cout << " " << msg.btrues(i);
    }
    std::cout << std::endl;

    std::cout << "bfalses:";
    for (int i = 0; i < msg.bfalses_size(); ++i) {
        std::cout << " " << msg.bfalses(i);
    }
    std::cout << std::endl;

    std::cout << "floats:";
    for (int i = 0; i < msg.fs_size(); ++i) {
        std::cout << " " << msg.fs(i);
    }
    std::cout << std::endl;

    std::cout << "doubles:";
    for (int i = 0; i < msg.ds_size(); ++i) {
        std::cout << " " << msg.ds(i);
    }
    std::cout << std::endl;

    std::cout << "genders:";
    for (int i = 0; i < msg.gs_size(); ++i) {
        std::cout << " " << msg.gs(i);
    }
    std::cout << std::endl;

    std::cout << "strings:";
    for (int i = 0; i < msg.ss_size(); ++i) {
        std::cout << " " << msg.ss(i);
    }
    std::cout << std::endl;

    std::cout << "user: .name=" << msg.user().name()
        << " .age=" << msg.user().age() << std::endl;

    std::cout << "classmates:" << std::endl;
    for (int i = 0; i < msg.classmates_size(); ++i) {
        std::cout << " .name=" << msg.classmates(i).name()
            << " .age=" << msg.classmates(i).age() << std::endl;
    }
    return 0;
}
