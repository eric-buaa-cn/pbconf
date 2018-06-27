#include "yaml_conf.h"

#include <boost/exception/diagnostic_information.hpp> 
#include <butil/strings/stringprintf.h>
#include <google/protobuf/message.h>
#include <string>
#include <yaml-cpp/yaml.h>

namespace pbconf {

using Descriptor = ::google::protobuf::Descriptor;
using FieldDescriptor = ::google::protobuf::FieldDescriptor;
using Message = ::google::protobuf::Message;
using Node = YAML::Node;
using Reflection = ::google::protobuf::Reflection;
using string = std::string;

static bool OnNode(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg);

static bool OnMap(const Node& node, Message& msg, string& err_msg) {
    if (!node.IsMap()) {
        return false;
    }

    const Descriptor* descriptor = msg.GetDescriptor();
    const Reflection* reflection = msg.GetReflection();

    // Collect all field descriptors of msg,
    // including the normal and extended fields.
    std::vector<const FieldDescriptor*> fields;
    for (int i = 0; i < descriptor->field_count(); ++i) {
        fields.push_back(descriptor->field(i));
    }
    for (int i = 0; i < descriptor->extension_range_count(); ++i) {
        auto range = descriptor->extension_range(i);
        for (int tag = range->start; tag < range->end; ++tag) {
            auto field = reflection->FindKnownExtensionByNumber(tag);
            if (field) {
                fields.push_back(field);
            }
        }
    }

	// Convert each sub-node.
    for (auto field : fields) {
        auto& field_node = node[field->name()];
        if (!OnNode(field_node, field, msg, err_msg)) {
            return false;
        }
	}

    return true;
}

static inline bool OnRootNode(const Node& node, Message& msg, string& err_msg) {
    // Root node is a map
    return OnMap(node, msg, err_msg);
}

template <typename T>
static bool OnNodeFor(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return false;
}

template <typename T>
static bool OnNodeForSingle(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return false;
}

template <typename T>
static bool OnNodeForRepeated(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return false;
}

template <typename T>
static inline bool get(const Node& node, T& value) {
    return false;
}

// Begin int32_t
template <>
inline bool get<int32_t>(const Node& node, int32_t& value) {
    try {
        value = node.as<int32_t>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<int32_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    int32_t value{0};
    if (!get(node, value)) {
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetInt32(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<int32_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    int32_t value{0};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(node, value)) {
            return false;
        }
        reflection->AddInt32(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<int32_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<int32_t>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<int32_t>(node, field, parent_msg, err_msg);
    }
}
// End int32_t

// Begin uint32_t
template <>
inline bool get<uint32_t>(const Node& node, uint32_t& value) {
    try {
        value = node.as<uint32_t>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<uint32_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    uint32_t value{0};
    if (!get(node, value)) {
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetUInt32(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<uint32_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    uint32_t value{0};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(node, value)) {
            return false;
        }
        reflection->AddUInt32(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<uint32_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<uint32_t>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<uint32_t>(node, field, parent_msg, err_msg);
    }
}
// End uint32_t

// Begin int64_t
template <>
inline bool get<int64_t>(const Node& node, int64_t& value) {
    try {
        value = node.as<int64_t>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<int64_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    int64_t value{0};
    if (!get(node, value)) {
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetInt64(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<int64_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    int64_t value{0};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(node, value)) {
            return false;
        }
        reflection->AddInt64(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<int64_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<int64_t>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<int64_t>(node, field, parent_msg, err_msg);
    }
}
// End int64_t

// Begin uint64_t
template <>
inline bool get<uint64_t>(const Node& node, uint64_t& value) {
    try {
        value = node.as<uint64_t>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<uint64_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    uint64_t value{0};
    if (!get(node, value)) {
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetUInt64(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<uint64_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    uint64_t value{0};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(node, value)) {
            return false;
        }
        reflection->AddUInt64(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<uint64_t>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<uint64_t>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<uint64_t>(node, field, parent_msg, err_msg);
    }
}
// End uint64_t

// Begin bool
template <>
inline bool get<bool>(const Node& node, bool& value) {
    try {
        value = node.as<bool>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<bool>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    bool value{false};
    if (!get(node, value)) {
        butil::StringAppendF(&err_msg, "Expect boolean value at:%s",
                field->full_name().c_str());
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetBool(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<bool>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    bool value{false};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(node, value)) {
            return false;
        }
        reflection->AddBool(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<bool>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<bool>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<bool>(node, field, parent_msg, err_msg);
    }
}
// End bool

class DummyMessage {};

template <>
inline bool OnNodeFor<DummyMessage>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return true;
}

static bool OnNode(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    // Missing the required field
    if (field->is_required() && (!node || node.IsNull())) {
        butil::StringAppendF(&err_msg, "Field is required:%s",
                field->full_name().c_str());
        return false;
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT32) {
        return OnNodeFor<int32_t>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_UINT32) {
        return OnNodeFor<uint32_t>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT64) {
        return OnNodeFor<int64_t>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_UINT64) {
        return OnNodeFor<uint64_t>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_BOOL) {
        return OnNodeFor<bool>(node, field, parent_msg, err_msg);
    }

    return false;
}

bool YamlConf::Load(const string& filename, Message& msg, string& err_msg) {
    try {
        const Node root = YAML::LoadFile(filename);
        return OnRootNode(root, msg, err_msg);
    } catch (YAML::ParserException e) {
        err_msg = e.what();
        return false;
    } catch (YAML::BadFile e) {
        err_msg = e.what();
        return false;
    } catch (...) {
        err_msg = boost::current_exception_diagnostic_information();
        return false;
    }
}

}
