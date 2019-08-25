#ifndef MACROS_H_
#define MACROS_H_

// namespace {
// #define DECLARE_ENUM(enum_name, opcode_name) enum_name,                         
// // }


// #define DECLARE_ENUM_CLASS(enum_list, enum_class_name)  \
//   enum class enum_class_name {                          \
//     enum_list(DECLARE_ENUM)                             \
//   };                                                                            
// }

  // std::string enum_class_name##ToString(enum_class_name enum_type) {            \
  //   switch (enum_type) {                                                        \
  //     enum_list(CASE_ENUM_TYPE_STRING)                                          \
  //   }                                                                           \
  // }                                                                             



// #define ENUM_CLASS_WITH_STRING(enum_list, enum_class_name)                      \
//   enum class enum_class_name {                                                  \
// #define DECLARE_ENUM(enum_name, opcode_name) enum_name,                         \
//     enum_list(DECLARE_ENUM)                                                     \
// #undef DECLARE_ENUM                                                             \
//   };                                                                            
//   std::string enum_class_name##ToString(enum_class_name enum_type) {            \
//     switch (enum_type) {                                                        \
// #define CASE_ENUM_TYPE_STRING(enum_name, enum_name_string)                      \
//     case enum_class_name::enum_name:                                            \
//       return enum_name_string;                                                  \
//       enum_list(CASE_ENUM_TYPE_STRING)                                          \
// #undef CASE_ENUM_TYPE_STRING                                                    \
//     }                                                                           \
//   }                                                                             \
// inline std::ostream& operator<<(std::ostream& os, enum_class_name enum_type) {  \
//   return os << enum_class_name##ToString(enum_type);                            \
// }


// enum class TokenType {
// #define DECLARE_ENUM(enum_name, opcode_name) enum_name,
//   TOKEN_TYPE_LIST(DECLARE_ENUM)
// #undef DECLARE_ENUM
// };

#endif // MACROS_H_
