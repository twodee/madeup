#include <iostream>

#include "MadeupRenderer.h"

using std::string;
using namespace td;

/* ------------------------------------------------------------------------- */

MadeupRenderer::MadeupRenderer() :
  color(1.0f, 0.0f, 0.0f, 1.0f),
  background_color(100 / 255.0f, 149 / 255.0f, 237 / 255.0f, 1.0f),
  program(NULL),
  vertex_array(NULL),
  vertex_attributes(NULL),
  uniform_manager(NULL) {
}

/* ------------------------------------------------------------------------- */

MadeupRenderer::~MadeupRenderer() {
  delete vertex_array; 
  delete program;
  delete vertex_attributes;
  delete trimesh;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
  program->Bind();
  vertex_array->Bind();
  vertex_array->DrawIndexed(GL_TRIANGLES);
  vertex_array->Unbind();
  program->Unbind();
  glDisable(GL_DEPTH_TEST);

  OpenGL::CheckError("after on draw");
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::InitializeGL() {
  glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
  glClearDepth(1.0);
  glDepthFunc(GL_LESS);

  Trimesh *trimesh = new Trimesh(0, 0);
  SetTrimesh(trimesh);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::SetTrimesh(td::Trimesh *trimesh) {
  delete this->trimesh;
  this->trimesh = trimesh;  
  UpdateVertexAttributes();
  UpdateShaderProgram();
  UpdateVertexArray();
  FitCameraToMesh();
  FitProjection();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::UpdateVertexAttributes() {
  delete vertex_attributes;

  // Create vertex attributes
  vertex_attributes = new VertexAttributes();
  vertex_attributes->AddAttribute("position", trimesh->GetVertexCount(), 3, trimesh->GetPositions());
  vertex_attributes->AddAttribute("normal", trimesh->GetVertexCount(), 3, trimesh->GetNormals());
  vertex_attributes->AddAttribute("color", trimesh->GetVertexCount(), 3, trimesh->GetColors());
  vertex_attributes->AddIndices(trimesh->GetFaceCount() * 3, trimesh->GetFaces());
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::FitCameraToMesh() {
  // Set up transformations
  center = trimesh->GetBoundingBox().GetCenter();
  radius = trimesh->GetBoundingBox().GetDiagonalLength() * 0.5f;
  float distance = radius / tan(45.0f * 0.5f * PI / 180.0f);

  to_center = td::QMatrix4<float>::GetTranslate(-center[0], -center[1], -center[2]);
  camera.LookAt(QVector3<float>(0.0f, 0.0f, 1.0f * (distance + radius)),
                QVector3<float>(0.0f, 0.0f, 0.0f),
                QVector3<float>(0.0f, 1.0f, 0.0f));

  UpdateUniforms();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::FitProjection() {
  float distance_to_center = (camera.GetTo() - camera.GetFrom()).GetLength();
  camera.SetOrthographic(distance_to_center, distance_to_center, GetAspectRatio(), distance_to_center - 2.01f * radius, distance_to_center + 1.01f * radius);
  camera.SetPerspective(45.0f, GetAspectRatio(), distance_to_center - 2.01f * radius, distance_to_center + 1.01f * radius);

  program->Bind();
  UpdateProjectionUniform();
  program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::UpdateUniforms() {
  program->Bind();
  UpdateModelviewUniform();
  UpdateProjectionUniform();
  program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::UpdateProjectionUniform() {
  program->SetUniform("projection", camera.GetProjectionMatrix());
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::UpdateModelviewUniform() {
  program->SetUniform("modelview", camera.GetViewMatrix() * trackball.GetMatrix() * to_center);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::UpdateVertexArray() {
  assert(program && vertex_attributes);
  delete vertex_array;
  vertex_array = new VertexArray(*program, *vertex_attributes);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::UpdateShaderProgram() {
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

  program->Bind();
  program->SetUniform("color", color[0], color[1], color[2], color[3]);
  program->Unbind();

  uniform_manager = new UniformManager();
  uniform_manager->SetProgram(program);

  UpdateUniforms();
}

/* ------------------------------------------------------------------------- */

float MadeupRenderer::GetAspectRatio() {
  return window_dimensions[0] / (float) window_dimensions[1];
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::Resize(int width, int height) {
  window_dimensions[0] = width;
  window_dimensions[1] = height;

  glViewport(0, 0, width, height); 
  trackball.SetViewport(width, height);
  FitProjection();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::Fit() {
  trackball.Reset();
  FitCameraToMesh();
}

/* ------------------------------------------------------------------------- */

const td::QVector4<float> MadeupRenderer::GetBackgroundColor() const {
  return background_color;
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::SetBackgroundColor(const td::QVector4<float> &color) {
  background_color = color; 
  glClearColor(background_color[0], background_color[1], background_color[2], background_color[3]);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::LeftMouseDownAt(int x, int y) {
  trackball.Start(x, y);
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::LeftMouseDraggedTo(int x, int y) {
  trackball.Drag(x, y, 3.0f);
  program->Bind();
  UpdateModelviewUniform();
  program->Unbind();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::LeftMouseUpAt(int x, int y) {
  trackball.Stop();
}

/* ------------------------------------------------------------------------- */

void MadeupRenderer::Scroll(int nclicks) {
  const float FACTOR = 0.05f;
  camera.Advance(radius * FACTOR * nclicks);
  program->Bind();
  UpdateModelviewUniform();
  program->Unbind();
}

/* ------------------------------------------------------------------------- */

