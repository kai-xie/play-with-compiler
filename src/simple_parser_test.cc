#include <memory>

#include "glog/logging.h"

#include "simple_parser.h"

void test_parser(int test_id, const std::string& script) {
  std::unique_ptr<SimpleParser> parser = std::make_unique<SimpleParser>();
  std::cout << "================== test " << test_id
            << " ==================" << std::endl;
  // script = "2+3+;";
  std::cout << "Parsing: " + script << std::endl;
  std::unique_ptr<SimpleAST> ast;
  try {
    // ast = std::move(parser->Parse(script));
    ast = parser->Parse(script);
    // auto ast = parser->Parse(script);
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return;
  } 
  // catch (std::logic_error& e) {
  //   std::cout << e.what() << std::endl;
  //   return;
  // }
  VLOG(1) << "parsing done...";
  auto root = ast->root_node();
  parser->DumpAST(root, "");
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  // to enable --v=x for VLOG control
  google::ParseCommandLineFlags(&argc, &argv, true);
  // 日志输出到stderr，不输出到日志文件。
  FLAGS_logtostderr = true;
  // 日志输出到stderr（终端屏幕），同时输出到日志文件。
  // FLAGS_alsologtostderr = true;

  FLAGS_colorlogtostderr = true;  //输出彩色日志到stderr
  std::unique_ptr<SimpleParser> parser = std::make_unique<SimpleParser>();
  std::string script;
  ASTNode* root = nullptr;

  test_parser(1, "int age = 45+2; age= 20; age1+10*2;");
  test_parser(2, "int age = 45+2 +3 +4;");

  // [!!!] Test 3 and 4 will fail
  test_parser(3, "2+3+;");
  test_parser(4, "2+3*;");

  test_parser(5, "var_name +3*4+_+6/7;");
  test_parser(6, "Int=another_int;");

  return 0;
}
