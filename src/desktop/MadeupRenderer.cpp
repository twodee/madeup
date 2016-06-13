#include <iostream>

#include "MadeupRenderer.h"
#include "madeup/Environment.h"

using std::string;
using namespace td;

/* ------------------------------------------------------------------------- */

MadeupRenderer::MadeupRenderer() :
  trimesh(NULL),
  background_color(1.0f),
  path_color(1.0f, 0.5f, 0.0f, 1.0f),
  vertex_color(0.0f, 0.0f, 0.0f, 1.0f),
  program(NULL),
  vertex_array(NULL),
  vertex_attributes(NULL),
  line_program(NULL),
  path_arrays(NULL),
  path_attributes(NULL),
  npaths(0),
  axes_array(NULL),
  axes_attributes(NULL),
  paths_bounding_box(td::QVector3<float>(0.0f), td::QVector3<float>(0.0f)),
  show_heading(true),
  show_stops(true),
  face_orientation(FaceOrientation::COUNTERCLOCKWISE),
  face_style(FaceStyle::FILLED),
  path_stroke_width(4.0f),
  axis_stroke_width(3.0f),
  grid_stroke_width(1.0f),
  vertex_size(6.0f),
  azimuth_angle(45.0f),
  elevation_angle(70.0f),
  shininess(0.0f),
  light_distance_factor(2.0f) {

  show_axis[0] = show_axis[1] = show_axis[2] = true;
  show_grid[0] = show_grid[1] = show_grid[2] = true;

  grid_extents[0] = grid_extents[1] = grid_extents[2] = 10.0f;
  grid_spacing[0] = grid_spacing[1] = grid_spacing[2] = 1.0f; 
  grid_arrays[0] = grid_arrays[1] = grid_arrays[2] = NULL;
  grid_attributes[0] = grid_attributes[1] = grid_attributes[2] = NULL;
}

/* ------------------------------------------------------------------------- */

MadeupRenderer::~MadeupRenderer() {
  deleteAxes();
  deleteGrids();
  deletePaths();
  deleteTrimesh();

  delete program;
  delete heading_array;
  delete heading_attributes;
  delete heading_program;
  delete line_program;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  float azimuth_angle_radians = azimuth_angle * td::PI / 180.0f;
  float elevation_angle_radians = elevation_angle * td::PI / 180.0f;
  td::QVector4<float> light_position_world;
  light_position_world[0] = cos(elevation_angle_radians) * cos(azimuth_angle_radians);
  light_position_world[1] = sin(elevation_angle_radians);
  light_position_world[2] = cos(elevation_angle_radians) * sin(azimuth_angle_radians);
  light_position_world[3] = 1.0f;

  {
    light_position_world *= light_distance_factor * bbox_radius;
    light_position_world[3] = 1.0f;
    VertexAttribute *attr = debug_attributes->GetAttribute("position");
    float positions[] = {
      0.0f, 0.0f, 0.0f,
      light_position_world[0], light_position_world[1], light_position_world[2]
    };
    attr->Update(positions);
  }

  glLineWidth(path_stroke_width);
  glPointSize(vertex_size);

  line_program->Bind();
  debug_array->Bind();
  debug_array->DrawSequence(GL_LINES);
  debug_array->Unbind();
  line_program->Unbind();

  program->Bind();
  td::QVector4<float> light_position_eye = camera.GetViewMatrix() * light_position_world;
  program->SetUniform("light_position_eye", light_position_eye[0], light_position_eye[1], light_position_eye[2]);
  program->SetUniform("shininess", shininess);
  vertex_array->Bind();
  vertex_array->DrawIndexed(GL_TRIANGLES);
  vertex_array->Unbind();
  program->Unbind();

  if (show_heading) {
    heading_program->Bind();
    heading_program->SetUniform("projection", camera.GetProjectionMatrix());
    heading_program->SetUniform("color", path_color[0], path_color[1], path_color[2], path_color[3]);
    heading_array->Bind();
    for (int i = 0; i < npaths; ++i) {
      const vector<madeup::Turtle> &path = paths[i];
      if (path.size() > 0) {
        const td::QVector3<float> last = path[path.size() - 1].position;
        td::QMatrix4<float> r = path[path.size() - 1].camera.GetViewMatrix().GetOrthonormalInverse();
        heading_program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * center_mesh_xform * td::QMatrix4<float>::GetTranslate(last[0], last[1], last[2]) * r);
        heading_array->DrawIndexed(GL_TRIANGLES);
      }
    }
    heading_array->Unbind();
    heading_program->Unbind();
  }

  line_program->Bind();

  // Paths
  for (int i = 0; i < npaths; ++i) {
    line_program->SetUniform("color", path_color[0], path_color[1], path_color[2], path_color[3]);

    // Disable depth writes on line strip so that the points appear on top of it.
    glDepthMask(GL_FALSE);
    path_arrays[i]->Bind();
    path_arrays[i]->DrawSequence(GL_LINE_STRIP);
    path_arrays[i]->Unbind();
    glDepthMask(GL_TRUE);

    if (show_stops) {
      line_program->SetUniform("color", vertex_color[0], vertex_color[1], vertex_color[2], vertex_color[3]);
      path_arrays[i]->Bind();
      path_arrays[i]->DrawSequence(GL_POINTS);
      path_arrays[i]->Unbind();
    }
  }

  glLineWidth(axis_stroke_width);
  axes_array->Bind();
  for (int d = 0; d < 3; ++d) {
    if (show_axis[d]) {
      QVector4<float> color(0.0f, 0.0f, 0.0f, 1.0f);
      color[d] = 1.0f;
      line_program->SetUniform("color", color[0], color[1], color[2], color[3]);
      axes_array->DrawSequence(GL_LINES, d * 2, 2);
    }
  }
  axes_array->Unbind();

  glLineWidth(grid_stroke_width);
  for (int d = 0; d < 3; ++d) {
    if (show_grid[d]) {
      grid_arrays[d]->Bind();
      QVector4<float> color(0.0f, 0.0f, 0.0f, 1.0f);
      color[d] = 1.0f;
      line_program->SetUniform("color", color[0], color[1], color[2], color[3]);
      grid_arrays[d]->DrawSequence(GL_LINES);
      grid_arrays[d]->Unbind();
    }
  }

  glDisable(GL_DEPTH_TEST);

  line_program->Unbind();

  OpenGL::CheckError("after on draw");
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::initializeGL() {
  glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  setFaceOrientation(face_orientation);

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
    "varying vec4 fposition;\n"
    "\n"
    "void main() {\n"
    "  fposition = modelview * vec4(position, 1.0);\n"
    "  gl_Position = projection * fposition;\n"
    "  fnormal = normalize(vec3(modelview * vec4(normal, 0.0)));\n"
    "}\n";

  string fragment_src =
    "#version 120\n"
    "uniform vec4 color;\n"
    "uniform vec3 light_position_eye;\n"
    "varying vec3 fnormal;\n"
    "varying vec4 fposition;\n"
    "\n"
    "void main() {\n"
    "  vec3 to_light = normalize(light_position_eye - fposition.xyz);\n"
    "  float n_dot_l = max(0.0, dot(normalize(fnormal), to_light));\n"
    "  gl_FragColor = vec4(vec3(n_dot_l) * color.rgb, color.a);\n"
    "}\n";

  heading_program = ShaderProgram::FromSource(vertex_src, fragment_src);
  heading_array = new VertexArray(*heading_program, *heading_attributes);

  updateShaderProgram();
  updateAxes();
  updateGrids();

  debug_attributes = new VertexAttributes();
  {
    float positions[] = {
      0.0f, 0.0f, 0.0f,
      20.0f, 20.0f, 20.0f,
    };

    debug_attributes->AddAttribute("position", 2, 3, positions);
  }
  debug_array = new VertexArray(*line_program, *debug_attributes);


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
  // First we need to know the space we wish to keep in view. We find the
  // bounding box that encloses the mesh, any paths, and any axes and grids.
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

    // Enclose grids and axes too, if shown.
    if (show_axis[d] || show_grid[d]) {
      if (-grid_extents[d] < min[d]) {
        min[d] = -grid_extents[d];
      }
      if (grid_extents[d] > max[d]) {
        max[d] = grid_extents[d];
      }
    }
  }

  /* std::cout << "min: " << min << std::endl; */
  /* std::cout << "max: " << max << std::endl; */

  // Make it a cube, using the extrema as the bounds.
  /* float minmin = min.GetMinimum(); */
  /* float maxmax = max.GetMaximum(); */
  /* min[0] = min[1] = min[2] = minmin; */
  /* max[0] = max[1] = max[2] = maxmax; */

  bbox = Box<float, 3>(max, min);

  // Set up transformations
  td::QVector3<float> bbox_center = bbox.GetCenter();
  bbox_radius = bbox.GetDiagonalLength() * 0.5f;

  float viewport_limit;
  /* std::cout << "GetVerticalFieldOfView(): " << camera.GetVerticalFieldOfView() << std::endl; */
  /* std::cout << "camera.GetHorizontalFieldOfView(getAspectRatio()): " << camera.GetHorizontalFieldOfView(getAspectRatio()) << std::endl; */
  if (getAspectRatio() >= 1) {
    viewport_limit = tan(camera.GetHorizontalFieldOfView(getAspectRatio()) * td::PI / 180.0f * 0.5f);
  } else {
    viewport_limit = tan(camera.GetVerticalFieldOfView() * td::PI / 180.0f * 0.5f);
  }
  /* std::cout << "viewport_limit: " << viewport_limit << std::endl; */

  /* std::cout << "bbox.GetSize(): " << bbox.GetSize() << std::endl; */
  /* std::cout << "bbox.GetSize().GetMaximum(): " << bbox.GetSize().GetMaximum() << std::endl; */
  /* push = bbox.GetSize().GetMaximum() / viewport_limit; */
  push = bbox_radius / viewport_limit;
  /* float distance = bbox_radius / tan(45.0f * 0.5f * PI / 180.0f); */

  center_mesh_xform = td::QMatrix4<float>::GetTranslate(-bbox_center[0], -bbox_center[1], -bbox_center[2]);
  camera.LookAt(QVector3<float>(0.0f, 0.0f, bbox_radius + push + 0.1f),
                QVector3<float>(0.0f, 0.0f, 0.0f),
                QVector3<float>(0.0f, 1.0f, 0.0f));

  fitProjection();
  updateUniforms();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::fitProjection() {
  /* camera.SetPerspective(45.0f, getAspectRatio(), 0.09f + push, push + bbox_radius * 2.0f + 0.1f); */
  camera.SetPerspective(45.0f, getAspectRatio(), 0.01f, (push + bbox_radius * 2.0f + 0.1f) * 15);
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

  line_program->Bind();
  line_program->SetUniform("projection", camera.GetProjectionMatrix());
  line_program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateModelviewUniform() {
  program->Bind();
  program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * center_mesh_xform);
  program->Unbind();

  line_program->Bind();
  line_program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * center_mesh_xform);
  line_program->Unbind();
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
    "varying vec4 fposition;\n"
    "\n"
    "void main() {\n"
    "  fposition = modelview * vec4(position, 1.0);\n"
    "  gl_Position = projection * fposition;\n"
    "  fnormal = normalize(vec3(modelview * vec4(normal, 0.0)));\n"
    "  fcolor = vec4(color, 1.0);\n"
    "}\n";

  string fragment_src =
    "#version 120\n"
    "uniform vec3 light_position_eye;\n"
    "uniform float shininess;\n"
    "varying vec3 fnormal;\n"
    "varying vec4 fcolor;\n"
    "varying vec4 fposition;\n"
    "\n"
    "void main() {\n"
    "  vec3 to_light = normalize(light_position_eye - fposition.xyz);\n"
    "  vec3 normal = normalize(fnormal);\n"
    "  vec3 halfway = normalize(to_light - fposition.xyz);\n"
    "  float n_dot_l = max(0.0, dot(normal, to_light));\n"
    "  float n_dot_h = max(0.0f, dot(normal, halfway));\n"
    "  gl_FragColor = vec4(fcolor.rgb * n_dot_l + vec3(1.0) * pow(n_dot_h, shininess), fcolor.a);\n"
    "}\n";

  program = ShaderProgram::FromSource(vertex_src, fragment_src);

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

  line_program = ShaderProgram::FromSource(vertex_src, fragment_src);

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

float MadeupRenderer::leftMouseUpAt(int x, int y) {
  trackball.Stop();
  const td::QVector2<int> &delta = trackball.GetDelta();
  float magnitude = sqrtf(delta[0] * delta[0] + delta[1] * delta[1]);
  return magnitude;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::scroll(int nclicks) {
  const float FACTOR = 0.05f;
  camera.Advance(bbox_radius * FACTOR * nclicks);
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

  if (npaths) {
    path_arrays = new VertexArray*[npaths];
    path_attributes = new VertexAttributes*[npaths];

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
      path_arrays[pi] = new VertexArray(*line_program, *path_attributes[pi]);
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

bool MadeupRenderer::showHeading() const {
  return show_heading;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::showHeading(bool show) {
  show_heading = show;
  // Effect will be seen next frame.
}

/* ------------------------------------------------------------------------- */

bool MadeupRenderer::showStops() const {
  return show_stops;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::showStops(bool show) {
  show_stops = show;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setFaceOrientation(int orientation) {
  if (orientation == FaceOrientation::COUNTERCLOCKWISE) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else if (orientation == FaceOrientation::CLOCKWISE) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
  } else if (orientation == FaceOrientation::BOTH) {
    glDisable(GL_CULL_FACE);
  } else if (orientation == FaceOrientation::NONE) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_AND_BACK);
  } else {
    std::cerr << "bad face orientation: " << orientation << std::endl;
    return;
  }
  face_orientation = orientation;
}

/* ------------------------------------------------------------------------- */

int MadeupRenderer::getFaceOrientation() const {
  return face_orientation;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setFaceStyle(int style) {
  if (style == FaceStyle::FILLED) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  } else if (style == FaceStyle::WIREFRAME) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else if (style == FaceStyle::VERTICES) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  } else {
    std::cerr << "bad face style: " << style << std::endl;
    return;
  }
  face_style = style;
}

/* ------------------------------------------------------------------------- */

int MadeupRenderer::getFaceStyle() const {
  return face_style; 
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::takeScreenshot(const std::string &path) {
  NField<unsigned char, 2> image(window_dimensions, 4);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  /* glPixelStorei(GL_PACK_ROW_LENGTH, window_dimensions[0]); */
  /* glPixelStorei(GL_PACK_IMAGE_HEIGHT, window_dimensions[1]); */
  glReadPixels(0, 0, window_dimensions[0], window_dimensions[1], GL_RGBA, GL_UNSIGNED_BYTE, image(0));
  image.Write(path);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::exportTrimesh(const std::string &path) {
  trimesh->WriteObj(path);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::autoRotate() {
  trackball.Autorotate(); 
  updateModelviewUniform();  
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::showAxis(int i, bool show) {
  assert(i >= 0 && i < 3);
  show_axis[i] = show;
}

/* ------------------------------------------------------------------------- */

bool MadeupRenderer::showAxis(int i) const {
  assert(i >= 0 && i < 3);
  return show_axis[i]; 
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::showGrid(int i, bool show) {
  assert(i >= 0 && i < 3);
  show_grid[i] = show;
}

/* ------------------------------------------------------------------------- */

bool MadeupRenderer::showGrid(int i) const {
  assert(i >= 0 && i < 3);
  return show_grid[i]; 
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateAxes() {
  deleteAxes();

  float positions[3 * 2 * 3];
  float *position = positions;

  for (int d = 0; d < 3; ++d) {
    position[(d + 0) % 3] = -grid_extents[d];
    position[(d + 1) % 3] = 0.0f;
    position[(d + 2) % 3] = 0.0f;
    position += 3;

    position[(d + 0) % 3] = grid_extents[d];
    position[(d + 1) % 3] = 0.0f;
    position[(d + 2) % 3] = 0.0f;
    position += 3;
  }

  axes_attributes = new VertexAttributes();
  axes_attributes->AddAttribute("position", 6, 3, positions);

  axes_array = new VertexArray(*line_program, *axes_attributes);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::deleteAxes() {
  delete axes_array;
  delete axes_attributes;

  axes_array = NULL;
  axes_attributes = NULL;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setGridExtent(int i, float extent) {
  assert(i >= 0 && i < 3);
  grid_extents[i] = extent;
  updateAxes();
  updateGrids();
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getGridExtent(int i) const {
  assert(i >= 0 && i < 3);
  return grid_extents[i];
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setGridSpacing(int i, float spacing) {
  assert(i >= 0 && i < 3);
  grid_spacing[i] = spacing;
  updateGrids();
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getGridSpacing(int i) const {
  assert(i >= 0 && i < 3);
  return grid_spacing[i];
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::updateGrids() {
  deleteGrids();

  for (int d = 0; d < 3; ++d) {
    int d1 = (d + 1) % 3;
    int d2 = (d + 2) % 3;

    int nlines1 = ((int) grid_extents[d1] / grid_spacing[d1]);
    int nlines2 = ((int) grid_extents[d2] / grid_spacing[d2]);
    int nvertices = (nlines1 * 2 + nlines2 * 2) * 2;

    float *positions = new float[nvertices * 3];
    float *position = positions;

    for (int i = 1; i <= nlines1; ++i) {
      float f = i * grid_spacing[d1];

      // Positive side
      position[d] = 0.0f;
      position[d1] = f;
      position[d2] = -grid_extents[d2];
      position += 3;

      position[d] = 0.0f;
      position[d1] = f;
      position[d2] = grid_extents[d2];
      position += 3;

      // Negative side
      position[d] = 0.0f;
      position[d1] = -f;
      position[d2] = -grid_extents[d2];
      position += 3;

      position[d] = 0.0f;
      position[d1] = -f;
      position[d2] = grid_extents[d2];
      position += 3;
    }

    for (int i = 1; i <= nlines2; ++i) {
      float f = i * grid_spacing[d2];

      // Positive side
      position[d] = 0.0f;
      position[d1] = -grid_extents[d1];
      position[d2] = f;
      position += 3;

      position[d] = 0.0f;
      position[d1] = grid_extents[d1];
      position[d2] = f;
      position += 3;

      // Negative side
      position[d] = 0.0f;
      position[d1] = -grid_extents[d1];
      position[d2] = -f;
      position += 3;

      position[d] = 0.0f;
      position[d1] = grid_extents[d1];
      position[d2] = -f;
      position += 3;
    }

    grid_attributes[d] = new VertexAttributes();
    grid_attributes[d]->AddAttribute("position", nvertices, 3, positions);

    grid_arrays[d] = new VertexArray(*line_program, *grid_attributes[d]);
  }

}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::deleteGrids() {
  for (int d = 0; d < 3; ++d) {
    delete grid_arrays[d];
    grid_arrays[d] = NULL;

    delete grid_attributes[d];
    grid_attributes[d] = NULL;
  }
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setAxisStrokeWidth(float width) {
  axis_stroke_width = width;
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getAxisStrokeWidth() const {
  return axis_stroke_width;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setGridStrokeWidth(float width) {
  grid_stroke_width = width;
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getGridStrokeWidth() const {
  return grid_stroke_width;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::viewFromAxis(int d, int sign) {
  trackball.Reset();
  fitCameraToMesh(); 
  if (d == 0) {
    trackball.Rotate(sign * -90.0f, td::QVector3<float>(0.0f, 1.0f, 0.0f));
  } else if (d == 1) {
    trackball.Rotate(sign * 90.0f, td::QVector3<float>(1.0f, 0.0f, 0.0f));
  } else if (d == 2 && sign < 0) {
    trackball.Rotate(180.0f, td::QVector3<float>(0.0f, 1.0f, 0.0f));
  }
  updateModelviewUniform();  
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getAzimuthAngle() const {
  return azimuth_angle;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setAzimuthAngle(float angle) {
  azimuth_angle = angle;
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getElevationAngle() const {
  return elevation_angle;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setElevationAngle(float angle) {
  elevation_angle = angle;
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getShininess() const {
  return shininess;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setShininess(float shininess) {
  this->shininess = shininess;
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::getLightDistanceFactor() const {
  return light_distance_factor;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::setLightDistanceFactor(float factor) {
  light_distance_factor = factor;
}

/* ------------------------------------------------------------------------- */

