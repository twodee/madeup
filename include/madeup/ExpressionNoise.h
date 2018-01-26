#ifndef EXPRESSIONNOISE_H
#define EXPRESSIONNOISE_H

#include "open-simplex-noise.h"
#include "madeup/Expression.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionNoise : public Expression {
  public:
    ExpressionNoise();

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;

  private:
    static struct osn_context *noise_context;
};

/* ------------------------------------------------------------------------- */

}

#endif
