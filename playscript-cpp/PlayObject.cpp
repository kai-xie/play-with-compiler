#include "PlayObject.h"

#include "NullObject.h"
#include "Variable.h"

namespace play {
antlrcpp::Any PlayObject::getValue(Variable* variable) {
  antlrcpp::Any rtn = fields[variable];
  // TODO 父类的属性如何返回？还是说都在这里了？

  //替换成自己的NullObject
  if (!rtn) {
    rtn = NullObject::instance();
  }
  return rtn;
}
}  // namespace play
