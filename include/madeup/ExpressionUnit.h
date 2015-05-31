#ifndef EXPRESSIONUNIT_H
#define EXPRESSIONUNIT_H

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionUnit : public Expression {
  public:
    ExpressionUnit() {
    }

    Co<Expression> evaluate(Environment& env) const {
      return Co<Expression>();
    }

    void write(ostream& out) const {
      out << "(unit)";
    }

  private:
};

/* ------------------------------------------------------------------------- */

}

#endif
