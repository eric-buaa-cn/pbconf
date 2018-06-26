#ifndef JSON_CONF_H
#define JSON_CONF_H

namespace pbconf {

class JsonConf final {
public:
    bool Load(
            const std::string& filename,
            ::google::protobuf::Message& msg,
            std::string& err_msg) {
        return true;
    }
};

}

#endif
