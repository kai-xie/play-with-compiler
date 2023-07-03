load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "PlayScript",
    srcs = glob(["**/*.cpp", "**/*.h"]),
    deps = [
        "//antlrtest/grammar:lexer_parser",
        # "//runtime:myruntime",
    ],
)
