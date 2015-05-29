#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <map>
#include <string>
#include <vector>
#include <stack>

#include "twodee/Co.h"
#include "twodee/Camera.h"
#include "twodee/Polyline.h"
#include "twodee/QMath.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector3.h"
#include "twodee/QVector4.h"
#include "twodee/Trimesh.h"

#include "madeup/GeometryMode.h"

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
  friend std::ostream& operator<<(std::ostream& out, const Environment env);
  public:
    Environment();
    Environment(const Environment& other);

    void Add(const string& id, Co<ExpressionClosure> expression);
    bool IsBound(const string& id) const;

    Co<ExpressionClosure> operator[](const string& id);

    void Prime();
    void RecordVertex();
    void RecordPreview();
    void Move(float distance);
    void MoveTo(float x, float y, float z);
    void Scale(float x, float y, float z);
    void Translate(float x, float y, float z);
    void Rotate(float x, float y, float z, float degrees);
    void Center();
    void Identity();
    void Echo();
    void Yaw(float degrees);
    void Pitch(float degrees);
    void Roll(float degrees);
    void Dowel();
    void Revolve();
    void Extrude(const QVector3<float>& axis, float length);
    void Dot();
    void Box();
    void Blobs(float grain, float iso);
    void Surface(int width, int height);
    void Forget();
    void Axis(float x, float y, float z);

    void Push();
    void Pop();

    Trimesh *GetMesh();
    std::string GetPathsJSON() const;
    void SetGeometryMode(GeometryMode::geometry_mode_t mode);

    float GetVariableAsFloat(const std::string& id);
    const Turtle& GetTurtle() const;

  private:
    bool HasMoved() const;

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

std::ostream& operator<<(std::ostream& out, const Environment env);

/* ------------------------------------------------------------------------- */

}

#endif
