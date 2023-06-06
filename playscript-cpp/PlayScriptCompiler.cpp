#include "PlayScriptCompiler.h"

#include "AntlrPlayScript.h"
#include "RefResolver.h"
#include "SemanticValidator.h"
#include "TypeAndScopeScanner.h"
#include "TypeChecker.h"
#include "TypeResolver.h"

namespace play {

std::shared_ptr<AnnotatedTree> PlayScriptCompiler::compile(
    const std::string& script, bool verbose, bool ast_dump) {
  at_.reset(new AnnotatedTree());

  // lexical analysis
  ANTLRInputStream input(script);
  lexer_.reset(new PlayScriptLexer(&input));
  CommonTokenStream tokens(lexer_.get());

  // syntax analysis
  parser_.reset(new PlayScriptParser(&tokens));
  at_->ast = parser_->prog();

  // semantic analysis
  ParseTreeWalker walker = ParseTreeWalker();
  // 优点：1.代码更清晰；2.允许使用在声明之前，这在支持面向对象、递归函数等特征时是必须的。
  // pass1：类型和Scope
  std::shared_ptr<TypeAndScopeScanner> pass1 =
      std::make_shared<TypeAndScopeScanner>(at_);
  walker.walk(pass1.get(), at_->ast);

  // pass2：把变量、类继承、函数声明的类型都解析出来。也就是所有声明时用到类型的地方。
  std::shared_ptr<TypeResolver> pass2 = std::make_shared<TypeResolver>(at_);
  walker.walk(pass2.get(), at_->ast);

  // pass3：消解有的变量应用、函数引用。另外还做了类型的推断。
  std::shared_ptr<RefResolver> pass3 = std::make_shared<RefResolver>(at_);
  walker.walk(pass3.get(), at_->ast);

  // pass4：类型检查
  std::shared_ptr<TypeChecker> pass4 = std::make_shared<TypeChecker>(at_);
  walker.walk(pass4.get(), at_->ast);

  // pass5：其他语义检查
  std::shared_ptr<SemanticValidator> pass5 =
      std::make_shared<SemanticValidator>(at_);
  walker.walk(pass5.get(), at_->ast);

  // //多步的语义解析。
  // //优点：1.代码更清晰；2.允许使用在声明之前，这在支持面向对象、递归函数等特征时是必须的。
  // //pass1：类型和Scope
  // TypeAndScopeScanner pass1 = new TypeAndScopeScanner(at);
  // walker.walk(pass1,at.ast);

  // //pass2：把变量、类继承、函数声明的类型都解析出来。也就是所有声明时用到类型的地方。
  // TypeResolver pass2 = new TypeResolver(at);
  // walker.walk(pass2,at.ast);

  // //pass3：消解有的变量应用、函数引用。另外还做了类型的推断。
  // RefResolver pass3 = new RefResolver(at);
  // walker.walk(pass3,at.ast);

  // //pass4：类型检查
  // TypeChecker pass4 = new TypeChecker(at);
  // walker.walk(pass4,at.ast);

  // //pass5：其他语义检查
  // SematicValidator pass5 = new SematicValidator(at);
  // walker.walk(pass5,at.ast);

  // //pass6：做闭包的分析
  // ClosureAnalyzer closureAnalyzer = new ClosureAnalyzer(at);
  // closureAnalyzer.analyzeClosures();

  return at_;
}

}  // namespace play
