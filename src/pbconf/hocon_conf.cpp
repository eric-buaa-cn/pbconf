#include "hocon_conf.h"

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
#include <limits>
#include <memory>
#include <iostream>
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
