load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "db",
    srcs = glob(
        [
            "**/*.cpp",
            "**/*.h",
        ],
    ),
    # copts = ["-I."],
    deps = [
        "//14_database/parser:db_lexer_parser",
        "@antlr4_runtimes//:cpp",
    ],
)
