#ifndef ELEMENTDEFINE_H
#define ELEMENTDEFINE_H

#include "madeup/Expression.h"
/* #include "madeup/ExpressionClosure.h" */
#include "madeup/ExpressionUnit.h"

namespace madeup {

class ExpressionClosure;

class FormalParameter {
  public:
    enum evaluation_mode_t {
      EAGER,
      LAZY
    };

    FormalParameter(const std::string &name,
                    evaluation_mode_t evaluation_mode) :
      name(name),
      evaluation_mode(evaluation_mode) {
    }

    evaluation_mode_t GetEvaluationMode() const {
      return evaluation_mode;
    }

    const std::string &GetName() const {
      return name;
    }

  private:
    std::string name;
    evaluation_mode_t evaluation_mode;
};

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const FormalParameter &parameter);

/* ------------------------------------------------------------------------- */

class ExpressionDefine : public Expression {
  public:
    ExpressionDefine(const std::string& name, Co<Expression> body);
    ExpressionDefine(const std::string& name, Co<Expression> body, const std::vector<FormalParameter>& formals);
    void AddFormal(const std::string& name, FormalParameter::evaluation_mode_t evaluation_mode = FormalParameter::EAGER);
    const FormalParameter& GetFormal(int i);
    unsigned int GetArity() const;
    Co<Expression> Evaluate(Environment& env);
    Co<Expression> GetBody() const;
    void Write(ostream& out) const;
    void IsDynamicallyScoped(bool enable);
    bool IsDynamicallyScoped() const;
    const std::string& GetName() const { return name; }

  private:
    std::string name;
    Co<Expression> body;
    std::vector<FormalParameter> formals;
    bool is_dynamically_scoped;
};

/* ------------------------------------------------------------------------- */

}

#endif
