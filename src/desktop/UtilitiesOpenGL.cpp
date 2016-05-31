#include <iostream>

#include "Log.h"
#include "Utilities.h"
#include "UtilitiesOpenGL.h"

namespace td {

/* ------------------------------------------------------------------------- */

#ifndef ANDROID
bool OpenGL::is_glew_initialized = false;
#endif

/* ------------------------------------------------------------------------- */

template<> const GLenum OpenGL::Types<unsigned char>::type = GL_UNSIGNED_BYTE;
template<> const GLenum OpenGL::Types<char>::type = GL_BYTE;
template<> const GLenum OpenGL::Types<unsigned short>::type = GL_UNSIGNED_SHORT;
template<> const GLenum OpenGL::Types<short>::type = GL_SHORT;
template<> const GLenum OpenGL::Types<unsigned int>::type = GL_UNSIGNED_INT;
template<> const GLenum OpenGL::Types<int>::type = GL_INT;
template<> const GLenum OpenGL::Types<float>::type = GL_FLOAT;
#ifndef ANDROID
template<> const GLenum OpenGL::Types<double>::type = GL_DOUBLE;
#endif

/* ------------------------------------------------------------------------- */

#ifndef ANDROID
template<> const GLenum OpenGL::Channels<1>::mode = GL_RED;
#endif
template<> const GLenum OpenGL::Channels<2>::mode = GL_LUMINANCE_ALPHA;
template<> const GLenum OpenGL::Channels<3>::mode = GL_RGB;
template<> const GLenum OpenGL::Channels<4>::mode = GL_RGBA;
 
/* ------------------------------------------------------------------------- */

GLuint OpenGL::CompileGLSLFile(GLenum shader_type, const char *path) {
  char *code = Utilities::FileToString(std::string(path));
  GLuint sid = CompileGLSL(shader_type, code);
  delete[] code;
  return sid;
}

/* ------------------------------------------------------------------------- */

GLuint OpenGL::CompileGLSL(GLenum shader_type, const char *src) {
  GLint succeeded;
  GLuint sid;

#if 0
  std::string glsl_version = std::string((const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
  glsl_version = glsl_version.substr(0, glsl_version.find_first_of(" "));

  std::string older_src;
  if (glsl_version == "1.30") {
    older_src = src;

    // Replace #version line with #version 130
    size_t i = older_src.find_first_of("\n");
    older_src = older_src.replace(0, i, "#version 130");

    src = older_src.c_str();
    std::cerr << "src: " << src << std::endl;
  }
#endif

  sid = glCreateShader(shader_type);
  // TODO why need a cast?
#ifdef ANDROID
  glShaderSource(sid, 1, &src, NULL);
#else
  glShaderSource(sid, 1, (const GLchar **) &src, NULL);
#endif
  glCompileShader(sid);
  glGetShaderiv(sid, GL_COMPILE_STATUS, &succeeded);

  GLint log_length;
  glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &log_length);

  if (log_length > 1) {
    char *log = new char[log_length];
    GLint chars_written;
    glGetShaderInfoLog(sid, log_length, &chars_written, log);
    std::cerr << log << std::endl;
    delete[] log;
  }

  if (succeeded != GL_TRUE) {
    Log::Error(src);
    Log::Error("Something went wrong with the shader.");
  }

  return sid;  
}

/* ------------------------------------------------------------------------- */

void OpenGL::LinkGLSL(GLuint pid, GLuint vertex_sid, GLuint fragment_sid, GLuint geo_sid) {
  glAttachShader(pid, vertex_sid);
  if (geo_sid) {
    glAttachShader(pid, geo_sid);
    /* OpenGL::CheckError("before program parameters"); */
    /* glProgramParameteriEXT(pid, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS); */
    /* glProgramParameteriEXT(pid, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP); */
    /* glProgramParameteriEXT(pid, GL_GEOMETRY_VERTICES_OUT_EXT, 4); */
    /* OpenGL::CheckError("after program parameters"); */
  }
  glAttachShader(pid, fragment_sid);
  glLinkProgram(pid);

  GLint succeeded;
  glGetProgramiv(pid, GL_LINK_STATUS, &succeeded);
  if (succeeded != GL_TRUE) {
    GLint log_length;
    glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length) {
      char *log = new char[log_length];
      GLint chars_written;
      glGetProgramInfoLog(pid, log_length, &chars_written, log);
      Log::Error(log);
      delete[] log;
    } else {
      Log::Error("Something went wrong with the shader.");
    }
  }
}

/* ------------------------------------------------------------------------- */

void OpenGL::CheckError(const std::string& label) {
  for (GLint error = glGetError(); error; error = glGetError()) {
    Log::Error("%s: %x", label.c_str(), error);
    if (error == 0x500) {
      Log::Error("GL_INVALID_ENUM");
    } else if (error == 0x501) {
      Log::Error("GL_INVALID_VALUE");
    } else if (error == 0x502) {
      Log::Error("GL_INVALID_OPERATION");
    } else if (error == 0x506) {
      Log::Error("GL_INVALID_FRAMEBUFFER_OPERATION");
      GLenum fbo_error = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
      if (fbo_error == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
        Log::Error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
      } else if (fbo_error == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
        Log::Error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
      } else if (fbo_error == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
        Log::Error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
      } else if (fbo_error == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
        Log::Error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
      } else if (fbo_error == GL_FRAMEBUFFER_UNSUPPORTED) {
        Log::Error("GL_FRAMEBUFFER_UNSUPPORTED");
      }
    }
  }  
}

/* ------------------------------------------------------------------------- */

}
