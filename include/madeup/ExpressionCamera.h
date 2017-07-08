#ifndef EXPRESSIONCAMERA_H
#define EXPRESSIONCAMERA_H

#include "madeup/Expression.h"
#include "madeup/ExpressionPrimitive.h"
#include "twodee/Camera.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

class ExpressionCamera : public ExpressionPrimitive {
  public:
    ExpressionCamera(const td::Camera &camera);

    td::Co<Expression> evaluate(Environment &env) const;
    void write(ostream &out) const;
    std::string toString() const;
    const td::Camera &toCamera() const;

  private:
    td::Camera camera;
};

/* ------------------------------------------------------------------------- */

}

#endif
