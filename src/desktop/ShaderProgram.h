#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include <string>

#include "Configuration.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector2.h"
#include "Texture.h"
#include "UtilitiesOpenGL.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 An abstraction of an OpenGL shader program object, with facilities for
 setting vertex/fragment/geometry code and uniforms.
 */
class ShaderProgram {
  public:
    ShaderProgram();

    static ShaderProgram *FromFiles(const std::string &vertex_path,
                                    const std::string &fragment_path,
                                    const std::string &geo_path = "",
                                    const char *outputs[] = NULL);

    static ShaderProgram *FromSource(const std::string &vertex_source,
                                     const std::string &fragment_source,
                                     const std::string &geo_source = "",
                                     const char *outputs[] = NULL);

    ~ShaderProgram();

    /**
     Gets ID of shader program.
     @return Shader program ID
     */
    GLuint GetID() const;

    /**
     Gets location of a shader uniform.
     @param name Name of uniform whose location is retrieved
     @return Location of uniform, < 0 if not found
     */
    GLint GetUniformLocation(const char *name);

    /**
     Sets the value of the uniform at the specified location.
     @param location Location of uniform
     @param a Uniform's new value
     */
    void SetUniform(GLint location, float a);

    /**
     Sets the value of the uniform at the specified location.
     @param location Location of uniform
     @param a Uniform's new value
     */
    void SetUniform(GLint location, int a);

    /**
     Sets the value of the vec2 uniform at the specified location.
     @param location Location of uniform
     @param a Uniform's new x-value
     @param b Uniform's new y-value
     */
    void SetUniform(GLint location, float a, float b);

    /**
     Sets the value of the vec3 uniform at the specified location.
     @param location Location of uniform
     @param a Uniform's new x-value
     @param b Uniform's new y-value
     @param c Uniform's new z-value
     */
    void SetUniform(GLint location, float a, float b, float c);

    /**
     Sets the value of the vec4 uniform at the specified location.
     @param location Location of uniform
     @param a Uniform's new x-value
     @param b Uniform's new y-value
     @param c Uniform's new z-value
     @param d Uniform's new w-value
     */
    void SetUniform(GLint location, float a, float b, float c, float d);

    /**
     Sets the value of the named uniform.
     @param name Name of uniform
     @param a Uniform's new value
     */
    void SetUniform(const char *name, float a);

    /**
     Sets the value of the named uniform.
     @param name Name of uniform
     @param a Uniform's new value
     */
    void SetUniform(const char *name, int a);

    /**
     Sets the value of the named vec2 uniform.
     @param name Name of uniform
     @param a Uniform's new x-value
     @param b Uniform's new y-value
     */
    void SetUniform(const char *name, float a, float b);

    /**
     Sets the value of the named vec3 uniform.
     @param name Name of uniform
     @param a Uniform's new x-value
     @param b Uniform's new y-value
     @param c Uniform's new z-value
     */
    void SetUniform(const char *name, float a, float b, float c);

    /**
     Sets the value of the named vec4 uniform.
     @param name Name of uniform
     @param a Uniform's new x-value
     @param b Uniform's new y-value
     @param c Uniform's new z-value
     @param d Uniform's new w-value
     */
    void SetUniform(const char *name, float a, float b, float c, float d);

    /**
     Sets the value of the mat4 uniform at the specified location.
     @param location Location of uniform
     @param matrix Uniform's new value
     */
    void SetUniform(GLint location, const QMatrix4<float>& matrix);

    /**
     Sets the value of the vec3 uniform at the specified location.
     @param location Location of uniform
     @param v Uniform's new value
     */
    void SetUniform(GLint location, const QVector3<float>& v);
    void SetUniform(GLint location, const QVector2<float>& v);

    /**
     Sets the value of the named mat4 uniform.
     @param name Name of uniform
     @param matrix Uniform's new value
     */
    void SetUniform(const char *name, const QMatrix4<float>& matrix);

    /**
     Sets the value of the named vec3 uniform.
     @param name Name of uniform
     @param v Uniform's new value
     */
    void SetUniform(const char *name, const QVector3<float>& v);
    void SetUniform(const char *name, const QVector2<float>& v);

    void SetUniform(GLint location, const Texture& texture);
    void SetUniform(const char *name, const Texture& texture);

    /**
     Enables shader program for drawing.
     */
    void Bind();

    /**
     Disables shader program.
     */
    void Unbind();

    bool IsBound() const;

  private:
    /**
     Compiles and links the code in the specified files into this shader
     program.
     @param vertex_path Path to vertex shader file
     @param fragment_path Path to fragment shader file
     @param geo_path Path to geometry shader file
     @param outputs Names of fragment shader output variables
     */
    void SetShaders(const char *vertex_path,
                    const char *fragment_path,
                    const char *geo_path = NULL,
                    const char *outputs[] = NULL);

    void SetFiles(const std::string& vertex_path,
                  const std::string& fragment_path,
                  const std::string& geo_path = "",
                  const char *outputs[] = NULL);
    void SetSource(const std::string &vertex_source,
                   const std::string &fragment_source,
                   const std::string &geo_source = "",
                   const char *outputs[] = NULL);


    /**
     Compiles and links the specified code into this shader program.
     @param vertex_source Vertex shader code
     @param fragment_source Fragment shader code
     @param geo_source Geometry shader code
     @param outputs Names of fragment shader output variables
     */
    void SetShaders(const std::string &vertex_source,
                    const std::string &fragment_source,
                    const std::string &geo_source = "",
                    const char *outputs[] = NULL);

    /** The shader program used to draw the vertex array */
    GLuint shader_pid;

    /** Mapping between uniform names and uniform locations */
    std::map<std::string, GLint> uniform_locations;

    GLuint vertex_shader_id;
    GLuint fragment_shader_id;
    GLuint geo_shader_id;
    bool is_bound;
};

/* ------------------------------------------------------------------------- */

}

#endif
