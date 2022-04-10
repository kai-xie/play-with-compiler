# workspace(name = "com_github_qikai_ita")

# load("//:play_with_compiler.bzl", "play_with_compiler_deps")

# play_with_compiler_deps()

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# abseil-cpp
git_repository(
    name = "com_google_absl",
    commit = "de71511109d967000e68baedb75de104adb2b778",  # 2021/09/17
    remote = "https://github.com/abseil/abseil-cpp",
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

http_archive(
    name = "com_github_google_glog",
    sha256 = "21bc744fb7f2fa701ee8db339ded7dce4f975d0d55837a97be7d46e8382dea5a",
    strip_prefix = "glog-0.5.0",
    urls = ["https://github.com/google/glog/archive/v0.5.0.zip"],
)

# Google Test
http_archive(
    name = "com_google_googletest",
    strip_prefix = "googletest-40dfd4b775a66979ad1bd19321cdfd0feadfea27",
    urls = ["https://github.com/google/googletest/archive/40dfd4b775a66979ad1bd19321cdfd0feadfea27.zip"],  # 2021/09/17
)


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
