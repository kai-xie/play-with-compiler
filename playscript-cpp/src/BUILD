load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

# load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("@rules_antlr//antlr:antlr4.bzl", antlr4 = "antlr")

antlr4(
    name = "antlr4_playscript_utils",
    srcs = glob(["grammar/*.g4"]),
    language = "Cpp",
    package = "play",
    visitor = True,
)

# cc_library(
#     name = "lib",
#     srcs = [":antlr4_playscript_utils"],
#     visibility = ["//visibility:public"],
#     deps = [
#         ":antlr4_playscript_utils",
#         "@antlr4_runtimes//:cpp",
#     ],
# )

cc_library(
    name = "play_utils",
    srcs = ["play_utils.cpp"],
    hdrs = [
        "play_flags.h",
        "play_utils.h",
    ],
    copts = [],
    deps = ["@com_google_absl//absl/types:optional"],
)

cc_binary(
    name = "PlayScript",
    srcs = ["PlayScript.cpp"],
    # copts = ["-std=c++14"],
    deps = [
        "play_utils",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_github_google_glog//:glog",
        ":antlr4_playscript_utils",
        "@antlr4_runtimes//:cpp",
        # "ASTEvaluator",
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
