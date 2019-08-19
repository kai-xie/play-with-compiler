cc_library(
    name = "token_type",
    srcs = ["token_type.h"],
    hdrs = ["token_type.h"],
    # deps = ["header files"],
)

cc_library(
    name = "token_base",
    srcs = ["token_base.h"],
    hdrs = ["token_base.h"],
    # deps = ["header files"],
)

cc_library(
    name = "token_reader_base",
    srcs = ["token_reader_base.h"],
    hdrs = ["token_reader_base.h"],
    # deps = ["header files"],
)

cc_binary(
    name = "simple_lexer",
    srcs = ["simple_lexer.cc"],
    copts = [
        "-std=c++14",
    ],
    deps = [
        ":token_base",
        ":token_reader_base",
        ":token_type",
    ],
)
