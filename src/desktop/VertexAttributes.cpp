#include <cassert>
#include <cstring>
#include <iostream>

#include "VertexAttributes.h"

namespace td {

/* ------------------------------------------------------------------------- */
/* VERTEXATTRIBUTE                                                           */
/* ------------------------------------------------------------------------- */

VertexAttribute::VertexAttribute(const std::string& name,
                                 int nvertices,
                                 int ncomponents,
                                 const float *buffer,
                                 GLenum usage)
  : ncomponents(ncomponents),
    nvertices(nvertices) {
  this->name = name;

  glGenBuffers(1, &bid);
  glBindBuffer(GL_ARRAY_BUFFER, bid);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nvertices * ncomponents, buffer, usage);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* ------------------------------------------------------------------------- */

VertexAttribute::~VertexAttribute() {
}

/* ------------------------------------------------------------------------- */

void VertexAttribute::Update(float *buffer) {
  glBindBuffer(GL_ARRAY_BUFFER, bid);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * nvertices * ncomponents, buffer);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/* ------------------------------------------------------------------------- */
/* VERTEXARRAY                                                               */
/* ------------------------------------------------------------------------- */

VertexAttributes::VertexAttributes()
  : nvertices(0),
    index_bid(0) {
}

/* ------------------------------------------------------------------------- */

VertexAttributes::~VertexAttributes() {
  std::vector<VertexAttribute *>::iterator attribute;
  for (attribute = attributes.begin();
       attribute != attributes.end();
       ++attribute) {
    delete *attribute;
  }
}

/* ------------------------------------------------------------------------- */

void VertexAttributes::AddAttribute(const std::string& name,
                                    int nvertices,
                                    int ncomponents,
                                    const float *buffer,
                                    GLenum usage) {
  assert(this->nvertices == nvertices || this->nvertices == 0);
  this->nvertices = nvertices;
  VertexAttribute *attribute = new VertexAttribute(name, nvertices, ncomponents, buffer, usage);
  attributes.push_back(attribute);
}

/* ------------------------------------------------------------------------- */

void VertexAttributes::AddIndices(int nindices,
                                  const int *buffer,
                                  GLenum usage) {
  this->nindices = nindices;

  glGenBuffers(1, &index_bid);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_bid);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * nindices, buffer, usage);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/* ------------------------------------------------------------------------- */

int VertexAttributes::GetAttributeCount() const {
  return attributes.size();
}

/* ------------------------------------------------------------------------- */

VertexAttribute *VertexAttributes::GetAttribute(int i) const {
  return attributes[i];
}

/* ------------------------------------------------------------------------- */

VertexAttribute *VertexAttributes::GetAttribute(const std::string& name) const {
  for (unsigned int i = 0; i < attributes.size(); ++i) {
    if (name.compare(attributes[i]->name) == 0) {
      return attributes[i];
    }
  }

  return NULL;
}

/* ------------------------------------------------------------------------- */

int VertexAttributes::GetVertexCount() const {
  return nvertices;
}

/* ------------------------------------------------------------------------- */

int VertexAttributes::GetIndexCount() const {
  return nindices;
}

/* ------------------------------------------------------------------------- */

GLuint VertexAttributes::GetIndexBufferID() const {
  return index_bid;
}

/* ------------------------------------------------------------------------- */

}
