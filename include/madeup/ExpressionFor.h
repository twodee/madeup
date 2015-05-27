#ifndef ELEMENTFOR_H
#define ELEMENTFOR_H

#include "madeup/ExpressionBlock.h"
#include "madeup/Expression.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/ExpressionInteger.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionFor : public Expression {
  public:
    ExpressionFor(const string& id, Co<Expression> start, Co<Expression> end, Co<Expression> delta, Co<ExpressionBlock> body, bool is_inclusive) :
      id(id),
      start(start),
      end(end),
      delta(delta),
      body(body),
      is_inclusive(is_inclusive) {
    }

    Co<Expression> Evaluate(Environment& env) {
      Co<Expression> start_value = start->Evaluate(env);
      Co<Expression> end_value = end->Evaluate(env);
      Co<Expression> delta_value = delta->Evaluate(env);

      ExpressionInteger *start_ivalue = dynamic_cast<ExpressionInteger *>(start_value.GetPointer());
      ExpressionInteger *end_ivalue = dynamic_cast<ExpressionInteger *>(end_value.GetPointer());
      ExpressionInteger *delta_ivalue = dynamic_cast<ExpressionInteger *>(delta_value.GetPointer());

      if (!start_ivalue) {
        throw MessagedException(start->GetSourceLocation() + ": A for loop's starting value must be an integer. " + start->GetSource() + " is not an integer.");
      }

      if (!end_ivalue) {
        throw MessagedException(end->GetSourceLocation() + ": A for loop's ending value must be an integer. " + end->GetSource() + " is not an integer.");
      }

      if (!delta_ivalue) {
        throw MessagedException(delta->GetSourceLocation() + ": A for loop's increment value must be an integer. " + delta->GetSource() + " is not an integer.");
      }

      int a = start_ivalue->GetInteger();
      int b = end_ivalue->GetInteger();
      int idelta = delta_ivalue->GetInteger();
 
      Co<Expression> value = new ExpressionUnit();

      if (idelta > 0) {
        if (is_inclusive) {
          ++b;
        }

        for (int i = a; i < b; i += idelta) {
          Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(id, Co<Expression>(new ExpressionInteger(i))));
          env.Add(id, Co<ExpressionClosure>(new ExpressionClosure(define, env)));
          value = body->Evaluate(env);
        }
      } else {
        if (is_inclusive) {
          --b;
        }
        
        for (int i = a; i > b; i += idelta) {
          Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(id, Co<Expression>(new ExpressionInteger(i))));
          env.Add(id, Co<ExpressionClosure>(new ExpressionClosure(define, env)));
          value = body->Evaluate(env);
        }
      }

      return value;
    }

    void Write(ostream& out) const {
      out << "(for " << id << " ";
      start->Write(out);
      out << " ";
      end->Write(out);
      out << " ";
      delta->Write(out);
      out << " ";
      body->Write(out);
      out << ")";
    }

  private:
    string id;
    Co<Expression> start;
    Co<Expression> end;
    Co<Expression> delta;
    Co<ExpressionBlock> body;
    bool is_inclusive;
};

/* ------------------------------------------------------------------------- */

}

#endif
