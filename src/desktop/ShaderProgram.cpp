#include <iostream>

#include "Log.h"
#include "ShaderProgram.h"
#include "twodee/Utilities.h"

namespace td {

/* ------------------------------------------------------------------------- */

ShaderProgram::ShaderProgram() :
  is_bound(false) {
}

/* ------------------------------------------------------------------------- */

ShaderProgram::~ShaderProgram() {
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  glDeleteShader(geo_shader_id);
  glDeleteProgram(shader_pid); 
}

/* ------------------------------------------------------------------------- */

GLuint ShaderProgram::GetID() const {
  return shader_pid;
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetFiles(const std::string& vertex_path,
                             const std::string& fragment_path,
                             const std::string& geo_path,
                             const char * outputs[]) {
  // Load and compile vertex shader
  char *src = Utilities::FileToString(vertex_path);
  std::string vertex_source(src);
  delete[] src;

  // Load and compile fragment shader
  src = Utilities::FileToString(fragment_path);
  std::string fragment_source(src);
  delete[] src;

  std::string geo_source;
  if (geo_path.size()) {
    src = Utilities::FileToString(geo_path);
    geo_source = src;
    delete[] src;
  }

  SetSource(vertex_source, fragment_source, geo_source, outputs);
}

/* ------------------------------------------------------------------------- */

GLint ShaderProgram::GetUniformLocation(const char *name) {
  std::map<std::string, GLint>::const_iterator match = uniform_locations.find(name); 
  return match != uniform_locations.end() ? match->second : -1;
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::Bind() {
  glUseProgram(GetID());
  is_bound = true;
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::Unbind() {
  glUseProgram(0);
  is_bound = false;
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name,
                               float a) {
  SetUniform(GetUniformLocation(name), a);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name,
                               int a) {
  SetUniform(GetUniformLocation(name), a);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name,
                               float a,
                               float b) {
  SetUniform(GetUniformLocation(name), a, b);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name,
                               float a,
                               float b,
                               float c) {
  SetUniform(GetUniformLocation(name), a, b, c);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name,
                               float a,
                               float b,
                               float c,
                               float d) {
  SetUniform(GetUniformLocation(name), a, b, c, d);
}

/* ------------------------------------------------------------------------- */

#ifdef USE_TWODEE
void ShaderProgram::SetUniform(const char *name,
                               const QMatrix4<float>& matrix) {
  SetUniform(GetUniformLocation(name), matrix);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name, const QVector3<float>& v) {
  SetUniform(GetUniformLocation(name), v);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name, const QVector2<float>& v) {
  SetUniform(GetUniformLocation(name), v);
}
#endif

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               float a) {
  glUniform1f(location, a);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               int a) {
  glUniform1i(location, a);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               float a,
                               float b) {
  glUniform2f(location, a, b);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               float a,
                               float b,
                               float c) {
  glUniform3f(location, a, b, c);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               float a,
                               float b,
                               float c,
                               float d) {
  glUniform4f(location, a, b, c, d);
}

/* ------------------------------------------------------------------------- */

#ifdef USE_TWODEE
void ShaderProgram::SetUniform(GLint location, const QMatrix4<float>& matrix) {
  glUniformMatrix4fv(location, 1, GL_FALSE, &matrix(0, 0));
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location, const QVector3<float>& v) {
  glUniform3fv(location, 1, &v[0]);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location, const QVector2<float>& v) {
  glUniform2fv(location, 1, &v[0]);
}
#endif

/* ------------------------------------------------------------------------- */

ShaderProgram *ShaderProgram::FromFiles(const std::string &vertex_path,
                                        const std::string &fragment_path,
                                        const std::string &geo_path,
                                        const char *outputs[]) {
  ShaderProgram *program = new ShaderProgram();
  program->SetFiles(vertex_path, fragment_path, geo_path, outputs);
  return program;
}

/* ------------------------------------------------------------------------- */

ShaderProgram *ShaderProgram::FromSource(const std::string &vertex_source,
                                        const std::string &fragment_source,
                                        const std::string &geo_source,
                                        const char *outputs[]) {
  ShaderProgram *program = new ShaderProgram();
  program->SetSource(vertex_source, fragment_source, geo_source, outputs);
  return program;
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetSource(const std::string &vertex_source,
                              const std::string &fragment_source,
                              const std::string &geo_source,
                              const char *outputs[]) {
  vertex_shader_id = OpenGL::CompileGLSL(GL_VERTEX_SHADER, vertex_source.c_str());
  fragment_shader_id = OpenGL::CompileGLSL(GL_FRAGMENT_SHADER, fragment_source.c_str());
  geo_shader_id = 0;

#ifndef ANDROID
  // OpenGL ES 2.0 doesn't support geometry shaders.
  if (geo_source.length() > 0) {
    geo_shader_id = OpenGL::CompileGLSL(GL_GEOMETRY_SHADER, geo_source.c_str());
  }
#endif

  // Link them together
  shader_pid = glCreateProgram();

#ifndef ANDROID
  // OpenGL ES 2.0 doesn't support multiple outputs.
  if (outputs) {
    const char **output = outputs;
    for (int i = 0; *output != NULL; ++i, ++output) {
      glBindFragDataLocation(shader_pid, i, *output);
      std::cerr << *output << " is color " << i << std::endl;
    } 
  }
#endif

  OpenGL::LinkGLSL(shader_pid, vertex_shader_id, fragment_shader_id, geo_shader_id);

  int nuniforms;
  glGetProgramiv(shader_pid, GL_ACTIVE_UNIFORMS, &nuniforms);
  for (int i = 0; i < nuniforms; ++i) {
    int length;
    int size;
    GLenum type;
    char name[256];
    name[255] = '\0';
    glGetActiveUniform(shader_pid, i, 255, &length, &size, &type, name);

    GLint loc = glGetUniformLocation(shader_pid, name);
    uniform_locations.insert(std::pair<std::string, GLint>(name, loc));
  }

#if !NDEBUG
  std::map<std::string, GLint>::const_iterator i;
  std::cerr << "Uniforms:" << std::endl;
  for (i = uniform_locations.begin(); i != uniform_locations.end(); ++i) {
    std::cerr << i->first << " -> " << i->second << std::endl;
  }
#endif
}

/* ------------------------------------------------------------------------- */

bool ShaderProgram::IsBound() const {
  return is_bound;
}

/* ------------------------------------------------------------------------- */

#ifdef USE_CS455
void ShaderProgram::SetUniform(const char *name,
                               const Matrix4& matrix) {
  SetUniform(GetUniformLocation(name), matrix);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name,
                               const Vector4& v) {
  SetUniform(GetUniformLocation(name), v);
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               const Matrix4& matrix) {
  glUniformMatrix4fv(location, 1, GL_FALSE, &matrix(0, 0));
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location,
                               const Vector4& v) {
  glUniform4fv(location, 1, &v[0]);
}
#endif

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(GLint location, const Texture& texture) {
  SetUniform(location, texture.GetUnit());
}

/* ------------------------------------------------------------------------- */

void ShaderProgram::SetUniform(const char *name, const Texture& texture) {
  SetUniform(GetUniformLocation(name), texture.GetUnit());
}

/* ------------------------------------------------------------------------- */

}
