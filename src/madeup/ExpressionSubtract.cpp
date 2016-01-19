#include <sstream>

#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionSubtract.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

#ifdef USE_IGL
#include "madeup/MeshBoolean.h"
#endif

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionSubtract::ExpressionSubtract(Co<Expression> left, Co<Expression> right) :
  Expression(),
  left(left),
  right(right) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubtract::evaluate(Environment &env) const {
  return subtract(left, right, getSource(), getSourceLocation(), env);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionSubtract::subtract(Co<Expression> l,
                                            Co<Expression> r,
                                            const std::string &source,
                                            const SourceLocation &location,
                                            Environment &env) {
  Co<Expression> l_value = l->evaluate(env);
  Co<Expression> r_value = r->evaluate(env);

  // Integers
  ExpressionInteger *l_integer = dynamic_cast<ExpressionInteger *>(l_value.GetPointer());
  ExpressionInteger *r_integer = dynamic_cast<ExpressionInteger *>(r_value.GetPointer());
  if (l_integer && r_integer) {
    return Co<Expression>(new ExpressionInteger(l_integer->toInteger() - r_integer->toInteger()));
  }

  // Any mix of numbers
  ExpressionNumber *l_number = dynamic_cast<ExpressionNumber *>(l_value.GetPointer());
  ExpressionNumber *r_number = dynamic_cast<ExpressionNumber *>(r_value.GetPointer());
  if (l_number && r_number) {
    return Co<Expression>(new ExpressionReal(l_number->toReal() - r_number->toReal()));
  }

  // Arrays
  ExpressionArrayReference *l_array = dynamic_cast<ExpressionArrayReference *>(l_value.GetPointer());
  ExpressionArrayReference *r_array = dynamic_cast<ExpressionArrayReference *>(r_value.GetPointer());
  if (l_array && r_array) {
    int nitems = l_array->getArray()->getSize();
    if (nitems == r_array->getArray()->getSize()) {
      Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
      for (int i = 0; i < nitems; ++i) {
        array->setElement(i, subtract((*l_array->getArray())[i], (*r_array->getArray())[i], source, location, env));
      }
      return Co<Expression>(new ExpressionArrayReference(array));
    } else {
      throw MessagedException(location.toAnchor() + ": Operator - doesn't know how to subtract arrays of different sizes.");
    }
  } 

  // Left only is an array
  if (l_array) {
    int nitems = l_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, subtract((*l_array->getArray())[i], r_value, source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }

  // Right only is an array
  if (r_array) {
    int nitems = r_array->getArray()->getSize();
    Co<ExpressionArray> array(new ExpressionArray(nitems, ExpressionUnit::getSingleton()));
    for (int i = 0; i < nitems; ++i) {
      array->setElement(i, subtract(l_value, (*r_array->getArray())[i], source, location, env));
    }
    return Co<Expression>(new ExpressionArrayReference(array));
  }
 
#ifdef USE_IGL
  // Meshes
  ExpressionMesh *l_mesh = dynamic_cast<ExpressionMesh *>(l_value.GetPointer());
  ExpressionMesh *r_mesh = dynamic_cast<ExpressionMesh *>(r_value.GetPointer());
  if (l_mesh && r_mesh) {
    return MeshBoolean::construct_and_color(l_mesh, r_mesh, env, MeshBoolean::DIFFERENCE);
  }
#endif

  throw MessagedException(location.toAnchor() + ": Operator - doesn't know how to join " + l->getSource() + " and " + r->getSource() + ".");
}

/* ------------------------------------------------------------------------- */

void ExpressionSubtract::write(ostream &out) const {
  out << "(- ";
  left->write(out);
  out << " ";
  right->write(out);
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
