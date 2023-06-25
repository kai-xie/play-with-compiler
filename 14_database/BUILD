load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "db",
    srcs = glob(
        ["**/*.cpp", "**/*.h"],

    ),
    # copts = ["-I."],
    deps = [
        # "@com_google_absl//absl/flags:parse",
        # "@com_google_absl//absl/strings",
        # "@com_google_absl//absl/types:optional",
        # "@glog",
        "@antlr4_runtimes//:cpp",
        "//14_database/parser:db_lexer_parser",
    ],
)

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
    deps = ["//14_database/parser:db_lexer_parser"],
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
