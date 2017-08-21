#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionPolarTo.h"
#include "madeup/ExpressionNumber.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionUtilities.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionPolarTo::ExpressionPolarTo() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionPolarTo::evaluate(Environment &env) const {
  float radius = ExpressionUtilities::lookup<ExpressionNumber>("radius", "number", "polarto", env)->toReal();
  float angle = ExpressionUtilities::lookup<ExpressionNumber>("angle", "number", "polarto", env)->toReal();
  Co<ExpressionArrayReference> origin = ExpressionUtilities::lookup<ExpressionArrayReference>("origin", "array of three numbers", "polarto", env);

  if (origin->getArray()->getSize() != 2 && origin->getArray()->getSize() != 3) {
    throw MessagedException(getSourceLocation().toAnchor() + ": I expect function polarto to be given an origin as an array of two or three numbers.");
  }

  QVector3<float> o(0.0f);
  for (int i = 0; i < origin->getArray()->getSize(); ++i) {
    Co<Expression> element = origin->getArray()->operator[](i);
    ExpressionNumber *number = dynamic_cast<ExpressionNumber *>(element.GetPointer());
    if (!number) {
      std::stringstream ss;
      ss << "I expect function polarto's origin parameter to be composed of numbers. But origin[" << i << "] is not a number.";
      throw UnlocatedException(ss.str());
    }
    o[i] = number->toReal();
  }

  float x = o[0] + radius * cos(angle * td::PI / 180.0f);
  float y = o[1] + radius * sin(angle * td::PI / 180.0f);
  float z = o[2];
  env.moveTo(x, y, z);

  return ExpressionUnit::getSingleton();
}

/* ------------------------------------------------------------------------- */

void ExpressionPolarTo::write(ostream &out) const {
  out << "(polarto)";
}

/* ------------------------------------------------------------------------- */

}
