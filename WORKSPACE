# workspace(name = "com_github_qikai_ita")

# load("//:play_with_compiler.bzl", "play_with_compiler_deps")

# play_with_compiler_deps()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# glog
git_repository(
    name = "glog",
    commit = "b33e3bad4c46c8a6345525fd822af355e5ef9446",  # release 0.6
    remote = "https://github.com/google/glog",
)

# bazel-skylib
http_archive(
    name = "bazel_skylib",
    sha256 = "b8a1527901774180afc798aeb28c4634bdccf19c4d98e7bdd1ce79d1fe9aaad7",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.4.1/bazel-skylib-1.4.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.4.1/bazel-skylib-1.4.1.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

# abseil-cpp
git_repository(
    name = "com_google_absl",
    commit = "c2435f8342c2d0ed8101cb43adfd605fdc52dca2",  # 2023.01.25.3
    remote = "https://github.com/abseil/abseil-cpp",
)

# # C++ rules for Bazel.
# This is for the known bug:
# https://github.com/deepmind/lab/issues/168
# https://github.com/deepmind/lab/commit/ae2d292f3ee08205058c0c604fd996b204450e20
http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-main",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/main.zip"],
)

# gflags
http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = [
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
)


# Google Test
http_archive(
    name = "com_google_googletest",
    sha256 = "ffa17fbc5953900994e2deec164bb8949879ea09b411e07f215bfbb1f87f4632",
    strip_prefix = "googletest-1.13.0",
    urls = ["https://github.com/google/googletest/archive/googletest-1.13.0.zip"],  # 2023
)

# # abseil-cpp
# http_archive(
#   name = "com_google_absl",
#   urls = ["https://github.com/abseil/abseil-cpp/archive/7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72.zip"],  # 2019-03-14
#   strip_prefix = "abseil-cpp-7c7754fb3ed9ffb57d35fe8658f3ba4d73a31e72",
#   sha256 = "71d00d15fe6370220b6685552fb66e5814f4dd2e130f3836fc084c894943753f",
# )

# # C++ rules for Bazel.
# http_archive(
#     name = "rules_cc",
#     sha256 = "67412176974bfce3f4cf8bdaff39784a72ed709fc58def599d1f68710b58d68b",
#     strip_prefix = "rules_cc-b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e",
#     urls = [
#         "https://mirror.bazel.build/github.com/bazelbuild/rules_cc/archive/b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e.zip",
#         "https://github.com/bazelbuild/rules_cc/archive/b7fe9697c0c76ab2fd431a891dbb9a6a32ed7c3e.zip",
#     ],
# )

# antlr4
http_archive(
    name = "rules_antlr",
    sha256 = "26e6a83c665cf6c1093b628b3a749071322f0f70305d12ede30909695ed85591",
    strip_prefix = "rules_antlr-0.5.0",
    urls = ["https://github.com/marcohu/rules_antlr/archive/0.5.0.tar.gz"],
)

load("@rules_antlr//antlr:repositories.bzl", "rules_antlr_dependencies")
load("@rules_antlr//antlr:lang.bzl", "CPP")

rules_antlr_dependencies("4.7.2", CPP)
