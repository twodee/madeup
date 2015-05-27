#ifndef EXPRESSIONNOTEQUAL_H
#define EXPRESSIONNOTEQUAL_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionString.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNotEqual : public Expression {
  public:
    ExpressionNotEqual(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

      ExpressionString *lstring = dynamic_cast<ExpressionString *>(lvalue.GetPointer());
      ExpressionString *rstring = dynamic_cast<ExpressionString *>(rvalue.GetPointer());
      ExpressionDecimal *ldecimal = dynamic_cast<ExpressionDecimal *>(lvalue.GetPointer());
      ExpressionDecimal *rdecimal = dynamic_cast<ExpressionDecimal *>(rvalue.GetPointer());
      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
      
      Co<Expression> r;
      if (lstring && rstring) {
        r = Co<Expression>(new ExpressionBoolean(lstring->GetString() != rstring->GetString()));
      } else if (ldecimal && rdecimal) {
        r = Co<Expression>(new ExpressionBoolean(ldecimal->GetDecimal() != rdecimal->GetDecimal()));
      } else if (linteger && rinteger) {
        r = Co<Expression>(new ExpressionBoolean(linteger->GetInteger() != rinteger->GetInteger()));
      } else if (lboolean && rboolean) {
        r = Co<Expression>(new ExpressionBoolean(lboolean->GetBoolean() != rboolean->GetBoolean()));
      } else {
        r = Co<Expression>(new ExpressionBoolean(true));
      }

      return r;
    }

    void Write(ostream& out) const {
      out << "(!= ";
      left->Write(out);
      out << " ";
      right->Write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
