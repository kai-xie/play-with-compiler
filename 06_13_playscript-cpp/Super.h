#pragma once

#include "Class.h"
#include "Variable.h"

namespace play {

class Super : public Variable {
 public:
  Super(Class* theClass, ParserRuleContext* ctx)
      : Variable("super", (Scope*)(theClass), ctx) {}

  Class* belongingClass() { return dynamic_cast<Class*>(enclosingScope); }
};

}  // namespace play
