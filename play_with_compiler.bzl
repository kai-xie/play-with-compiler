load('@bazel_tools//tools/build_defs/repo:git.bzl', 'git_repository', 'new_git_repository')
# load


def play_with_compiler_deps():

  # if "glog" not in native.existing_rules():
  #   git_repository(
  #     name = "glog",
  #     remote = "https://github.com/google/glog",
  #     commit = "41f4bf9cbc3e8995d628b459f6a239df43c2b84a",
  #   )
  
  # if "com_google_absl" not in native.existing_rules():
  #   git_repository(
  #     name = "com_google_absl",
  #     remote = "https://github.com/abseil/abseil-cpp",
  #     commit = "2d2d7fbc283315b676159716376e739d3d23ed94",
  #   )

if "com_github_gflags_gflags" not in native.existing_rules():
    native.http_archive(
        name = "com_github_gflags_gflags",
        urls = [
            "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
        ],
        sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
        strip_prefix = "gflags-2.2.2",
    )
