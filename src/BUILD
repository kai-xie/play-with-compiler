load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

cc_library(
    name = "token_type",
    srcs = ["token_type.cc"],
    hdrs = ["token_type.h"],
)

cc_library(
    name = "token_base",
    srcs = ["token_base.h"],
    hdrs = ["token_base.h"],
)

cc_library(
    name = "token_reader_base",
    srcs = ["token_reader_base.h"],
    hdrs = ["token_reader_base.h"],
)

cc_library(
    name = "macros",
    srcs = ["macros.h"],
    hdrs = ["macros.h"],
)

cc_library(
    name = "simple_token_reader",
    srcs = ["simple_token_reader.h"],
    hdrs = ["simple_token_reader.h"],
    deps = [":token_reader_base"],
)

cc_library(
    name = "simple_lexer",
    srcs = ["simple_lexer.cc"],
    hdrs = ["simple_lexer.h"],
    copts = [
        "-std=c++14",
    ],
    deps = [
        ":simple_token_reader",
        ":token_base",
        ":token_type",
    ],
)

cc_binary(
    name = "simple_lexer_test",
    srcs = ["simple_lexer_test.cc"],
    copts = [
        "-std=c++14",
    ],
    deps = [
        ":simple_lexer",
    ],
)

cc_library(
    name = "ast_node_type",
    srcs = ["ast_node_type.cc"],
    hdrs = ["ast_node_type.h"],
    # deps = [
    #     "macros",
    # ],
)

cc_library(
    name = "ast_node_base",
    srcs = ["ast_node_base.h"],
    hdrs = ["ast_node_base.h"],
    deps = ["ast_node_type"],
)

# # https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
# cc_library(
#     name = "util",
#     srcs = ["util.h"],
#     hdrs = ["util.h"],
#     # deps = ["header files"],
# )

cc_library(
    name = "simple_ast",
    srcs = ["simple_ast.cc"],
    hdrs = ["simple_ast.h"],
    copts = [
        "-std=c++17",
    ],
    deps = [
        ":ast_node_base",
        # ":util",
        "@com_google_absl//absl/memory",
        "@glog",
    ],
)

cc_library(
    name = "simple_parser",
    srcs = ["simple_parser.cc"],
    hdrs = ["simple_parser.h"],
    copts = ["-std=c++14"],
    deps = [
        "ast_node_base",
        "ast_node_type",
        "map_util",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/memory",
        "@glog",
        "token_type",
        "token_reader_base",
        "simple_ast",
        "token_base",
        "simple_lexer",
    ],
)

cc_binary(
    name = "simple_parser_test",
    srcs = ["simple_parser_test.cc"],
    copts = [
        "-std=c++14",
    ],
    deps = [
        ":simple_parser",
    ],
)

cc_library(
    name = "map_util",
    srcs = ["map_util.h"],
    hdrs = [
        "map_util.h",
    ],
    # visibility = ["//visibility:public"],
    deps = [
        "@com_google_absl//absl/algorithm:container",
        "@com_google_absl//absl/container:flat_hash_map",
    ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "test",
    srcs = ["test.cc"],
    copts = [
        "-std=c++17",
    ],
    deps = [
        "@glog",
        "@com_google_absl//absl/memory",
    ],
)
