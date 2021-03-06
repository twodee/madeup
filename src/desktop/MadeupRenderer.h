#ifndef MADEUPRENDERER_H
#define MADEUPRENDERER_H

#include "madeup/Environment.h"
#include "twodee/Trimesh.h"
#include "twodee/Camera.h"
#include "twodee/VertexArray.h"
#include "twodee/Trackball.h"

namespace RenderStyle {
  enum {
    FILLED,
    WIREFRAME,
    VERTICES
  };
}

class MadeupRenderer {
  public:

    MadeupRenderer();
    ~MadeupRenderer();

    void setRenderStyle(int style);
    int getRenderStyle() const;
    void exportTrimesh(const std::string &path);
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
    void showHeading(bool show);
    bool showHeading() const;
    void showPath(bool show);
    bool showPath() const;
    void showStops(bool show);
    bool showStops() const;
    void isTwoSided(bool enabled);
    bool isTwoSided() const;
    bool hasSpecular() const;
    void hasSpecular(bool enabled);
    bool hasAutorotate() const;
    void hasAutorotate(bool enabled);

    void leftMouseDownAt(int x, int y);
    void leftMouseDraggedTo(int x, int y);
    float leftMouseUpAt(int x, int y);
    void scroll(int nclicks);
    void takeScreenshot(const std::string &path);
    void autoRotate();

    void showAxis(int i, bool show);
    bool showAxis(int i) const;
    void showGrid(int i, bool show);
    bool showGrid(int i) const;
    void setGridExtent(int i, float extent);
    float getGridExtent(int i) const;
    void setGridSpacing(int i, float spacing);
    float getGridSpacing(int i) const;

    void setAxisStrokeWidth(float width);
    float getAxisStrokeWidth() const;
    void setGridStrokeWidth(float width);
    float getGridStrokeWidth() const;
    void viewFromAxis(int d, int sign);

    float getAzimuthAngle() const;
    void setAzimuthAngle(float angle);

    float getElevationAngle() const;
    void setElevationAngle(float angle);

    float getShininess() const;
    void setShininess(float shininess);

    float getLightDistanceFactor() const;
    void setLightDistanceFactor(float factor);

  private:
    void deleteAxes();
    void deleteGrids();
    void deletePaths();
    void deleteTrimesh();
    void fitCameraToMesh();
    void fitProjection();
    void updateUniforms();
    void updateProjectionUniform();
    void updateModelviewUniform();
    void updateShaderProgram();
    void updateAxes();
    void updateGrids();
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

    td::ShaderProgram *line_program;
    td::VertexArray **path_arrays;
    td::VertexAttributes **path_attributes;
    int npaths;

    td::VertexArray *axes_array;
    td::VertexAttributes *axes_attributes;

    td::VertexArray *grid_arrays[3];
    td::VertexAttributes *grid_attributes[3];

    std::vector<std::vector<madeup::Turtle> > paths;

    td::Box<float, 3> bbox;
    td::Box<float, 3> paths_bounding_box;
    td::QMatrix4<float> center_mesh_xform;
    td::Camera camera;
    td::Trackball trackball;
    float bbox_radius;
    float push;

    bool show_heading;
    bool show_path;
    bool show_stops;
    bool is_two_sided;
    bool has_specular;
    bool has_autorotate;
    int render_style;

    float path_stroke_width;
    float axis_stroke_width;
    float grid_stroke_width;
    float vertex_size;

    bool show_axis[3];
    bool show_grid[3];
    float grid_extents[3];
    float grid_spacing[3];

    float azimuth_angle;
    float elevation_angle;
    float shininess;
    float light_distance_factor;

    td::VertexAttributes *debug_attributes;
    td::VertexArray *debug_array;
};

#endif
