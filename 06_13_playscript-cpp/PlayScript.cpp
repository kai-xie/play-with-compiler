#include <iostream>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
// #include "absl/strings/str_cat.h"
// #include "ASTEvaluator.h"
#include "absl/strings/substitute.h"
#include "antlr4-runtime.h"
#include "glog/logging.h"
// #include "IRGen.h"
// #include "PlayScriptJIT.h"

// #include "PlayScriptLexer.h"
// #include "PlayScriptParser.h"
#include "AnnotatedTree.h"
#include "PlayScriptCompiler.h"
#include "Utils.h"
#include "play_flags.h"
#include "play_utils.h"

using namespace play;
using namespace antlr4;

void REPL(bool verbose, bool ast_dump) {
  PrintLn("Enjoy PlayScript!");

  std::shared_ptr<PlayScriptCompiler> compiler =
      std::make_shared<PlayScriptCompiler>();
  std::string script = "";
  std::string scriptLet = "";
  std::cout << "\n>";

  // A func performs reading input, just like the `input()` in python.
  auto input = [](std::string prompt = "") -> std::string {
    std::string line;
    std::cout << prompt;
    std::cin >> line;
    absl::RemoveExtraAsciiWhitespace(&line);
    return line;
  };

  while (true) {
    try {
      std::string line = input();
      if (line == "exit();") {
        PrintLn("good bye!");
        break;
      }

      scriptLet += line + "\n";
      if (line[line.size() - 1] == ';') {
        // 解析整个脚本文件
        std::shared_ptr<AnnotatedTree> at =
            compiler->compile(script + scriptLet, verbose, ast_dump);

        // 重新执行整个脚本
        if (!at->hasCompilationError()) {
          antlrcpp::Any result = compiler->Execute(at);
          std::cout << utils::toString(result) << std::endl;
          script = script + scriptLet;
        }

        std::cout << "\n>";  // 提示符
        scriptLet = "";
      }

    } catch (std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      std::cout << "\n>";
      scriptLet = "";
    }
  }
}

int main(int argc, char* argv[]) {
  // InitializeNativeTarget();
  // InitializeNativeTargetAsmPrinter();
  // InitializeNativeTargetAsmParser();

  // ANTLRInputStream input("double a = 3.0; double foo(double x, double
  // y){return x + y + 2.0;}; foo(3.0, 6.0); a+10.0;");

  google::InitGoogleLogging(argv[0]);

  absl::SetProgramUsageMessage(absl::Substitute(
      "Sample usage:\n$0 [--help | --o outputfile | --<no>S | --<no>bc | "
      "--<no>verbose | --<no>ast-dump] [scriptfile]\n\n"
      "examples:\n"
      "\t$0\n"
      "\t>>interactive REPL mode\n\n"
      "\t$0 -v\n"
      "\t>>enter REPL with verbose mode, dump ast and symbols\n\n"
      "\t$0 scratch.play\n"
      "\t>>compile and execute scratch.play\n\n"
      "\t$0 -bc scratch.play\n"
      "\t>>compile to bytecode, save as DefaultPlayClass.class and run it\n\n",
      argv[0]));
  std::vector<char*> positional_arguments = absl::ParseCommandLine(argc, argv);

  // 日志输出到stderr，不输出到日志文件。
  FLAGS_logtostderr = true;
  // 日志输出到stderr（终端屏幕），同时输出到日志文件。
  // FLAGS_alsologtostderr = true;
  FLAGS_colorlogtostderr = true;  // 输出彩色日志到stderr

  bool genAsm = absl::GetFlag(FLAGS_S);
  bool genByteCode = absl::GetFlag(FLAGS_bc);
  bool verbose = absl::GetFlag(FLAGS_verbose);
  bool ast_dump = absl::GetFlag(FLAGS_ast_dump);

  VLOG(0) << "positional arguments:";
  for (size_t i = 0; i < positional_arguments.size(); ++i) {
    VLOG(0) << positional_arguments[i];
  }

  // std::string script_file;
  std::string script;
  if (positional_arguments.size() == 1) {  // no arguments
    // LOG(INFO) << absl::ProgramUsageMessage();
    script = "";
  } else {  // get the scriptfile contents.
    std::string script_file = positional_arguments[1];
    // LOG(INFO) << "script file: " << script_file;
    auto script_or_not = ReadFile(script_file);
    if (script_or_not) {
      script = script_or_not.value();
      if (script.empty()) {
        LOG(WARNING) << "no contents in file: " << script_file;
        return 0;
      }
    } else {
      LOG(FATAL) << "reading file: " << script_file << " failed.";
    }
  }

  LOG(INFO) << "genAsm: " << std::boolalpha << genAsm;
  LOG(WARNING) << "genByteCode: " << std::boolalpha << genByteCode;
  LOG(ERROR) << "verbose: " << std::boolalpha << verbose;
  LOG(WARNING) << "ast_dump: " << std::boolalpha << ast_dump;

  if (script.empty()) {
    REPL(verbose, ast_dump);
  } else if (genAsm) {       // generate ASM code
  } else if (genByteCode) {  // generate byte code

  }
  // run script
  else {
    std::shared_ptr<PlayScriptCompiler> compiler =
        std::make_shared<PlayScriptCompiler>();
    try {
      std::shared_ptr<AnnotatedTree> at =
          compiler->compile(script, verbose, ast_dump);

      if (!at->hasCompilationError()) {
        antlrcpp::Any result = compiler->Execute(at);
        std::cout << utils::toString(result) << std::endl;
      }
    } catch (std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
    }
  }

  return 0;
}
