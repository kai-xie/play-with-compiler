# workspace(name = "com_github_qikai_ita")

# load("//:play_with_compiler.bzl", "play_with_compiler_deps")

# play_with_compiler_deps()


load('@bazel_tools//tools/build_defs/repo:git.bzl', 'git_repository', 'new_git_repository')
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# glog
git_repository(
  name = "glog",
  remote = "https://github.com/google/glog",
  commit = "ba8a9f6952d04d1403b97df24e6836227751454e",
)

# abseil-cpp
git_repository(
  name = "com_google_absl",
  remote = "https://github.com/abseil/abseil-cpp",
  commit = "b86fff162e15ad8ee534c25e58bf522330e8376d",
)    

# # C++ rules for Bazel.
# This is for the known bug:
# https://github.com/deepmind/lab/issues/168
# https://github.com/deepmind/lab/commit/ae2d292f3ee08205058c0c604fd996b204450e20
http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-master",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/master.zip"],
)

# gflags
http_archive(
    name = "com_github_gflags_gflags",
    urls = [
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
)

# http_archive(
#     name = "com_github_gflags_gflags",
#     sha256 = "ae27cdbcd6a2f935baa78e4f21f675649271634c092b1be01469440495609d0e",
#     strip_prefix = "gflags-2.2.1",
#     urls = [
#         "https://storage.googleapis.com/mirror.tensorflow.org/github.com/gflags/gflags/archive/v2.2.1.tar.gz",
#         "https://github.com/gflags/gflags/archive/v2.2.1.tar.gz",
#     ],
# )


# Google Test
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/8b6d3f9c4a774bef3081195d422993323b6bb2e0.zip"],  # 2019-03-05
  strip_prefix = "googletest-8b6d3f9c4a774bef3081195d422993323b6bb2e0",
  sha256 = "d21ba93d7f193a9a0ab80b96e8890d520b25704a6fac976fe9da81fffb3392e3",
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
