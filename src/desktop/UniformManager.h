#ifndef UNIFORMMANAGER_H
#define UNIFORMMANAGER_H

#include "ShaderProgram.h"

namespace td {

/* ------------------------------------------------------------------------- */

enum ShaderUniformType {
  FLOAT,
  FLOAT_VEC2,
  FLOAT_VEC3,
  FLOAT_VEC4,
  FLOAT_MAT4,
  INT,
  INT_VEC2,
  INT_VEC3,
  INT_VEC4,
  UNSUPPORTED
};

/* ------------------------------------------------------------------------- */

/* using namespace ShaderUniformType; */

class ShaderUniform {
  public:
    ShaderUniform(const std::string& name,
                  ShaderUniformType type,
                  int location,
                  int nelements = 1);

    virtual const std::string& GetName() const;
    virtual ShaderUniformType GetType() const;
    virtual int GetLocation() const;
    virtual int GetElementCount() const;

  private:
    std::string name;
    ShaderUniformType type;
    int location;
    int nelements;
};

/* ------------------------------------------------------------------------- */

class ShaderUniformf : public ShaderUniform {
  public:
    ShaderUniformf(const std::string& name,
                   ShaderUniformType type,
                   int location,
                   int nelements = 1);
    // TODO: dtor

    float GetElement(int i) const;
    float *GetElements();
    const float *GetElements() const;

  private:
    float *elements;
};

/* ------------------------------------------------------------------------- */

class ShaderUniformi : public ShaderUniform {
  public:
    ShaderUniformi(const std::string& name,
                   ShaderUniformType type,
                   int location,
                   int nelements = 1);

    int GetElement(int i) const;
    int *GetElements();
    const int *GetElements() const;
    
    // TODO: dtor

  private:
    int *elements;
};

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out,
                         const ShaderUniform& uniform);
std::ostream& operator<<(std::ostream& out,
                         const ShaderUniformi& uniform);
std::ostream& operator<<(std::ostream& out,
                         const ShaderUniformf& uniform);

/* ------------------------------------------------------------------------- */

class UniformManager {
  public:
    UniformManager(ShaderProgram *program = NULL);

    void SetProgram(ShaderProgram *shader_program);

    int GetUniformCount() const;

    const ShaderUniform& GetUniform(int i) const;
    void SetUniform(int uniformID,
                    int elementID,
                    float value);
    void SetUniform(int uniformID,
                    int elementID,
                    int value);
    void RefreshUniforms();

  private:
    std::vector<ShaderUniform *> uniforms;
    ShaderProgram *program;
};

/* ------------------------------------------------------------------------- */

}

#endif
