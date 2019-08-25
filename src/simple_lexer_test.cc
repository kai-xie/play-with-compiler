#include "simple_lexer.h"

#include <memory>
#include <string>

#include "simple_token_reader.h"

// template<typename T=std::string>
inline void TestLexer(const std::string& script) {
  // std::string script;
  // script = str;
  // VLOG(0) << "parse: " << script;
  std::cout << "parse: " << script << std::endl;
  std::unique_ptr<SimpleLexer> lexer = std::make_unique<SimpleLexer>();

  lexer->Dump(*(lexer->Tokenize(script)));
}

int main(int argc, char* argv[]) {
  // std::string script;

  // script = "int age = 45;";
  TestLexer("int age = 45;");
  // VLOG(0) << "parse: " << script;
  // SimpleTokenReader token_reader = Tokenize(script);
  // Dump(token_reader);

  // Test parsing "inta"
  TestLexer("inta age = 45;");

  // Test parsing "in"
  TestLexer("in age = 45;");

  TestLexer("age >= 45;");

  TestLexer("age > 45;");
}
