#ifndef HOCON_CONF_H
#define HOCON_CONF_H

namespace pbconf {

class HoconConf final {
public:
    bool Load(
            const std::string& filename,
            ::google::protobuf::Message& msg,
            std::string& err_msg) {
        err_msg = "Have no implementation yet";
        return false;
    }
};

}

#endif
