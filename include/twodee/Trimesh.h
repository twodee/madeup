#ifndef TRIMESH_H
#define TRIMESH_H

#include <vector>

#include "Ball.h"
#include "Box.h"
#include "NField.h"
#include "QVector3.h"
#include "QVector4.h"
#include "QMatrix4.h"

namespace td {

/* ------------------------------------------------------------------------- */

/**
 A mesh composed of triangles.
 */
class Trimesh {
  friend std::ostream& operator<<(std::ostream& stream, const Trimesh& mesh);

  public:
    /**
     Loads a mesh from the specified file
     @param path Path to mesh file
     */
    Trimesh(const std::string& path);

    /**
     Creates a mesh with the specified number of vertices and faces, but
     assigns no geometry data.
     @param nvertices Number of vertices
     @param nfaces Number of faces
     */
    Trimesh(int nvertices, int nfaces);

    Trimesh(const Trimesh& other);

    /**
     Creates a mesh with the vertices and faces.
     @param positions Vector of vertices
     @param faces Vector of faces
     */
    Trimesh(const std::vector<td::QVector3<float> >& positions, const std::vector<td::QVector3<int> >& faces);

    /**
     Creates a mesh from the given list of triangles.
     @param ntriangles Number of triangles whose vertices are listed in <code>positions</code>.
     @param position List of vertex coordinates.
     */
    Trimesh(int ntriangles, const float *positions);

    /**
     Frees resources consumed by mesh.
     */
    ~Trimesh();

    /**
     Gets the number of vertices in mesh.
     @return Number of vertices
     */
    int GetVertexCount() const;

    /**
     Gets the number of faces in mesh.
     @return Number of faces
     */
    int GetFaceCount() const;

    /**
     Gets the positions buffer.
     @return Positions buffer
     */
    const float *GetPositions() const;

    /**
     Gets the positions buffer.
     @return Positions buffer
     */
    float *GetPositions();

    /**
     Gets the face connectivity buffer.
     @return Faces buffer
     */
    const int *GetFaces() const;

    /**
     Gets the face connectivity buffer.
     @return Faces buffer
     */
    int *GetFaces();

    /**
     Gets the per-vertex normals buffer.
     @return Positions buffer
     */
    const float *GetNormals() const;

    /**
     Gets the per-vertex colors buffer.
     @return Positions buffer
     */
    const float *GetColors() const;
    float *AllocateVertexColors(int nchannels = 3);

    const float *GetVertexMetas() const;
    float *AllocateVertexMetas(int nmetas);
    void MigrateVertexMetasToColors(int r, int g, int b);

    /**
     Gets the per-vertex ambient occlusion buffer.
     @return Occlusion buffer
     */
    const float *GetOcclusion() const;

    /**
     Gets the per-vertex 2-D texture coordinates buffer.
     @return 2-D texture coordinates buffer
     */
    const float *GetTex2DCoords() const;
    const float *GetTex3DCoords() const;
    const float *GetTex1DCoords() const;

    /**
     Gets a sphere which completely bounds this mesh. It's centered on the
     mesh's centroid.
     @return Bounding sphere
     */
    const Ball<float, 3>& GetBoundingSphere() const;

    /**
     Gets a box which completely bounds this mesh. It snugly encapsulates the
     mesh on each axis.
     @return Bounding box
     */
    const Box<float, 3>& GetBoundingBox() const;

    /**
     Reads mesh in OFF format. Normals are automatically calculated.
     @param path Path to mesh file
     */
    void ReadOff(const std::string& path);

    /**
     Reads mesh in OBJ format. Normals are automatically calculated.
     @param path Path to mesh file
     */
    void ReadObj(const std::string& path);

    /**
     Reads mesh in PLY format. Normals are automatically calculated.
     @param path Path to mesh file
     */
    void ReadPly(const std::string& path);

    /**
     Writes mesh to file in a format determined by the file's extension.
     @param path Path to mesh file
     */
    void Write(const std::string& path);

    /**
     Writes mesh to file in a OFF format.
     @param path Path to mesh file
     */
    void WriteOff(const std::string& path);

    /**
     Writes mesh to file in a OBJ format.
     @param path Path to mesh file
     */
    void WriteObj(const std::string& path);

    void WriteJSON(const std::string& path);

    /**
     Writes mesh to file in a PLY format.
     @param path Path to mesh file
     */
    void WritePly(const std::string& path);

    /**
     Coalesces vertices whose attributes are identical in value. Currently only
     spatial attribute is considered.
     @param ndigits Number of digits used to compose vertex key used for IDing
     */
    void CoalesceVertices(int ndigits);

    /**
     Removes triangles where two or three vertices are coincident.
     */
    void RemoveDegenerateFaces();

    /**
     Generates 1-D texture coordinates for each vertex. The coordinate value is
     computed as dot(s, v.position).
     @param s Vector dotted with position to get X value
     */
    void GenerateTexCoords(const QVector4<float>& s);

    void GenerateFittedTexCoords(int d);
    void GenerateFittedTexCoords();

    /**
     Generates 2-D texture coordinates for each vertex. The coordinate value is
     computed as [dot(s, v.position), dot(t, v.position)].
     @param s Vector dotted with position to get X value
     @param t Vector dotted with position to get Y value
     */
    void GenerateTexCoords(const QVector4<float>& s,
                           const QVector4<float>& t);

    void GenerateTexCoords(const QVector4<float>& s,
                           const QVector4<float>& t,
                           const QVector4<float>& q);

    /**
     Computes bounding geometry and per-vertex normals of mesh.
     */
    void ComputeMeta(bool do_normals = true);

    /**
     Computes per-vertex normals of mesh.
     */
    void ComputeNormals();

    /**
     Computes bounding geometry of mesh.
     */
    void ComputeBounds();

#if 0
    void ComputeAmbientOcclusion(int nrays, float radius);
#endif

    /**
     Reverse ordering of faces.
     */
    void ReverseWinding();

    /**
     Flip direction of normals.
     */
    void FlipNormals();

    /**
     Normalize mesh coordinates so that it lands in [min, max].
     Aspect ratios are maintained.
     @param max Upper bound on positions
     @param min Lower bound on positions
     */
    void Normalize(const QVector3<float>& max = (QVector3<float>(1.0f)),
                   const QVector3<float>& min = (QVector3<float>(0.0f)));

    /**
     Builds a mesh for a UV-sphere. Its centered on the origin and has the
     specified radius.
     @param nslices Number of columns along longitude
     @param nstacks Number of rows along latitude
     @param radius Radius of sphere
     @return Mesh, to be deallocated by caller
     */
    static Trimesh *GetSphere(int nslices,
                              int nstacks,
                              float radius = 1.0f);

    static Trimesh *GetBox(float radius = 1.0f);
    
    /**
     Builds a mesh for a subdivided quadrilateral. Its first point is at the
     origin and each vertex falls on an integral Cartesian coordinate, up to
     [ncols - 1, nrows - 1].
     @param nrows Number of rows on y-axis.
     @param ncols Number of columns on x-axis.
     @return Mesh, to be deallocated by caller
     */
    static Trimesh *GetQuadrilateral(int nrows,
                                     int ncols);

    static Trimesh *GetArrow(const QVector3<float>& axis,
                             float barrel_length,
                             float barrel_radius,
                             float tip_length,
                             float tip_radius,
                             int nrotations);

    /**
     Builds a mesh from an image in which each pixel's RGB-value represents a
     vertex location. Adjacent pixels/vertices are connected. Surface may
     optionally be closed along both x- and y-seams.
     @param quadric_map Image containing 3 channels for vertex locations.
     @param wrap_x True if surface should close along x-dimension.
     @param wrap_y True if surface should close along y-dimension.
     @param generate_texcoords True if 2-D texture coordinates should be generated
     @return Mesh, to be deallocated by caller
     */
    static Trimesh *GetParametric(const NField<float, 2>& quadric_map,
                                  bool wrap_x = false,
                                  bool wrap_y = false,
                                  bool generate_texcoords = false);

    QVector3<float> *GetFaceNormals();

    void DisconnectFaces();

    Trimesh& operator+=(const Trimesh& other);
    Trimesh& operator+=(const QVector3<float>& delta);
    Trimesh& operator*=(float factor);
    Trimesh& operator*=(const QMatrix4<float>& xform);

  private:
    /**
     Null out various attribute buffers.
     */
    void NullBuffers();

    /** Number of vertices in mesh */
    int nvertices;

    /** Number of faces in mesh */
    int nfaces;

    /** Positions of vertices */
    float *positions;

    /** List of connected vertices */
    int *faces;

    /** Per-vertex normals */
    float *normals;

    /** Per-vertex colors */
    float *colors;
    int ncolor_channels;

    float *vertex_metas;
    int nvertex_metas;

    /** Per-vertex 1-D texture coordinates */
    float *tex1D_coords;

    /** Per-vertex 2-D texture coordinates */
    float *tex2D_coords;

    /** Per-vertex 3-D texture coordinates */
    float *tex3D_coords;

    /** Per-vertex occlusion factor */
    float *occlusion;

    /** Bounding sphere of mesh */
    Ball<float, 3> bounding_sphere;

    /** Bounding box of mesh */
    Box<float, 3> bounding_box;

    /** Center point of mesh, average of min and max positions */
    QVector3<float> centroid;
};

/* ------------------------------------------------------------------------- */

}

#endif
