#ifndef EXPRESSIONADD_H
#define EXPRESSIONADD_H

#include <sstream>

#include "madeup/Expression.h"
#include "madeup/ExpressionString.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionBoolean.h"
#include "twodee/MessagedException.h"

using std::ostringstream;
using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionAdd : public Expression {
  public:
    ExpressionAdd(Co<Expression> left, Co<Expression> right) :
      Expression(),
      left(left),
      right(right) {
    }

    Co<Expression> evaluate(Environment& env) const {
      Co<Expression> lvalue = left->evaluate(env);
      Co<Expression> rvalue = right->evaluate(env);

      ExpressionString *lstring = dynamic_cast<ExpressionString *>(lvalue.GetPointer());
      ExpressionString *rstring = dynamic_cast<ExpressionString *>(rvalue.GetPointer());
      ExpressionNumber *lnumber = dynamic_cast<ExpressionNumber *>(lvalue.GetPointer());
      ExpressionNumber *rnumber = dynamic_cast<ExpressionNumber *>(rvalue.GetPointer());
      ExpressionInteger *linteger = dynamic_cast<ExpressionInteger *>(lvalue.GetPointer());
      ExpressionInteger *rinteger = dynamic_cast<ExpressionInteger *>(rvalue.GetPointer());
      ExpressionBoolean *lboolean = dynamic_cast<ExpressionBoolean *>(lvalue.GetPointer());
      ExpressionBoolean *rboolean = dynamic_cast<ExpressionBoolean *>(rvalue.GetPointer());
      
      Co<Expression> value;

      if (lstring && rstring) {
        value = Co<Expression>(new ExpressionString(lstring->GetString() + rstring->GetString()));
      }

      // Left is a string. Convert the other.
      else if (lstring) {
        ostringstream ss;
        ss << lstring->GetString();
        if (rinteger) {
          ss << rinteger->toInteger();
        } else if (rnumber) {
          ss << rnumber->toReal();
        } else if (rboolean) {
          ss << (rboolean->toBoolean() ? "true" : "false");
        } else {
          throw MessagedException(right->getSourceLocation().toAnchor() + ": Operator + doesn't know how to join a string to " + right->getSource());
        }
        return Co<Expression>(new ExpressionString(ss.str()));
      }

      // Right is a string. Convert the other.
      else if (rstring) {
        ostringstream ss;
        if (linteger) {
          ss << linteger->toInteger();
        } else if (lnumber) {
          ss << lnumber->toReal();
        } else if (lboolean) {
          ss << lboolean->toBoolean();
        } else {
          throw MessagedException(right->getSourceLocation().toAnchor() + ": Operator + doesn't know how to join a string to " + right->getSource() + ".");
        }
        ss << rstring->GetString();
        value = Co<Expression>(new ExpressionString(ss.str()));
      }

      else if (linteger && rinteger) {
        value = Co<Expression>(new ExpressionInteger(linteger->toInteger() + rinteger->toInteger()));
      }

      else if (lnumber && rnumber) {
        value = Co<Expression>(new ExpressionReal(lnumber->toReal() + rnumber->toReal()));
      }

      else {
        throw MessagedException(getSourceLocation().toAnchor() + ": Operator + doesn't know how to join " + left->getSource() + " and " + right->getSource() + ".");
      }

      return value;
    }

    void write(ostream& out) const {
      out << "(+ ";
      left->write(out);
      out << " ";
      right->write(out);
      out << ")";
    }

  private:
    Co<Expression> left;
    Co<Expression> right;
};

/* ------------------------------------------------------------------------- */

}

#endif
