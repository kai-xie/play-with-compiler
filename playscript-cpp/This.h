#pragma once

#include "Class.h"
#include "Variable.h"

namespace play {

class This : public Variable {
 public:
  This(Class* theClass, ParserRuleContext* ctx)
      : Variable("this", (Scope*)theClass, ctx) {}

  Class* belongingClass() { return dynamic_cast<Class*>(enclosingScope); }
};

}  // namespace play
