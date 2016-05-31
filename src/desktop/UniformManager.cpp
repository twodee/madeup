#include <cassert>
#include <iostream>

#include "FieldType.h"
#include "Log.h"
#include "UniformManager.h"

namespace td {

/* ------------------------------------------------------------------------- */

ShaderUniform::ShaderUniform(const std::string& name,
                             ShaderUniformType type,
                             int location,
                             int nelements) :
  name(name),
  type(type),
  location(location),
  nelements(nelements) {
}

/* ------------------------------------------------------------------------- */

const std::string& ShaderUniform::GetName() const {
  return name;
}

/* ------------------------------------------------------------------------- */

ShaderUniformType ShaderUniform::GetType() const {
  return type;
}

/* ------------------------------------------------------------------------- */

int ShaderUniform::GetLocation() const {
  return location;
}

/* ------------------------------------------------------------------------- */

UniformManager::UniformManager(ShaderProgram *program) {
  SetProgram(program); 
}

/* ------------------------------------------------------------------------- */

void UniformManager::SetProgram(ShaderProgram *program) {
  this->program = program;

  // TODO: clear out state
  if (program != NULL) {
    RefreshUniforms();
  }
}

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out,
                         const ShaderUniform& uniform) {
  out << "Uniform:    " << uniform.GetName() << std::endl;  
  out << " type:      " << uniform.GetType() << std::endl;  
  out << " location:  " << uniform.GetLocation() << std::endl;  
  out << " # elements:" << uniform.GetElementCount() << std::endl;  

  return out;
}

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out,
                         const ShaderUniformf& uniform) {
  out << "Uniform:    " << uniform.GetName() << std::endl;  
  out << " type:      " << uniform.GetType() << std::endl;  
  out << " location:  " << uniform.GetLocation() << std::endl;  
  out << " # elements:" << uniform.GetElementCount() << std::endl;  
  for (int i = 0; i < uniform.GetElementCount(); ++i) {
    out << uniform.GetElements()[i] << ", ";
  }

  return out;
}

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out,
                         const ShaderUniformi& uniform) {
  out << "Uniform:    " << uniform.GetName() << std::endl;  
  out << " type:      " << uniform.GetType() << std::endl;  
  out << " location:  " << uniform.GetLocation() << std::endl;  
  out << " # elements:" << uniform.GetElementCount() << std::endl;  
  for (int i = 0; i < uniform.GetElementCount(); ++i) {
    out << uniform.GetElements()[i] << ", ";
  }

  return out;
}

/* ------------------------------------------------------------------------- */

int UniformManager::GetUniformCount() const {
  return uniforms.size();
}

/* ------------------------------------------------------------------------- */

void UniformManager::SetUniform(int uniformID,
                                int elementID,
                                float value) {
  ShaderUniformf *uniform = dynamic_cast<ShaderUniformf *>(uniforms[uniformID]);
  uniform->GetElements()[elementID] = value;
 
  // Bind the program if not already bound.
  bool was_bound = program->IsBound();
  if (!was_bound) {
    program->Bind();
  }

  if (uniform->GetType() == ShaderUniformType::FLOAT_VEC3) {
    glUniform3fv(uniform->GetLocation(), 1, uniform->GetElements());
  } else if (uniform->GetType() == ShaderUniformType::FLOAT_VEC4) {
    glUniform4fv(uniform->GetLocation(), 1, uniform->GetElements());
  } else if (uniform->GetType() == ShaderUniformType::FLOAT_MAT4) {
    glUniformMatrix4fv(uniform->GetLocation(), 1, false, uniform->GetElements());
  }

  // If the program wasn't bound before, let's restore that unboundedness now.
  if (!was_bound) {
    program->Unbind();
  }
}

/* ------------------------------------------------------------------------- */

void UniformManager::SetUniform(int uniformID,
                                int elementID,
                                int value) {
  assert("Not implemented yet!" || false);
  ShaderUniformi *uniform = dynamic_cast<ShaderUniformi *>(uniforms[uniformID]);
  uniform->GetElements()[elementID] = value;
}

/* ------------------------------------------------------------------------- */

void UniformManager::RefreshUniforms() {
  assert(program);

  uniforms.clear();

  // Bind the program if not already bound.
  bool was_bound = program->IsBound();
  if (!was_bound) {
    program->Bind();
  }

  int nuniforms;
  glGetProgramiv(program->GetID(), GL_ACTIVE_UNIFORMS, &nuniforms);
  Log::Debug("# of uniforms: %d", nuniforms);

  for (int i = 0; i < nuniforms; ++i) {
    int length;
    int size;
    GLenum gl_type;
    char name[256];
    name[255] = '\0';
    glGetActiveUniform(program->GetID(), i, 255, &length, &size, &gl_type, name);
    FieldType::FieldType element_type;
    int nelements = -1;

    Log::Debug("name: %s", name);

    ShaderUniformType type;
    switch (gl_type) {
      case GL_FLOAT:
        type = ShaderUniformType::FLOAT;
        element_type = FieldType::FLOAT;
        nelements = 1;
        break;
      case GL_FLOAT_VEC2:
        type = ShaderUniformType::FLOAT_VEC2;
        element_type = FieldType::FLOAT;
        nelements = 2;
        break;
      case GL_FLOAT_VEC3:
        type = ShaderUniformType::FLOAT_VEC3;
        element_type = FieldType::FLOAT;
        nelements = 3;
        break;
      case GL_FLOAT_VEC4:
        type = ShaderUniformType::FLOAT_VEC4;
        element_type = FieldType::FLOAT;
        nelements = 4;
        break;
      case GL_FLOAT_MAT4:
        type = ShaderUniformType::FLOAT_MAT4;
        element_type = FieldType::FLOAT;
        nelements = 16;
        break;
      case GL_INT:
        type = ShaderUniformType::INT;
        element_type = FieldType::INT;
        nelements = 1;
        break;
      case GL_INT_VEC2:
        type = ShaderUniformType::INT_VEC2;
        element_type = FieldType::INT;
        nelements = 2;
        break;
      case GL_INT_VEC3:
        type = ShaderUniformType::INT_VEC3;
        element_type = FieldType::INT;
        nelements = 3;
        break;
      case GL_INT_VEC4:
        type = ShaderUniformType::INT_VEC4;
        element_type = FieldType::INT;
        nelements = 4;
        break;
      default:
        std::cerr << "uniform " << name << " is of an unsupported type" << std::endl;
        type = ShaderUniformType::UNSUPPORTED;
    }

    if (type != ShaderUniformType::UNSUPPORTED) {
      GLint location = glGetUniformLocation(program->GetID(), name);
      ShaderUniform *uniform = NULL;

      if (element_type == FieldType::FLOAT) {
        ShaderUniformf *uniformf = new ShaderUniformf(name, type, location, nelements);
        glGetUniformfv(program->GetID(), location, uniformf->GetElements());
        uniform = uniformf;
      } else if (element_type == FieldType::INT) {
        ShaderUniformi *uniformi = new ShaderUniformi(name, type, location, nelements);
        glGetUniformiv(program->GetID(), location, uniformi->GetElements());
        uniform = uniformi;
      } else {
        assert(false);
      }

      uniforms.push_back(uniform);
    }
  }


  // If the program wasn't bound before, let's restore that unboundedness now.
  if (!was_bound) {
    program->Unbind();
  }
}

/* ------------------------------------------------------------------------- */

const ShaderUniform& UniformManager::GetUniform(int i) const {
  return *uniforms[i];
}

/* ------------------------------------------------------------------------- */

int ShaderUniform::GetElementCount() const {
  return nelements;
}

/* ------------------------------------------------------------------------- */

ShaderUniformf::ShaderUniformf(const std::string& name,
                               ShaderUniformType type,
                               int location,
                               int nelements) :
  ShaderUniform(name, type, location, nelements),
  elements(new float[nelements]) {
}

/* ------------------------------------------------------------------------- */

float ShaderUniformf::GetElement(int i) const {
  return elements[i];
}

/* ------------------------------------------------------------------------- */

float *ShaderUniformf::GetElements() {
  return elements;
}

/* ------------------------------------------------------------------------- */

const float *ShaderUniformf::GetElements() const {
  return elements;
}

/* ------------------------------------------------------------------------- */

ShaderUniformi::ShaderUniformi(const std::string& name,
                               ShaderUniformType type,
                               int location,
                               int nelements) :
  ShaderUniform(name, type, location, nelements),
  elements(new int[nelements]) {
}

/* ------------------------------------------------------------------------- */

int ShaderUniformi::GetElement(int i) const {
  return elements[i];
}

/* ------------------------------------------------------------------------- */

int *ShaderUniformi::GetElements() {
  return elements;
}

/* ------------------------------------------------------------------------- */

const int *ShaderUniformi::GetElements() const {
  return elements;
}

/* ------------------------------------------------------------------------- */

}
