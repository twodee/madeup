#include <iostream>

#include "MadeupRenderer.h"
#include "madeup/Environment.h"

using std::string;
using namespace td;

/* ------------------------------------------------------------------------- */

MadeupRenderer::MadeupRenderer() :
  trimesh(NULL),
  background_color(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 1.0f),
  path_color(1.0f, 0.5f, 0.0f, 1.0f),
  vertex_color(0.0f, 0.0f, 0.0f, 1.0f),
  program(NULL),
  vertex_array(NULL),
  vertex_attributes(NULL),
  uniform_manager(NULL),
  path_program(NULL),
  path_arrays(NULL),
  path_attributes(NULL),
  npaths(0),
  paths_bounding_box(td::QVector3<float>(0.0f), td::QVector3<float>(0.0f)),
  path_stroke_width(4.0f),
  vertex_size(6.0f) {
}

/* ------------------------------------------------------------------------- */

MadeupRenderer::~MadeupRenderer() {
  deletePaths();
  deleteTrimesh();

  delete program;
  delete heading_array;
  delete heading_attributes;
  delete heading_program;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  program->Bind();
  vertex_array->Bind();
  vertex_array->DrawIndexed(GL_TRIANGLES);
  vertex_array->Unbind();
  program->Unbind();

  glLineWidth(path_stroke_width);
  glPointSize(vertex_size);

  heading_program->Bind();
  heading_program->SetUniform("projection", camera.GetProjectionMatrix());
  heading_program->SetUniform("color", path_color[0], path_color[1], path_color[2], path_color[3]);
  heading_array->Bind();
  for (int i = 0; i < npaths; ++i) {
    const vector<madeup::Turtle> &path = paths[i];
    if (path.size() > 0) {
      const td::QVector3<float> last = path[path.size() - 1].position;
      td::QMatrix4<float> r = path[path.size() - 1].camera.GetViewMatrix().GetOrthonormalInverse();
      heading_program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * to_center * td::QMatrix4<float>::GetTranslate(last[0], last[1], last[2]) * r);
      heading_array->DrawIndexed(GL_TRIANGLES);
    }
  }
  heading_array->Unbind();

  heading_program->Unbind();

  path_program->Bind();

  // Paths
  for (int i = 0; i < npaths; ++i) {
    path_program->SetUniform("color", path_color[0], path_color[1], path_color[2], path_color[3]);

    // Disable depth writes on line strip so that the points appear on top of it.
    glDepthMask(GL_FALSE);
    path_arrays[i]->Bind();
    path_arrays[i]->DrawSequence(GL_LINE_STRIP);
    path_arrays[i]->Unbind();
    glDepthMask(GL_TRUE);

    path_program->SetUniform("color", vertex_color[0], vertex_color[1], vertex_color[2], vertex_color[3]);
    path_arrays[i]->Bind();
    path_arrays[i]->DrawSequence(GL_POINTS);
    path_arrays[i]->Unbind();
  }

  glDisable(GL_DEPTH_TEST);

  path_program->Unbind();

  OpenGL::CheckError("after on draw");
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::initializeGL() {
  glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);

  float positions[] = {
    -0.5f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.3f, -0.2f
  };

  int faces[] = {
    1, 0, 2,
    0, 1, 3,
    3, 2, 0,
    1, 2, 3
  };

  Trimesh heading_mesh(4, 4);
  memcpy(heading_mesh.GetPositions(), positions, sizeof(float) * 4 * 3);
  memcpy(heading_mesh.GetFaces(), faces, sizeof(int) * 4 * 3);
  heading_mesh.DisconnectFaces();
  heading_mesh.ComputeMeta();

  heading_attributes = new VertexAttributes();
  heading_attributes->AddAttribute("position", heading_mesh.GetVertexCount(), 3, heading_mesh.GetPositions());
  heading_attributes->AddAttribute("normal", heading_mesh.GetVertexCount(), 3, heading_mesh.GetNormals());
  heading_attributes->AddIndices(heading_mesh.GetFaceCount() * 3, heading_mesh.GetFaces());

  string vertex_src =
    "#version 120\n"
    "uniform mat4 projection;\n"
    "uniform mat4 modelview;\n"
    "attribute vec3 position;\n"
    "attribute vec3 normal;\n"
    "varying vec3 fnormal;\n"
    "\n"
    "void main() {\n"
    "  gl_Position = projection * modelview * vec4(position, 1.0);\n"
    "  fnormal = normalize(vec3(modelview * vec4(normal, 0.0)));\n"
    "}\n";

  string fragment_src =
    "#version 120\n"
    "uniform vec4 color;\n"
    "varying vec3 fnormal;\n"
    "\n"
    "void main() {\n"
    "  float n_dot_l = max(0.0, dot(normalize(fnormal), vec3(0.0, 0.0, 1.0)));\n"
    "  gl_FragColor = vec4(vec3(n_dot_l) * color.rgb, color.a);\n"
    "}\n";

  heading_program = ShaderProgram::FromSource(vertex_src, fragment_src);

  heading_array = new VertexArray(*heading_program, *heading_attributes);

  updateShaderProgram();

  Trimesh *trimesh = new Trimesh(0, 0);
  setTrimesh(trimesh);

  fitProjection();
  fitCameraToMesh();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setTrimesh(td::Trimesh *trimesh) {
  deletePaths();
  deleteTrimesh();

  this->trimesh = trimesh;  

  // Create vertex attributes
  vertex_attributes = new VertexAttributes();
  vertex_attributes->AddAttribute("position", trimesh->GetVertexCount(), 3, trimesh->GetPositions());
  vertex_attributes->AddAttribute("normal", trimesh->GetVertexCount(), 3, trimesh->GetNormals());
  vertex_attributes->AddAttribute("color", trimesh->GetVertexCount(), 3, trimesh->GetColors());
  vertex_attributes->AddIndices(trimesh->GetFaceCount() * 3, trimesh->GetFaces());

  vertex_array = new VertexArray(*program, *vertex_attributes);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::fitCameraToMesh() {
  td::QVector3<float> min;
  td::QVector3<float> max;

  Box<float, 3> trimesh_bounding_box = trimesh->GetBoundingBox();
  const QVector3<float> &tmin = trimesh_bounding_box.GetMinimum();
  const QVector3<float> &tmax = trimesh_bounding_box.GetMaximum();
  const QVector3<float> &pmin = paths_bounding_box.GetMinimum();
  const QVector3<float> &pmax = paths_bounding_box.GetMaximum();
  for (int d = 0; d < 3; ++d) {
    min[d] = tmin[d] < pmin[d] ? tmin[d] : pmin[d];
    max[d] = tmax[d] > pmax[d] ? tmax[d] : pmax[d];
  }

  bounding_box = Box<float, 3>(max, min);

  // Set up transformations
  center = bounding_box.GetCenter();
  radius = bounding_box.GetDiagonalLength() * 0.5f;
  float distance = radius / tan(45.0f * 0.5f * PI / 180.0f);

  to_center = td::QMatrix4<float>::GetTranslate(-center[0], -center[1], -center[2]);
  camera.LookAt(QVector3<float>(0.0f, 0.0f, 1.0f * (distance + radius)),
                QVector3<float>(0.0f, 0.0f, 0.0f),
                QVector3<float>(0.0f, 1.0f, 0.0f));

  fitProjection();

  updateUniforms();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::fitProjection() {
  float distance_to_center = (camera.GetTo() - camera.GetFrom()).GetLength();
  camera.SetOrthographic(distance_to_center, distance_to_center, getAspectRatio(), distance_to_center - 2.01f * radius, distance_to_center + 1.01f * radius);
  camera.SetPerspective(45.0f, getAspectRatio(), distance_to_center - 2.01f * radius, distance_to_center + 1.01f * radius);

  updateProjectionUniform();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateUniforms() {
  updateModelviewUniform();
  updateProjectionUniform();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateProjectionUniform() {
  program->Bind();
  program->SetUniform("projection", camera.GetProjectionMatrix());
  program->Unbind();

  path_program->Bind();
  path_program->SetUniform("projection", camera.GetProjectionMatrix());
  path_program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateModelviewUniform() {
  program->Bind();
  program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * to_center);
  program->Unbind();

  path_program->Bind();
  path_program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * to_center);
  path_program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateShaderProgram() {
  delete program;

  string vertex_src =
    "#version 120\n"
    "uniform mat4 projection;\n"
    "uniform mat4 modelview;\n"
    "attribute vec3 position;\n"
    "attribute vec3 normal;\n"
    "attribute vec3 color;\n"
    "varying vec3 fnormal;\n"
    "varying vec4 fcolor;\n"
    "\n"
    "void main() {\n"
    "  gl_Position = projection * modelview * vec4(position, 1.0);\n"
    "  fnormal = normalize(vec3(modelview * vec4(normal, 0.0)));\n"
    "  fcolor = vec4(color, 1.0);\n"
    "}\n";

  string fragment_src =
    "#version 120\n"
    "varying vec3 fnormal;\n"
    "varying vec4 fcolor;\n"
    "\n"
    "void main() {\n"
    "  float n_dot_l = max(0.0, dot(normalize(fnormal), vec3(0.0, 0.0, 1.0)));\n"
    "  gl_FragColor = vec4(vec3(n_dot_l) * fcolor.rgb, fcolor.a);\n"
    "}\n";

  std::cout << "vertex_src: " << vertex_src << std::endl;
  std::cout << "fragment_src: " << fragment_src << std::endl;

  program = ShaderProgram::FromSource(vertex_src, fragment_src);

  uniform_manager = new UniformManager();
  uniform_manager->SetProgram(program);

  // Path program
  vertex_src =
    "#version 120\n"
    "uniform mat4 projection;\n"
    "uniform mat4 modelview;\n"
    "attribute vec3 position;\n"
    "\n"
    "void main() {\n"
    "  gl_Position = projection * modelview * vec4(position, 1.0);\n"
    "}\n";

  fragment_src =
    "#version 120\n"
    "\n"
    "uniform vec4 color;\n"
    "\n"
    "void main() {\n"
    "  gl_FragColor = color;\n"
    "}\n";

  path_program = ShaderProgram::FromSource(vertex_src, fragment_src);

  updateUniforms();
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getAspectRatio() {
  return window_dimensions[0] / (float) window_dimensions[1];
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::resize(int width, int height) {
  window_dimensions[0] = width;
  window_dimensions[1] = height;

  glViewport(0, 0, width, height); 
  trackball.SetViewport(width, height);
  fitProjection();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::fit() {
  trackball.Reset();
  fitCameraToMesh();
}

/* ------------------------------------------------------------------------- */

const td::QVector4<float> MadeupRenderer::getBackgroundColor() const {
  return background_color;
}

/* ------------------------------------------------------------------------- */

const td::QVector4<float> MadeupRenderer::getPathColor() const {
  return path_color;
}

/* ------------------------------------------------------------------------- */

const td::QVector4<float> MadeupRenderer::getVertexColor() const {
  return vertex_color;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setBackgroundColor(const td::QVector4<float> &color) {
  background_color = color; 
  glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setPathColor(const td::QVector4<float> &color) {
  path_color = color; 
  // Color gets set each frame, so no update action needed if redraw triggered.
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setVertexColor(const td::QVector4<float> &color) {
  vertex_color = color; 
  // Color gets set each frame, so no update action needed if redraw triggered.
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::leftMouseDownAt(int x, int y) {
  trackball.Start(x, y);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::leftMouseDraggedTo(int x, int y) {
  trackball.Drag(x, y, 3.0f);
  program->Bind();
  updateModelviewUniform();
  program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::leftMouseUpAt(int x, int y) {
  trackball.Stop();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::scroll(int nclicks) {
  const float FACTOR = 0.05f;
  camera.Advance(radius * FACTOR * nclicks);
  program->Bind();
  updateModelviewUniform();
  program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setPaths(const std::vector<std::vector<madeup::Turtle> > &paths) {
  deletePaths();
  deleteTrimesh();

  setTrimesh(new Trimesh(0, 0));

  this->paths = paths;
  npaths = paths.size();

  for (int pi = 0; pi < npaths; ++pi) {
    std::cout << "paths[" << pi << "].size(): " << paths[pi].size() << std::endl;
  }

  if (npaths) {
    std::cerr << "npaths: " << npaths << std::endl;
    path_arrays = new VertexArray*[npaths];
    path_attributes = new VertexAttributes*[npaths];

    std::cerr << "paths[0].size(): " << paths[0].size() << std::endl;
    td::QVector3<float> min = paths[0].size() > 0 ? paths[0][0].position : td::QVector3<float>(0.0f);
    td::QVector3<float> max = min;

    for (int pi = 0; pi < npaths; ++pi) {
      float *vertices = new float[paths[pi].size() * 3];

      float *vertex = vertices;
      for (int vi = 0; vi < paths[pi].size(); ++vi, vertex += 3) {
        td::QVector3<float> v = paths[pi][vi].position;
        for (int d = 0; d < 3; ++d) {
          vertex[d] = v[d];
          if (v[d] < min[d]) {
            min[d] = v[d];
          } else if (v[d] > max[d]) {
            max[d] = v[d];
          }
        }
      }

      paths_bounding_box = Box<float, 3>(max, min);
      
      path_attributes[pi] = new VertexAttributes();
      path_attributes[pi]->AddAttribute("position", paths[pi].size(), 3, vertices);
      path_arrays[pi] = new VertexArray(*path_program, *path_attributes[pi]);
    }
  }
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getPathStrokeWidth() const {
  return path_stroke_width;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setPathStrokeWidth(float width) {
  path_stroke_width = width;
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getVertexSize() const {
  return vertex_size;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setVertexSize(float size) {
  vertex_size = size;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::deletePaths() {
  // Delete previous paths.
  for (int i = 0; i < npaths; ++i) {
    delete path_arrays[i];
    delete path_attributes[i];
  }

  delete path_arrays;
  delete path_attributes;

  path_arrays = NULL;
  path_attributes = NULL;

  npaths = 0;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::deleteTrimesh() {
  delete vertex_array; 
  delete vertex_attributes;
  delete trimesh;

  vertex_array = NULL;
  vertex_attributes = NULL;
  trimesh = NULL;
}

/* ------------------------------------------------------------------------- */

