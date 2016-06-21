#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#include "twodee/MessagedException.h"
#include "twodee/NField.h"
#include "twodee/QVector2.h"
#include "twodee/Polyline.h"
#include "twodee/Scanner.h"
#include "twodee/Trimesh.h"
#include "twodee/Utilities.h"

using std::map;
using std::pair;
using std::string;

namespace td {

/* ------------------------------------------------------------------------- */

struct vertex_key_t {
  int i_position;
  int i_tex2D_coord;
  int i_normal;

  bool operator<(const vertex_key_t &other) const {
    return i_position < other.i_position || (i_position == other.i_position && (i_tex2D_coord < other.i_tex2D_coord || (i_tex2D_coord == other.i_tex2D_coord && i_normal < other.i_normal)));
  }
};

std::ostream &operator<<(std::ostream &out, const vertex_key_t &key) {
  out << "{" << key.i_position << ", " << key.i_tex2D_coord << ", " << key.i_normal << "}";
  return out;
}

typedef map<vertex_key_t, int> uniquer_t;
typedef pair<vertex_key_t, int> uniquer_pair_t;

/* ------------------------------------------------------------------------- */

Trimesh::Trimesh(const std::string& path) {
  NullBuffers();
  if (path.rfind(".off") == path.length() - 4) {
    ReadOff(path);
  } else if (path.rfind(".obj") == path.length() - 4) {
    ReadObj(path);
  } else if (path.rfind(".ply") == path.length() - 4) {
    ReadPly(path);
  } else {
    throw MessagedException(std::string("illegal mesh format"));
  }
}

/* ------------------------------------------------------------------------- */

Trimesh::Trimesh(int nvertices, int nfaces) :
  nvertices(nvertices),
  nfaces(nfaces),
  positions(new float[nvertices * 3]),
  faces(new int[nfaces * 3]),
  normals(NULL),
  ncolor_channels(0),
  colors(NULL),
  nvertex_metas(0),
  vertex_metas(NULL),
  tex1D_coords(NULL),
  tex2D_coords(NULL),
  tex3D_coords(NULL),
  occlusion(NULL) {
}

/* ------------------------------------------------------------------------- */

Trimesh::Trimesh(const std::vector<td::QVector3<float> >& positions, const std::vector<td::QVector3<int> >& faces) :
  nvertices(positions.size()),
  nfaces(faces.size()),
  positions(new float[nvertices * 3]),
  faces(new int[nfaces * 3]),
  normals(NULL),
  ncolor_channels(0),
  colors(NULL),
  nvertex_metas(0),
  vertex_metas(NULL),
  tex1D_coords(NULL),
  tex2D_coords(NULL),
  tex3D_coords(NULL),
  occlusion(NULL) {

  float *position = this->positions;
  for (vector<QVector3<float> >::const_iterator i = positions.begin(); i != positions.end(); ++i) {
    memcpy(position, &(*i)[0], sizeof(float) * 3);
    position += 3;
  }

  int *face = this->faces;
  for (vector<QVector3<int> >::const_iterator i = faces.begin(); i != faces.end(); ++i) {
    memcpy(face, &(*i)[0], sizeof(int) * 3);
    face += 3;
  }

  ComputeMeta();
}

/* ------------------------------------------------------------------------- */

Trimesh::Trimesh(int ntriangles, const float *positions) :
  nvertices(ntriangles * 3),
  nfaces(ntriangles),
  positions(new float[ntriangles * 3 * 3]),
  faces(new int[ntriangles * 3]),
  normals(NULL),
  ncolor_channels(0),
  colors(NULL),
  nvertex_metas(0),
  vertex_metas(NULL),
  tex1D_coords(NULL),
  tex2D_coords(NULL),
  tex3D_coords(NULL),
  occlusion(NULL) {

  int *face = faces;
  for (int i = 0; i < nfaces * 3; i += 3) {
    face[0] = i;
    face[1] = i + 2;
    face[2] = i + 1;
    face += 3;
  }

  memcpy(this->positions, positions, sizeof(float) * 3 * nvertices);
  ComputeMeta();
}

/* ------------------------------------------------------------------------- */

Trimesh::~Trimesh() {
  delete[] positions;
  delete[] faces;
  delete[] normals;  
  delete[] colors;  
  delete[] vertex_metas;  
  delete[] tex1D_coords;
  delete[] tex2D_coords;
  delete[] tex3D_coords;
  delete[] occlusion;
}

/* ------------------------------------------------------------------------- */

Trimesh *Trimesh::GetSphere(int nslices, int nstacks, float radius) {
  // Slices are the vertical wedges, which wrap around and reconnect. Stacks
  // are the horizontal stripes, not counting the top and bottom crowns.
  Trimesh *mesh = new Trimesh(nslices * (nstacks + 1) + 2, nslices * nstacks * 2 + nslices * 2);

  float stack_delta = PI / (nstacks + 2);
  float slice_delta = 2.0f * PI / nslices;

  float *position = mesh->positions;
  mesh->tex2D_coords = new float[mesh->nvertices * 2];
  float *tex2D_coord = mesh->tex2D_coords;

  float stack_at = stack_delta;
  for (int r = 0; r <= nstacks; ++r, stack_at += stack_delta) {
    float slice_at = 0.0f;
    for (int c = 0; c < nslices; ++c, slice_at += slice_delta) {
      position[0] = radius * cos(slice_at) * sin(stack_at);
      position[1] = radius *                 cos(stack_at);
      position[2] = radius * sin(slice_at) * sin(stack_at);
      position += 3;

      tex2D_coord[0] = c / (float) nslices;
      tex2D_coord[1] = r / (float) nstacks;
      tex2D_coord += 2;
    }
  }

  // Top
  position[0] = 0.0f;
  position[1] = radius;
  position[2] = 0.0f;
  position += 3;

  // Bottom
  position[0] = 0.0f;
  position[1] = -radius;
  position[2] = 0.0f;

  int *face = mesh->faces;
  for (int r = 0; r < nstacks; ++r) {
    for (int c = 0; c < nslices; ++c) {
      int next_stack = (r + 1) % (nstacks + 1);
      int next_slice = (c + 1) % (nslices + 0);

      face[0] = r * nslices + c;
      face[1] = r * nslices + next_slice;
      face[2] = next_stack * nslices + c;
      face[3] = face[2];
      face[4] = face[1];
      face[5] = next_stack * nslices + next_slice;
      face += 6;
    }
  }

  // Now incorporate the bottom and top.
  int itop = nslices * (nstacks + 1);
  int ibottom = nslices * (nstacks + 1) + 1;
  for (int c = 0; c < nslices; ++c) {
    face[0] = (c + 1) % nslices;
    face[1] = c;
    face[2] = itop;
    face += 3;

    face[0] = nstacks * nslices + c;
    face[1] = nstacks * nslices + (c + 1) % nslices;
    face[2] = ibottom;
    face += 3;
  }

  // TODO do this manually
  /* mesh->ComputeMeta(); */

  return mesh;
}

/* ------------------------------------------------------------------------- */

Trimesh *Trimesh::GetBox(float r) {
  Trimesh *mesh = new Trimesh(24, 12);

  float *positions = mesh->positions;

  // Front
  positions[0] = -r;
  positions[1] = -r;
  positions[2] = r;
  positions[3] = r;
  positions[4] = -r;
  positions[5] = r;
  positions[6] = -r;
  positions[7] = r;
  positions[8] = r;
  positions[9] = r;
  positions[10] = r;
  positions[11] = r;

  // Back
  positions[12] = r;
  positions[13] = -r;
  positions[14] = -r;
  positions[15] = -r;
  positions[16] = -r;
  positions[17] = -r;
  positions[18] = r;
  positions[19] = r;
  positions[20] = -r;
  positions[21] = -r;
  positions[22] = r;
  positions[23] = -r;
 
  // Left
  positions[24] = -r;
  positions[25] = -r;
  positions[26] = -r;
  positions[27] = -r;
  positions[28] = -r;
  positions[29] = r;
  positions[30] = -r;
  positions[31] = r;
  positions[32] = -r;
  positions[33] = -r;
  positions[34] = r;
  positions[35] = r;
 
  // Right
  positions[36] = r;
  positions[37] = -r;
  positions[38] = r;
  positions[39] = r;
  positions[40] = -r;
  positions[41] = -r;
  positions[42] = r;
  positions[43] = r;
  positions[44] = r;
  positions[45] = r;
  positions[46] = r;
  positions[47] = -r;
 
  // Top
  positions[48] = -r;
  positions[49] = r;
  positions[50] = r;
  positions[51] = r;
  positions[52] = r;
  positions[53] = r;
  positions[54] = -r;
  positions[55] = r;
  positions[56] = -r;
  positions[57] = r;
  positions[58] = r;
  positions[59] = -r;
 
  // Bottom
  positions[60] = -r;
  positions[61] = -r;
  positions[62] = -r;
  positions[63] = r;
  positions[64] = -r;
  positions[65] = -r;
  positions[66] = -r;
  positions[67] = -r;
  positions[68] = r;
  positions[69] = r;
  positions[70] = -r;
  positions[71] = r;

  int *faces = mesh->faces;
  faces[0] = 0;
  faces[1] = 1;
  faces[2] = 3;

  faces[3] = 0;
  faces[4] = 3;
  faces[5] = 2;

  faces[6] = 4;
  faces[7] = 5;
  faces[8] = 7;

  faces[9] = 4;
  faces[10] = 7;
  faces[11] = 6;

  faces[12] = 8;
  faces[13] = 9;
  faces[14] = 11;

  faces[15] = 8;
  faces[16] = 11;
  faces[17] = 10;

  faces[18] = 12;
  faces[19] = 13;
  faces[20] = 15;

  faces[21] = 12;
  faces[22] = 15;
  faces[23] = 14;

  faces[24] = 16;
  faces[25] = 17;
  faces[26] = 19;

  faces[27] = 16;
  faces[28] = 19;
  faces[29] = 18;

  faces[30] = 20;
  faces[31] = 21;
  faces[32] = 23;

  faces[33] = 20;
  faces[34] = 23;
  faces[35] = 22;

  mesh->ComputeMeta();

  return mesh;
}

/* ------------------------------------------------------------------------- */

int Trimesh::GetVertexCount() const {
  return nvertices;
}

/* ------------------------------------------------------------------------- */

int Trimesh::GetFaceCount() const {
  return nfaces;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetPositions() const {
  return positions;
}

/* ------------------------------------------------------------------------- */

float *Trimesh::GetPositions() {
  return positions;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetNormals() const {
  return normals;
}

/* ------------------------------------------------------------------------- */

float *Trimesh::GetNormals() {
  return normals;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetColors() const {
  return colors;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetOcclusion() const {
  return occlusion;
}

/* ------------------------------------------------------------------------- */

const int *Trimesh::GetFaces() const {
  return faces;
}

/* ------------------------------------------------------------------------- */

int *Trimesh::GetFaces() {
  return faces;
}

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& stream, const Trimesh& mesh) {
  stream << "nvertices: " << mesh.nvertices << std::endl;
  stream << "nfaces: " << mesh.nfaces << std::endl;
  stream << "bounding_sphere.GetCenter(): " << mesh.GetBoundingSphere().GetCenter() << std::endl;
  stream << "bounding_sphere.GetRadius(): " << mesh.GetBoundingSphere().GetRadius() << std::endl;
  /* std::cerr << "bounding_box.min: " << mesh.GetBoundingBox().min << std::endl; */
  /* std::cerr << "bounding_box.max: " << mesh.GetBoundingBox().max << std::endl; */

  /* for (int i = 0; i < mesh.nvertices; i++) { */
    /* std::cerr << "vertex[" << i << "]: " << mesh.positions[i * 3 + 0] << ", " << mesh.positions[i * 3 + 1] << ", " << mesh.positions[i * 3 + 2] << std::endl; */
  /* } */

  return stream;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetTex3DCoords() const {
  return tex3D_coords;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetTex2DCoords() const {
  return tex2D_coords;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetTex1DCoords() const {
  return tex1D_coords;
}

/* ------------------------------------------------------------------------- */

void Trimesh::CoalesceVertices(int ndigits) {
  std::map<std::string, int> pos2idx;
  float *position = positions;
  std::stringstream ss(std::stringstream::out);

  // For each vertex, try adding it to a hash. Postcondition: pos2idx contains
  // a map between positions to the first vertex having that position.
  for (int i = 0; i < nvertices; ++i) {

    // Compose key based on the specified number of digits
    ss.str("");
    ss << std::fixed << std::setprecision(ndigits) << position[0] << "," << position[1] << "," << position[2];
    std::string key = ss.str();

    // Look for key in hash, if not found, add an entry pointing to this vertex
    // ID
    std::map<std::string, int>::const_iterator match = pos2idx.find(key);
    if (match == pos2idx.end()) {
      pos2idx.insert(std::pair<std::string, int>(key, i));
    }

    position += 3;
  }

  // for each hash vertex
  //   get v->second, lookup into positions
  //   dump position into new buffer
  std::map<std::string, int>::iterator vi;
  float *new_positions = new float[pos2idx.size() * 3];
  int i;
  for (vi = pos2idx.begin(), i = 0; vi != pos2idx.end(); ++vi, ++i) {
    memcpy(&new_positions[i * 3], &positions[vi->second * 3], 3 * sizeof(float));
    vi->second = i;
  }

  int *old2new_idx = new int[nvertices];
  position = positions;
  for (int i = 0; i < nvertices; ++i) {
    // compose key
    ss.str("");
    ss << std::fixed << std::setprecision(ndigits) << position[0] << "," << position[1] << "," << position[2];
    std::string key = ss.str();

    std::map<std::string, int>::const_iterator match = pos2idx.find(key);
    old2new_idx[i] = match->second;

    position += 3;
  }

  // for each face
  //   replace each index with lut lookup
  int *face = faces;
  for (int fi = 0; fi < nfaces; ++fi) {
    for (int i = 0; i < 3; ++i) {
      face[i] = old2new_idx[face[i]];
    }
    face += 3;
  }

  // remove degenerate faces
  RemoveDegenerateFaces();

  nvertices = pos2idx.size();
  
  delete[] positions;
  positions = new_positions;

  delete[] normals;
  normals = NULL;

  delete[] tex2D_coords;
  tex2D_coords = NULL;

  delete[] tex1D_coords;
  tex1D_coords = NULL;

  delete[] tex3D_coords;
  tex3D_coords = NULL;

  delete[] old2new_idx;
}

/* ------------------------------------------------------------------------- */

void Trimesh::ReadOff(const std::string& path) {
  // http://people.sc.fsu.edu/~jburkardt/html/off_format.html
  
  std::ifstream f(path.c_str());
  if (f.fail()) {
    throw MessagedException("can't open " + path + " for reading");
  }

  // read first lines, optionally OFF
  // read nvertices nfaces nedges
  std::string line;
  std::istringstream line_stream;
  int nedges;
  while (getline(f, line)) {
    line_stream.clear();
    line_stream.str(line);
    line_stream >> nvertices >> nfaces >> nedges;
    if (!line_stream.fail()) {
      break;
    }
  }

  // for each vertex
  //    read it
  positions = new float[nvertices * 3];
  float *position = positions;
  for (int i = 0; i < nvertices && getline(f, line); ++i) {
    line_stream.clear();
    line_stream.str(line);
    line_stream >> position[0] >> position[1] >> position[2];

    if (line_stream.fail()) {
      throw MessagedException("couldn't parse vertex");
    }
    position += 3;
  }
  if (f.fail()) {
    throw MessagedException("couldn't read line from file");
  }

  // record position
  int face_ptr = f.tellg();

  // for each face
  //   for each triangle
  //     read it
  int actual_nfaces = 0;
  for (int i = 0; i < nfaces && getline(f, line); ++i) {
    // epcot.off has a blank line in it
    if (line.size() == 0) {
      --i;
      continue;
    }
    line_stream.clear();
    line_stream.str(line);
    int nindices = 0;
    line_stream >> nindices;
    if (line_stream.fail()) {
      throw MessagedException("couldn't parse face line");
    }
    actual_nfaces += nindices - 2;
  }
  if (f.fail()) {
    throw MessagedException("couldn't read line from file");
  }

  // adjust number of faces
  faces = new int[actual_nfaces * 3];

  // seek recorded position
  f.seekg(face_ptr);
 
  // for each face
  //   for each triangle
  //     read it
  int *face = faces;
  for (int i = 0; i < nfaces && getline(f, line); ++i) {
    if (line.size() == 0) {
      --i;
      continue;
    }

    /*
     I'm backward from the rest of the world. I want a left-handed coordinate
     system. Off models were probably modeled in a right-handed one, so we
     silently reorder vertices.
     */

    int nindices;
    line_stream.clear();
    line_stream.str(line);
    line_stream >> nindices >> face[0] >> face[1] >> face[2];

    int first = face[0];
    int last = face[2];
    face += 3;
    for (int j = 1; j < nindices - 2; ++j) {
      face[0] = first;
      face[1] = last;
      line_stream >> face[2];
      last = face[2];
      face += 3;
    }
  }

  nfaces = actual_nfaces;

  f.close();
  ComputeMeta();
}

/* ------------------------------------------------------------------------- */

int Keyify(uniquer_t &uniquer, const string &token) {
  vertex_key_t key = {0, 0};
  std::istringstream line_splitter;
  std::istringstream type_converter;
  string position_text;
  string tex2D_coord_text;
  string normal_text;

  // Make this token parseable.
  line_splitter.str(token);
  line_splitter.clear();

  // Grab just the position ID.
  getline(line_splitter, position_text, '/');

  // Grab the texcoord and normal IDs, if present.
  if (getline(line_splitter, tex2D_coord_text, '/')) {
    type_converter.str(tex2D_coord_text);
    type_converter.clear();
    type_converter >> key.i_tex2D_coord;
    --key.i_tex2D_coord;
    if (getline(line_splitter, normal_text, '/')) {
      type_converter.str(normal_text);
      type_converter.clear();
      type_converter >> key.i_normal;
      --key.i_normal;
    }
  }
  
  // Now turn the position text into a real int.
  type_converter.str(position_text);
  type_converter.clear();
  type_converter >> key.i_position;

  // OBJ is 1-based. I'm 0-based.
  --key.i_position;

  uniquer_t::iterator match = uniquer.find(key);
  int key_id;
  if (match == uniquer.end()) {
    key_id = uniquer.size();
    uniquer.insert(uniquer_pair_t(key, key_id));
  } else {
    key_id = match->second;
  }
 
  return key_id;
}

/* ------------------------------------------------------------------------- */

void Trimesh::ReadObj(const std::string& path) {
  int ntexcoords = 0;
  int nnormals = 0;
  int actual_nfaces = 0;

  // count the number of vertices and faces
  Scanner in(path, Scanner::SOURCE_FILE);
  in.Mark();
  while (in.HasNextLine()) {
    Scanner lineIn(in.NextLine(), Scanner::SOURCE_STRING);

    if (!lineIn.HasNext()) {
      continue;
    }
    std::string type = lineIn.Next();

    if (type.compare("v") == 0) {
      ++nvertices;
    } else if (type.compare("vt") == 0) {
      ++ntexcoords;
    } else if (type.compare("vn") == 0) {
      ++nnormals;
    } else if (type.compare("f") == 0) {
      int nverticesInFace = 0;
      while (lineIn.HasNext()) {
        lineIn.Next();
        ++nverticesInFace;
      }
      assert(nverticesInFace >= 3);
      actual_nfaces += nverticesInFace - 2;
    }
  }
  in.Rewind();

  // Allocate enough space for the actual data. The texcoords and normals
  // might not really exist, but it's safe to delete[] new[0].
  float *unmatched_positions = new float[nvertices * 3];
  float *unmatched_tex2D_coords = new float[ntexcoords * 2];
  float *unmatched_normals = new float[nnormals * 3];
  faces = new int[actual_nfaces * 3];

  // Read all that again, this time storing the data.
  float *position = unmatched_positions;
  float *tex2D_coord = unmatched_tex2D_coords;
  float *normal = unmatched_normals;
  int *face = faces;

  uniquer_t uniquer;
  while (in.HasNextLine()) {
    std::string line = in.NextLine();
    Scanner lineIn(line, Scanner::SOURCE_STRING);

    if (!lineIn.HasNext()) {
      continue;
    }
    std::string type = lineIn.Next();

    if (type.compare("v") == 0) {
      position[0] = lineIn.NextFloat();
      position[1] = lineIn.NextFloat();
      position[2] = lineIn.NextFloat();
      position += 3;
    } else if (type.compare("vt") == 0) {
      tex2D_coord[0] = lineIn.NextFloat();
      tex2D_coord[1] = lineIn.NextFloat();
      tex2D_coord += 2;
    } else if (type.compare("vn") == 0) {
      normal[0] = lineIn.NextFloat();
      normal[1] = lineIn.NextFloat();
      normal[2] = lineIn.NextFloat();
      normal += 3;
    } else if (type.compare("f") == 0) {
      string token;

      for (int i = 0; i < 3; ++i) {
        // Grab next token, which has the form \d+(/\d+(/\d+)?)?.
        token = lineIn.Next();
        face[i] = Keyify(uniquer, token);
      }

      int first = face[0];
      int last = face[2];

      face += 3;

      while (lineIn.HasNext()) {
        face[0] = first;
        face[1] = last;

        token = lineIn.Next();
        face[2] = Keyify(uniquer, token);

        last = face[2];
        face += 3;
      }
    }
  }
  in.Close();

  nfaces = actual_nfaces;

  int actual_nvertices = uniquer.size();
  positions = new float[actual_nvertices * 3];
  if (ntexcoords)
    tex2D_coords = new float[actual_nvertices * 2];
  if (nnormals)
    normals = new float[actual_nvertices * 3];

  for (uniquer_t::iterator i = uniquer.begin(); i != uniquer.end(); ++i) {
    memcpy(positions + i->second * 3, unmatched_positions + i->first.i_position * 3, sizeof(float) * 3);
    if (ntexcoords)
      memcpy(tex2D_coords + i->second * 2, unmatched_tex2D_coords + i->first.i_tex2D_coord * 2, sizeof(float) * 2);
    if (nnormals)
      memcpy(normals + i->second * 3, unmatched_normals + i->first.i_normal * 3, sizeof(float) * 3);
  }

  nvertices = actual_nvertices;

  delete[] unmatched_positions;
  delete[] unmatched_tex2D_coords;
  delete[] unmatched_normals;

  ComputeMeta(normals == NULL);
}

/* ------------------------------------------------------------------------- */

void Trimesh::ReadPly(const std::string& path) {
  std::ifstream f(path.c_str());
  if (f.fail()) {
    throw MessagedException("can't open " + path + " for reading");
  }

  std::string tag;
  std::string line;
  std::stringstream line_stream;
  std::string element;
  bool is_ascii = true;
  int nbytes_per_vertex = 0;
  bool toggle_endian = false;
  int nbytes_before_face = 0;
  int nbytes_after_face = 0;
  bool saw_list = false;
  bool has_vcolors = false;

  while (tag != "end_header") {
    getline(f, line);
    line_stream.clear();
    line_stream.str(line);

    line_stream >> tag;

    if (tag == "element") {
      line_stream >> element;
      if (element == "vertex") {
        line_stream >> nvertices;
      } else {
        line_stream >> nfaces;
      }
    }
    
    else if (tag == "format") {
      std::string format;
      line_stream >> format;
      is_ascii = format == "ascii";
      if (!is_ascii) {
        // TODO
        bool is_big_endian = format == "binary_big_endian";
        toggle_endian = Utilities::IsLittleEndian() == is_big_endian;
      }
    }

    else if (element == "vertex" && tag == "property") {
      std::string type;
      line_stream >> type;
      if (type == "float") {
        nbytes_per_vertex += 4;
      } else if (type == "uchar") {
        nbytes_per_vertex += 1;
        has_vcolors = true;
      }
    }

    else if (element == "face" && tag == "property") {
      std::string next;
      line_stream >> next;

      if (!saw_list) {
        if (next == "list") {
          saw_list = true;
        } else if (next == "uchar") {
          nbytes_before_face += 1;
        } else if (next == "int") {
          nbytes_before_face += 4;
        }
      } else {
        if (next == "uchar") {
          nbytes_after_face += 1;
        } else if (next == "int") {
          nbytes_after_face += 4;
        }
      }
    }
  }

  positions = new float[nvertices * 3];
  if (has_vcolors) {
    colors = new float[nvertices * 3];
  }

  if (is_ascii) {
    // Get all the vertex positions
    float *position = positions;
    for (int i = 0; i < nvertices; ++i, position += 3) {
      f >> position[0] >> position[1] >> position[2];
      getline(f, line);
    }

    // Get the real number of faces, calculated with each n-gon as triangles
    int face_ptr = f.tellg();
    int actual_nfaces = 0;
    int nindices;
    for (int i = 0; i < nfaces; ++i) {
      getline(f, line);
      line_stream.clear();
      line_stream.str(line);
      line_stream >> nindices;
      actual_nfaces += nindices - 2;
    }
    f.seekg(face_ptr);

    // Read in the faces now, breaking up n-gons as necessary
    faces = new int[actual_nfaces * 3];
    int *face = faces;
    for (int i = 0; i < nfaces; ++i, position += 3) {
      f >> nindices >> face[0] >> face[1] >> face[2];
      int first = face[0];
      int last = face[2];
      face += 3;

      for (int vi = 1; vi < nindices - 2; ++vi) {
        face[0] = first;
        face[1] = last;
        f >> face[2];
        last = face[2];
        face += 3;
      }
    }
  } else {
    int binary_ptr = f.tellg();
    f.close();
    f.open(path.c_str(), std::ios_base::in | std::ios_base::binary);
    f.seekg(binary_ptr);

    // Read in all the vertex positions
    char *vertex_buffer = new char[nbytes_per_vertex];
    float *position = positions;
    float *color = colors;
    for (int i = 0; i < nvertices; ++i, position += 3) {
      f.read(vertex_buffer, nbytes_per_vertex);
      for (int d = 0; d < 3; ++d) {
        position[d] = *((float *) &vertex_buffer[d * 4]);
        if (toggle_endian) {
          Utilities::ReverseEndian(&position[d], sizeof(float));
        }
        position[2] = -position[2];
      }
      if (has_vcolors) {
        for (int d = 0; d < 3; ++d) {
          color[d] = *((unsigned char *) &vertex_buffer[12 + d]) / 255.0f;
        }
        color += 3;
      }
    }
    delete[] vertex_buffer;

    // Get the real number of faces, calculated with each n-gon as triangles
    int face_ptr = f.tellg();
    int actual_nfaces = 0;
    char nindices;
    for (int i = 0; i < nfaces; ++i) {
      f.seekg(nbytes_before_face, std::ios_base::cur);
      f.read(&nindices, sizeof(char));
      f.seekg(sizeof(int) * nindices + nbytes_after_face, std::ios_base::cur);
      actual_nfaces += nindices - 2;
    }
    f.seekg(face_ptr);

    // Read in the faces now, breaking up n-gons as necessary
    faces = new int[actual_nfaces * 3];
    int *face = faces;
    for (int i = 0; i < nfaces; ++i, position += 3) {
      f.seekg(nbytes_before_face, std::ios_base::cur);
      f.read(&nindices, sizeof(unsigned char));
      f.read((char *) face, sizeof(int) * 3);
      f.seekg(nbytes_after_face, std::ios_base::cur);
      if (toggle_endian) {
        for (int d = 0; d < 3; ++d) {
          Utilities::ReverseEndian(&face[d], sizeof(int));
        }
      }
      /* int tmp = face[1]; */
      /* face[1] = face[2]; */
      /* face[2] = tmp; */

      int first = face[0];
      int last = face[2];
      face += 3;

      for (int vi = 1; vi < nindices - 2; ++vi) {
        face[0] = first;
        face[1] = last;
        f.read((char *) &face[2], sizeof(int));
        if (toggle_endian) Utilities::ReverseEndian(&face[2], sizeof(int));
        last = face[2];
        face += 3;
      }
    }
  }

  f.close();
  ComputeMeta();
}

/* ------------------------------------------------------------------------- */

void Trimesh::NullBuffers() {
  nvertices = 0;
  nfaces = 0;
  ncolor_channels = 0;
  positions = normals = colors = tex1D_coords = tex2D_coords = tex3D_coords = occlusion = NULL;
  faces = NULL;
  nvertex_metas = 0;
  vertex_metas = NULL;
}

/* ------------------------------------------------------------------------- */

void Trimesh::Write(const std::string& path) {
  if (path.rfind(".off") == path.length() - 4) {
    WriteOff(path);
  } else if (path.rfind(".obj") == path.length() - 4) {
    WriteObj(path);
  } else if (path.rfind(".ply") == path.length() - 4) {
    WritePly(path);
  } else if (path.rfind(".json") == path.length() - 5) {
    WriteJSON(path);
  } else {
    throw MessagedException(std::string("illegal mesh format"));
  }
}

/* ------------------------------------------------------------------------- */

void Trimesh::WriteOff(const std::string& path) {
  // open file
  std::ofstream f(path.c_str());  

  // write header
  f << "OFF" << std::endl;
  f << nvertices << " " << nfaces << " " << 0 << std::endl;

  // for each vertex
  //   write out
  // I NEGATE Z!
  float *position = positions;
  for (int i = 0; i < nvertices; ++i) {
    f << position[0] << " " << position[1] << " " << -position[2] << std::endl;
    position += 3;
  }

  // for each face
  //   write out
  int *face = faces;
  for (int i = 0; i < nfaces; ++i) {
    f << "3 " << face[0] << " " << face[1] << " " << face[2] << std::endl;
    face += 3;
  }

  f.close();
}

/* ------------------------------------------------------------------------- */

void Trimesh::WriteObj(const std::string& path) {
  // open file
  std::ofstream f(path.c_str());
  f << std::fixed;

  // write vertices
  float *position = positions;
  for (int i = 0; i < nvertices; ++i, position += 3) {
    f << "v " << position[0] << " " << position[1] << " " << position[2] << std::endl;
  }

  // Only write normals if we have them.
  /* if (normals) { */
    /* float *normal = normals; */
    /* for (int i = 0; i < nvertices; ++i, normal += 3) { */
      /* f << "vn " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl; */
    /* } */
  /* } */

  // write texcoords
  if (tex2D_coords) {
    float *texcoord = tex2D_coords;
    for (int i = 0; i < nvertices; ++i, texcoord += 2) {
      f << "vt " << texcoord[0] << " " << texcoord[1] << std::endl;
    }
  }
 
  // write faces as # id0+1 id1+1 id2+1
  int *face = faces;
  for (int i = 0; i < nfaces; ++i, face += 3) {
    f << "f " << face[0] + 1 << " " << face[1] + 1 << " " << face[2] + 1 << std::endl;
  }
 
  // close file
  f.close();
}

/* ------------------------------------------------------------------------- */

void Trimesh::WriteJSON(const std::string& path) {
  bool has_vertex_colors = colors != NULL;

  // open file
  std::ofstream f(path.c_str());
  f << std::fixed;

  f << "{" << std::endl;

  // write metadata
  f << "  \"metadata\": {" << std::endl; 
  f << "    \"formatVersion\": 3, " << std::endl;
  f << "    \"generatedBy\": \"libtwodee\", " << std::endl;
  f << "    \"vertices\": " << GetVertexCount() << "," << std::endl;
  f << "    \"faces\": " << GetFaceCount() << "," << std::endl;
  f << "    \"normals\": " << GetVertexCount() << "," << std::endl;
  f << "    \"colors\": " << (has_vertex_colors ? GetVertexCount() : 0) << "," << std::endl;
  f << "    \"uvs\": " << 0 << "," << std::endl;
  f << "    \"materials\": " << 0 << "," << std::endl;
  f << "    \"morphTargets\": 0" << std::endl;
  f << "  }," << std::endl;
  f << "  \"scale\": 1.0," << std::endl;

  // write vertices
  f << "  \"vertices\": [";
  if (GetVertexCount() > 0) {
    float *position = positions;
    f << position[0] << "," << position[1] << "," << position[2];
    position += 3;
    for (int i = 1; i < nvertices; ++i, position += 3) {
      f << "," << position[0] << "," << position[1] << "," << position[2];
    }
  }
  f << "]," << std::endl;
 
  // write normals
  f << "  \"normals\": [";
  if (GetVertexCount() > 0) {
    float *normal = normals;
    f << normal[0] << "," << normal[1] << "," << normal[2];
    normal += 3;
    for (int i = 1; i < nvertices; ++i, normal += 3) {
      f << "," << normal[0] << "," << normal[1] << "," << normal[2];
    }
  }
  f << "]," << std::endl;
 
  // write colors
  if (has_vertex_colors) {
    f << "  \"colors\": [";
    if (GetVertexCount() > 0) {

      float *color = colors;

      int r = (int) (color[0] * 255);
      int g = (int) (color[1] * 255);
      int b = (int) (color[2] * 255);
      int rgb = (r << 16) | (g << 8) | (b << 0);
      f << rgb;

      color += 3;
      for (int i = 1; i < nvertices; ++i, color += 3) {
        r = (int) (color[0] * 255);
        g = (int) (color[1] * 255);
        b = (int) (color[2] * 255);
        rgb = (r << 16) | (g << 8) | (b << 0);
        f << "," << rgb;
      }
    }
    f << "]," << std::endl;
  }

  int face_type = has_vertex_colors ? 128 + 32 : 0 + 32;

  // write face indices
  f << "  \"faces\": [";
  if (GetFaceCount() > 0) {
    int *face = faces;

    f << face_type << "," << face[0] << "," << face[1] << "," << face[2];
    if (has_vertex_colors) {
      f << "," << face[0] << "," << face[1] << "," << face[2];
    }
    f << "," << face[0] << "," << face[1] << "," << face[2]; // normals
    face += 3;

    for (int i = 1; i < GetFaceCount(); ++i) {
      f << ", " << face_type << "," << face[0] << "," << face[1] << "," << face[2];
      if (has_vertex_colors) {
        f << "," << face[0] << "," << face[1] << "," << face[2];
      }
      f << "," << face[0] << "," << face[1] << "," << face[2]; // normals
      face += 3;
    }
  }
  f << "]" << std::endl;
  
  f << "}";
 
  // close file
  f.close();
}

/* ------------------------------------------------------------------------- */

void Trimesh::WritePly(const std::string& path) {
  // open file
  std::ofstream f(path.c_str());

  // write header
  f << "ply" << std::endl;
  f << "format ascii 1.0" << std::endl;
  f << "element vertex " << nvertices << std::endl;
  f << "property float x" << std::endl;
  f << "property float y" << std::endl;
  f << "property float z" << std::endl;
  f << "element face " << nfaces << std::endl;
  f << "property list uchar int vertex_index" << std::endl;
  f << "end_header" << std::endl;

  // write vertices as x y z
  float *position = positions;
  for (int i = 0; i < nvertices; ++i, position += 3) {
    f << position[0] << " " << position[1] << " " << position[2] << std::endl;
  }

  // write faces as # id0 id1 id2 
  int *face = faces;
  for (int i = 0; i < nfaces; ++i, face += 3) {
    f << "3 " << face[0] << " " << face[1] << " " << face[2] << std::endl;
  }

  // close file
  f.close();
}

/* ------------------------------------------------------------------------- */

void Trimesh::RemoveDegenerateFaces() {
  int new_nfaces = 0;
  int *face = faces;
  for (int i = 0; i < nfaces; ++i) {
    if (face[0] != face[1] && face[1] != face[2] && face[0] != face[2]) {
      ++new_nfaces;
    }
    face += 3;
  }

  int *new_faces = new int[new_nfaces * 3];
  int *new_face = new_faces;
  face = faces;
  for (int i = 0; i < nfaces; ++i) {
    if (face[0] != face[1] && face[1] != face[2] && face[0] != face[2]) {
      memcpy(new_face, face, sizeof(int) * 3);
      new_face += 3;
    }
    face += 3;
  }

  nfaces = new_nfaces;
  delete[] faces;
  faces = new_faces;
}

/* ------------------------------------------------------------------------- */

Trimesh *Trimesh::GetQuadrilateral(int nrows, int ncols) {
  NField<float, 2> map(QVector2<int>(ncols + 1, nrows + 1), 3);
  NFieldIterator<2> c(map.GetDimensions());
  while (c.HasNext()) {
    c.Next();
    map(c)[0] = c[0];
    map(c)[1] = c[1];
    map(c)[2] = 0.0f;
  }
  return GetParametric(map);
}

/* ------------------------------------------------------------------------- */

Trimesh *Trimesh::GetParametric(const NField<float, 2>& quadric_map, bool wrap_x, bool wrap_y, bool generate_texcoords) {
  assert(quadric_map.GetChannelCount() >= 3);

  const QVector2<int>& dims = quadric_map.GetDimensions();
  int nrow_quads;
  int ncol_quads;

  int nvertices = quadric_map.GetElementCount();

  if (wrap_x) {
    nrow_quads = dims[0];
  } else {
    nrow_quads = dims[0] - 1;
  }
  if (wrap_y) {
    ncol_quads = dims[1];
  } else {
    ncol_quads = dims[1] - 1;
  }
  int nfaces = 2 * nrow_quads * ncol_quads;

  Trimesh *mesh = new Trimesh(nvertices, nfaces);
  int nmetas = quadric_map.GetChannelCount() - 3;

  // Store vertex positions; each coordinate is in one element of the
  // three channel image.

  if (nmetas == 0) {
    memcpy(mesh->positions, quadric_map.GetData(), sizeof(float) * 3 * nvertices);
  } else {
    float *position = mesh->GetPositions();
    float *vertex_meta = mesh->AllocateVertexMetas(nmetas);
    for (int i = 0; i < nvertices; ++i) {
      memcpy(position, quadric_map(i), sizeof(float) * 3);
      memcpy(vertex_meta, &quadric_map(i)[3], sizeof(float) * nmetas);
      position += 3;
      vertex_meta += nmetas;
    }
  }

  // The face connectivity is trivial to compute.  Just hook up adjacent
  // nodes of the image grid.  We're decomposing into triangles, so two
  // triangles cover each "pixel."
  int r, c;
  int *face = mesh->faces;
  for (r = 0; r < ncol_quads; ++r) {
    for (c = 0; c < nrow_quads; ++c) {
      face[0] = r * dims[0] + c; // base vertex
      face[1] = r * dims[0] + (c + 1) % dims[0]; // next v in curr row
      face[2] = ((r + 1) % dims[1]) * dims[0] +
        (c + 1) % dims[0]; // next v in next row

      face[3] = face[0]; // base vertex
      face[4] = face[2]; // next v in next row
      face[5] = ((r + 1) % dims[1]) * dims[0] + c; // v in next row

      face += 6;
    }
  }

  if (generate_texcoords) {
    mesh->tex2D_coords = new float[nvertices * 2];
    QVector2<float> scale(1.0f / (dims[0] - 1.0f), 1.0f / (dims[1] - 1.0f));

    NFieldIterator<2> c(dims);
    float *tex2D_coord = mesh->tex2D_coords;
    while (c.HasNext()) {
      c.Next();
      tex2D_coord[0] = c[0] * scale[0];
      tex2D_coord[1] = c[1] * scale[1];
      tex2D_coord += 2;
    }
  }
  /* mesh->ComputeMeta(); */

  return mesh;
}

/* ------------------------------------------------------------------------- */

void Trimesh::GenerateTexCoords(const QVector4<float>& s) {
   delete[] tex1D_coords;
   tex1D_coords = new float[nvertices];

   float *position = positions;
   for (int i = 0; i < nvertices; ++i) {
     QVector4<float> pvector(position[0], position[1], position[2], 1.0f);
     tex1D_coords[i] = s.Dot(pvector);
     position += 3;
   }
}

/* ------------------------------------------------------------------------- */

void Trimesh::GenerateTexCoords(const QVector4<float>& s,
                                const QVector4<float>& t) {
   delete[] tex2D_coords;
   tex2D_coords = new float[nvertices * 2];

   float *position = positions;
   float *texcoord = tex2D_coords;
   for (int i = 0; i < nvertices; ++i) {
     QVector4<float> pvector(position[0], position[1], position[2], 1.0f);
     texcoord[0] = s.Dot(pvector);
     texcoord[1] = t.Dot(pvector);
     position += 3;
     texcoord += 2;
   }
}

/* ------------------------------------------------------------------------- */

void Trimesh::GenerateTexCoords(const QVector4<float>& s,
                                const QVector4<float>& t,
                                const QVector4<float>& q) {
   delete[] tex3D_coords;
   tex3D_coords = new float[nvertices * 3];

   float *position = positions;
   float *texcoord = tex3D_coords;
   for (int i = 0; i < nvertices; ++i) {
     QVector4<float> pvector(position[0], position[1], position[2], 1.0f);
     texcoord[0] = s.Dot(pvector);
     texcoord[1] = t.Dot(pvector);
     texcoord[2] = q.Dot(pvector);
     position += 3;
     texcoord += 3;
   }
}

/* ------------------------------------------------------------------------- */

const Ball<float, 3>& Trimesh::GetBoundingSphere() const {
  return bounding_sphere;  
}

/* ------------------------------------------------------------------------- */

void Trimesh::ComputeBounds() {
  float *position = positions;
  QVector<float, 3> min;
  QVector<float, 3> max;
  // Prime the min and max
  for (int d = 0; d < 3; ++d) {
    min[d] = max[d] = position[d];
  }

  for (int i = 1; i < nvertices; ++i, position += 3) {
    for (int d = 0; d < 3; ++d) {
      if (position[d] < min[d]) {
        min[d] = position[d];
      } else if (position[d] > max[d]) {
        max[d] = position[d];
      }
    }
  }

  for (int d = 0; d < 3; ++d) {
    centroid[d] = (min[d] + max[d]) * 0.5f;
  }

  bounding_sphere = Ball<float, 3>(bounding_box.GetDiagonalLength() * 0.5f, centroid);
  bounding_box = Box<float, 3>(max, min);
}

/* ------------------------------------------------------------------------- */

void Trimesh::ComputeMeta(bool do_normals) {
  ComputeBounds();
  if (do_normals) {
    ComputeNormals();
  }
}

/* ------------------------------------------------------------------------- */

QVector3<float> *Trimesh::GetFaceNormals() {
  QVector3<float> a, b, c;
  QVector3<float> b2a, b2c;
  QVector3<float> fnormal;
  QVector3<float> *fnormals = new QVector3<float>[nfaces];

  int *face = faces;
  for (int i = 0; i < nfaces; i++, face += 3) {
     a = QVector3<float>(&positions[face[0] * 3]);
     b = QVector3<float>(&positions[face[1] * 3]);
     c = QVector3<float>(&positions[face[2] * 3]);
     b2a = a - b;
     b2c = c - b;
     fnormal = b2c.Cross(b2a);
     fnormal.Normalize();
     memcpy(&(fnormals[i][0]), &fnormal[0], sizeof(float) * 3);
  }

  return fnormals;
}

/* ------------------------------------------------------------------------- */

void Trimesh::ComputeNormals() {
  // Calculate face normals
  QVector3<float> *fnormals = GetFaceNormals();
  
  // Average out face normals around a vertex to produce vertex normals
  QVector3<float> *vnormals = new QVector3<float>[nvertices];

  // Initialize to 0
  for (int i = 0; i < nvertices; i++) {
    vnormals[i] = QVector3<float>(0.0f);
  }

  // Go through each face's vertices and add the face normal to vertices'
  // normals
  int *face = faces;
  for (int i = 0; i < nfaces; i++, face += 3) {
    for (int j = 0; j < 3; j++) {
      vnormals[face[j]] += fnormals[i];
    }
  }

  delete[] fnormals;

  // Make normals have unit length
  delete[] normals;
  normals = new float[3 * nvertices];
  float *normal = normals;
  for (int i = 0; i < nvertices; i++, normal += 3) {
    vnormals[i].Normalize();
    for (int d = 0; d < 3; ++d) {
      normal[d] = vnormals[i][d];
    }
  }

  delete[] vnormals;
}

/* ------------------------------------------------------------------------- */

const Box<float, 3>& Trimesh::GetBoundingBox() const {
  return bounding_box;
}

/* ------------------------------------------------------------------------- */

Trimesh *Trimesh::GetArrow(const QVector3<float>& axis, float barrel_length, float barrel_radius, float tip_length, float tip_radius, int nrotations) {
  Polyline<float> poly(5, 3, Polyline<float>::OPEN);
  poly.Clear();
  QVector3<float> tmpvec(0.0f, 0.0f, 0.0f);
  // TODO
  QVector3<float> perpvec;// = axis.GetPerpendicular();

  // Center of non-tip end.
  poly(0)[0] = tmpvec[0];
  poly(0)[1] = tmpvec[1];
  poly(0)[2] = tmpvec[2];

  // Perimeter of non-tip end.
  tmpvec += perpvec * barrel_radius;
  poly(1)[0] = tmpvec[0];
  poly(1)[1] = tmpvec[1];
  poly(1)[2] = tmpvec[2];

  // Perimeter of tip end of barrel.
  tmpvec += axis * barrel_length;
  poly(2)[0] = tmpvec[0];
  poly(2)[1] = tmpvec[1];
  poly(2)[2] = tmpvec[2];

  // Perimeter of broad tip end;
  tmpvec += perpvec * (tip_radius - barrel_radius);
  poly(3)[0] = tmpvec[0];
  poly(3)[1] = tmpvec[1];
  poly(3)[2] = tmpvec[2];

  // Point of tip.
  tmpvec = axis * (tip_length + barrel_length);
  poly(4)[0] = tmpvec[0];
  poly(4)[1] = tmpvec[1];
  poly(4)[2] = tmpvec[2];

  return poly.Revolve(axis, nrotations);
}

/* ------------------------------------------------------------------------- */

#if 0
void Trimesh::ComputeAmbientOcclusion(int nrays, float radius) {
  // Calcule vectors in hemisphere of size radius. Generate twice as
  // many for the sphere, and we'll just take the (roughly) half in the
  // top hemisphere.
  QVector3<float> *rays = new QVector3<float>[nrays * 2];
  Sphere<float>::GetDistributedPoints(1.0f, nrays * 2, rays);

  assert(rays[0][1] < 0.0f);

  QVector3<float> ydown(0.0f, -1.0f, 0.0f);
  int i;
  for (i = 0; i < nrays * 2 && rays[i][1] < 0; ++i) {
    /* std::cerr << rays[i] << std::endl; */
  }

  // Only this many occur in the top hemisphere.
  nrays = i;
  /* std::cerr << "nrays: " << nrays << std::endl; */

  // Now we apply this hemisphere to each vertex and see what all those rays
  // hit.
  delete[] occlusion;
  occlusion = new float[nvertices];

  GeoLog log("geo.log");
  // TODO vertices
  /* std::ofstream f("mesh.txt"); */
  /* std::ofstream g("normals.txt"); */
  for (int vi = 0; vi < nvertices; ++vi) {
    QVector3<float> pt(&positions[vi * 3]);
    log.Write<float>(0, pt, QVector3<float>(0.0f, 1.0f, 0.0f));
    log.Write<float>(1, pt, QVector3<float>(0.0f), pt + QVector3<float>(&normals[vi * 3]) * 0.1f, QVector3<float>(0.0f, 0.0f, 1.0f));
    /* f << positions[vi * 3 + 0] << ", " << positions[vi * 3 + 1] << ", " << positions[vi * 3 + 2] << std::endl; */
    /* g << normals[vi * 3 + 0] << ", " << normals[vi * 3 + 1] << ", " << normals[vi * 3 + 2] << std::endl; */
  }
  /* f.close(); */
  /* g.close(); */

  for (int vi = 0; vi < nvertices; ++vi) {
    int nintersections = 0;

    // How can we rotate hemisphere so it aligns with normal?
    QVector3<float> normal(&normals[vi * 3]);
    /* std::cerr << "normal: " << normal << std::endl; */
    float y_dot_n = -normal[1];
    Matrix4x4<float> rotation;
    if (y_dot_n > 0.999999f) {
      rotation = Matrix4x4<float>::GetIdentity();
    } else {
      float degrees = acos(y_dot_n) * 180.0f / CJ_PI;
      /* std::cerr << "degrees: " << degrees << std::endl; */
      QVector3<float> cross;
      cross.Cross(normal, ydown);
      cross.Normalize();
      /* std::cerr << "cross: " << cross << std::endl; */
      /* std::cerr << "cross: " << cross << std::endl; */
      rotation = Matrix4x4<float>::GetRotate(cross, -degrees);
    }

    // For each ray in hemisphere, we rotate it and see if we hit any neighbor
    // faces. Push start away from vertex a bit so we don't intersect face of
    // which this vertex is a part at the start end.
    QVector3<float> start(&positions[vi * 3]);
    start += normal * (0.01f * radius);
    /* std::cerr << "start: " << start << std::endl; */
    /* std::cerr << "normal: " << normal << std::endl; */
    for (int ri = 0; ri < nrays; ++ri) {
      /* std::cerr << "rays[ri]: " << rays[ri] << std::endl; */
      QVector3<float> ray = rotation * rays[ri];
      /* std::cerr << ray << std::endl; */
      bool was_hit = false;


      int *face = faces;
      float length;
      for (int fi = 0; fi < nfaces; ++fi, face += 3) {
        QVector3<float> a(&positions[3 * face[0]]);
        QVector3<float> b(&positions[3 * face[1]]);
        QVector3<float> c(&positions[3 * face[2]]);
        /* std::cerr << "a: " << a << std::endl; */
        /* std::cerr << "b: " << b << std::endl; */
        /* std::cerr << "c: " << c << std::endl; */
        /* std::cerr << "" << std::endl; */
        if (RayIntersectsTriangle(start, ray, a, b, c, length) && length > 0.000001f && length <= radius) {
          was_hit = true;
          break;
        }
      }

      if (was_hit) {
        ++nintersections;
        log.Write<float>(1, start, QVector3<float>(0.0f), start + ray * radius, QVector3<float>(1.0f, 0.0f, 0.0f));
      } else {
        log.Write<float>(1, start, QVector3<float>(0.0f), start + ray * radius, QVector3<float>(0.4f, 0.4f, 0.4f));
      }
    }

    occlusion[vi] = nintersections / (float) nrays;
    std::cerr << "occlusion[" << vi << "]: " << occlusion[vi] << std::endl;
    /* std::cerr << "" << std::endl; */
  }

  log.Close();

  delete[] rays;
}
#endif

/* ------------------------------------------------------------------------- */

void Trimesh::ReverseWinding() {
  int *face = faces;
  for (int i = 0; i < nfaces; ++i, face += 3) {
    int tmp = face[1];
    face[1] = face[2];
    face[2] = tmp;
  } 
  ComputeNormals();
}

/* ------------------------------------------------------------------------- */

void Trimesh::FlipNormals() {
  if (!normals) {
    ComputeNormals();
  }

  float *normal = normals;
  for (int i = 0; i < nvertices; ++i, normal += 3) {
    normal[0] = -normal[0];
    normal[1] = -normal[1];
    normal[2] = -normal[2];
  }
}

/* ------------------------------------------------------------------------- */

void Trimesh::Normalize(const QVector3<float>& max,
                        const QVector3<float>& min) {
  // put mesh at origin: (p - min)
  // scale all uniformly: (p - min) / (maxspan)

  // Get maximum span.
  QVector3<float> span = bounding_box.GetMaximum() - bounding_box.GetMinimum();
  float max_span = span[0];
  for (int i = 1; i < 3; ++i) {
    if (span[i] > max_span) {
      max_span = span[i];
    }
  }

  // Bias and scale.
  QVector3<float> scale = max / max_span;
  float *position = positions;
  for (int i = 0; i < nvertices; ++i) {
    position[0] = (position[0] - bounding_box.GetMinimum()[0]) * scale[0] + min[0];
    position[1] = (position[1] - bounding_box.GetMinimum()[1]) * scale[1] + min[1];
    position[2] = (position[2] - bounding_box.GetMinimum()[2]) * scale[2] + min[2];
    position += 3;
  }

  ComputeBounds();
}

/* ------------------------------------------------------------------------- */

void Trimesh::DisconnectFaces() {
  //   for each vertex
  //     look up position, append
  //     add face normal as attribute
  QVector3<float> *fnormals = GetFaceNormals();

  int new_nvertices = nfaces * 3;
  float *new_positions = new float[3 * new_nvertices];
  float *new_normals = new float[3 * new_nvertices];

  bool has_vertex_colors = colors != NULL;
  float *new_colors = NULL;
  if (has_vertex_colors) {
    new_colors = new float[new_nvertices * ncolor_channels];
  }

  float *new_vertex_metas = NULL;
  if (nvertex_metas > 0) {
    new_vertex_metas = new float[new_nvertices * nvertex_metas];
  }

  delete[] tex1D_coords;
  delete[] tex2D_coords;
  delete[] occlusion;

  // Go through each face's vertices. Create new vertex with position +
  // face normal as attributes. Update face to point to new vertex.
  int *face = faces;
  float *position = new_positions;
  float *normal = new_normals;
  float *color = new_colors;
  float *vertex_meta = new_vertex_metas;

  for (int fi = 0, i = 0; fi < nfaces; fi++) {
    for (int vi = 0; vi < 3; ++vi, ++i) {
      memcpy(position, &positions[face[vi] * 3], sizeof(float) * 3);
      memcpy(normal, &(fnormals[fi][0]), sizeof(float) * 3);

      if (has_vertex_colors) {
        memcpy(color, &colors[face[vi] * 3], sizeof(float) * ncolor_channels);
      }

      if (nvertex_metas > 0) {
        memcpy(vertex_meta, &vertex_metas[face[vi] * nvertex_metas], sizeof(float) * nvertex_metas);
      }

      face[vi] = i;
      position += 3;
      normal += 3;
      color += 3;
      vertex_meta += nvertex_metas;
    }

    face += 3;
  }

  delete[] fnormals;
  fnormals = NULL;

  delete[] positions;
  positions = new_positions;

  delete[] colors;
  colors = new_colors;

  delete[] vertex_metas;
  vertex_metas = new_vertex_metas;

  delete[] normals;
  normals = new_normals;

  nvertices = new_nvertices;
}

/* ------------------------------------------------------------------------- */

Trimesh& Trimesh::operator+=(const Trimesh& other) {
  int new_nvertices = GetVertexCount() + other.GetVertexCount();
  int new_nfaces = GetFaceCount() + other.GetFaceCount();

  // Copy positions
  float *new_positions = new float[new_nvertices * 3];
  memcpy(new_positions, positions, sizeof(float) * GetVertexCount() * 3);
  memcpy(new_positions + GetVertexCount() * 3, other.positions, sizeof(float) * other.GetVertexCount() * 3);

  // Copy colors
  float *new_colors = NULL;
  if (colors != NULL && other.colors != NULL && ncolor_channels == other.ncolor_channels) {
    new_colors = new float[new_nvertices * ncolor_channels];
    memcpy(new_colors, colors, sizeof(float) * GetVertexCount() * ncolor_channels);
    memcpy(new_colors + GetVertexCount() * ncolor_channels, other.colors, sizeof(float) * other.GetVertexCount() * ncolor_channels);
  }
 
  // Copy metas
  float *new_vertex_metas = NULL;
  if (vertex_metas != NULL && other.vertex_metas != NULL && nvertex_metas == other.nvertex_metas) {
    new_vertex_metas = new float[new_nvertices * nvertex_metas];
    memcpy(new_vertex_metas, vertex_metas, sizeof(float) * GetVertexCount() * nvertex_metas);
    memcpy(new_vertex_metas + GetVertexCount() * nvertex_metas, other.vertex_metas, sizeof(float) * other.GetVertexCount() * nvertex_metas);
  } else {
    nvertex_metas = 0;
  }

  // Copy faces
  int *new_faces = new int[new_nfaces * 3];
  memcpy(new_faces, faces, sizeof(int) * GetFaceCount() * 3);
  memcpy(new_faces + GetFaceCount() * 3, other.faces, sizeof(int) * other.GetFaceCount() * 3);

  // Update vertex indices of appended mesh.
  int offset = nvertices;
  int *face = new_faces + nfaces * 3;
  for (int i = 0; i < other.GetFaceCount(); ++i) {
    face[0] += offset;
    face[1] += offset;
    face[2] += offset;
    face += 3;
  }

  delete[] positions;
  delete[] faces;
  delete[] colors;
  delete[] vertex_metas;

  nvertices = new_nvertices;
  nfaces = new_nfaces;

  positions = new_positions;
  faces = new_faces;
  colors = new_colors;
  vertex_metas = new_vertex_metas;

  // TODO copy normals, texcoords, etc.

  /* ComputeMeta(); */
  return *this;
}

/* ------------------------------------------------------------------------- */

Trimesh& Trimesh::operator+=(const QVector3<float>& delta) {
  float *position = positions; 

  for (int i = 0; i < nvertices; ++i) {
    position[0] += delta[0];
    position[1] += delta[1];
    position[2] += delta[2];
    position += 3;
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

Trimesh& Trimesh::operator*=(float factor) {
  float *position = positions; 

  for (int i = 0; i < nvertices; ++i) {
    position[0] *= factor;
    position[1] *= factor;
    position[2] *= factor;
    position += 3;
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

Trimesh& Trimesh::operator*=(const QMatrix4<float>& xform) {
  float *position = positions; 
  for (int i = 0; i < nvertices; ++i) {
    QVector3<float> new_position = xform * QVector3<float>(position);
    position[0] = new_position[0];
    position[1] = new_position[1];
    position[2] = new_position[2];
    position += 3;
  }

  return *this;
}

/* ------------------------------------------------------------------------- */

void Trimesh::GenerateFittedTexCoords(int d) {
  assert(d >= 0 && d < 4);

  QVector3<float> range = GetBoundingBox().GetSize();
  QVector3<float> min = GetBoundingBox().GetMinimum();

  QVector4<float> s(0.0f); 
  s[d] = 1.0f / range[d];
  s[3] = -min[d] / range[d];
  GenerateTexCoords(s);
}

/* ------------------------------------------------------------------------- */

void Trimesh::GenerateFittedTexCoords() {
  QVector3<float> range = GetBoundingBox().GetSize();
  QVector3<float> min = GetBoundingBox().GetMinimum();

  QVector4<float> s(0.0f); 
  QVector4<float> t(0.0f); 
  QVector4<float> q(0.0f); 

  s[0] = 1.0f / range[0];
  s[3] = -min[0] / range[0];
  t[1] = 1.0f / range[1];
  t[3] = -min[1] / range[1];
  q[2] = 1.0f / range[2];
  q[3] = -min[2] / range[2];

  GenerateTexCoords(s, t, q);
}

/* ------------------------------------------------------------------------- */

Trimesh::Trimesh(const Trimesh& other) {
  NullBuffers();

  nvertices = other.nvertices;
  nfaces = other.nfaces;
  bounding_sphere = other.bounding_sphere;
  bounding_box = other.bounding_box;
  centroid = other.centroid;

  positions = new float[3 * nvertices];
  memcpy(positions, other.positions, sizeof(float) * 3 * nvertices);

  faces = new int[3 * nfaces];
  memcpy(faces, other.faces, sizeof(int) * 3 * nfaces);

  if (other.normals) {
    normals = new float[3 * nvertices];
    memcpy(normals, other.normals, sizeof(float) * 3 * nvertices);
  }

  nvertex_metas = other.nvertex_metas;
  if (other.nvertex_metas > 0) {
    vertex_metas = new float[nvertex_metas * nvertices];
    memcpy(vertex_metas, other.vertex_metas, sizeof(float) * nvertex_metas * nvertices);
  }

  ncolor_channels = other.ncolor_channels;
  if (other.colors) {
    colors = new float[3 * nvertices];
    memcpy(colors, other.colors, sizeof(float) * 3 * nvertices);
  }

  if (other.tex1D_coords) {
    tex1D_coords = new float[3 * nvertices];
    memcpy(tex1D_coords, other.tex1D_coords, sizeof(float) * 3 * nvertices);
  }

  if (other.tex2D_coords) {
    tex2D_coords = new float[3 * nvertices];
    memcpy(tex2D_coords, other.tex2D_coords, sizeof(float) * 3 * nvertices);
  }

  if (other.tex3D_coords) {
    tex3D_coords = new float[3 * nvertices];
    memcpy(tex3D_coords, other.tex3D_coords, sizeof(float) * 3 * nvertices);
  }

  if (other.occlusion) {
    occlusion = new float[nvertices];
    memcpy(occlusion, other.occlusion, sizeof(float) * nvertices);
  }
}

/* ------------------------------------------------------------------------- */

float *Trimesh::AllocateVertexColors(int nchannels) {
  if (colors) delete[] colors;

  ncolor_channels = nchannels;
  colors = new float[nchannels * nvertices];
  return colors;
}

/* ------------------------------------------------------------------------- */

float *Trimesh::AllocateVertexMetas(int nvertex_metas) {
  if (vertex_metas) delete[] vertex_metas;

  this->nvertex_metas = nvertex_metas;
  vertex_metas = new float[nvertex_metas * nvertices];
  return vertex_metas;
}

/* ------------------------------------------------------------------------- */

const float *Trimesh::GetVertexMetas() const {
  return vertex_metas; 
}

/* ------------------------------------------------------------------------- */

void Trimesh::MigrateVertexMetasToColors(int r, int g, int b) {
  if (colors == NULL) {
    AllocateVertexColors(3);
  }

  float *color = colors;
  float *vertex_meta = vertex_metas;

  for (int i = 0; i < GetVertexCount(); ++i) {
    color[0] = vertex_meta[r];
    color[1] = vertex_meta[g];
    color[2] = vertex_meta[b];

    color += 3;
    vertex_meta += nvertex_metas;
  }
}

/* ------------------------------------------------------------------------- */

float *Trimesh::AllocateNormals() {
  normals = new float[nvertices * 3];
  return normals;
}

/* ------------------------------------------------------------------------- */

}
