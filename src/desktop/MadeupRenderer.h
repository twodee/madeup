#ifndef MADEUPRENDERER_H
#define MADEUPRENDERER_H

#include "madeup/Environment.h"
#include "twodee/Trimesh.h"
#include "twodee/Camera.h"
#include "VertexArray.h"
#include "UniformManager.h"
#include "Trackball.h"

class MadeupRenderer {
  public:
    MadeupRenderer();
    ~MadeupRenderer();

    void setTrimesh(td::Trimesh *trimesh);
    void setPaths(const std::vector<std::vector<madeup::Turtle> > &paths);
    float getPathStrokeWidth() const;
    void setPathStrokeWidth(float width);
    float getVertexSize() const;
    void setVertexSize(float size);
    void render();
    void resize(int width, int height);
    void initializeGL();
    void fit();
    const td::QVector4<float> getBackgroundColor() const;
    const td::QVector4<float> getPathColor() const;
    const td::QVector4<float> getVertexColor() const;
    void setBackgroundColor(const td::QVector4<float> &color);
    void setPathColor(const td::QVector4<float> &color);
    void setVertexColor(const td::QVector4<float> &color);

    void leftMouseDownAt(int x, int y);
    void leftMouseDraggedTo(int x, int y);
    void leftMouseUpAt(int x, int y);
    void scroll(int nclicks);

  private:
    void deletePaths();
    void deleteTrimesh();
    void fitCameraToMesh();
    void fitProjection();
    void updateUniforms();
    void updateProjectionUniform();
    void updateModelviewUniform();
    void updateShaderProgram();
    float getAspectRatio();

    td::Trimesh *trimesh;
    td::QVector2<int> window_dimensions;
    td::QVector4<float> background_color;
    td::QVector4<float> path_color;
    td::QVector4<float> vertex_color;

    td::ShaderProgram *program;
    td::VertexArray *vertex_array;
    td::VertexAttributes *vertex_attributes;
    
    td::ShaderProgram *heading_program;
    td::VertexArray *heading_array;
    td::VertexAttributes *heading_attributes;

    td::UniformManager *uniform_manager;

    td::ShaderProgram *path_program;
    td::VertexArray **path_arrays;
    td::VertexAttributes **path_attributes;
    int npaths;

    std::vector<std::vector<madeup::Turtle> > paths;

    td::Box<float, 3> bounding_box;
    td::Box<float, 3> paths_bounding_box;
    td::QVector3<float> center;
    td::QMatrix4<float> to_center;
    td::Camera camera;
    td::Trackball trackball;
    float radius;
    float path_stroke_width;
    float vertex_size;
};

#endif
