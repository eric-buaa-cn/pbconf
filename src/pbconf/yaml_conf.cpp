#include "yaml_conf.h"

#include <yaml-cpp/yaml.h>

namespace pbconf {

static bool OnMap();
static bool OnSequence();
static bool OnScalar();

bool YamlConf::Load(
        const std::string& filename,
        ::google::protobuf::Message& msg,
        std::string& err_msg) {
    try {
        YAML::Node root = YAML::LoadFile(filename);
        return true;
    } catch (YAML::ParserException e) {
        err_msg = e.what();
        return false;
    } catch (YAML::BadFile e) {
        err_msg = e.what();
        return false;
    } catch (...) {
        err_msg = "Unknown error";
        return false;
    }
}

}
