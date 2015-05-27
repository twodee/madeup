#include <iostream>
#include <sstream>

#include "madeup/ExpressionBlobs.h"
#include "madeup/Environment.h"
#include "madeup/ExpressionAbsoluteValue.h"
#include "madeup/ExpressionAxis.h"
#include "madeup/ExpressionBox.h"
#include "madeup/ExpressionCenter.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionCosine.h"
#include "madeup/ExpressionDebug.h"
#include "madeup/ExpressionDecimal.h"
#include "madeup/ExpressionDot.h"
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
#include "madeup/ExpressionPop.h"
#include "madeup/ExpressionPrint.h"
#include "madeup/ExpressionPush.h"
#include "madeup/ExpressionRandom.h"
#include "madeup/ExpressionRevolve.h"
#include "madeup/ExpressionRoll.h"
#include "madeup/ExpressionRotate.h"
#include "madeup/ExpressionScale.h"
#include "madeup/ExpressionSign.h"
#include "madeup/ExpressionSine.h"
#include "madeup/ExpressionSurface.h"
#include "madeup/ExpressionTangent.h"
#include "madeup/ExpressionTranslate.h"
#include "madeup/ExpressionDowel.h"
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
geometry_mode_t Environment::geometry_mode = SURFACE;
stack<QMatrix4<float> > Environment::xforms;

/* ------------------------------------------------------------------------- */

Environment::Environment() :
  id_to_expression() {
}

/* ------------------------------------------------------------------------- */

Environment::Environment(const Environment& other) :
  id_to_expression() {
  for (map<string, Co<ExpressionClosure> >::const_iterator i = other.id_to_expression.begin();
       i != other.id_to_expression.end();
       ++i) {
    Add(i->first, i->second);
  }
}

/* ------------------------------------------------------------------------- */

void Environment::Prime() {
  shapes = new Trimesh(0, 0);
  Add("radius", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("radius", Co<Expression>(new ExpressionDecimal(1.0f)))), Environment())));
  Add("nsides", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("nsides", Co<Expression>(new ExpressionInteger(4)))), Environment())));
  Add("fracture", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("fracture", Co<Expression>(new ExpressionDecimal(100)))), Environment())));
  Add("pi", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("pi", Co<Expression>(new ExpressionDecimal(td::PI)))), Environment())));
  Add("e", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("e", Co<Expression>(new ExpressionDecimal(td::E)))), Environment())));
  Add("twist", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("twist", Co<Expression>(new ExpressionDecimal(45.0f)))), Environment())));
  Add("axisx", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisx", Co<Expression>(new ExpressionDecimal(0.0f)))), Environment())));
  Add("axisy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisy", Co<Expression>(new ExpressionDecimal(1.0f)))), Environment())));
  Add("axisz", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisz", Co<Expression>(new ExpressionDecimal(0.0f)))), Environment())));
  Add("energy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("energy", Co<Expression>(new ExpressionDecimal(100.0f)))), Environment())));
  Add("halflife", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("halflife", Co<Expression>(new ExpressionDecimal(1.0f)))), Environment())));

  Environment globals;
  globals.Add("nsides", (*this)["nsides"]);
  globals.Add("radius", (*this)["radius"]);
  globals.Add("fracture", (*this)["fracture"]);
  globals.Add("twist", (*this)["twist"]);
  globals.Add("axisx", (*this)["axisx"]);
  globals.Add("axisy", (*this)["axisy"]);
  globals.Add("axisz", (*this)["axisz"]);
  globals.Add("energy", (*this)["energy"]);
  globals.Add("halflife", (*this)["halflife"]);

  Co<ExpressionDefine> define_sine(new ExpressionDefine("sin", Co<Expression>(new ExpressionSine())));
  define_sine->AddFormal("degrees");
  Add("sin", Co<ExpressionClosure>(new ExpressionClosure(define_sine, globals)));

  Co<ExpressionDefine> define_cosine(new ExpressionDefine("cos", Co<Expression>(new ExpressionCosine())));
  define_cosine->AddFormal("degrees");
  Add("cos", Co<ExpressionClosure>(new ExpressionClosure(define_cosine, globals)));

  Co<ExpressionDefine> define_log(new ExpressionDefine("log", Co<Expression>(new ExpressionLog())));
  define_log->AddFormal("base");
  define_log->AddFormal("x");
  Add("log", Co<ExpressionClosure>(new ExpressionClosure(define_log, globals)));

  Co<ExpressionDefine> define_tangent(new ExpressionDefine("tan", Co<Expression>(new ExpressionTangent())));
  define_tangent->AddFormal("degrees");
  Add("tan", Co<ExpressionClosure>(new ExpressionClosure(define_tangent, globals)));

  Co<ExpressionDefine> define_inverse_sine(new ExpressionDefine("asin", Co<Expression>(new ExpressionInverseTangent())));
  define_inverse_sine->AddFormal("ratio");
  Add("asin", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_sine, globals)));

  Co<ExpressionDefine> define_inverse_cosine(new ExpressionDefine("acos", Co<Expression>(new ExpressionInverseTangent())));
  define_inverse_cosine->AddFormal("ratio");
  Add("acos", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_cosine, globals)));

  Co<ExpressionDefine> define_inverse_tangent(new ExpressionDefine("atan", Co<Expression>(new ExpressionInverseTangent())));
  define_inverse_tangent->AddFormal("ratio");
  Add("atan", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_tangent, globals)));

  Co<ExpressionDefine> define_inverse_tangent2(new ExpressionDefine("atan2", Co<Expression>(new ExpressionInverseTangent2())));
  define_inverse_tangent2->AddFormal("opposite");
  define_inverse_tangent2->AddFormal("adjacent");
  Add("atan2", Co<ExpressionClosure>(new ExpressionClosure(define_inverse_tangent2, globals)));

  Co<ExpressionDefine> define_yaw(new ExpressionDefine("yaw", Co<Expression>(new ExpressionYaw())));
  define_yaw->AddFormal("degrees");
  Add("yaw", Co<ExpressionClosure>(new ExpressionClosure(define_yaw, globals)));

  Co<ExpressionDefine> define_pitch(new ExpressionDefine("pitch", Co<Expression>(new ExpressionPitch())));
  define_pitch->AddFormal("degrees");
  Add("pitch", Co<ExpressionClosure>(new ExpressionClosure(define_pitch, globals)));

  Co<ExpressionDefine> define_roll(new ExpressionDefine("roll", Co<Expression>(new ExpressionRoll())));
  define_roll->AddFormal("degrees");
  Add("roll", Co<ExpressionClosure>(new ExpressionClosure(define_roll, globals)));

  Co<ExpressionDefine> define_push(new ExpressionDefine("push", Co<Expression>(new ExpressionPush())));
  Add("push", Co<ExpressionClosure>(new ExpressionClosure(define_push, globals)));

  Co<ExpressionDefine> define_pop(new ExpressionDefine("pop", Co<Expression>(new ExpressionPop())));
  Add("pop", Co<ExpressionClosure>(new ExpressionClosure(define_pop, globals)));

  Co<ExpressionDefine> define_print(new ExpressionDefine("print", Co<Expression>(new ExpressionPrint())));
  define_print->AddFormal("message");
  Add("print", Co<ExpressionClosure>(new ExpressionClosure(define_print, globals)));

  Co<ExpressionDefine> define_debug(new ExpressionDefine("debug", Co<Expression>(new ExpressionDebug())));
  define_debug->AddFormal("message", FormalParameter::LAZY);
  Add("debug", Co<ExpressionClosure>(new ExpressionClosure(define_debug, globals)));

  Co<ExpressionDefine> define_where(new ExpressionDefine("where", Co<Expression>(new ExpressionWhere())));
  Add("where", Co<ExpressionClosure>(new ExpressionClosure(define_where, globals)));

  Co<ExpressionDefine> define_move(new ExpressionDefine("move", Co<Expression>(new ExpressionMove())));
  define_move->AddFormal("length");
  Add("move", Co<ExpressionClosure>(new ExpressionClosure(define_move, globals)));

  Co<ExpressionDefine> define_moveto(new ExpressionDefine("moveto", Co<Expression>(new ExpressionMoveTo())));
  define_moveto->AddFormal("x");
  define_moveto->AddFormal("y");
  define_moveto->AddFormal("z");
  Add("moveto", Co<ExpressionClosure>(new ExpressionClosure(define_moveto, globals)));

  Co<ExpressionDefine> define_scale(new ExpressionDefine("scale", Co<Expression>(new ExpressionScale())));
  define_scale->AddFormal("x");
  define_scale->AddFormal("y");
  define_scale->AddFormal("z");
  Add("scale", Co<ExpressionClosure>(new ExpressionClosure(define_scale, globals)));

  Co<ExpressionDefine> define_translate(new ExpressionDefine("translate", Co<Expression>(new ExpressionTranslate())));
  define_translate->AddFormal("x");
  define_translate->AddFormal("y");
  define_translate->AddFormal("z");
  Add("translate", Co<ExpressionClosure>(new ExpressionClosure(define_translate, globals)));

  Co<ExpressionDefine> define_rotate(new ExpressionDefine("rotate", Co<Expression>(new ExpressionRotate())));
  define_rotate->AddFormal("x");
  define_rotate->AddFormal("y");
  define_rotate->AddFormal("z");
  define_rotate->AddFormal("degrees");
  Add("rotate", Co<ExpressionClosure>(new ExpressionClosure(define_rotate, globals)));

  Co<ExpressionDefine> define_identity(new ExpressionDefine("identity", Co<Expression>(new ExpressionIdentity())));
  Add("identity", Co<ExpressionClosure>(new ExpressionClosure(define_identity, globals)));

  Co<ExpressionDefine> define_center(new ExpressionDefine("center", Co<Expression>(new ExpressionCenter())));
  Add("center", Co<ExpressionClosure>(new ExpressionClosure(define_center, globals)));

  Co<ExpressionDefine> define_echo(new ExpressionDefine("echo", Co<Expression>(new ExpressionEcho())));
  Add("echo", Co<ExpressionClosure>(new ExpressionClosure(define_echo, globals)));

  Co<ExpressionDefine> define_axis(new ExpressionDefine("axis", Co<Expression>(new ExpressionAxis())));
  define_axis->AddFormal("x");
  define_axis->AddFormal("y");
  define_axis->AddFormal("z");
  Add("axis", Co<ExpressionClosure>(new ExpressionClosure(define_axis, globals)));

  Co<ExpressionDefine> define_surface(new ExpressionDefine("surface", Co<Expression>(new ExpressionSurface())));
  define_surface->AddFormal("width");
  define_surface->AddFormal("height");
  Add("surface", Co<ExpressionClosure>(new ExpressionClosure(define_surface, globals)));

  Co<ExpressionDefine> define_blobs(new ExpressionDefine("blobs", Co<Expression>(new ExpressionBlobs())));
  define_blobs->AddFormal("grain");
  define_blobs->AddFormal("iso");
  Add("blobs", Co<ExpressionClosure>(new ExpressionClosure(define_blobs, globals)));

  Co<ExpressionDefine> define_random(new ExpressionDefine("random", Co<Expression>(new ExpressionRandom())));
  define_random->AddFormal("min");
  define_random->AddFormal("max");
  Add("random", Co<ExpressionClosure>(new ExpressionClosure(define_random, globals)));

  Co<ExpressionDefine> define_max(new ExpressionDefine("max", Co<Expression>(new ExpressionMax())));
  define_max->AddFormal("a");
  define_max->AddFormal("b");
  Add("max", Co<ExpressionClosure>(new ExpressionClosure(define_max, globals)));

  Co<ExpressionDefine> define_min(new ExpressionDefine("min", Co<Expression>(new ExpressionMin())));
  define_min->AddFormal("a");
  define_min->AddFormal("b");
  Add("min", Co<ExpressionClosure>(new ExpressionClosure(define_min, globals)));

  Co<ExpressionDefine> define_sign(new ExpressionDefine("sign", Co<Expression>(new ExpressionSign())));
  define_sign->AddFormal("n");
  Add("sign", Co<ExpressionClosure>(new ExpressionClosure(define_sign, globals)));

  Co<ExpressionDefine> define_dowel(new ExpressionDefine("dowel", Co<Expression>(new ExpressionDowel())));
  Add("dowel", Co<ExpressionClosure>(new ExpressionClosure(define_dowel, globals)));
  Add("stick", Co<ExpressionClosure>(new ExpressionClosure(define_dowel, globals)));

  Co<ExpressionDefine> define_dot(new ExpressionDefine("dot", Co<Expression>(new ExpressionDot())));
  Add("dot", Co<ExpressionClosure>(new ExpressionClosure(define_dot, globals)));
  Add("ball", Co<ExpressionClosure>(new ExpressionClosure(define_dot, globals)));
  Add("dots", Co<ExpressionClosure>(new ExpressionClosure(define_dot, globals)));

  Co<ExpressionDefine> define_box(new ExpressionDefine("box", Co<Expression>(new ExpressionBox())));
  Add("box", Co<ExpressionClosure>(new ExpressionClosure(define_box, globals)));
  Add("boxes", Co<ExpressionClosure>(new ExpressionClosure(define_box, globals)));

  Co<ExpressionDefine> define_revolve(new ExpressionDefine("revolve", Co<Expression>(new ExpressionRevolve())));
  define_revolve->AddFormal("x");
  define_revolve->AddFormal("y");
  define_revolve->AddFormal("z");
  define_revolve->AddFormal("degrees");
  Add("revolve", Co<ExpressionClosure>(new ExpressionClosure(define_revolve, globals)));

  Co<ExpressionDefine> define_extrude(new ExpressionDefine("extrude", Co<Expression>(new ExpressionExtrude())));
  define_extrude->AddFormal("x");
  define_extrude->AddFormal("y");
  define_extrude->AddFormal("z");
  define_extrude->AddFormal("length");
  Add("extrude", Co<ExpressionClosure>(new ExpressionClosure(define_extrude, globals)));

  Co<ExpressionDefine> define_forget(new ExpressionDefine("forget", Co<Expression>(new ExpressionForget())));
  Add("forget", Co<ExpressionClosure>(new ExpressionClosure(define_forget, globals)));

  xforms.push(QMatrix4<float>(1.0f));
}

/* ------------------------------------------------------------------------- */

void Environment::Add(const string& id, Co<ExpressionClosure> closure) {
  // If a binding already exists for this closure, let's replace the innards of
  // the old one.  Other environments may have an alias to this closure, so we
  // don't want to insert a brand new closure instance.
  map<string, Co<ExpressionClosure> >::iterator match = id_to_expression.find(id);
  if (match != id_to_expression.end()) {
    match->second->SetDefine(closure->GetDefine());
    match->second->SetEnvironment(*closure->GetEnvironment());
    match->second->SetSource(closure->GetSource(), closure->GetStartLine(), closure->GetStartIndex(), closure->GetEndLine(), closure->GetEndIndex());
  } else {
    id_to_expression[id] = closure;
  }
}

/* ------------------------------------------------------------------------- */

Co<ExpressionClosure> Environment::operator[](const string& id) {
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

void Environment::RecordVertex() {
  Co<Expression> v = (*this)["radius"]->Evaluate(*this);
  ExpressionDecimal *radius_value = dynamic_cast<ExpressionDecimal *>(v.GetPointer());
  float radius;
  if (radius_value) {
    radius = radius_value->GetDecimal();
  } else {
    ExpressionInteger *radius_value = dynamic_cast<ExpressionInteger *>(v.GetPointer());
    radius = radius_value->GetInteger();
  }

  v = (*this)["energy"]->Evaluate(*this);
  ExpressionNumber *energy_value = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  float energy;
  if (energy_value) {
    energy = energy_value->GetDecimal();
  } else {
  }

  v = (*this)["halflife"]->Evaluate(*this);
  ExpressionNumber *halflife_value = dynamic_cast<ExpressionNumber *>(v.GetPointer());
  float halflife;
  if (halflife_value) {
    halflife = halflife_value->GetDecimal();
  } else {
  }

  Node cursor = {xforms.top() * turtle.position, radius, energy, halflife};
  run.push_back(cursor);
}

/* ------------------------------------------------------------------------- */

void Environment::RecordPreview() {
  if (paths.size() == 0) {
    paths.push_back(vector<Turtle>());
  }
  paths[paths.size() - 1].push_back(turtle);
  vector<Turtle>& last_path = paths[paths.size() - 1];
  last_path[last_path.size() - 1].position = xforms.top() * last_path[last_path.size() - 1].position;
}

/* ------------------------------------------------------------------------- */

void Environment::Move(float distance) {
  turtle.position += turtle.camera.GetTo() * distance;
  RecordVertex();
  RecordPreview();
}

/* ------------------------------------------------------------------------- */

void Environment::MoveTo(float x, float y, float z) {
  turtle.position[0] = x;
  turtle.position[1] = y;
  turtle.position[2] = z;
  RecordVertex();
  RecordPreview();
}

/* ------------------------------------------------------------------------- */

void Environment::Scale(float x, float y, float z) {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = QMatrix4<float>::GetScale(x, y, z) * xform;
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

void Environment::Translate(float x, float y, float z) {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = QMatrix4<float>::GetTranslate(x, y, z) * xform;
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

void Environment::Rotate(float x, float y, float z, float degrees) {
  QMatrix4<float> xform = xforms.top();
  xforms.pop();
  xform = QMatrix4<float>::GetRotate(degrees, QVector3<float>(x, y, z)) * xform;
  xforms.push(xform);
}

/* ------------------------------------------------------------------------- */

void Environment::Identity() {
  xforms.pop();
  xforms.push(QMatrix4<float>(1.0f));
}

/* ------------------------------------------------------------------------- */

void Environment::Center() {
  if (run.size() == 0) {
    return;
  }

  QVector3<float> min = run[0].position;
  QVector3<float> max = run[0].position;
  for (unsigned int i = 1; i < run.size(); ++i) {
    const QVector3<float>& position = run[i].position;

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

void Environment::Echo() {
  for (unsigned int i = 0; i < run.size(); ++i) {
    /* Node cursor = {xforms.top() * run[run.size() - 1 - i].position, run[run.size() - 1 - i].radius}; */
    /* run.push_back(cursor); */
  }
}

/* ------------------------------------------------------------------------- */

void Environment::Roll(float degrees) {
  turtle.camera.Roll(degrees);
 
  // Only issue a node in the preview path if we've moved at least once.
  // Otherwise, it looks like we're generating a node in the geometry when
  // we really aren't.
  if (HasMoved()) {
    RecordPreview();
  }
}

/* ------------------------------------------------------------------------- */

void Environment::Pitch(float degrees) {
  turtle.camera.Pitch(degrees);
 
  // See Roll.
  if (HasMoved()) {
    RecordPreview();
  }
}

/* ------------------------------------------------------------------------- */

void Environment::Yaw(float degrees) {
  turtle.camera.Yaw(degrees);

  // See Roll.
  if (HasMoved()) {
    RecordPreview();
  }
}

/* ------------------------------------------------------------------------- */

Trimesh *Environment::GetMesh() {
  shapes->ComputeNormals();
  shapes->DisconnectFaces();
  return shapes;
}

/* ------------------------------------------------------------------------- */

void Environment::Push() {
  previous_turtles.push(turtle);
  xforms.push(xforms.top());
}

/* ------------------------------------------------------------------------- */

void Environment::Pop() {
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

void Environment::Dowel() {
  if (geometry_mode == SURFACE) {
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

        Co<Expression> nsides_value = (*this)["nsides"]->Evaluate(*this);
        int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->GetInteger();

        Co<Expression> fracture_value = (*this)["fracture"]->Evaluate(*this);
        ExpressionDecimal *fracture_number = dynamic_cast<ExpressionDecimal *>(fracture_value.GetPointer());
        float fracture;
        if (fracture_number) {
          fracture = fracture_number->GetDecimal();
        } else {
          ExpressionInteger *fracture_number = dynamic_cast<ExpressionInteger *>(fracture_value.GetPointer());
          fracture = fracture_number->GetInteger();
        }

        Co<Expression> twist_value = (*this)["twist"]->Evaluate(*this);
        ExpressionDecimal *twist_number = dynamic_cast<ExpressionDecimal *>(twist_value.GetPointer());
        float twist;
        if (twist_number) {
          twist = twist_number->GetDecimal();
        } else {
          ExpressionInteger *twist_number = dynamic_cast<ExpressionInteger *>(twist_value.GetPointer());
          twist = twist_number->GetInteger();
        }

        /* line->Fracture(fracture); */
        /* line->AdaptiveFracture(30.0f, 1.0f); */

        // Output for Desmos.
#if 1
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

void Environment::Revolve() {
  if (geometry_mode == SURFACE) {
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

        Co<Expression> nsides_value = (*this)["nsides"]->Evaluate(*this);
        int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->GetInteger();

        Co<Expression> x_value = (*this)["x"]->Evaluate(*this);
        float x = dynamic_cast<ExpressionNumber *>(x_value.GetPointer())->GetDecimal();
        Co<Expression> y_value = (*this)["y"]->Evaluate(*this);
        float y = dynamic_cast<ExpressionNumber *>(y_value.GetPointer())->GetDecimal();
        Co<Expression> z_value = (*this)["z"]->Evaluate(*this);
        float z = dynamic_cast<ExpressionNumber *>(z_value.GetPointer())->GetDecimal();

        Co<Expression> fracture_value = (*this)["fracture"]->Evaluate(*this);
        ExpressionNumber *fracture_number = dynamic_cast<ExpressionNumber *>(fracture_value.GetPointer());
        float fracture = fracture_number->GetDecimal();
        line->Fracture(fracture);

        Co<Expression> degrees_expr = (*this)["degrees"]->Evaluate(*this);
        ExpressionNumber *degrees_number = dynamic_cast<ExpressionNumber *>(degrees_expr.GetPointer());
        float degrees = degrees_number->GetDecimal();

        Trimesh *trimesh = line->Revolve(QVector3<float>(x, y, z), nsides, degrees);

        *shapes += *trimesh;
      }
    }
  }

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::Extrude(const QVector3<float>& axis, float length) {
  if (geometry_mode == SURFACE) {
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

void Environment::Dot() {
  if (geometry_mode == SURFACE) {
    Co<Expression> nsides_value = (*this)["nsides"]->Evaluate(*this);
    int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->GetInteger();

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

void Environment::Box() {
  if (geometry_mode == SURFACE) {
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

void Environment::Blobs(float grain, float iso) {
  if (geometry_mode == SURFACE) {
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

void Environment::Surface(int width, int height) {
  if (geometry_mode == SURFACE) {
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

void Environment::Forget() {
  // Erase last preview path.
  paths.pop_back();

  paths.push_back(vector<Turtle>());
  run.clear();
}

/* ------------------------------------------------------------------------- */

void Environment::Axis(float x, float y, float z) {
  Add("axisx", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisx", new ExpressionDecimal(x))), Environment())));
  Add("axisy", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisy", new ExpressionDecimal(y))), Environment())));
  Add("axisz", Co<ExpressionClosure>(new ExpressionClosure(Co<ExpressionDefine>(new ExpressionDefine("axisz", new ExpressionDecimal(z))), Environment())));
}

/* ------------------------------------------------------------------------- */

std::ostream& operator<<(std::ostream& out, const Environment env) {
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

bool Environment::IsBound(const string& id) const {
  return id_to_expression.find(id) != id_to_expression.end();
}

/* ------------------------------------------------------------------------- */

void Environment::SetGeometryMode(geometry_mode_t mode) {
  geometry_mode = mode; 
}

/* ------------------------------------------------------------------------- */

std::string Environment::GetPathsJSON() const {
  std::stringstream out;

  out << std::fixed;
  out << "[" << std::endl;
  for (unsigned int pi = 0; pi < paths.size(); ++pi) {
    out << "  {" << std::endl;
    out << "    \"vertices\": [";
    for (unsigned int vi = 0; vi < paths[pi].size(); ++vi) {
      const QVector3<float>& position = paths[pi][vi].position;
      out << "[" << position[0] << "," << position[1] << "," << position[2] << "]" << (vi == paths[pi].size() - 1 ? "" : ",");
    }
    out << "], " << std::endl;
    out << "    \"orientation\": [";
    if (paths[pi].size()) {
      const Camera& camera = paths[pi][paths[pi].size() - 1].camera;
      const QMatrix4<float>& matrix = camera.GetViewMatrix();
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

float Environment::GetVariableAsFloat(const std::string& id) {
  Co<ExpressionClosure> closure = (*this)[id];
  Co<Expression> v = closure->Evaluate(*this);

  ExpressionDecimal *decimal = dynamic_cast<ExpressionDecimal *>(v.GetPointer());
  if (decimal) {
    return decimal->GetDecimal();
  }

  ExpressionInteger *integer = dynamic_cast<ExpressionInteger *>(v.GetPointer());
  if (integer) {
    return integer->GetInteger();
  }

  std::stringstream ss;
  ss << "Expected a number named " << id;
  throw MessagedException(ss.str());
}

/* ------------------------------------------------------------------------- */

bool Environment::HasMoved() const {
  return paths.size() > 0 && paths[paths.size() - 1].size() > 0; 
}

/* ------------------------------------------------------------------------- */

const Turtle& Environment::GetTurtle() const {
  return turtle;
}

/* ------------------------------------------------------------------------- */

}
