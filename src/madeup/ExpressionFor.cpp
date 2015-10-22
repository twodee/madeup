#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionDefine.h"
#include "madeup/ExpressionFor.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionFor::ExpressionFor(const string &id, Co<Expression> start, Co<Expression> end, Co<Expression> delta, Co<Expression> body, bool is_inclusive) :
  id(id),
  start(start),
  end(end),
  delta(delta),
  body(body),
  is_inclusive(is_inclusive) {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionFor::evaluate(Environment &env) const {
  Co<Expression> start_value = start->evaluate(env);
  Co<Expression> end_value = end->evaluate(env);
  Co<Expression> delta_value = delta->evaluate(env);

  ExpressionInteger *start_ivalue = dynamic_cast<ExpressionInteger *>(start_value.GetPointer());
  ExpressionInteger *end_ivalue = dynamic_cast<ExpressionInteger *>(end_value.GetPointer());
  ExpressionInteger *delta_ivalue = dynamic_cast<ExpressionInteger *>(delta_value.GetPointer());

  if (!start_ivalue) {
    throw MessagedException(start->getSourceLocation().toAnchor() + ": A for loop's starting value must be an integer. " + start->getSource() + " is not an integer.");
  }

  if (!end_ivalue) {
    throw MessagedException(end->getSourceLocation().toAnchor() + ": A for loop's ending value must be an integer. " + end->getSource() + " is not an integer.");
  }

  if (!delta_ivalue) {
    throw MessagedException(delta->getSourceLocation().toAnchor() + ": A for loop's increment value must be an integer. " + delta->getSource() + " is not an integer.");
  }

  int a = start_ivalue->toInteger();
  int b = end_ivalue->toInteger();
  int idelta = delta_ivalue->toInteger();
 
  Co<Expression> value = ExpressionUnit::getSingleton();

  if (idelta > 0) {
    if (is_inclusive) {
      ++b;
    }

    for (int i = a; i < b; i += idelta) {
      env.checkTimeout(getSourceLocation());
      Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(id, Co<Expression>(new ExpressionInteger(i))));
      env.add(id, Co<ExpressionClosure>(new ExpressionClosure(define, env)));
      value = body->evaluate(env);
    }
  } else {
    if (is_inclusive) {
      --b;
    }
    
    for (int i = a; i > b; i += idelta) {
      env.checkTimeout(getSourceLocation());
      Co<ExpressionDefine> define = Co<ExpressionDefine>(new ExpressionDefine(id, Co<Expression>(new ExpressionInteger(i))));
      env.add(id, Co<ExpressionClosure>(new ExpressionClosure(define, env)));
      value = body->evaluate(env);
    }
  }

  return value;
}

/* ------------------------------------------------------------------------- */

void ExpressionFor::write(ostream &out) const {
  const ExpressionInteger *start_integer = dynamic_cast<const ExpressionInteger *>(start.GetPointer());
  const ExpressionInteger *delta_integer = dynamic_cast<const ExpressionInteger *>(delta.GetPointer());
  // to or through
  if (start_integer && start_integer->toInteger() == 0) {
    // through
    if (is_inclusive) {
      if (delta_integer && delta_integer->toInteger() == 1) {
        out << "(for-through " << id << " ";
        end->write(out);
        out << " ";
        body->write(out);
        out << ")";
      } else {
        out << "(for-through-by " << id << " ";
        end->write(out);
        out << " ";
        delta->write(out);
        out << " ";
        body->write(out);
        out << ")";
      }
    }

    // to
    else {
      if (delta_integer && delta_integer->toInteger() == 1) {
        out << "(for-to " << id << " ";
        end->write(out);
        out << " ";
        body->write(out);
        out << ")";
      } else {
        out << "(for-to-by " << id << " ";
        end->write(out);
        out << " ";
        delta->write(out);
        out << " ";
        body->write(out);
        out << ")";
      }
    }
  } else {
    if (delta_integer && delta_integer->toInteger() == 1) {
      out << "(for-in " << id << " ";
      start->write(out);
      out << " ";
      end->write(out);
      out << " ";
      body->write(out);
      out << ")";
    } else {
      out << "(for-in-by " << id << " ";
      start->write(out);
      out << " ";
      end->write(out);
      out << " ";
      delta->write(out);
      out << " ";
      body->write(out);
      out << ")";
    }
  }
}

/* ------------------------------------------------------------------------- */

}
