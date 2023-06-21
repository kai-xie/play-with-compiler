load("@rules_cc//cc:defs.bzl", "cc_binary")

# cc_binary(
#     name = "bin",
#     srcs = ["main.cpp"],
#     deps = ["//playscript-cpp/grammar:lexer_parser"],
# )

cc_library(
    name = "play_utils",
    srcs = ["play_utils.cpp"],
    hdrs = [
        "play_flags.h",
        "play_utils.h",
    ],
    # copts = [],
    # includes = ["src"],
    deps = ["@com_google_absl//absl/types:optional"],
)

cc_binary(
    name = "PlayScript",
    srcs = ["PlayScript.cpp"],
    # includes = ["./"],
    # hdrs = glob(["**/*.h"]),
    # includes = ["./"],
    copts = ["-I."],
    deps = [
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@glog",
        # "@antlr-cpp-runtime//runtime",
        "@antlr4_runtimes//:cpp",
        "//06_13_playscript-cpp/grammar:lexer_parser",
        # "ASTEvaluator",
        "play_utils",
        "compiler",
    ],
)

# cc_library(
#     name = "ASTEvaluator",
#     srcs = [
#         "grammer/ASTEvaluator.cpp",
#         # ":antlr4_playscript_utils",
#     ],
#     hdrs = ["grammer/ASTEvaluator.h"],
#     visibility = ["//visibility:public"],
#     deps = [
#         ":antlr4_playscript_utils",
#         "@antlr4_runtimes//:cpp",
#     ],
#     strip_include_prefix = "grammer",
# )

cc_library(
    name = "compiler",
    srcs = glob(
        ["**/*.cpp"],
        [
            "PlayScript.cpp",
            "play_utils.cpp",
        ],
    ),
    # data = [],
    hdrs = glob(
        ["**/*.h"],
        [
            "play_flags.h",
            "play_utils.h",
        ],
    ),
    # alwayslink = False,
    # copts = [""],
    # defines = [""],
    # implementation_deps = [],
    # include_prefix = "",
    # includes = ["grammar"],
    deps = ["//06_13_playscript-cpp/grammar:lexer_parser"],
    # linkopts = [""],
    # linkstamp = "",
    # linkstatic = False,
    # local_defines = [""],
    # nocopts = "",
    # strip_include_prefix = "",
    # tags = [""],
    # textual_hdrs = [],
    # toolchains = [],
    # visibility = [""],
    # win_def_file = "",
)
