#ifndef PLAY_FLAGS_H_
#define PLAY_FLAGS_H_

ABSL_FLAG(bool, S, false, "compile to assembly code");
ABSL_FLAG(bool, bc, false, "compile to byte code");
ABSL_FLAG(bool, verbose, false, "verbose mode : dump AST and symbols");
ABSL_FLAG(bool, ast_dump, false, "dump AST in lisp style");
ABSL_FLAG(std::string, o, "",
          "outputfile : file pathname used to save generated code, eg. "
          "assembly code");

#endif  // PLAY_FLAGS_H_
