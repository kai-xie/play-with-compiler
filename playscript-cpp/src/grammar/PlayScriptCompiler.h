#pragma once

#include "antlr4-runtime.h"

#include "PlayScriptLexer.h"
#include "PlayScriptParser.h"

namespace play {
class CharStreams;
class PlayScriptLexer;
class PlayScriptParser;


class PlayScriptCompiler {
 public:
  AnnotatedTree* compile(std::string& script, bool verbose, bool ast_dump) {
    at_ = new AnnotatedTree();

    // lexical analysis
    ANTLRInputStream input(script);
    lexer = new PlayScriptLexer(CharStream fromString(script));

  }

private:
  AnnotatedTree* at_;
  PlayScriptLexer lexer_;
  PlayScriptParser parser_;

};

}
