#include "hocon_conf.h"

#include <algorithm>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/lexical_cast.hpp>
#include <butil/strings/stringprintf.h>
#include <google/protobuf/message.h>
#include <string>
#include <hocon/config.hpp>
#include <hocon/config_list.hpp>
#include <hocon/config_object.hpp>
#include <hocon/config_parse_options.hpp>
#include <hocon/config_syntax.hpp>
#include <hocon/config_value.hpp>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
//#include <internal/values/config_int.hpp>

namespace pbconf {

using Descriptor = ::google::protobuf::Descriptor;
using EnumValueDescriptor = ::google::protobuf::EnumValueDescriptor;
using FieldDescriptor = ::google::protobuf::FieldDescriptor;
using Message = ::google::protobuf::Message;
using Reflection = ::google::protobuf::Reflection;
using string = std::string;

using shared_object = ::hocon::shared_object;
using shared_value = ::hocon::shared_value;

static bool IsMap(shared_value node) {
    return node->value_type() == ::hocon::config_value::type::OBJECT;
}

static bool IsNull(shared_value node) {
    return node->value_type() == ::hocon::config_value::type::CONFIG_NULL;
}

template <typename T>
static bool OnNodeFor(
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return false;
}

template <typename T>
static bool OnNodeForSingle(
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return false;
}

template <typename T>
static bool OnNodeForRepeated(
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    return false;
}

template <typename T>
static inline bool get(shared_value node, T& value) {
    return false;
}

// Begin int32_t
template <>
inline bool get<int32_t>(shared_value node, int32_t& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::NUMBER) {
        return false;
    }
    string literal = node->transform_to_string();

    try {
        value = boost::lexical_cast<int32_t>(literal);
    } catch (boost::bad_lexical_cast& e) {
        return false;
    }
    return true;
}

template <>
inline bool OnNodeForSingle<int32_t>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    int32_t value{0};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddInt32(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<int32_t>(
        shared_value node,
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

// Begin int64_t
template <>
inline bool get<int64_t>(shared_value node, int64_t& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::NUMBER) {
        return false;
    }
    string literal = node->transform_to_string();

    try {
        value = boost::lexical_cast<int64_t>(literal);
    } catch (boost::bad_lexical_cast& e) {
        return false;
    }
    return true;
}

template <>
inline bool OnNodeForSingle<int64_t>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    int64_t value{0};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddInt64(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<int64_t>(
        shared_value node,
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

// Begin uint32_t
template <>
inline bool get<uint32_t>(shared_value node, uint32_t& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::NUMBER) {
        return false;
    }
    string literal = node->transform_to_string();

    try {
        value = boost::lexical_cast<uint32_t>(literal);
    } catch (boost::bad_lexical_cast& e) {
        return false;
    }
    return true;
}

template <>
inline bool OnNodeForSingle<uint32_t>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    uint32_t value{0};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddUInt32(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<uint32_t>(
        shared_value node,
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

// Begin uint64_t
template <>
inline bool get<uint64_t>(shared_value node, uint64_t& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::NUMBER) {
        return false;
    }
    string literal = node->transform_to_string();

    try {
        value = boost::lexical_cast<uint64_t>(literal);
    } catch (boost::bad_lexical_cast& e) {
        return false;
    }
    return true;
}

template <>
inline bool OnNodeForSingle<uint64_t>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    uint64_t value{0};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddUInt64(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<uint64_t>(
        shared_value node,
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
inline bool get<bool>(shared_value node, bool& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::BOOLEAN) {
        return false;
    }

    string literal = node->transform_to_string();
    if (literal.compare("true") == 0) {
        value = true;
        return true;
    }
    if (literal.compare("false") == 0) {
        value = false;
        return true;
    }

    return false;
}

template <>
inline bool OnNodeForSingle<bool>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    bool value{false};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddBool(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<bool>(
        shared_value node,
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
inline bool get<float>(shared_value node, float& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::NUMBER) {
        return false;
    }
    string literal = node->transform_to_string();

    try {
        value = boost::lexical_cast<float>(literal);
    } catch (boost::bad_lexical_cast& e) {
        return false;
    }
    return true;
}

template <>
inline bool OnNodeForSingle<float>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    float value{0.};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddFloat(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<float>(
        shared_value node,
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
inline bool get<double>(shared_value node, double& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::NUMBER) {
        return false;
    }
    string literal = node->transform_to_string();

    try {
        value = boost::lexical_cast<double>(literal);
    } catch (boost::bad_lexical_cast& e) {
        return false;
    }
    return true;
}

template <>
inline bool OnNodeForSingle<double>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    double value{0.};
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddDouble(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<double>(
        shared_value node,
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
inline bool get<string>(shared_value node, string& value) {
    if (!node || node->value_type() != ::hocon::config_value::type::STRING) {
        return false;
    }
    value = node->transform_to_string();
    return true;
}

template <>
inline bool OnNodeForSingle<string>(
        shared_value node,
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
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    if (!node || node->value_type() != ::hocon::config_value::type::LIST) {
        butil::StringAppendF(&err_msg, "Wrong type");
        return false;
    }

    auto real_node = std::static_pointer_cast<const ::hocon::config_list>(node);
    const Reflection* reflection = parent_msg.GetReflection();

    string value;
    for (auto citr = real_node->begin(); citr != real_node->end(); ++citr) {
        if (!get(*citr, value)) {
            return false;
        }
        reflection->AddString(&parent_msg, field, value);
    }
    return true;
}

template <>
inline bool OnNodeFor<string>(
        shared_value node,
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

static bool OnNode(
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg);

static bool OnMap(shared_object node, Message& msg, string& err_msg) {
    if (!node || !IsMap(node)) {
        butil::StringAppendF(&err_msg, "Expect an map/object");
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
        auto field_node = (*node)[field->name()];
        if (!OnNode(field_node, field, msg, err_msg)) {
            return false;
        }
	}

    return true;
}

static inline bool OnRootNode(shared_object node, Message& msg, string& err_msg) {
    // Root node is an object (which is a map)
    return OnMap(node, msg, err_msg);
}

static bool OnNode(
        shared_value node,
        const FieldDescriptor* field,
        Message& parent_msg,
        string& err_msg) {
    // Missing the required field
    if (field->is_required() && (!node || IsNull(node))) {
        butil::StringAppendF(&err_msg, "Field is required:%s",
                field->full_name().c_str());
        return false;
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT32) {
        return OnNodeFor<int32_t>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_INT64) {
        return OnNodeFor<int64_t>(node, field, parent_msg, err_msg);
    }
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_UINT32) {
        return OnNodeFor<uint32_t>(node, field, parent_msg, err_msg);
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
    if (field->cpp_type() == FieldDescriptor::CPPTYPE_STRING) {
        return OnNodeFor<string>(node, field, parent_msg, err_msg);
    }
    return true;
}

bool HoconConf::Load(const string& filename, Message& msg, string& err_msg) {
    hocon::config_parse_options option;
    option.set_syntax(config_syntax::CONF);
    option.set_allow_missing(true);

    try {
        hocon::shared_config conf =
            hocon::config::parse_file_any_syntax(filename, option);
        shared_object root = conf->root();
        return OnRootNode(root, msg, err_msg);
    } catch (...) {
        err_msg = boost::current_exception_diagnostic_information();
        return false;
    }
}

}
