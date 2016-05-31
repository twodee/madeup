#include <iostream>

#include "Texture.h"

namespace td {

/* ------------------------------------------------------------------------- */

int Texture::last_unit_assigned = -1;

/* ------------------------------------------------------------------------- */

Texture::Texture(int texture_unit)
  : are_parameters_set(false) {

  // Assign texture to next available unit if unit not specified.
  if (texture_unit < 0) {
    ++last_unit_assigned;
    this->texture_unit = last_unit_assigned;
  } else {
    this->texture_unit = texture_unit;
  }

  // Create ID to uniquely identify texture to GPU.
  glGenTextures(1, &this->texture_id);

  dims[0] = dims[1] = dims[2] = 0;

  Channels(RGB);
  Wrap(REPEAT);
  Magnify(MAGNIFY_LINEAR);
  Minify(MINIFY_LINEAR);
}

/* ------------------------------------------------------------------------- */

void Texture::Bind() {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(gl_target, texture_id);
}

/* ------------------------------------------------------------------------- */

void Texture::Unbind() {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(gl_target, 0);
}

/* ------------------------------------------------------------------------- */

Texture &Texture::Channels(channels_t channels) {
  this->channels = channels;

  switch (channels) {
    case DEPTH:
      gl_channels = GL_DEPTH_COMPONENT;
      break;
#ifndef ANDROID
    case GRAYSCALE:
      gl_channels = GL_RED;
      break;
#endif
    case LUMINANCE_ALPHA:
      gl_channels = GL_LUMINANCE_ALPHA;
      break;
    case RGB:
      gl_channels = GL_RGB;
      break;
    case RGBA:
      gl_channels = GL_RGBA;
      break;
    default:
      throw MessagedException("illegal channels");
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

Texture &Texture::Wrap(wrap_t mode, int dim) {
  GLenum gl_mode;
  switch (mode) {
    case REPEAT:
      gl_mode = GL_REPEAT;
      break;
#ifndef ANDROID
    case CLAMP_TO_BORDER:
      gl_mode = GL_CLAMP_TO_BORDER;
      break;
#endif
    case CLAMP_TO_EDGE:
      gl_mode = GL_CLAMP_TO_EDGE;
      break;
    case MIRRORED_REPEAT:
      gl_mode = GL_MIRRORED_REPEAT;
      break;
    default:
      throw MessagedException("unknown wrap mode");
  }

  for (int d = 0; d < 3; ++d) {
    if (dim == d || dim == -1) {
      gl_wrap_modes[d] = gl_mode;
    }
  }

  if (are_parameters_set) {
    SetParameters();
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

Texture &Texture::Magnify(magnification_t filter) {
  switch (filter) {
    case MAGNIFY_LINEAR:
      gl_magnification = GL_LINEAR;
      break;
    case MAGNIFY_NEAREST:
      gl_magnification = GL_NEAREST;
      break;
    default:
      throw MessagedException("illegal filter for magnify");
  }

  if (are_parameters_set) {
    SetParameters();
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

Texture &Texture::Minify(minification_t filter) {
  switch (filter) {
    case MINIFY_LINEAR:
      gl_minification = GL_LINEAR;
      break;
    case MINIFY_NEAREST:
      gl_minification = GL_NEAREST;
      break;
    case MINIFY_NEAREST_ON_NEAREST_MIPMAP:
      gl_minification = GL_NEAREST_MIPMAP_NEAREST;
      break;
    case MINIFY_LINEAR_ON_NEAREST_MIPMAP:
      gl_minification = GL_LINEAR_MIPMAP_NEAREST;
      break;
    case MINIFY_NEAREST_BETWEEN_MIPMAPS:
      gl_minification = GL_NEAREST_MIPMAP_LINEAR;
      break;
    case MINIFY_LINEAR_BETWEEN_MIPMAPS:
      gl_minification = GL_LINEAR_MIPMAP_LINEAR;
      break;
    default:
      throw MessagedException("illegal filter for minify");
  }

  if (are_parameters_set) {
    SetParameters();
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

Texture &Texture::Target(target_t target) {
  switch (target) {
#ifndef ANDROID
    case ONE:
      gl_target = GL_TEXTURE_1D;
      break;
    case THREE:
      gl_target = GL_TEXTURE_3D;
      break;
    case CUBEMAP:
      gl_target = GL_TEXTURE_CUBE_MAP;
      break;
#endif
    case TWO:
      gl_target = GL_TEXTURE_2D;
      break;
    default:
      throw MessagedException("illegal target for upload");
  }
  
  return *this;
}

/* ------------------------------------------------------------------------- */

void Texture::Delete() {
  glDeleteTextures(1, &this->texture_id); 
}

/* ------------------------------------------------------------------------- */

int Texture::GetUnit() const {
  return texture_unit;
}

/* ------------------------------------------------------------------------- */

void Texture::SetParameters() {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(gl_target, texture_id);
  glTexParameteri(gl_target, GL_TEXTURE_MIN_FILTER, gl_minification);
  glTexParameteri(gl_target, GL_TEXTURE_MAG_FILTER, gl_magnification);
  glTexParameteri(gl_target, GL_TEXTURE_WRAP_S, gl_wrap_modes[0]);
  glTexParameteri(gl_target, GL_TEXTURE_WRAP_T, gl_wrap_modes[1]);
#ifndef ANDROID
  glTexParameteri(gl_target, GL_TEXTURE_WRAP_R, gl_wrap_modes[2]);
#endif
  are_parameters_set = true;
}

/* ------------------------------------------------------------------------- */

int Texture::GetWidth() const {
  return dims[0];
}

/* ------------------------------------------------------------------------- */

int Texture::GetHeight() const {
  return dims[1];
}

/* ------------------------------------------------------------------------- */

int Texture::GetDepth() const {
  return dims[2];
}

/* ------------------------------------------------------------------------- */

int Texture::GetID() const {
  return texture_id;
}

/* ------------------------------------------------------------------------- */

Texture &Texture::Allocate(int width, int height) {
  unsigned char *p = NULL;
  return Upload(width, height, p);
}

/* ------------------------------------------------------------------------- */

Texture::channels_t Texture::GetChannels() const {
  return channels;
}

/* ------------------------------------------------------------------------- */

}
