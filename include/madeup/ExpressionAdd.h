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

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> lvalue = left->Evaluate(env);
      Co<Expression> rvalue = right->Evaluate(env);

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
          ss << rinteger->GetInteger();
        } else if (rnumber) {
          ss << rnumber->GetReal();
        } else if (rboolean) {
          ss << (rboolean->GetBoolean() ? "true" : "false");
        } else {
          throw MessagedException(right->GetSourceLocation().toAnchor() + ": Operator + doesn't know how to join a string to " + right->GetSource());
        }
        return Co<Expression>(new ExpressionString(ss.str()));
      }

      // Right is a string. Convert the other.
      else if (rstring) {
        ostringstream ss;
        if (linteger) {
          ss << linteger->GetInteger();
        } else if (lnumber) {
          ss << lnumber->GetReal();
        } else if (lboolean) {
          ss << lboolean->GetBoolean();
        } else {
          throw MessagedException(right->GetSourceLocation().toAnchor() + ": Operator + doesn't know how to join a string to " + right->GetSource() + ".");
        }
        ss << rstring->GetString();
        value = Co<Expression>(new ExpressionString(ss.str()));
      }

      else if (linteger && rinteger) {
        value = Co<Expression>(new ExpressionInteger(linteger->GetInteger() + rinteger->GetInteger()));
      }

      else if (lnumber && rnumber) {
        value = Co<Expression>(new ExpressionReal(lnumber->GetReal() + rnumber->GetReal()));
      }

      else {
        throw MessagedException(GetSourceLocation().toAnchor() + ": Operator + doesn't know how to join " + left->GetSource() + " and " + right->GetSource() + ".");
      }

      return value;
    }

    void Write(ostream& out) const {
      out << "(+ ";
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
