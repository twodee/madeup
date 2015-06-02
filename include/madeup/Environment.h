#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <string>
#include <vector>
#include <stack>

#include "madeup/GeometryMode.h"
#include "twodee/Camera.h"
#include "twodee/Co.h"
#include "twodee/Polyline.h"
#include "twodee/QMath.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector3.h"
#include "twodee/QVector4.h"
#include "twodee/Trimesh.h"

using std::map;
using std::pair;
using std::stack;
using std::string;
using std::vector;
using namespace td;

struct Node {
  QVector3<float> position;
  float radius;
  float energy;
  float halflife;
};

struct Turtle {
  QVector3<float> position;
  Camera camera;
};

namespace madeup {

class ExpressionClosure;

/* ------------------------------------------------------------------------- */

class Environment {
  friend std::ostream &operator<<(std::ostream &out, const Environment env);
  public:
    Environment();
    Environment(const Environment &other);

    void add(const string &id, Co<ExpressionClosure> expression);
    bool isBound(const string &id) const;

    Co<ExpressionClosure> operator[](const string &id);

    void prime();
    void recordVertex();
    void recordPreview();
    void move(float distance);
    void moveTo(float x, float y, float z);
    void scale(float x, float y, float z);
    void translate(float x, float y, float z);
    void rotate(float x, float y, float z, float degrees);
    void center();
    void identity();
    void echo();
    void yaw(float degrees);
    void pitch(float degrees);
    void roll(float degrees);
    void dowel();
    void revolve();
    void extrude(const QVector3<float> &axis, float length);
    void dot();
    void box();
    void blobs(float grain, float iso);
    void surface(int width, int height);
    void forget();
    void axis(float x, float y, float z);

    void push();
    void pop();

    Trimesh *getMesh();
    std::string getPathsJSON() const;
    void setGeometryMode(GeometryMode::geometry_mode_t mode);

    float getVariableAsFloat(const std::string &id);
    const Turtle &getTurtle() const;

  private:
    bool hasMoved() const;

    map<string, Co<ExpressionClosure> > id_to_expression;

    static Turtle turtle;
    static stack<Turtle> previous_turtles;
    static vector<Node> run;
    static Trimesh *shapes;
    static std::vector<std::vector<Turtle> > paths;
    static std::stack<QMatrix4<float> > xforms;
    static GeometryMode::geometry_mode_t geometry_mode;
};

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const Environment env);

/* ------------------------------------------------------------------------- */

}

#endif
