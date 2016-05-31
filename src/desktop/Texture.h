#ifndef TEXTURE_H
#define TEXTURE_H

#include "Configuration.h"
#include "twodee/MessagedException.h"
#include "UtilitiesOpenGL.h"

namespace td {

// TODO
// mipmaps
// non power of 2

/* ------------------------------------------------------------------------- */

class Texture {
  public:
    enum wrap_t {
      REPEAT,
      MIRRORED_REPEAT,
      CLAMP_TO_EDGE,
      CLAMP_TO_BORDER
    };

    enum channels_t {
      GRAYSCALE = 1,
      LUMINANCE_ALPHA = 2,
      RGB = 3,
      RGBA = 4,
      DEPTH = 5
    };

    enum target_t {
      ONE,
      TWO,
      THREE,
      CUBEMAP
    };

    enum minification_t {
      MINIFY_LINEAR,
      MINIFY_NEAREST,
      MINIFY_NEAREST_ON_NEAREST_MIPMAP,
      MINIFY_LINEAR_ON_NEAREST_MIPMAP,
      MINIFY_NEAREST_BETWEEN_MIPMAPS,
      MINIFY_LINEAR_BETWEEN_MIPMAPS
    };

    enum magnification_t {
      MAGNIFY_LINEAR,
      MAGNIFY_NEAREST
    };

    enum type_t {
      UNSIGNED_BYTE,
      BYTE,
      UNSIGNED_SHORT,
      SHORT,
      UNSIGNED_INT,
      INT,
      FLOAT,
      DOUBLE
    };

    Texture(int texture_unit = -1);
    template<typename T> Texture &Upload(int width, const T *texels);
    template<typename T> Texture &Upload(int width, int height, const T *texels);
    template<typename T> Texture &Upload(int width, int height, const T *left, const T *right, const T *down, const T *up, const T *back, const T *front);
    template<typename T> Texture &Upload(int width, int height, int depth, const T *texels);
    Texture &Channels(channels_t channels);

    Texture &Allocate(int width, int height);

    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    int GetID() const;

    /**
     Sets the texture's wrap mode, which determines how an out-of-bounds
     texture coordinate is remapped to a legal texture coordinates.
     @param mode The new wrap mode.
     @param dim The texture dimension to modify. Use -1 to modify all
     dimensions.
     @return The modified texture.
     */
    Texture &Wrap(wrap_t mode, int dim = -1);

    /**
     Sets interpolation mode for when one texel spans several pixels.
     @param filter Interpolation mode.
     @return The modified texture.
     */
    Texture &Magnify(magnification_t filter);

    /**
     Sets interpolation mode for when several texels cover one pixel.
     @param filter Interpolation mode.
     @return The modified texture.
     */
    Texture &Minify(minification_t filter);

    /**
     Make texture's hardware unit active and bind this texture to the unit's
     corresponding texture target. Assumes context is current.
     */
    void Bind();

    /**
     Make texture's hardware unit active and unbinds this texture from the
     texture target. Assumes context is current.
     */
    void Unbind();

    void Delete();

    int GetUnit() const;
    channels_t GetChannels() const;
    void SetParameters();

  private:
    Texture &Target(target_t target);

    channels_t channels;

    int texture_unit;
    GLuint texture_id;
    GLenum gl_target;
    GLenum gl_channels;

    GLenum gl_minification;
    GLenum gl_magnification;
    GLenum gl_wrap_modes[3];

    bool are_parameters_set;

    int dims[3];

    static int last_unit_assigned;
};

/* ------------------------------------------------------------------------- */

template<typename T>
Texture &Texture::Upload(int width, const T *texels) {
  Target(ONE);
  SetParameters();
  glTexImage1D(gl_target, 0, gl_channels, width, 0, gl_channels, OpenGL::Types<T>::type, texels);
  dims[0] = width;
  return *this;
}

/* ------------------------------------------------------------------------- */

template<typename T>
Texture &Texture::Upload(int width, int height, const T *texels) {
  Target(TWO);
  SetParameters();
  glTexImage2D(gl_target, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, texels);
  dims[0] = width;
  dims[1] = height;
  return *this;
}

/* ------------------------------------------------------------------------- */

template<typename T> Texture &Texture::Upload(int width, int height, int depth, const T *texels) {
  Target(THREE);
  SetParameters();
  glTexImage3D(gl_target, 0, gl_channels, width, height, depth, 0, gl_channels, OpenGL::Types<T>::type, texels);
  dims[0] = width;
  dims[1] = height;
  dims[2] = depth;
  return *this;
}

/* ------------------------------------------------------------------------- */

template<typename T> Texture &Texture::Upload(int width, int height, const T *left, const T *right, const T *down, const T *up, const T *back, const T *front) {
  Target(CUBEMAP); 
  SetParameters();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, left);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, right);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, down);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, up);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, back);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, gl_channels, width, height, 0, gl_channels, OpenGL::Types<T>::type, front);
  return *this;
}

/* ------------------------------------------------------------------------- */

}

#endif
