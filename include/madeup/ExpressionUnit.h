#ifndef EXPRESSIONUNIT_H
#define EXPRESSIONUNIT_H

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUnit : public Expression {
  public:
    ExpressionUnit() {
    }

    Co<Expression> Evaluate(Environment& env) {
      return Co<Expression>();
    }

    void Write(ostream& out) const {
      out << "(unit)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
