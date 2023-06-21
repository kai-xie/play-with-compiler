#include "Scope.h"

#include "Class.h"
#include "Function.h"

namespace play {

// class Class;

Variable* Scope::getVariable(Scope* scope, const std::string& name) {
  for (Symbol* s : scope->symbols) {
    if (antlrcpp::is<Variable*>(s) && s->getName() == name) {
      return dynamic_cast<Variable*>(s);
    }
  }
  return nullptr;
}

Function* Scope::getFunction(Scope* scope, const std::string& name,
                             const std::vector<Type*>& paramTypes) {
  Function* rtn = nullptr;
  for (Symbol* s : scope->symbols) {
    if (antlrcpp::is<Function*>(s) && s->name == name) {
      Function* function = dynamic_cast<Function*>(s);
      if (function->matchParameterTypes(paramTypes)) {
        rtn = function;
        break;
      }
    }
  }
  return rtn;
}

Variable* Scope::getFunctionVariable(Scope* scope, const std::string& name,
                                     const std::vector<Type*>& paramTypes) {
  Variable* rtn = nullptr;
  for (Symbol* s : scope->symbols) {
    if (Variable* v = dynamic_cast<Variable*>(s)) {
      if (FunctionType* functionType = dynamic_cast<FunctionType*>(v->type)) {
        if (s->name == name) {
          if (functionType->matchParameterTypes(paramTypes)) {
            rtn = v;
            break;
          }
        }
      }
    }
  }
  return rtn;
}

Class* Scope::getClass(Scope* scope, const std::string& name) {
  for (Symbol* s : scope->symbols) {
    if (antlrcpp::is<Class*>(s) && s->name == name) {
      return dynamic_cast<Class*>(s);
    }
  }
  return nullptr;
}

}  // namespace play
