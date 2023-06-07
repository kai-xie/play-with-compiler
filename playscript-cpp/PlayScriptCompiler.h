#pragma once

#include <iostream>
#include <memory>

#include "AnnotatedTree.h"
#include "PlayScriptLexer.h"
#include "PlayScriptParser.h"
#include "antlr4-runtime.h"

namespace play {
class CharStreams;
// class PlayScriptLexer;
// class PlayScriptParser;

class PlayScriptCompiler {
 public:
  std::shared_ptr<AnnotatedTree> compile(const std::string& script,
                                         bool verbose, bool ast_dump);

  std::shared_ptr<AnnotatedTree> compile(const std::string& script) {
    return compile(script, false, false);
  }

  void dumpSymbols() {
    if (at_) {
      std::cout << at_->getScopeTreeString() << std::endl;
    }
  }

  void dumpAST() {
    if (at_) {
      std::cout << at_->ast->toStringTree() << std::endl;
    }
  }

  void dumpCompilationLogs() {
    if (at_) {
      for (auto& log : at_->logs) {
        std::cout << log << std::endl;
      }
    }
  }

 private:
  std::shared_ptr<AnnotatedTree> at_{nullptr};
  std::shared_ptr<PlayScriptLexer> lexer_{nullptr};
  std::shared_ptr<PlayScriptParser> parser_{nullptr};
};

}  // namespace play
