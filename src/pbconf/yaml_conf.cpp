#include "yaml_conf.h"

#include <boost/exception/diagnostic_information.hpp> 
#include <butil/strings/stringprintf.h>
#include <google/protobuf/message.h>
#include <string>
#include <yaml-cpp/yaml.h>

namespace pbconf {

using Descriptor = ::google::protobuf::Descriptor;
using EnumValueDescriptor = ::google::protobuf::EnumValueDescriptor;
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
        if (!get(*citr, value)) {
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
        if (!get(*citr, value)) {
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
        if (!get(*citr, value)) {
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
        if (!get(*citr, value)) {
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
        if (!get(*citr, value)) {
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

// Begin float
template <>
inline bool get<float>(const Node& node, float& value) {
    try {
        value = node.as<float>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<float>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    float value{0.};
    if (!get(node, value)) {
        butil::StringAppendF(&err_msg, "Expect float value at:%s",
                field->full_name().c_str());
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetFloat(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<float>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    float value{0.};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddFloat(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<float>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<float>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<float>(node, field, parent_msg, err_msg);
    }
}
// End float

// Begin double
template <>
inline bool get<double>(const Node& node, double& value) {
    try {
        value = node.as<double>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<double>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    double value{0.};
    if (!get(node, value)) {
        butil::StringAppendF(&err_msg, "Expect double value at:%s",
                field->full_name().c_str());
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetDouble(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<double>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    double value{0.};
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddDouble(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<double>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<double>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<double>(node, field, parent_msg, err_msg);
    }
}
// End double

// Begin string
template <>
inline bool get<string>(const Node& node, string& value) {
    try {
        value = node.as<string>();
        return true;
    } catch (...) {
        return false;
    }
}

template <>
inline bool OnNodeForSingle<string>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    string value;
    if (!get(node, value)) {
        butil::StringAppendF(&err_msg, "Expect double value at:%s",
                field->full_name().c_str());
        return false;
    }
    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetString(&parent_msg, field, value);
    return true;
}

template <>
inline bool OnNodeForRepeated<string>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    string value;
    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddString(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<string>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    if (field->is_repeated()) {
        return OnNodeForRepeated<string>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<string>(node, field, parent_msg, err_msg);
    }
}
// End string

// Begin enum
enum DummyEnum {};

template <>
inline bool OnNodeForSingle<enum DummyEnum>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const EnumValueDescriptor* enumd = nullptr;

    int32_t value{0};
    if (get<int32_t>(node, value)) {
        enumd = field->enum_type()->FindValueByNumber(value);
    }

    std::string literal;
    if (!enumd && get<std::string>(node, literal)) {
        enumd = field->enum_type()->FindValueByName(literal);
    }

    if (!enumd) {
        butil::StringAppendF(&err_msg, "Expect enum value at:%s",
                field->full_name().c_str());
        return false;
    }

    const Reflection* reflection = parent_msg.GetReflection();
    reflection->SetEnum(&parent_msg, field, enumd);
    return true;
}

template <>
inline bool OnNodeForRepeated<enum DummyEnum>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        const EnumValueDescriptor* enumd = nullptr;

        int32_t value{0};
        if (get<int32_t>(*citr, value)) {
            enumd = field->enum_type()->FindValueByNumber(value);
        }

        std::string literal;
        if (!enumd && get<std::string>(*citr, literal)) {
            enumd = field->enum_type()->FindValueByName(literal);
        }

        if (!enumd) {
            butil::StringAppendF(&err_msg, "Expect enum value at:%s",
                    field->full_name().c_str());
            return false;
        }

        const Reflection* reflection = parent_msg.GetReflection();
        reflection->AddEnum(&parent_msg, field, enumd);
    }
    return true;
}

template <>
inline bool OnNodeFor<enum DummyEnum>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (field->is_repeated()) {
        return OnNodeForRepeated<enum DummyEnum>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<enum DummyEnum>(node, field, parent_msg, err_msg);
    }
}
// End enum

// Begin message
class DummyClass {};

template <>
inline bool OnNodeForSingle<DummyClass>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    Message& child_msg = *(reflection->MutableMessage(&parent_msg, field));
    return OnMap(node, child_msg, err_msg);
}

template <>
inline bool OnNodeForRepeated<DummyClass>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    const Reflection* reflection = parent_msg.GetReflection();

    for (auto citr = node.begin(); citr != node.end(); ++citr) {
        Message& child_msg = *(reflection->AddMessage(&parent_msg, field));
        if (!OnMap(*citr, child_msg, err_msg)) {
            return false;
        }
    }
    return true;
}

template <>
inline bool OnNodeFor<DummyClass>(
        const Node& node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (field->is_repeated()) {
        return OnNodeForRepeated<DummyClass>(node, field, parent_msg, err_msg);
    } else {
        return OnNodeForSingle<DummyClass>(node, field, parent_msg, err_msg);
    }
}
// End message

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
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_FLOAT) {
        return OnNodeFor<float>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_DOUBLE) {
        return OnNodeFor<double>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_ENUM) {
        return OnNodeFor<enum DummyEnum>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_STRING) {
        return OnNodeFor<string>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE) {
        return OnNodeFor<DummyClass>(node, field, parent_msg, err_msg);
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
