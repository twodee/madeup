#include <iostream>
#include <sstream>

#include "madeup/Environment.h"
#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionAll.h"
#include "madeup/ExpressionAny.h"
#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionAxis.h"
#include "madeup/ExpressionBoolean.h"
#include "madeup/ExpressionBlobs.h"
#include "madeup/ExpressionBoxes.h"
#include "madeup/ExpressionCenter.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionCosine.h"
#include "madeup/ExpressionDebug.h"
#include "madeup/ExpressionSpheres.h"
#include "madeup/ExpressionDowel.h"
#include "madeup/ExpressionEcho.h"
#include "madeup/ExpressionExtrude.h"
#include "madeup/ExpressionForget.h"
#include "madeup/ExpressionHome.h"
#include "madeup/ExpressionIdentity.h"
#include "madeup/ExpressionInteger.h"
#include "madeup/ExpressionInverseSine.h"
#include "madeup/ExpressionInverseCosine.h"
#include "madeup/ExpressionInverseTangent.h"
#include "madeup/ExpressionLog.h"
#include "madeup/ExpressionMax.h"
#include "madeup/ExpressionMesh.h"
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
#include "madeup/ExpressionReframe.h"
#include "madeup/ExpressionReverse.h"
#include "madeup/ExpressionRevolve.h"
#include "madeup/ExpressionRoll.h"
#include "madeup/ExpressionRotate.h"
#include "madeup/ExpressionScale.h"
#include "madeup/ExpressionSign.h"
#include "madeup/ExpressionSine.h"
#include "madeup/ExpressionSurface.h"
#include "madeup/ExpressionTangent.h"
#include "madeup/ExpressionTranslate.h"
#include "madeup/ExpressionTube.h"
#include "madeup/ExpressionWhere.h"
#include "madeup/ExpressionYaw.h"
#include "twodee/Log.h"
#include "twodee/Polyline.h"
#include "twodee/QMath.h"
#include "twodee/Utilities.h"

namespace madeup {

/* ------------------------------------------------------------------------- */

Turtle Environment::turtle = {
  QVector3<float>(0.0f),
  Camera(QVector3<float>(0.0f, 0.0f, 0.0f),
         QVector3<float>(0.0f, 1.0f, 0.0f),
         QVector3<float>(0.0f, 0.0f, 1.0f))
};
stack<Turtle> Environment::previous_turtles;
vector<Node> Environment::run;
Trimesh *Environment::shapes = NULL; //new Trimesh(0, 0);
vector<vector<Turtle> > Environment::paths;
GeometryMode::geometry_mode_t Environment::geometry_mode = GeometryMode::SURFACE;
stack<QMatrix4<float> > Environment::xforms;

/* ------------------------------------------------------------------------- */

Environment::Environment() :
  id_to_expression(),
  start_time(std::chrono::high_resolution_clock::now()),
  max_seconds(-1) {
}

/* ------------------------------------------------------------------------- */

Environment::Environment(const Environment &other) :
  id_to_expression(),
  start_time(other.start_time),
  max_seconds(other.max_seconds) {
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

  add(".outerRadius", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine(".outerRadius", Co<Expression>(new ExpressionReal(1.0f)))), Environment())));
  add(".innerRadius", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine(".innerRadius", Co<Expression>(new ExpressionReal(0.5f)))), Environment())));
  add("nsides", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("nsides", Co<Expression>(new ExpressionInteger(4)))), Environment())));
  add("fracture", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("fracture", Co<Expression>(new ExpressionReal(100)))), Environment())));
  add("pi", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("pi", Co<Expression>(new ExpressionReal(td::PI)))), Environment())));
  add("e", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("e", Co<Expression>(new ExpressionReal(td::E)))), Environment())));
  add("twist", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("twist", Co<Expression>(new ExpressionReal(45.0f)))), Environment())));
  add("maxBend", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("maxBend", Co<Expression>(new ExpressionReal(361.0f)))), Environment())));
  add("axisx", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisx", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add("axisy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisy", Co<Expression>(new ExpressionReal(1.0f)))), Environment())));
  add("axisz", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisz", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add(".energy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("energy", Co<Expression>(new ExpressionReal(100.0f)))), Environment())));
  add(".halflife", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("halflife", Co<Expression>(new ExpressionReal(1.0f)))), Environment())));
  add("x", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("x", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add("y", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("y", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add("z", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("z", Co<Expression>(new ExpressionReal(0.0f)))), Environment())));
  add("flip", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("flip", Co<Expression>(new ExpressionBoolean(false)))), Environment())));

  globals->add(".outerRadius", (*this)[".outerRadius"]);
  globals->add(".innerRadius", (*this)[".innerRadius"]);
  globals->add("nsides", (*this)["nsides"]);
  globals->add("fracture", (*this)["fracture"]);
  globals->add("pi", (*this)["pi"]);
  globals->add("e", (*this)["e"]);
  globals->add("twist", (*this)["twist"]);
  globals->add("maxBend", (*this)["maxBend"]);
  globals->add("axisx", (*this)["axisx"]);
  globals->add("axisy", (*this)["axisy"]);
  globals->add("axisz", (*this)["axisz"]);
  globals->add(".energy", (*this)[".energy"]);
  globals->add(".halflife", (*this)[".halflife"]);
  globals->add("x", (*this)["x"]);
  globals->add("y", (*this)["y"]);
  globals->add("z", (*this)["z"]);
  globals->add("flip", (*this)["flip"]);

  Co<ExpressionDefine> define_sine(new ExpressionDefine("sin", Co<Expression>(new ExpressionSine())));
  define_sine->addFormal("degrees");

  Co<ExpressionDefine> define_cosine(new ExpressionDefine("cos", Co<Expression>(new ExpressionCosine())));
  define_cosine->addFormal("degrees");

  Co<ExpressionDefine> define_size(new ExpressionDefine("size", Co<Expression>(new ExpressionArraySize())));
  define_size->addFormal("array");

  Co<ExpressionDefine> define_log(new ExpressionDefine("log", Co<Expression>(new ExpressionLog())));
  define_log->addFormal("base");
  define_log->addFormal("x");

  Co<ExpressionDefine> define_tangent(new ExpressionDefine("tan", Co<Expression>(new ExpressionTangent())));
  define_tangent->addFormal("degrees");

  Co<ExpressionDefine> define_inverse_sine(new ExpressionDefine("asin", Co<Expression>(new ExpressionInverseSine())));
  define_inverse_sine->addFormal("ratio");

  Co<ExpressionDefine> define_inverse_cosine(new ExpressionDefine("acos", Co<Expression>(new ExpressionInverseCosine())));
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

  Co<ExpressionDefine> define_reframe(new ExpressionDefine("reframe", Co<Expression>(new ExpressionReframe())));

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
  Co<ExpressionDefine> define_home(new ExpressionDefine("home", Co<Expression>(new ExpressionHome())));
  Co<ExpressionDefine> define_reverse(new ExpressionDefine("reverse", Co<Expression>(new ExpressionReverse())));

  Co<ExpressionDefine> define_center(new ExpressionDefine("center", Co<Expression>(new ExpressionCenter())));

  Co<ExpressionDefine> define_echo(new ExpressionDefine("echo", Co<Expression>(new ExpressionEcho())));
  define_echo->addFormal("mesh");

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

  Co<ExpressionDefine> define_all(new ExpressionDefine("all", Co<Expression>(new ExpressionAll())));
  define_all->addFormal("array");

  Co<ExpressionDefine> define_any(new ExpressionDefine("any", Co<Expression>(new ExpressionAny())));
  define_any->addFormal("array");

  Co<ExpressionDefine> define_dowel(new ExpressionDefine("dowel", Co<Expression>(new ExpressionDowel())));
  define_dowel->addFormal("twist");
  define_dowel->addFormal("maxBend");

  Co<ExpressionDefine> define_tube(new ExpressionDefine("tube", Co<Expression>(new ExpressionTube())));
  define_tube->addFormal("twist");
  define_tube->addFormal("maxBend");

  Co<ExpressionDefine> define_polygon(new ExpressionDefine("polygon", Co<Expression>(new ExpressionPolygon())));
  define_polygon->addFormal("flip");

  Co<ExpressionDefine> define_spheres(new ExpressionDefine("spheres", Co<Expression>(new ExpressionSpheres())));

  Co<ExpressionDefine> define_boxes(new ExpressionDefine("boxes", Co<Expression>(new ExpressionBoxes())));

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
  add("size", Co<ExpressionClosure>(new ExpressionClosure(define_size, globals)));
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
  add("reframe", Co<ExpressionClosure>(new ExpressionClosure(define_reframe, globals)));
  add("move", Co<ExpressionClosure>(new ExpressionClosure(define_move, globals)));
  add("moveto", Co<ExpressionClosure>(new ExpressionClosure(define_moveto, globals)));
  add("scale", Co<ExpressionClosure>(new ExpressionClosure(define_scale, globals)));
  add("translate", Co<ExpressionClosure>(new ExpressionClosure(define_translate, globals)));
  add("rotate", Co<ExpressionClosure>(new ExpressionClosure(define_rotate, globals)));
  add("identity", Co<ExpressionClosure>(new ExpressionClosure(define_identity, globals)));
  add("home", Co<ExpressionClosure>(new ExpressionClosure(define_home, globals)));
  add("reverse", Co<ExpressionClosure>(new ExpressionClosure(define_reverse, globals)));
  add("center", Co<ExpressionClosure>(new ExpressionClosure(define_center, globals)));
  add("echo", Co<ExpressionClosure>(new ExpressionClosure(define_echo, globals)));
  add("axis", Co<ExpressionClosure>(new ExpressionClosure(define_axis, globals)));
  add("blobs", Co<ExpressionClosure>(new ExpressionClosure(define_blobs, globals)));
  add("surface", Co<ExpressionClosure>(new ExpressionClosure(define_surface, globals)));
  add("random", Co<ExpressionClosure>(new ExpressionClosure(define_random, globals)));
  add("sign", Co<ExpressionClosure>(new ExpressionClosure(define_sign, globals)));
  add("all", Co<ExpressionClosure>(new ExpressionClosure(define_all, globals)));
  add("any", Co<ExpressionClosure>(new ExpressionClosure(define_any, globals)));
  add("max", Co<ExpressionClosure>(new ExpressionClosure(define_max, globals)));
  add("min", Co<ExpressionClosure>(new ExpressionClosure(define_min, globals)));
  add("dowel", Co<ExpressionClosure>(new ExpressionClosure(define_dowel, globals)));
  add("tube", Co<ExpressionClosure>(new ExpressionClosure(define_tube, globals)));
  add("polygon", Co<ExpressionClosure>(new ExpressionClosure(define_polygon, globals)));
  add("sphere", Co<ExpressionClosure>(new ExpressionClosure(define_spheres, globals)));
  add("spheres", Co<ExpressionClosure>(new ExpressionClosure(define_spheres, globals)));
  add("box", Co<ExpressionClosure>(new ExpressionClosure(define_boxes, globals)));
  add("boxes", Co<ExpressionClosure>(new ExpressionClosure(define_boxes, globals)));
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
  globals->add("tube", (*this)["tube"]);
  globals->add("polygon", (*this)["polygon"]);
  globals->add("revolve", (*this)["revolve"]);
  globals->add("box", (*this)["box"]);
  globals->add("boxes", (*this)["boxes"]);
  globals->add("sphere", (*this)["sphere"]);
  globals->add("spheres", (*this)["spheres"]);
  globals->add("print", (*this)["print"]);
  globals->add("debug", (*this)["debug"]);
  globals->add("atan2", (*this)["atan2"]);
  globals->add("where", (*this)["where"]);
  globals->add("reframe", (*this)["reframe"]);
  globals->add("acos", (*this)["acos"]);
  globals->add("move", (*this)["move"]);
  globals->add("moveto", (*this)["moveto"]);
  globals->add("scale", (*this)["scale"]);
  globals->add("translate", (*this)["translate"]);
  globals->add("rotate", (*this)["rotate"]);
  globals->add("identity", (*this)["identity"]);
  globals->add("home", (*this)["home"]);
  globals->add("reverse", (*this)["reverse"]);
  globals->add("center", (*this)["center"]);
  globals->add("echo", (*this)["echo"]);
  globals->add("axis", (*this)["axis"]);
  globals->add("surface", (*this)["surface"]);
  globals->add("random", (*this)["random"]);
  globals->add("max", (*this)["max"]);
  globals->add("min", (*this)["min"]);
  globals->add("sign", (*this)["sign"]);
  globals->add("all", (*this)["all"]);
  globals->add("any", (*this)["any"]);
  globals->add("asin", (*this)["asin"]);
  globals->add("tan", (*this)["tan"]);
  globals->add("log", (*this)["log"]);
  globals->add("cos", (*this)["cos"]);
  globals->add("size", (*this)["size"]);
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
  Co<Expression> v = (*this)[".outerRadius"]->evaluate(*this);
  ExpressionReal *radius_value = dynamic_cast<ExpressionReal *>(v.GetPointer());
  float outer_radius;
  if (radius_value) {
    outer_radius = radius_value->toReal();
  } else {
    ExpressionInteger *radius_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
    outer_radius = radius_value->toInteger();
  }

  v = (*this)[".innerRadius"]->evaluate(*this);
  radius_value = dynamic_cast<ExpressionReal *>(v.GetPointer());
  float inner_radius;
  if (radius_value) {
    inner_radius = radius_value->toReal();
  } else {
    ExpressionInteger *radius_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
    inner_radius = radius_value->toInteger();
  }

  v = (*this)[".energy"]->evaluate(*this);
  ExpressionNumber *energy_value = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  float energy;
  if (energy_value) {
    energy = energy_value->toReal();
  } else {
    energy = 0.0f;
  }

  v = (*this)[".halflife"]->evaluate(*this);
  ExpressionNumber *halflife_value = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  float halflife;
  if (halflife_value) {
    halflife = halflife_value->toReal();
  } else {
    halflife = 0.0f;
  }

  Node cursor = {turtle.position, outer_radius, inner_radius, energy, halflife};
  run.push_back(cursor);
}

/* ------------------------------------------------------------------------- */

void Environment::recordPreview() {
  if (paths.size() == 0) {
    paths.push_back(vector<Turtle>());
  }
  paths[paths.size() - 1].push_back(turtle);
  vector<Turtle> &last_path = paths[paths.size() - 1];
  last_path[last_path.size() - 1].position = last_path[last_path.size() - 1].position;
}

/* ------------------------------------------------------------------------- */

void Environment::move(float distance) {
  QVector3<float> offset(QVector4<float>(turtle.camera.GetTo() * distance, 0.0f));

  // Do we apply the transform to the offset between our old position and 
  // our new? No, not for the time being.
  turtle.position += xforms.top() * offset;
  /* turtle.position += offset; */

  // DON'T DO THIS. Move is a relative command. We want to transform the offset.
  /* turtle.position = xforms.top() * turtle.position; */

  recordVertex();
  recordPreview();
}

/* ------------------------------------------------------------------------- */

void Environment::moveTo(float x, float y, float z) {
  turtle.position = xforms.top() * QVector3<float>(x, y, z);
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

void Environment::home() {
  if (run.size() == 0) {
    throw new MessagedException("No home to return to.");
  }

  run.push_back(*run.begin());
}

/* ------------------------------------------------------------------------- */

void Environment::reverse() {
  std::reverse(run.begin(), run.end());  
  std::reverse(paths[paths.size() - 1].begin(), paths[paths.size() - 1].end());
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

void Environment::echo(Co<Trimesh> mesh) {
  Trimesh *trimesh(new Trimesh(*mesh));
  *trimesh *= xforms.top();
  *shapes += *trimesh;
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
  turtle.camera.Pitch(-degrees);
 
  // See Roll.
  if (hasMoved()) {
    recordPreview();
  }
}

/* ------------------------------------------------------------------------- */

void Environment::yaw(float degrees) {
  turtle.camera.Yaw(-degrees);

  // See Roll.
  if (hasMoved()) {
    recordPreview();
  }
}

/* ------------------------------------------------------------------------- */

Trimesh *Environment::getMesh() {
  /* shapes->ComputeNormals(); */
  /* shapes->DisconnectFaces(); */
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

Co<Trimesh> Environment::polygon(bool is_flipped) {
  Co<Trimesh> trimesh(new Trimesh(0, 0));
  
  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() > 0) {
      QVector3<float> diff = run[0].position - run[run.size() - 1].position;
      float squared_length = diff.GetSquaredLength();
      if (fabs(squared_length) < 1.0e-6f) {
        run.pop_back();
      }

      Polyline<float> *line = new Polyline<float>(run.size(), 3, Polyline<float>::CLOSED);
      for (unsigned int i = 0; i < run.size(); ++i) {
        (*line)(i)[0] = run[i].position[0];
        (*line)(i)[1] = run[i].position[1];
        (*line)(i)[2] = run[i].position[2];
      }

      trimesh = Co<Trimesh>(line->Triangulate());
      if (is_flipped) {
        trimesh->ReverseWinding();
      }
      *trimesh *= xforms.top();
      *shapes += *trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::dowel(float twist, float max_bend) {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

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
        float radius = run[0].outer_radius;
        bool is_homoradius = true;

        Polyline<float> *line = new Polyline<float>(run.size(), 4, mode);
        for (unsigned int i = 0; i < run.size(); ++i) {
          (*line)(i)[0] = run[i].position[0];
          (*line)(i)[1] = run[i].position[1];
          (*line)(i)[2] = run[i].position[2];
          (*line)(i)[3] = run[i].outer_radius;
          is_homoradius = is_homoradius && fabs(run[i].outer_radius - radius) < 1.0e-3f;
        }

        if (!is_homoradius) {
          radius = -1;
        }

        Co<Expression> nsides_value = (*this)["nsides"]->evaluate(*this);
        int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

        trimesh = Co<Trimesh>(line->Dowel(nsides, radius, true, twist, max_bend));
        *trimesh *= xforms.top();

        delete line;
        line = NULL;

        *shapes += *trimesh;
      }
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::tube(float twist, float max_bend) {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

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
        float radius = run[0].outer_radius;
        bool is_homoradius = true;

        Polyline<float> *line = new Polyline<float>(run.size(), 4, mode);
        for (unsigned int i = 0; i < run.size(); ++i) {
          (*line)(i)[0] = run[i].position[0];
          (*line)(i)[1] = run[i].position[1];
          (*line)(i)[2] = run[i].position[2];
          (*line)(i)[3] = run[i].outer_radius;
          is_homoradius = is_homoradius && fabs(run[i].outer_radius - radius) < 1.0e-3f;
        }

        if (!is_homoradius) {
          radius = -1;
        }

        Co<Expression> nsides_value = (*this)["nsides"]->evaluate(*this);
        int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

        trimesh = Co<Trimesh>(line->Dowel(nsides, radius, false, twist, max_bend));
        *trimesh *= xforms.top();

        delete line;
        line = NULL;

        {
          float radius = run[0].inner_radius;
          bool is_homoradius = true;

          Polyline<float> *line = new Polyline<float>(run.size(), 4, mode);
          for (unsigned int i = 0; i < run.size(); ++i) {
            (*line)(i)[0] = run[i].position[0];
            (*line)(i)[1] = run[i].position[1];
            (*line)(i)[2] = run[i].position[2];
            (*line)(i)[3] = run[i].inner_radius;
            is_homoradius = is_homoradius && fabs(run[i].outer_radius - radius) < 1.0e-3f;
          }

          if (!is_homoradius) {
            radius = -1;
          }

          Trimesh *innermesh = line->Dowel(nsides, radius, false, twist, max_bend);
          *innermesh *= xforms.top();
          innermesh->ReverseWinding();

          delete line;
          line = NULL;

          if (mode == Polyline<float>::OPEN) {
            Trimesh *cap_a = new Trimesh(nsides * 2, nsides * 2);

            // Copy over vertex positions from two shells.
            float *positions = cap_a->GetPositions();
            memcpy(positions, innermesh->GetPositions(), sizeof(float) * 3 * nsides);
            memcpy(positions + 3 * nsides, trimesh->GetPositions(), sizeof(float) * 3 * nsides);

            float *position = positions;
            for (int vi = 0; vi < nsides * 2; ++vi) {
              position += 3;
            }

            // Connect up the vertices.
            int *face = cap_a->GetFaces();
            for (int fi = 0; fi < nsides; ++fi) {
              face[0] = fi;
              face[1] = nsides + (fi + 1) % nsides;
              face[2] = nsides + fi;
              face += 3;

              face[0] = fi;
              face[1] = (fi + 1) % nsides;
              face[2] = nsides + (fi + 1) % nsides;
              face += 3;
            }

            // SECOND CAP
            Trimesh *cap_b = new Trimesh(nsides * 2, nsides * 2);

            // Copy over vertex positions from two shells.
            positions = cap_b->GetPositions();
            memcpy(positions, innermesh->GetPositions() + (innermesh->GetVertexCount() - nsides) * 3, sizeof(float) * 3 * nsides);
            memcpy(positions + 3 * nsides, trimesh->GetPositions() + (trimesh->GetVertexCount() - nsides) * 3, sizeof(float) * 3 * nsides);

            position = positions;
            for (int vi = 0; vi < nsides * 2; ++vi) {
              position += 3;
            }

            // Connect up the vertices.
            face = cap_b->GetFaces();
            for (int fi = 0; fi < nsides; ++fi) {
              face[0] = fi;
              face[1] = nsides + fi;
              face[2] = nsides + (fi + 1) % nsides;
              face += 3;

              face[0] = fi;
              face[1] = nsides + (fi + 1) % nsides;
              face[2] = (fi + 1) % nsides;
              face += 3;
            }

            *trimesh += *cap_a;
            *trimesh += *cap_b;
            delete cap_a;
            delete cap_b;
          }

          *trimesh += *innermesh;
        }

        *shapes += *trimesh;
      }
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::revolve() {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

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

        /* Co<Expression> fracture_value = (*this)["fracture"]->evaluate(*this); */
        /* ExpressionNumber *fracture_number = dynamic_cast<ExpressionNumber *>(fracture_value.GetPointer()); */
        /* float fracture = fracture_number->toReal(); */
        /* line->Fracture(fracture); */

        Co<Expression> degrees_expr = (*this)["degrees"]->evaluate(*this);
        ExpressionNumber *degrees_number = dynamic_cast<ExpressionNumber *>(degrees_expr.GetPointer());
        float degrees = degrees_number->toReal();

        trimesh = Co<Trimesh>(line->Revolve(QVector3<float>(x, y, z), nsides, degrees));
        *trimesh *= xforms.top();
        *shapes += *trimesh;
      }
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::extrude(const QVector3<float> &axis, float length) {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() > 0) {
      QVector3<float> diff = run[0].position - run[run.size() - 1].position;
      float squared_length = diff.GetSquaredLength();
      if (fabs(squared_length) < 1.0e-6f) {
        run.pop_back();
      }

      Polyline<float> *line = new Polyline<float>(run.size(), 3, Polyline<float>::CLOSED);
      for (unsigned int i = 0; i < run.size(); ++i) {
        (*line)(i)[0] = run[i].position[0];
        (*line)(i)[1] = run[i].position[1];
        (*line)(i)[2] = run[i].position[2];
      }

      trimesh = Co<Trimesh>(line->Extrude(axis, length, xforms.top()));
      *shapes += *trimesh;
    } 
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::spheres() {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

  if (geometry_mode == GeometryMode::SURFACE) {
    Co<Expression> nsides_value = (*this)["nsides"]->evaluate(*this);
    int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

    for (unsigned int i = 0; i < run.size(); ++i) {
      trimesh = Co<Trimesh>(Trimesh::GetSphere(nsides, nsides / 2, run[i].outer_radius));

      *trimesh *= xforms.top();
      *trimesh += run[i].position;
      *shapes += *trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::boxes() {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

  if (geometry_mode == GeometryMode::SURFACE) {
    for (unsigned int i = 0; i < run.size(); ++i) {
      trimesh = Co<Trimesh>(Trimesh::GetBox(run[i].outer_radius));

      *trimesh *= xforms.top();
      *trimesh += run[i].position;
      *shapes += *trimesh;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::blobs(float grain, float iso) {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

  if (geometry_mode == GeometryMode::SURFACE) {
    if (run.size() > 0) {

      // Find min and max.
      QVector3<float> min = run[0].position;
      QVector3<float> max = run[0].position;

      for (unsigned int i = 1; i < run.size(); ++i) {
        for (int d = 0; d < 3; ++d) {
          if (run[i].position[d] < min[d]) {
            min[d] = run[i].position[d];
          } else if (run[i].position[d] > max[d]) {
            max[d] = run[i].position[d];
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

        QVector3<int> start((mu - min) / grain - 50.0f);
        QVector3<int> end((mu - min) / grain + 50.0f);
        for (int d = 0; d < 3; ++d) {
          if (start[d] < 0) start[d] = 0;
          if (end[d] > dims[d] - 1) end[d] = dims[d] - 1;
        }

        NFieldIterator<3> c(end + 1, start);
        while (c.HasNext()) {
          c.Next();
          QVector3<float> diff = QVector3<float>(c) * grain + min - mu;
          double boost = energy * exp((diff.GetSquaredLength() * log(0.5)) / (halflife * halflife));
          field(c)[0] += (float) boost;
        }
      }

      field.Write("foo.f20");

      /* NFieldIterator<3> c(field.GetDimensions()); */
      /* while (c.HasNext()) { */
        /* c.Next(); */
      /* } */

      int ntriangles;
      float *positions;
      field.GetIsocontour(iso, ntriangles, positions);
      trimesh = Co<Trimesh>(new Trimesh(ntriangles, positions));
      *shapes += *trimesh;
      *trimesh += min;
      *trimesh *= xforms.top();

      delete[] positions;
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
}

/* ------------------------------------------------------------------------- */

Co<Trimesh> Environment::surface(int width, int height) {
  Co<Trimesh> trimesh(new Trimesh(0, 0));

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

    trimesh = Co<Trimesh>(Trimesh::GetParametric(grid));
    *trimesh *= xforms.top();
    *shapes += *trimesh;
  }

  paths.push_back(vector<Turtle>());
  run.clear();

  return trimesh;
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
    out << i->first << " -> " << i->second << std::endl;
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
      const QMatrix4<float> matrix = camera.GetViewMatrix();
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

void Environment::checkTimeout(const SourceLocation &location) {
  if (max_seconds < 0) return;

  auto now_time = std::chrono::high_resolution_clock::now();
  if (std::chrono::duration<double>(now_time - start_time).count() > max_seconds) {
    std::stringstream ss;
    ss << location.toAnchor() << ": Uh oh. Your program was taking a long time to finish, and I was worried that it would run forever. I stopped it after " << max_seconds << " seconds.";
    throw MessagedException(ss.str());
  }
}

/* ------------------------------------------------------------------------- */

void Environment::setTimeout(int max_seconds) {
  this->max_seconds = max_seconds;
}

/* ------------------------------------------------------------------------- */

void Environment::reframe() {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = xform * QMatrix4<float>::GetTranslate(turtle.position[0], turtle.position[1], turtle.position[2]) * turtle.camera.GetViewMatrix();
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

}
