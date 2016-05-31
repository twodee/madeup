#ifndef MADEUPRENDERER_H
#define MADEUPRENDERER_H

#include "twodee/Trimesh.h"
#include "twodee/Camera.h"
#include "VertexArray.h"
#include "UniformManager.h"
#include "Trackball.h"

class MadeupRenderer {
  public:
    MadeupRenderer();
    ~MadeupRenderer();

    void SetTrimesh(td::Trimesh *trimesh);
    void Render();
    void Resize(int width, int height);
    void InitializeGL();
    void Fit();
    const td::QVector4<float> GetBackgroundColor() const;
    void SetBackgroundColor(const td::QVector4<float> &color);

    void LeftMouseDownAt(int x, int y);
    void LeftMouseDraggedTo(int x, int y);
    void LeftMouseUpAt(int x, int y);
    void Scroll(int nclicks);

  private:
    void UpdateVertexAttributes();
    void FitCameraToMesh();
    void FitProjection();
    void UpdateUniforms();
    void UpdateProjectionUniform();
    void UpdateModelviewUniform();
    void UpdateVertexArray();
    void UpdateShaderProgram();
    float GetAspectRatio();

    td::Trimesh *trimesh;
    td::QVector2<int> window_dimensions;
    td::QVector4<float> color;
    td::QVector4<float> background_color;
    td::ShaderProgram *program;
    td::VertexArray *vertex_array;
    td::VertexAttributes *vertex_attributes;
    td::UniformManager *uniform_manager;

    td::QVector3<float> center;
    td::QMatrix4<float> to_center;
    td::Camera camera;
    td::Trackball trackball;
    float radius;
};

#endif
