#ifndef VERTEXATTRIBUTES_H
#define VERTEXATTRIBUTES_H

#include <vector>

#include "twodee/Utilities.h"
#include "UtilitiesOpenGL.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A single named vertex attribute.
 */
struct VertexAttribute {
  /**
   Creates a new vertex attribute.
   @param name Name of attribute
   @param nelements Number of vertices
   @param ncomponents Number of components in this attribute
   @param buffer Attributes buffer
   @param usage OpenGL usage hint
   */
  VertexAttribute(const std::string& name,
                  int nelements,
                  int ncomponents,
                  const float *buffer,
                  GLenum usage);

  /**
   Frees resources consumed by this attribute.
   */
  ~VertexAttribute();

  /**
   Updates the VBO to hold the new values stored in <code>buffer</code>
   @param buffer Buffer of new values, same size as original
   */
  void Update(float *buffer);

  /** Attribute name */
  std::string name;

  /** OpenGL buffer ID */
  GLuint bid;

  /** Number of components in a single vertex's attribute */
  int ncomponents;

  /** Number of vertices/elements */
  int nvertices;
};

/* ------------------------------------------------------------------------- */

/**
 A set of named vertex attributes.
 */
class VertexAttributes {
  public:
    /**
     Creates a new set of vertex attributes.
     */
    VertexAttributes();

    /**
     Frees the resources consumed by this vertex attributes set.
     */
    ~VertexAttributes();

    /**
     Gets the number of vertices.
     @return Number of vertices
     */
    int GetVertexCount() const;

    /**
     Gets the number of indices used to define the connectivity of the vertices.
     @return Number of indices
     */
    int GetIndexCount() const;

    /**
     Gets the index buffer's ID.
     @return Index buffer ID
     */
    GLuint GetIndexBufferID() const;

    /**
     Get the number of attributes that have been added to this array.
     @return The number of attributes
     */
    int GetAttributeCount() const;

    /**
     Gets the attribute at the specified index.
     @param i Index, where the first attributed added is at index 0
     @return The vertex attribute at index <code>i</code>
     */
    VertexAttribute *GetAttribute(int i) const;

    /**
     Gets the attribute with the specified name.
     @param name Name of attribute
     @return The vertex attribute with name <code>name</code>
     */
    VertexAttribute *GetAttribute(const std::string& name) const;

    /**
     Add an attribute to the vertex array.
     @param name Name of attribute, as referenced in the vertex shader
     @param nelements Number of elements in attribute array; all attributes must have the same number of elements
     @param ncomponents Number of components in this vertex attribute
     @param buffer Location of client buffer contain attribute data
     @param usage Usage flag used by driver to determine best location of buffer
     */
    void AddAttribute(const std::string& name, int nelements, int ncomponents, const float *buffer, GLenum usage = GL_STATIC_DRAW);

    /**
     Add indices describing how the vertices connect to form geometry.
     @param nindices Number of indices
     @param buffer Buffer containing indices
     @param usage usage Hint of OpenGL storage
     */
    void AddIndices(int nindices, const int *buffer, GLenum usage = GL_STATIC_DRAW);

  private:
    /** The number of vertices in the vertex array */
    int nvertices;

    /** The number of indices describing the connectivity of the vertices */
    int nindices;

    /** Index buffer ID */
    GLuint index_bid;

    /** Set of vertex attributes */
    std::vector<VertexAttribute *> attributes;
};

/* ------------------------------------------------------------------------- */

}

#endif
