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
)

cc_library(
    name = "ast_node_base",
    srcs = ["ast_node_base.h"],
    hdrs = ["ast_node_base.h"],
    deps = ["ast_node_type"],
)

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
    name = "simple_ast_node",
    srcs = ["simple_ast_node.h"],
    hdrs = ["simple_ast_node.h"],
    copts = ["-std=c++14"],
    deps = [
        "ast_node_base",
        # "@com_google_absl//absl/container:flat_hash_set",
    ],
)

cc_library(
    name = "simple_parser",
    srcs = ["simple_parser.cc"],
    hdrs = ["simple_parser.h"],
    copts = ["-std=c++14"],
    deps = [
        "map_util",
        "simple_ast",
        "simple_ast_node",
        "simple_lexer",
        "@com_google_absl//absl/container:flat_hash_set",
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
    name = "simple_script",
    srcs = ["simple_script.cc"],
    hdrs = ["simple_script.h"],
    # copts = ["-std=c++14"],
    deps = [
        "ast_node_base",
        "map_util",
        "simple_parser",
    ],
)

cc_binary(
    name = "simple_script_test",
    srcs = ["simple_script_test.cc"],
    copts = [
        "-std=c++14",
    ],
    deps = [
        "simple_script",
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
        "@glog",
    ],
)

cc_binary(
    name = "test",
    srcs = ["test.cc"],
    copts = [
        "-std=c++17",
    ],
    deps = [
        "@com_google_absl//absl/memory",
        "@glog",
    ],
)
