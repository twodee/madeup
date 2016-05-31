#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <map>
#include <ostream>

#include "Configuration.h"
#include "UtilitiesOpenGL.h"
#include "ShaderProgram.h"
#include "VertexAttributes.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 The marriage between a vertex attribute set and a shader program.
 */
class VertexArray {
  public:
    /**
     Create a pairing between the specified shader program and a set of vertex
     attributes.
     @param program The program to pair
     @param attributes The vertex attributes to pair
     */
    VertexArray(const ShaderProgram& program,
                const VertexAttributes& attributes);

    /**
     Frees the resources consumed by this vertex array.
     */
    ~VertexArray();

    /**
     Make this array active.
     */
    void Bind();

    /**
     Make this array inactive.
     */
    void Unbind();

    /**
     Draw geometry sequentially from this array.
     @param mode The kind of geometric primitive to draw
     */
    void DrawSequence(GLenum mode);

    /**
     Draw geometry sequentially from this array, starting at the specified
     vertex and consuming only the specified number of vertices.
     @param mode The kind of geometric primitive to draw
     @param first Index of starting vertex
     @param count Number of vertices to draw
     */
    void DrawSequence(GLenum mode, int first, int count);

#ifndef ANDROID
    /**
     Draw the specified number of instances of this geometry sequentially from
     this array.
     @param mode The kind of geometric primitive to draw
     @param ninstances Number of times to draw each primitive
     */
    void DrawSequence(GLenum mode, int ninstances);
#endif

#ifndef ANDROID
    /**
     Draw the specified number of instances of this geometry sequentially from
     this array, starting at the specified vertex and consuming only the
     specified number of vertices.
     @param mode The kind of geometric primitive to draw
     @param first Index of starting vertex
     @param count Number of vertices to draw
     @param ninstances Number of times to draw each primitive
     */
    void DrawSequence(GLenum mode, int first, int count, int ninstances);
#endif

    /**
     Draw geometry using the indices.
     @param mode The kind of geometric primitive to draw
     */
    void DrawIndexed(GLenum mode);

  private:
    /** Is the vertex array bound? */
    bool is_bound;

    /** Shader program used to draw geometry */
    const ShaderProgram& program;

    /** Attributes used to feed data to shaders */
    const VertexAttributes& attributes;

#ifdef USE_VAO
    /** OpenGL vertex array ID */
    GLuint vertex_aid;
#else
    /** Locations of in variables within shader program */
    GLint *locations;
#endif
};

/* ------------------------------------------------------------------------- */

}

#endif
