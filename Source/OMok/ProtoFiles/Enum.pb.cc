/*@ fixed */

#if defined(_MSC_VER)
  #pragma warning (disable:4018) // 'expression' : signed/unsigned mismatch
  #pragma warning (disable:4065) // switch statement contains 'default' but no 'case' labels
  #pragma warning (disable:4146) // unary minus operator applied to unsigned type, result still unsigned
  #pragma warning (disable:4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data
  #pragma warning (disable:4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
  #pragma warning (disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
  #pragma warning (disable:4305) // 'identifier' : truncation from 'type1' to 'type2'
  #pragma warning (disable:4307) // 'operator' : integral constant overflow
  #pragma warning (disable:4309) // 'conversion' : truncation of constant value
  #pragma warning (disable:4334) // 'operator' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
  #pragma warning (disable:4355) // 'this' : used in base member initializer list
  #pragma warning (disable:4506) // no definition for inline function 'function'
  #pragma warning (disable:4800) // 'type' : forcing value to bool 'true' or 'false' (performance warning)
  #pragma warning (disable:4996) // The compiler encountered a deprecated declaration.
  #pragma warning (disable:4125) // decimal digit terminates octal escape sequence
#endif
// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Enum.proto

#include "Enum.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
namespace Protocol {
}  // namespace Protocol
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_Enum_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_Enum_2eproto = nullptr;
const uint32_t TableStruct_Enum_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_Enum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nEnum.proto\022\010Protocol*Y\n\017BoardGameResul"
  "t\022\010\n\004NONE\020\000\022\017\n\013ERROR_RANGE\020\001\022\016\n\nERROR_HA"
  "VE\020\002\022\017\n\013NOT_HAPPEND\020\003\022\n\n\006WINNER\020\004b\006proto"
  "3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_Enum_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Enum_2eproto = {
  false, false, 121, descriptor_table_protodef_Enum_2eproto, "Enum.proto", 
  &descriptor_table_Enum_2eproto_once, nullptr, 0, 0,
  schemas, file_default_instances, TableStruct_Enum_2eproto::offsets,
  nullptr, file_level_enum_descriptors_Enum_2eproto, file_level_service_descriptors_Enum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable* descriptor_table_Enum_2eproto_getter() {
  return &descriptor_table_Enum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_Enum_2eproto(&descriptor_table_Enum_2eproto);
namespace Protocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* BoardGameResult_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[0];
}
bool BoardGameResult_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
