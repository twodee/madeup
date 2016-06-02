#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <chrono>
#include <map>
#include <string>
#include <vector>
#include <stack>

#include "madeup/GeometryMode.h"
#include "madeup/SourceLocation.h"
#include "twodee/Camera.h"
#include "twodee/Co.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector3.h"
#include "twodee/QVector4.h"
#include "twodee/Trimesh.h"

using std::map;
using std::pair;
using std::stack;
using std::string;
using std::vector;

namespace madeup {

struct Node {
  td::QVector3<float> position;
  td::QVector3<float> rgb;
  float outer_radius;
  float inner_radius;
  float energy;
  float halflife;
};

struct Turtle {
  td::QVector3<float> position;
  td::Camera camera;
};

class ExpressionClosure;

/* ------------------------------------------------------------------------- */

class Environment {
  friend std::ostream &operator<<(std::ostream &out, const Environment env);
  public:
    Environment();
    Environment(const Environment &other);

    void add(const string &id, td::Co<ExpressionClosure> expression);
    void replace(const string &id, td::Co<ExpressionClosure> expression);
    bool isBound(const string &id) const;

    td::Co<ExpressionClosure> operator[](const string &id);

    void prime();
    void recordVertex();
    void recordPreview();
    void move(float distance);
    void movex(float distance);
    void moveTo(float x, float y, float z);
    void scale(float x, float y, float z);
    void translate(float x, float y, float z);
    void rotate(float x, float y, float z, float degrees);
    void center();
    void identity();
    int coalesce(float threshold);
    void home();
    void reverse();
    void echo(td::Co<td::Trimesh> mesh);
    void echo(const std::vector<Node> &path);
    void echoWithoutTransform(td::Co<td::Trimesh> mesh);
    void yaw(float degrees);
    void pitch(float degrees);
    void roll(float degrees);
    td::Co<td::Trimesh> dowel(float twist, float max_bend);
    td::Co<td::Trimesh> tube(float twist, float max_bend);
    td::Co<td::Trimesh> polygon(bool is_flipped);
    td::Co<td::Trimesh> revolve();
    td::Co<td::Trimesh> extrude(const td::QVector3<float> &axis, float length);
    td::Co<td::Trimesh> spheres();
    td::Co<td::Trimesh> boxes();
    td::Co<td::Trimesh> blobs(float grain, float iso);
    td::Co<td::Trimesh> surface(int width, int height);
    void forget();
    void axis(float x, float y, float z);
    void reframe();

    void push();
    void pop();

    void checkTimeout(const SourceLocation &location);

    td::Trimesh *getMesh();
    std::string getPathsJSON() const;
    void setGeometryMode(GeometryMode::geometry_mode_t mode);
    GeometryMode::geometry_mode_t getGeometryMode() const;

    float getVariableAsFloat(const std::string &id);
    const Turtle &getTurtle() const;
    void setTimeout(int max_seconds);
    std::vector<Node> popPath();
    static const std::vector<std::vector<Turtle> > getPaths();

  private:
    bool hasMoved() const;

    map<string, td::Co<ExpressionClosure> > id_to_expression;
    std::chrono::high_resolution_clock::time_point start_time;
    int max_seconds;

    static Turtle turtle;
    static stack<Turtle> previous_turtles;
    static vector<Node> run;
    static td::Trimesh *shapes;
    static std::vector<std::vector<Turtle> > paths;
    static std::stack<td::QMatrix4<float> > xforms;
    static GeometryMode::geometry_mode_t geometry_mode;
};

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const Environment env);

/* ------------------------------------------------------------------------- */

}

#endif
