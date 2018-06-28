#include "pbconf.h"

#include <algorithm>
#include <butil/file_util.h>
#include <butil/files/file_path.h>
#include <butil/strings/string_util.h>
#include <google/protobuf/message.h>
#include <string>
#include <vector>

#include "yaml_conf.h"
#include "json_conf.h"
#include "hocon_conf.h"

namespace pbconf {

bool PbConf::Load(::google::protobuf::Message& msg) {
    std::vector<std::string> ordered_filenames = {
        "conf/application.yml", "conf/application.json", "conf/application.conf"
    };

    auto file_exists = [](const std::string& filename) {
        return butil::PathExists(butil::FilePath(filename));
    };

    if (_filename.empty()) {
        auto best_choice = std::find_if(std::begin(ordered_filenames),
                std::end(ordered_filenames), file_exists);
        if (best_choice == std::end(ordered_filenames)) {
            return false;
        }
        _filename = *best_choice;
    }

    if (EndsWith(_filename, ".yml", true)) {
        return YamlConf().Load(_filename, msg, _error_msg);
    }
    if (EndsWith(_filename, ".json", true)) {
        return JsonConf().Load(_filename, msg, _error_msg);
    }
    if (EndsWith(_filename, ".conf", true)) {
        return HoconConf().Load(_filename, msg, _error_msg);
    }

    return false;
}

}
