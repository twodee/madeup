#ifndef UTILITIESOPENGL_H
#define UTILITIESOPENGL_H

#define GL_GLEXT_PROTOTYPES
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <string>
#include <vector>

/* #define GL_GLEXT_PROTOTYPES */
/* #ifdef ANDROID */
/* #include <GLES2/gl2.h> */
/* #include <GLES2/gl2ext.h> */
/* #else */
/* #include <GL/glew.h> */
/* #ifdef __APPLE__ */
/* #define GL3_PROTOTYPES */
/* #include <OpenGL/gl3.h> */
/* #undef glGenVertexArrays */
/* #define glGenVertexArrays glGenVertexArraysAPPLE */
/* #undef glBindVertexArray */
/* #define glBindVertexArray glBindVertexArrayAPPLE */
/* #define */
/* #endif */
/* #endif */

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A container for several OpenGL utility methods.
 */
class OpenGL {
  public:

    template<typename T>
    class Types {
      public:
        static const GLenum type;
    };

    template<int ndims>
    class Channels {
      public:
        static const GLenum mode;
    };

    /**
     Compiles the GLSL code in the specified file and returns the OpenGL shader
     object ID.
     @param shader_type The type of shader, GL_VERTEX_SHADER,
     GL_FRAGMENT_SHADER, or GL_GEOMETRY_SHADER
     @param path The name of the shader file
     @return Shader object ID
     */
    static GLuint CompileGLSLFile(GLenum shader_type,
                                  const char *path);

    /**
     Compiles the specified GLSL code and returns the OpenGL shader object ID.
     @param shader_type The type of shader, GL_VERTEX_SHADER,
     GL_FRAGMENT_SHADER, or GL_GEOMETRY_SHADER
     @param src The shader code
     @return Shader object ID
     */
    static GLuint CompileGLSL(GLenum shader_type,
                              const char *src);

    /**
     Links the vertex, fragment, and geometry shaders together in the specified
     shader program.
     @param pid Shader program ID
     @param vertex_sid Vertex shader object ID
     @param fragment_sid Fragment shader object ID
     @param geo_sid Geometry shader object ID
     */
    static void LinkGLSL(GLuint pid,
                         GLuint vertex_sid,
                         GLuint fragment_sid,
                         GLuint geo_sid = 0);

    /**
     Checks for an OpenGL error. If one has been raised, prints the specified
     label.
     @param label Text to print if error has been raised
     */
    static void CheckError(const std::string& label);

#ifndef ANDROID
    static void SetupGLEW();
    static bool is_glew_initialized;
#endif
};


/* ------------------------------------------------------------------------- */

}

#endif
