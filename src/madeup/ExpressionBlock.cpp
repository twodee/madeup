#include "madeup/ExpressionBlock.h"
#include "madeup/ExpressionUnit.h"

using std::vector;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionBlock::ExpressionBlock() :
  Expression(),
  statements() {
}

/* ------------------------------------------------------------------------- */

void ExpressionBlock::append(Co<Expression> element) {
  statements.push_back(element);
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBlock::evaluate(Environment &env) const {
  Co<Expression> value(ExpressionUnit::getSingleton());
  for (vector<Co<Expression> >::const_iterator i = statements.begin(); i != statements.end(); ++i) {
    value = (*i)->evaluate(env);
  }
  return value;
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionBlock::operator[](size_t i) {
  /* throw 5; */
  /* std::cout << "i: " << i << std::endl; */
  assert(0 <= i && i < statements.size());
  return statements[i];
}

/* ------------------------------------------------------------------------- */

int ExpressionBlock::getLength() const {
  return statements.size();
}

/* ------------------------------------------------------------------------- */

void ExpressionBlock::write(ostream &out) const {
  out << "(block ";
  vector<Co<Expression> >::const_iterator i = statements.begin();
  if (i != statements.end()) {
    (*i)->write(out);
    for (++i; i != statements.end(); ++i) {
      out << " ";
      (*i)->write(out);
    }
  }
  out << ")";
}

/* ------------------------------------------------------------------------- */

}
