#ifndef YAML_CONF_H
#define YAML_CONF_H

namespace pbconf {

class YamlConf final {
public:
    // Treat the specified file named `filename'
    // as a yaml-formatted conf file.
    // Load the conf info into msg.
    // Returns True if success; otherwise False.
    bool Load(
            const std::string& filename,
            ::google::protobuf::Message& msg,
            std::string& err_msg);
};

}

#endif
