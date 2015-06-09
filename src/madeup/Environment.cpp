#include <iostream>
#include <sstream>

#include "madeup/Environment.h"
#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionAxis.h"
#include "madeup/ExpressionBlobs.h"
#include "madeup/ExpressionBox.h"
#include "madeup/ExpressionCenter.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionCosine.h"
#include "madeup/ExpressionDebug.h"
#include "madeup/ExpressionDot.h"
#include "madeup/ExpressionDowel.h"
#include "madeup/ExpressionEcho.h"
#include "madeup/ExpressionExtrude.h"
#include "madeup/ExpressionForget.h"
#include "madeup/ExpressionIdentity.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionInverseTangent.h"
#include "madeup/ExpressionLog.h"
#include "madeup/ExpressionMax.h"
#include "madeup/ExpressionMin.h"
#include "madeup/ExpressionMove.h"
#include "madeup/ExpressionMoveTo.h"
#include "madeup/ExpressionPitch.h"
#include "madeup/ExpressionPolygon.h"
#include "madeup/ExpressionPop.h"
#include "madeup/ExpressionPrint.h"
#include "madeup/ExpressionPush.h"
#include "madeup/ExpressionRandom.h"
#include "madeup/ExpressionReal.h"
#include "madeup/ExpressionRevolve.h"
#include "madeup/ExpressionRoll.h"
#include "madeup/ExpressionRotate.h"
#include "madeup/ExpressionScale.h"
#include "madeup/ExpressionSign.h"
#include "madeup/ExpressionSine.h"
#include "madeup/ExpressionSurface.h"
#include "madeup/ExpressionTangent.h"
#include "madeup/ExpressionTranslate.h"
#include "madeup/ExpressionWhere.h"
#include "madeup/ExpressionYaw.h"
#include "twodee/Log.h"
#include "twodee/Utilities.h"

namespace madeup {

Turtle Environment::turtle = {
  QVector3<float>(0.0f),
  Camera(QVector3<float>(0.0f, 0.0f, 0.0f),
         QVector3<float>(0.0f, 1.0f, 0.0f),
         QVector3<float>(0.0f, 0.0f, -1.0f))
};
stack<Turtle> Environment::previous_turtles;
vector<Node> Environment::run;
Trimesh *Environment::shapes = NULL; //new Trimesh(0, 0);
vector<vector<Turtle> > Environment::paths;
GeometryMode::geometry_mode_t Environment::geometry_mode = GeometryMode::SURFACE;
stack<QMatrix4<float> > Environment::xforms;

/* ------------------------------------------------------------------------- */

Environment::Environment() :
  id_to_expression() {
}

/* ------------------------------------------------------------------------- */

Environment::Environment(const Environment &other) :
  id_to_expression() {
  for (map<string, Co<ExpressionClosure> >::const_iterator i = other.id_to_expression.begin();
       i != other.id_to_expression.end();
       ++i) {
    add(i->first, i->second);
  }
}

/* ------------------------------------------------------------------------- */

void Environment::prime() {
  shapes = new Trimesh(0, 0);

  Co<Environment> globals(new Environment());;

  add("radius", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("radius", Co<Expression>(new ExpressionReal(1.0f)))), Environment())));
  add("nsides", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("nsides", Co<Expression>(new ExpressionInteger(4)))), Environment())));
  add("fracture", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("fracture", Co<Expression>(new ExpressionReal(100)))), Environment())));
  add("pi", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("pi", Co<Expression>(new ExpressionReal(td::PI)))), Environment())));
  add("e", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("e", Co<Expression>(new ExpressionReal(td::E)))), Environment())));
  add("twist", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("twist", Co<Expression>(new ExpressionReal(45.0f)))), Environment())));
  add("axisx", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisx", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add("axisy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisy", Co<Expression>(new ExpressionReal(1.0f)))), Environment())));
  add("axisz", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisz", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add("energy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("energy", Co<Expression>(new ExpressionReal(100.0f)))), Environment())));
  add("halflife", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("halflife", Co<Expression>(new ExpressionReal(1.0f)))), Environment())));

  globals->add("radius", (*this)["radius"]);
  globals->add("nsides", (*this)["nsides"]);
  globals->add("fracture", (*this)["fracture"]);
  globals->add("pi", (*this)["pi"]);
  globals->add("e", (*this)["e"]);
  globals->add("twist", (*this)["twist"]);
  globals->add("axisx", (*this)["axisx"]);
  globals->add("axisy", (*this)["axisy"]);
  globals->add("axisz", (*this)["axisz"]);
  globals->add("energy", (*this)["energy"]);
  globals->add("halflife", (*this)["halflife"]);

  Co<ExpressionDefine> define_sine(new ExpressionDefine("sin", Co<Expression>(new ExpressionSine())));
  define_sine->addFormal("degrees");

  Co<ExpressionDefine> define_cosine(new ExpressionDefine("cos", Co<Expression>(new ExpressionCosine())));
  define_cosine->addFormal("degrees");

  Co<ExpressionDefine> define_log(new ExpressionDefine("log", Co<Expression>(new ExpressionLog())));
  define_log->addFormal("base");
  define_log->addFormal("x");

  Co<ExpressionDefine> define_tangent(new ExpressionDefine("tan", Co<Expression>(new ExpressionTangent())));
  define_tangent->addFormal("degrees");

  Co<ExpressionDefine> define_inverse_sine(new ExpressionDefine("asin", Co<Expression>(new ExpressionInverseTangent())));
  define_inverse_sine->addFormal("ratio");

  Co<ExpressionDefine> define_inverse_cosine(new ExpressionDefine("acos", Co<Expression>(new ExpressionInverseTangent())));
  define_inverse_cosine->addFormal("ratio");

  Co<ExpressionDefine> define_inverse_tangent(new ExpressionDefine("atan", Co<Expression>(new ExpressionInverseTangent())));
  define_inverse_tangent->addFormal("ratio");

  Co<ExpressionDefine> define_inverse_tangent2(new ExpressionDefine("atan2", Co<Expression>(new ExpressionInverseTangent2())));
  define_inverse_tangent2->addFormal("opposite");
  define_inverse_tangent2->addFormal("adjacent");

  Co<ExpressionDefine> define_yaw(new ExpressionDefine("yaw", Co<Expression>(new ExpressionYaw())));
  define_yaw->addFormal("degrees");

  Co<ExpressionDefine> define_pitch(new ExpressionDefine("pitch", Co<Expression>(new ExpressionPitch())));
  define_pitch->addFormal("degrees");

  Co<ExpressionDefine> define_roll(new ExpressionDefine("roll", Co<Expression>(new ExpressionRoll())));
  define_roll->addFormal("degrees");

  Co<ExpressionDefine> define_push(new ExpressionDefine("push", Co<Expression>(new ExpressionPush())));

  Co<ExpressionDefine> define_pop(new ExpressionDefine("pop", Co<Expression>(new ExpressionPop())));

  Co<ExpressionDefine> define_print(new ExpressionDefine("print", Co<Expression>(new ExpressionPrint())));
  define_print->addFormal("message");

  Co<ExpressionDefine> define_debug(new ExpressionDefine("debug", Co<Expression>(new ExpressionDebug())));
  define_debug->addFormal("message");

  Co<ExpressionDefine> define_where(new ExpressionDefine("where", Co<Expression>(new ExpressionWhere())));

  Co<ExpressionDefine> define_move(new ExpressionDefine("move", Co<Expression>(new ExpressionMove())));
  define_move->addFormal("length");

  Co<ExpressionDefine> define_moveto(new ExpressionDefine("moveto", Co<Expression>(new ExpressionMoveTo())));
  define_moveto->addFormal("x");
  define_moveto->addFormal("y");
  define_moveto->addFormal("z");

  Co<ExpressionDefine> define_scale(new ExpressionDefine("scale", Co<Expression>(new ExpressionScale())));
  define_scale->addFormal("x");
  define_scale->addFormal("y");
  define_scale->addFormal("z");

  Co<ExpressionDefine> define_translate(new ExpressionDefine("translate", Co<Expression>(new ExpressionTranslate())));
  define_translate->addFormal("x");
  define_translate->addFormal("y");
  define_translate->addFormal("z");

  Co<ExpressionDefine> define_rotate(new ExpressionDefine("rotate", Co<Expression>(new ExpressionRotate())));
  define_rotate->addFormal("x");
  define_rotate->addFormal("y");
  define_rotate->addFormal("z");
  define_rotate->addFormal("degrees");

  Co<ExpressionDefine> define_identity(new ExpressionDefine("identity", Co<Expression>(new ExpressionIdentity())));

  Co<ExpressionDefine> define_center(new ExpressionDefine("center", Co<Expression>(new ExpressionCenter())));

  Co<ExpressionDefine> define_echo(new ExpressionDefine("echo", Co<Expression>(new ExpressionEcho())));

  Co<ExpressionDefine> define_axis(new ExpressionDefine("axis", Co<Expression>(new ExpressionAxis())));
  define_axis->addFormal("x");
  define_axis->addFormal("y");
  define_axis->addFormal("z");

  Co<ExpressionDefine> define_surface(new ExpressionDefine("surface", Co<Expression>(new ExpressionSurface())));
  define_surface->addFormal("width");
  define_surface->addFormal("height");

  Co<ExpressionDefine> define_blobs(new ExpressionDefine("blobs", Co<Expression>(new ExpressionBlobs())));
  define_blobs->addFormal("grain");
  define_blobs->addFormal("iso");

  Co<ExpressionDefine> define_random(new ExpressionDefine("random", Co<Expression>(new ExpressionRandom())));
  define_random->addFormal("min");
  define_random->addFormal("max");

  Co<ExpressionDefine> define_max(new ExpressionDefine("max", Co<Expression>(new ExpressionMax())));
  define_max->addFormal("a");
  define_max->addFormal("b");

  Co<ExpressionDefine> define_min(new ExpressionDefine("min", Co<Expression>(new ExpressionMin())));
  define_min->addFormal("a");
  define_min->addFormal("b");

  Co<ExpressionDefine> define_sign(new ExpressionDefine("sign", Co<Expression>(new ExpressionSign())));
  define_sign->addFormal("n");

  Co<ExpressionDefine> define_dowel(new ExpressionDefine("dowel", Co<Expression>(new ExpressionDowel())));

  Co<ExpressionDefine> define_polygon(new ExpressionDefine("polygon", Co<Expression>(new ExpressionPolygon())));

  Co<ExpressionDefine> define_dot(new ExpressionDefine("dot", Co<Expression>(new ExpressionDot())));

  Co<ExpressionDefine> define_box(new ExpressionDefine("box", Co<Expression>(new ExpressionBox())));

  Co<ExpressionDefine> define_revolve(new ExpressionDefine("revolve", Co<Expression>(new ExpressionRevolve())));
  define_revolve->addFormal("x");
  define_revolve->addFormal("y");
  define_revolve->addFormal("z");
  define_revolve->addFormal("degrees");

  Co<ExpressionDefine> define_extrude(new ExpressionDefine("extrude", Co<Expression>(new ExpressionExtrude())));
  define_extrude->addFormal("x");
  define_extrude->addFormal("y");
  define_extrude->addFormal("z");
  define_extrude->addFormal("length");

  Co<ExpressionDefine> define_forget(new ExpressionDefine("forget", Co<Expression>(new ExpressionForget())));

  add("sin", Co<ExpressionClosure>(new ExpressionClosure(define_sine, globals)));
  add("cos", Co<ExpressionClosure>(new ExpressionClosure(define_cosine, globals)));
  add("log", Co<ExpressionClosure>(new ExpressionClosure(define_log, globals)));
  add("tan", Co<ExpressionClosure>(new ExpressionClosure(define_tangent, globals)));
  add("asin", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_sine, globals)));
  add("acos", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_cosine, globals)));
  add("atan", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_tangent, globals)));
  add("atan2", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_tangent2, globals)));
  add("pitch", Co<ExpressionClosure>(new ExpressionClosure(define_pitch, globals)));
  add("yaw", Co<ExpressionClosure>(new ExpressionClosure(define_yaw, globals)));
  add("roll", Co<ExpressionClosure>(new ExpressionClosure(define_roll, globals)));
  add("debug", Co<ExpressionClosure>(new ExpressionClosure(define_debug, globals)));
  add("where", Co<ExpressionClosure>(new ExpressionClosure(define_where, globals)));
  add("move", Co<ExpressionClosure>(new ExpressionClosure(define_move, globals)));
  add("moveto", Co<ExpressionClosure>(new ExpressionClosure(define_moveto, globals)));
  add("scale", Co<ExpressionClosure>(new ExpressionClosure(define_scale, globals)));
  add("translate", Co<ExpressionClosure>(new ExpressionClosure(define_translate, globals)));
  add("rotate", Co<ExpressionClosure>(new ExpressionClosure(define_rotate, globals)));
  add("identity", Co<ExpressionClosure>(new ExpressionClosure(define_identity, globals)));
  add("center", Co<ExpressionClosure>(new ExpressionClosure(define_center, globals)));
  add("echo", Co<ExpressionClosure>(new ExpressionClosure(define_echo, globals)));
  add("axis", Co<ExpressionClosure>(new ExpressionClosure(define_axis, globals)));
  add("blobs", Co<ExpressionClosure>(new ExpressionClosure(define_blobs, globals)));
  add("surface", Co<ExpressionClosure>(new ExpressionClosure(define_surface, globals)));
  add("random", Co<ExpressionClosure>(new ExpressionClosure(define_random, globals)));
  add("sign", Co<ExpressionClosure>(new ExpressionClosure(define_sign, globals)));
  add("max", Co<ExpressionClosure>(new ExpressionClosure(define_max, globals)));
  add("min", Co<ExpressionClosure>(new ExpressionClosure(define_min, globals)));
  add("dowel", Co<ExpressionClosure>(new ExpressionClosure(define_dowel, globals)));
  add("stick", Co<ExpressionClosure>(new ExpressionClosure(define_dowel, globals)));
  add("polygon", Co<ExpressionClosure>(new ExpressionClosure(define_polygon, globals)));
  add("dot", Co<ExpressionClosure>(new ExpressionClosure(define_dot, globals)));
  add("ball", Co<ExpressionClosure>(new ExpressionClosure(define_dot, globals)));
  add("dots", Co<ExpressionClosure>(new ExpressionClosure(define_dot, globals)));
  add("box", Co<ExpressionClosure>(new ExpressionClosure(define_box, globals)));
  add("boxes", Co<ExpressionClosure>(new ExpressionClosure(define_box, globals)));
  add("revolve", Co<ExpressionClosure>(new ExpressionClosure(define_revolve, globals)));
  add("extrude", Co<ExpressionClosure>(new ExpressionClosure(define_extrude, globals)));
  add("forget", Co<ExpressionClosure>(new ExpressionClosure(define_forget, globals)));
  add("push", Co<ExpressionClosure>(new ExpressionClosure(define_push, globals)));
  add("pop", Co<ExpressionClosure>(new ExpressionClosure(define_pop, globals)));
  add("print", Co<ExpressionClosure>(new ExpressionClosure(define_print, globals)));

  globals->add("atan", (*this)["atan"]);
  globals->add("yaw", (*this)["yaw"]);
  globals->add("pitch", (*this)["pitch"]);
  globals->add("roll", (*this)["roll"]);
  globals->add("push", (*this)["push"]);
  globals->add("pop", (*this)["pop"]);
  globals->add("dowel", (*this)["dowel"]);
  globals->add("polygon", (*this)["polygon"]);
  globals->add("ball", (*this)["ball"]);
  globals->add("box", (*this)["box"]);
  globals->add("revolve", (*this)["revolve"]);
  globals->add("boxes", (*this)["boxes"]);
  globals->add("dot", (*this)["dot"]);
  globals->add("dots", (*this)["dots"]);
  globals->add("stick", (*this)["stick"]);
  globals->add("print", (*this)["print"]);
  globals->add("debug", (*this)["debug"]);
  globals->add("atan2", (*this)["atan2"]);
  globals->add("where", (*this)["where"]);
  globals->add("acos", (*this)["acos"]);
  globals->add("move", (*this)["move"]);
  globals->add("moveto", (*this)["moveto"]);
  globals->add("scale", (*this)["scale"]);
  globals->add("translate", (*this)["translate"]);
  globals->add("rotate", (*this)["rotate"]);
  globals->add("identity", (*this)["identity"]);
  globals->add("center", (*this)["center"]);
  globals->add("echo", (*this)["echo"]);
  globals->add("axis", (*this)["axis"]);
  globals->add("surface", (*this)["surface"]);
  globals->add("random", (*this)["random"]);
  globals->add("max", (*this)["max"]);
  globals->add("min", (*this)["min"]);
  globals->add("sign", (*this)["sign"]);
  globals->add("asin", (*this)["asin"]);
  globals->add("tan", (*this)["tan"]);
  globals->add("log", (*this)["log"]);
  globals->add("cos", (*this)["cos"]);
  globals->add("sin", (*this)["sin"]);
  globals->add("extrude", (*this)["extrude"]);
  globals->add("forget", (*this)["forget"]);

  xforms.push(QMatrix4<float>(1.0f));
}

/* ------------------------------------------------------------------------- */

void Environment::add(const string &id, Co<ExpressionClosure> closure) {
  // If a binding already exists for this closure, let's replace the innards of
  // the old one.  Other environments may have an alias to this closure, so we
  // don't want to insert a brand new closure instance.
  map<string, Co<ExpressionClosure> >::iterator match = id_to_expression.find(id);
  if (match != id_to_expression.end()) {
    match->second->setDefine(closure->getDefine());
    match->second->setEnvironment(*closure->getEnvironment());
    match->second->setSource(closure->getSource(), closure->getSourceLocation());
  } else {
    id_to_expression[id] = closure;
  }
}

/* ------------------------------------------------------------------------- */

void Environment::replace(const string &id, Co<ExpressionClosure> closure) {
  id_to_expression[id] = closure;
}

/* ------------------------------------------------------------------------- */

Co<ExpressionClosure> Environment::operator[](const string &id) {
  map<string, Co<ExpressionClosure> >::iterator match = id_to_expression.find(id);
  if (match != id_to_expression.end()) {
    return match->second;
  } else {
    return Co<ExpressionClosure>();
    /* std::stringstream ss; */
    /* ss << "no such thing " << id; */
    /* throw MessagedException(ss.str()); */
  }
}

/* ------------------------------------------------------------------------- */

void Environment::recordVertex() {
  Co<Expression> v = (*this)["radius"]->evaluate(*this);
  ExpressionReal *radius_value = dynamic_cast<ExpressionReal *>(v.GetPointer());
  float radius;
  if (radius_value) {
    radius = radius_value->toReal();
  } else {
    ExpressionInteger *radius_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
    radius = radius_value->toInteger();
  }

  v = (*this)["energy"]->evaluate(*this);
  ExpressionNumber *energy_value = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  float energy;
  if (energy_value) {
    energy = energy_value->toReal();
  } else {
    energy = 0.0f;
  }

  v = (*this)["halflife"]->evaluate(*this);
  ExpressionNumber *halflife_value = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  float halflife;
  if (halflife_value) {
    halflife = halflife_value->toReal();
  } else {
    halflife = 0.0f;
  }

  Node cursor = {xforms.top() * turtle.position, radius, energy, halflife};
  run.push_back(cursor);
}

/* ------------------------------------------------------------------------- */

void Environment::recordPreview() {
  if (paths.size() == 0) {
    paths.push_back(vector<Turtle>());
  }
  paths[paths.size() - 1].push_back(turtle);
  vector<Turtle> &last_path = paths[paths.size() - 1];
  last_path[last_path.size() - 1].position = xforms.top() * last_path[last_path.size() - 1].position;
}

/* ------------------------------------------------------------------------- */

void Environment::move(float distance) {
  turtle.position += turtle.camera.GetTo() * distance;
  recordVertex();
  recordPreview();
}

/* ------------------------------------------------------------------------- */

void Environment::moveTo(float x, float y, float z) {
  turtle.position[0] = x;
  turtle.position[1] = y;
  turtle.position[2] = z;
  recordVertex();
  recordPreview();
}

/* ------------------------------------------------------------------------- */

void Environment::scale(float x, float y, float z) {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = QMatrix4<float>::GetScale(x, y, z) * xform;
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

void Environment::translate(float x, float y, float z) {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = QMatrix4<float>::GetTranslate(x, y, z) * xform;
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

void Environment::rotate(float x, float y, float z, float degrees) {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = QMatrix4<float>::GetRotate(degrees, QVector3<float>(x, y, z)) * xform;
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

void Environment::identity() {
  xforms.pop();
  xforms.push(QMatrix4<float>(1.0f));
}

/* ------------------------------------------------------------------------- */

void Environment::center() {
  if (run.size() == 0) {
    return;
  }

  QVector3<float> min = run[0].position;
  QVector3<float> max = run[0].position;
  for (unsigned int i = 1; i < run.size(); ++i) {
    const QVector3<float> &position = run[i].position;

    for (int d = 0; d < 3; ++d) {
      if (position[d] < min[d]) {
        min[d] = position[d];
      } else if (position[d] > max[d]) {
        max[d] = position[d];
      }
    }
  }

  QVector3<float> centroid = (max + min) * 0.5f;

  for (unsigned int i = 0; i < run.size(); ++i) {
    run[i].position -= centroid;
  }
}

/* ------------------------------------------------------------------------- */

void Environment::echo() {
  for (unsigned int i = 0; i < run.size(); ++i) {
    /* Node cursor = {xforms.top() * run[run.size() - 1 - i].position, run[run.size() - 1 - i].radius}; */
    /* run.push_back(cursor); */
  }
}

/* ------------------------------------------------------------------------- */

void Environment::roll(float degrees) {
  turtle.camera.Roll(degrees);
 
  // Only issue a node in the preview path if we've moved at least once.
  // Otherwise, it looks like we're generating a node in the geometry when
  // we really aren't.
  if (hasMoved()) {
    recordPreview();
  }
}

/* ------------------------------------------------------------------------- */

void Environment::pitch(float degrees) {
  turtle.camera.Pitch(degrees);
 
  // See Roll.
  if (hasMoved()) {
    recordPreview();
  }
}

/* ------------------------------------------------------------------------- */

void Environment::yaw(float degrees) {
  turtle.camera.Yaw(degrees);

  // See Roll.
  if (hasMoved()) {
    recordPreview();
  }
}

/* ------------------------------------------------------------------------- */

Trimesh *Environment::getMesh() {
  shapes->ComputeNormals();
  shapes->DisconnectFaces();
  return shapes;
}

/* ------------------------------------------------------------------------- */

void Environment::push() {
  previous_turtles.push(turtle);
  xforms.push(xforms.top());
}

/* ------------------------------------------------------------------------- */

void Environment::pop() {
  if (previous_turtles.size() == 0) {
    throw MessagedException("Turtle stack is empty");
  }

  turtle = previous_turtles.top();
  previous_turtles.pop();

  if (xforms.size() == 0) {
    throw MessagedException("Transform stack is empty");
  }
  xforms.pop();
}

/* ------------------------------------------------------------------------- */

void Environment::polygon() {
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() > 0) {
      Polyline<float> *line = new Polyline<float>(run.size(), 3, Polyline<float>::CLOSED);
      for (unsigned int i = 0; i < run.size(); ++i) {
        (*line)(i)[0] = run[i].position[0];
        (*line)(i)[1] = run[i].position[1];
        (*line)(i)[2] = run[i].position[2];
      }

      Trimesh *trimesh = line->Triangulate();
      *shapes += *trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::dowel() {
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() >= 2) {
      QVector3<float> diff = run[0].position - run[run.size() - 1].position;
      float squared_length = diff.GetSquaredLength();

      // We're closed if the first and last positions are pretty close to each other.
      // In which case, let's just drop the last position all together and let the
      // Extuber method seal up the join.
      int mode;
      if (fabs(squared_length) < 1.0e-6f) {
        mode = Polyline<float>::CLOSED; 
        run.pop_back();
      } else {
        mode = Polyline<float>::OPEN; 
      }

      if (run.size() > 0) {
        Polyline<float> *line = new Polyline<float>(run.size(), 4, mode);
        for (unsigned int i = 0; i < run.size(); ++i) {
          (*line)(i)[0] = run[i].position[0];
          (*line)(i)[1] = run[i].position[1];
          (*line)(i)[2] = run[i].position[2];
          (*line)(i)[3] = run[i].radius;
        }
        /* line->Write("line.f20"); */

        Co<Expression> nsides_value = (*this)["nsides"]->evaluate(*this);
        int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

        Co<Expression> fracture_value = (*this)["fracture"]->evaluate(*this);
        ExpressionReal *fracture_number = dynamic_cast<ExpressionReal *>(fracture_value.GetPointer());
        float fracture;
        if (fracture_number) {
          fracture = fracture_number->toReal();
        } else {
          ExpressionInteger *fracture_number = dynamic_cast<ExpressionInteger *>(fracture_value.GetPointer());
          fracture = fracture_number->toInteger();
        }

        Co<Expression> twist_value = (*this)["twist"]->evaluate(*this);
        ExpressionReal *twist_number = dynamic_cast<ExpressionReal *>(twist_value.GetPointer());
        float twist;
        if (twist_number) {
          twist = twist_number->toReal();
        } else {
          ExpressionInteger *twist_number = dynamic_cast<ExpressionInteger *>(twist_value.GetPointer());
          twist = twist_number->toInteger();
        }

        /* line->Fracture(fracture); */
        /* line->AdaptiveFracture(30.0f, 1.0f); */

        // Output for Desmos.
#if 0
        std::cout << "(" << (*line)(0)[0] << ", " << (*line)(0)[1] << ")";
        for (int i = 1; i < line->GetElementCount(); ++i) {
          std::cout << ",(" << (*line)(i)[0] << ", " << (*line)(i)[1] << ")";
        }
        std::cout << "" << std::endl;
#endif

        Trimesh *trimesh = line->Dowel(nsides, -1, twist);

        delete line;
        line = NULL;

        *shapes += *trimesh;

        delete trimesh;
        trimesh = NULL;
      }
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::revolve() {
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() >= 2) {
      QVector3<float> diff = run[0].position - run[run.size() - 1].position;
      float squared_length = diff.GetSquaredLength();

      // We're closed if the first and last positions are pretty close to each other.
      // In which case, let's just drop the last position all together and let the
      // Extuber method seal up the join.
      int mode;
      if (fabs(squared_length) < 1.0e-6f) {
        mode = Polyline<float>::CLOSED; 
        run.pop_back();
      } else {
        mode = Polyline<float>::OPEN; 
      }

      if (run.size() > 0) {
        Polyline<float> *line = new Polyline<float>(run.size(), 3, mode);
        for (unsigned int i = 0; i < run.size(); ++i) {
          (*line)(i)[0] = run[i].position[0];
          (*line)(i)[1] = run[i].position[1];
          (*line)(i)[2] = run[i].position[2];
        }

        Co<Expression> nsides_value = (*this)["nsides"]->evaluate(*this);
        int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

        Co<Expression> x_value = (*this)["x"]->evaluate(*this);
        float x = dynamic_cast<ExpressionNumber *>(x_value.GetPointer())->toReal();
        Co<Expression> y_value = (*this)["y"]->evaluate(*this);
        float y = dynamic_cast<ExpressionNumber *>(y_value.GetPointer())->toReal();
        Co<Expression> z_value = (*this)["z"]->evaluate(*this);
        float z = dynamic_cast<ExpressionNumber *>(z_value.GetPointer())->toReal();

        Co<Expression> fracture_value = (*this)["fracture"]->evaluate(*this);
        ExpressionNumber *fracture_number = dynamic_cast<ExpressionNumber *>(fracture_value.GetPointer());
        float fracture = fracture_number->toReal();
        line->Fracture(fracture);

        Co<Expression> degrees_expr = (*this)["degrees"]->evaluate(*this);
        ExpressionNumber *degrees_number = dynamic_cast<ExpressionNumber *>(degrees_expr.GetPointer());
        float degrees = degrees_number->toReal();

        Trimesh *trimesh = line->Revolve(QVector3<float>(x, y, z), nsides, degrees);

        *shapes += *trimesh;
      }
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::extrude(const QVector3<float> &axis, float length) {
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() > 0) {
      Polyline<float> *line = new Polyline<float>(run.size(), 3, Polyline<float>::CLOSED);
      for (unsigned int i = 0; i < run.size(); ++i) {
        (*line)(i)[0] = run[i].position[0];
        (*line)(i)[1] = run[i].position[1];
        (*line)(i)[2] = run[i].position[2];
      }

      Trimesh *trimesh = line->Extrude(axis, length, xforms.top());
      *shapes += *trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::dot() {
  if (geometry_mode == GeometryMode::SURFACE) {
    Co<Expression> nsides_value = (*this)["nsides"]->evaluate(*this);
    int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

    for (unsigned int i = 0; i < run.size(); ++i) {
      Trimesh *trimesh = Trimesh::GetSphere(nsides, nsides / 2, run[i].radius);

      *trimesh += xforms.top() * run[i].position;
      *shapes += *trimesh;
      delete trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::box() {
  if (geometry_mode == GeometryMode::SURFACE) {
    for (unsigned int i = 0; i < run.size(); ++i) {
      Trimesh *trimesh = Trimesh::GetBox(run[i].radius);
      *trimesh += run[i].position;
      *shapes += *trimesh;
      delete trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::blobs(float grain, float iso) {
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() > 0) {

      // Find min and max.
      QVector3<float> min = run[0].position;
      QVector3<float> max = run[0].position;
      /* QVector3<float> min_stdevs(run[0].radius, run[0].radius, run[0].radius); */
      /* QVector3<float> max_stdevs(run[0].radius, run[0].radius, run[0].radius); */

      for (unsigned int i = 1; i < run.size(); ++i) {
        for (int d = 0; d < 3; ++d) {
          if (run[i].position[d] < min[d]) {
            min[d] = run[i].position[d];
            /* min_stdevs[d] = run[i].radius; */
          } else if (run[i].position[d] > max[d]) {
            max[d] = run[i].position[d];
            /* max_stdevs[d] = run[i].radius; */
          }
        }
      }

      min -= 10;
      max += 10;

      Log::Debug("min: %f %f %f", min[0], min[1], min[2]);
      Log::Debug("max: %f %f %f", max[0], max[1], max[2]);

      // Calculate dimensions.
      QVector3<int> dims;
      for (int d = 0; d < 3; ++d) {
        dims[d] = (int) ceil((max[d] - min[d]) / (double) grain);
      }
      Log::Debug("dims: %d %d %d", dims[0], dims[1], dims[2]);

      // Generate field.
      NField<float, 3> field(dims);
      field.Clear(0.0f);

      for (unsigned int i = 0; i < run.size(); ++i) {
        Log::Debug("node %d", i);

        QVector3<float> mu = run[i].position;
        double energy = run[i].energy;
        double halflife = run[i].halflife;
        /* double normer = 1.0 / (stdev * stdev * stdev * pow(2 * PI, 3.0 / 2.0)); */
        /* std::cout << "normer: " << normer << std::endl; */

        QVector3<int> start((mu - min) / grain - 50.0f);
        QVector3<int> end((mu - min) / grain + 50.0f);
        for (int d = 0; d < 3; ++d) {
          if (start[d] < 0) start[d] = 0;
          if (end[d] > dims[d] - 1) end[d] = dims[d] - 1;
        }

        std::cout << "start: " << start << std::endl;
        std::cout << "end: " << end << std::endl;
        NFieldIterator<3> c(end + 1, start);
        while (c.HasNext()) {
          c.Next();
          QVector3<float> diff = QVector3<float>(c) * grain + min - mu;
          /* std::cout << "c: " << c << std::endl; */
          /* std::cout << "diff: " << diff << std::endl; */
          double boost = energy * exp((diff.GetSquaredLength() * log(0.5)) / (halflife * halflife));
          /* std::cout << "energy: " << energy << std::endl; */
          field(c)[0] += (float) boost;
        }
      }

      field.Write("foo.f20");

      /* NFieldIterator<3> c(field.GetDimensions()); */
      /* while (c.HasNext()) { */
        /* c.Next(); */
        /* std::cout << "field(" << c << "): " << field(c)[0] << std::endl; */
      /* } */

      int ntriangles;
      float *positions;
      field.GetIsocontour(iso, ntriangles, positions);
      Trimesh *trimesh = new Trimesh(ntriangles, positions);
      *shapes += *trimesh;
      *trimesh += min;

      delete[] positions;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::surface(int width, int height) {
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() != (unsigned int) (width * height)) {
      std::stringstream ss;
      ss << "The surface command expected " << width << "x" << height << " vertices. However, " << run.size() << " vertices were visited.";
      throw UnlocatedException(ss.str());
    }

    NField<float, 2> grid(QVector2<int>(width, height), 3);
    NFieldIterator<2> c(grid.GetDimensions());
    int i = 0;
    while (c.HasNext()) {
      c.Next();
      grid(c)[0] = run[i].position[0];
      grid(c)[1] = run[i].position[1];
      grid(c)[2] = run[i].position[2];
      ++i;
    }
    
    /* grid.Write("grid.f20"); */

    Trimesh *trimesh = Trimesh::GetParametric(grid);
    *shapes += *trimesh;
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::forget() {
  // Erase last preview path.
  paths.pop_back();

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::axis(float x, float y, float z) {
  add("axisx", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisx", new ExpressionReal(x))), Environment())));
  add("axisy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisy", new ExpressionReal(y))), Environment())));
  add("axisz", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisz", new ExpressionReal(z))), Environment())));
}

/* ------------------------------------------------------------------------- */

std::ostream &operator<<(std::ostream &out, const Environment env) {
  for (map<string, Co<ExpressionClosure> >::const_iterator i = env.id_to_expression.begin();
       i != env.id_to_expression.end();
       ++i) {
    out << "i->first: " << i->first << std::endl;
    out << "i->second: " << i->second << std::endl;
    out << "" << std::endl;
  }

  return out;
}

/* ------------------------------------------------------------------------- */

bool Environment::isBound(const string &id) const {
  return id_to_expression.find(id) != id_to_expression.end();
}

/* ------------------------------------------------------------------------- */

void Environment::setGeometryMode(GeometryMode::geometry_mode_t mode) {
  geometry_mode = mode; 
}

/* ------------------------------------------------------------------------- */

std::string Environment::getPathsJSON() const {
  std::stringstream out;

  out << std::fixed;
  out << "[" << std::endl;
  for (unsigned int pi = 0; pi < paths.size(); ++pi) {
    out << "  {" << std::endl;
    out << "    \"vertices\": [";
    for (unsigned int vi = 0; vi < paths[pi].size(); ++vi) {
      const QVector3<float> &position = paths[pi][vi].position;
      out << "[" << position[0] << "," << position[1] << "," << position[2] << "]" << (vi == paths[pi].size() - 1 ? "" : ",");
    }
    out << "], " << std::endl;
    out << "    \"orientation\": [";
    if (paths[pi].size()) {
      const Camera &camera = paths[pi][paths[pi].size() - 1].camera;
      const QMatrix4<float> &matrix = camera.GetViewMatrix();
      out << matrix(0, 0) << "," << matrix(0, 1) << "," << matrix(0, 2) << "," << matrix(0, 3) << ","
          << matrix(1, 0) << "," << matrix(1, 1) << "," << matrix(1, 2) << "," << matrix(1, 3) << ","
          << matrix(2, 0) << "," << matrix(2, 1) << "," << matrix(2, 2) << "," << matrix(2, 3) << ","
          << matrix(3, 0) << "," << matrix(3, 1) << "," << matrix(3, 2) << "," << matrix(3, 3);
    }
    out << "]" << std::endl;
    out << "  }" << (pi == paths.size() - 1 ? "" : ",") << std::endl;
  }
  out << "]" << std::endl;

  return out.str();
}

/* ------------------------------------------------------------------------- */

float Environment::getVariableAsFloat(const std::string &id) {
  Co<ExpressionClosure> closure = (*this)[id];
  Co<Expression> v = closure->evaluate(*this);

  ExpressionReal *decimal = dynamic_cast<ExpressionReal *>(v.GetPointer());
  if (decimal) {
    return decimal->toReal();
  }

  ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(v.GetPointer());
  if (integer) {
    return integer->toInteger();
  }

  std::stringstream ss;
  ss << "Expected a number named " << id;
  throw MessagedException(ss.str());
}

/* ------------------------------------------------------------------------- */

bool Environment::hasMoved() const {
  return paths.size() > 0 && paths[paths.size() - 1].size() > 0; 
}

/* ------------------------------------------------------------------------- */

const Turtle &Environment::getTurtle() const {
  return turtle;
}

/* ------------------------------------------------------------------------- */

}
