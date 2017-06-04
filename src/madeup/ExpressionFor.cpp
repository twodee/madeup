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

ExpressionFor::ExpressionFor(Co<Expression> iterator, Co<Expression> start, Co<Expression> end, Co<Expression> delta, Co<Expression> body, bool is_inclusive) :
  iterator(iterator),
  start(start),
  end(end),
  delta(delta),
  body(body),
  is_inclusive(is_inclusive) {
  isEmitter(false);
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
      iterator->assign(env, Co<Expression>(new ExpressionInteger(i)));
      value = body->evaluate(env);
    }
  } else {
    if (is_inclusive) {
      --b;
    }
    
    for (int i = a; i > b; i += idelta) {
      env.checkTimeout(getSourceLocation());
      iterator->assign(env, Co<Expression>(new ExpressionInteger(i)));
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
        out << "(for-through ";
        iterator->write(out);
        out << " ";
        end->write(out);
        out << " ";
        body->write(out);
        out << ")";
      } else {
        out << "(for-through-by ";
        iterator->write(out);
        out << " ";
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
        out << "(for-to ";
        iterator->write(out);
        out << " ";
        end->write(out);
        out << " ";
        body->write(out);
        out << ")";
      } else {
        out << "(for-to-by "; 
        iterator->write(out);
        out << " ";
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
      out << "(for-in ";
      iterator->write(out);
      out << " ";
      start->write(out);
      out << " ";
      end->write(out);
      out << " ";
      body->write(out);
      out << ")";
    } else {
      out << "(for-in-by ";
      iterator->write(out);
      out << " ";
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
