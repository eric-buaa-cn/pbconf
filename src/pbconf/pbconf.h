#ifndef PBCONF_H
#define PBCONF_H

#include <google/protobuf/message.h>
#include <string>

namespace pbconf {

class PbConf final {
public:
    // We can set the filename explicitly.
    // If not set(_filename is empty), the default filename will be used.
    // The default filename has the following order:
    // application.yml(yaml format)
    //     > application.json(json format)
    //         > application.conf(hocon format)
    PbConf& SetFilename(const std::string& filename) {
        _filename = filename;
        return *this;
    }

    // Load conf into the specified ProtoBuf msg,
    // then, we can use conf value at ease.
    // Returns True if success; otherwise False.
    bool Load(::google::protobuf::Message& msg);

    std::string ErrorMessage() const {
        return _error_msg;
    }
private:
    std::string _filename;
    std::string _error_msg;
};

}

#endif
