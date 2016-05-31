#include <cassert>
#include <iostream>
#include <vector>

#include "UtilitiesOpenGL.h"
#include "VertexArray.h"

namespace td {

/* ------------------------------------------------------------------------- */

VertexArray::VertexArray(const ShaderProgram& program,
                         const VertexAttributes& attributes)
  : program(program),
    attributes(attributes) {
#ifdef USE_VAO
  OpenGL::CheckError("before vertex array");
  glGenVertexArrays(1, &vertex_aid);
  OpenGL::CheckError("generated vertex array");
  glBindVertexArray(vertex_aid);
  OpenGL::CheckError("bound vertex array");
  for (int i = 0; i < attributes.GetAttributeCount(); ++i) {
    const VertexAttribute *attribute = attributes.GetAttribute(i);
    GLint loc = glGetAttribLocation(this->program.GetID(), attribute->name.c_str());
    OpenGL::CheckError("got attrib location");
    if (loc < 0) {
      std::cerr << "Attribute " << attribute->name << " is not referenced in the shaders." << std::endl;
    } else {
      glBindBuffer(GL_ARRAY_BUFFER, attribute->bid);
      OpenGL::CheckError("bound buffer");
      glVertexAttribPointer(loc, attribute->ncomponents, GL_FLOAT, GL_FALSE, 0, 0);
      OpenGL::CheckError("pointer");
      glEnableVertexAttribArray(loc);
      OpenGL::CheckError("enable");
    }
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());
  glBindVertexArray(0);
#else
  locations = new GLint[attributes.GetAttributeCount()];
  for (int i = 0; i < attributes.GetAttributeCount(); ++i) {
    const VertexAttribute *attribute = attributes.GetAttribute(i);
    locations[i] = glGetAttribLocation(this->program.GetID(), attribute->name.c_str());
    if (locations[i] < 0) {
      std::cerr << "Attribute " << attribute->name << " is not referenced in the shaders." << std::endl;
    } else {
      glBindBuffer(GL_ARRAY_BUFFER, attribute->bid);
      glVertexAttribPointer(locations[i], attribute->ncomponents, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(locations[i]);
    }
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());
#endif
}

/* ------------------------------------------------------------------------- */

VertexArray::~VertexArray() {
#ifndef USE_VAO
  delete[] locations;
#endif
}

/* ------------------------------------------------------------------------- */

void VertexArray::Bind() {
#ifdef USE_VAO
  glBindVertexArray(vertex_aid);
#else
  for (int i = 0; i < attributes.GetAttributeCount(); ++i) {
    const VertexAttribute *attribute = attributes.GetAttribute(i);
    if (locations[i] >= 0) {
      glBindBuffer(GL_ARRAY_BUFFER, attribute->bid);
      glVertexAttribPointer(locations[i], attribute->ncomponents, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(locations[i]);
    }
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attributes.GetIndexBufferID());
#endif
  is_bound = true;
}

/* ------------------------------------------------------------------------- */

void VertexArray::Unbind() {
#ifdef USE_VAO
  glBindVertexArray(0);
#else
  for (int i = 0; i < attributes.GetAttributeCount(); ++i) {
    /* const VertexAttribute *attribute = attributes.GetAttribute(i); */
    if (locations[i] >= 0) {
      glDisableVertexAttribArray(locations[i]);
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
  is_bound = false;
}

/* ------------------------------------------------------------------------- */

void VertexArray::DrawSequence(GLenum mode) {
  assert(is_bound);
  glDrawArrays(mode, 0, attributes.GetVertexCount()); 
}

/* ------------------------------------------------------------------------- */

void VertexArray::DrawSequence(GLenum mode, int first, int count) {
  assert(first >= 0 && first < attributes.GetVertexCount());
  assert(first + count <= attributes.GetVertexCount());
  glDrawArrays(mode, first, count); 
}

/* ------------------------------------------------------------------------- */

#ifndef ANDROID
void VertexArray::DrawSequence(GLenum mode, int ninstances) {
  glDrawArraysInstanced(mode, 0, attributes.GetVertexCount(), ninstances); 
}
#endif

/* ------------------------------------------------------------------------- */

#ifndef ANDROID
void VertexArray::DrawSequence(GLenum mode, int first, int count, int ninstances) {
  assert(first >= 0 && first < attributes.GetVertexCount());
  assert(first + count <= attributes.GetVertexCount());
  glDrawArraysInstanced(mode, first, count, ninstances); 
}
#endif

/* ------------------------------------------------------------------------- */

void VertexArray::DrawIndexed(GLenum mode) {
  assert(is_bound);
  glDrawElements(mode, attributes.GetIndexCount(), GL_UNSIGNED_INT, 0);
}

/* ------------------------------------------------------------------------- */

}
