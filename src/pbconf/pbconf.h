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
    // application.yaml > application.json > application.hocon
    // > application.properties
    PbConf& SetFilename(const std::string& filename) {
        _filename = filename;
        return *this;
    }

    // Load conf into the specified ProtoBuf msg,
    // then, we can use conf value at ease.
    // Returns True if success; otherwise False.
    bool Load(::google::protobuf::Message& msg);

    std::string error_message() const {
        return _error_msg;
    }
private:
    std::string _filename;
    std::string _error_msg;
};

}

#endif
