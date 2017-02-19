#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionLoft.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionLoft::ExpressionLoft() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionLoft::evaluate(Environment &env) const {
  Co<ExpressionClosure> paths_closure = env["paths"];
  if (paths_closure.IsNull()) {
    throw UnlocatedException("I expect function loft to be given a value named paths, but no paths was given.");
  }

  // First get an array reference out of that parameter.
  Co<Expression> value = paths_closure->evaluate(env);
  ExpressionArrayReference *paths_value = dynamic_cast<ExpressionArrayReference *>(value.GetPointer());
  if (!paths_value) {
    throw MessagedException(paths_closure->getSourceLocation().toAnchor() + ": I expect function loft to be given an array of paths, but that's not what you gave it.");
  }

  // Then force it into an array of paths.
  Co<ExpressionArray> paths_array = paths_value->getArray();
  vector<vector<Node>> paths;
  for (int i = 0; i < paths_array->getSize(); ++i) {
    Co<Expression> element = (*paths_array)[i];
    ExpressionNodes *nodes = dynamic_cast<ExpressionNodes *>(element.GetPointer());
    if (nodes) {
      paths.push_back(nodes->getPath());
    } else {
      throw MessagedException(element->getSourceLocation().toAnchor() + ": I expect function loft to be given an array of paths, but that's not what you gave it.");
    }
  }

  /* Co<ExpressionClosure> debug = env["xxx"]; */
  /* std::cout << "debug: " << *debug << std::endl; */
  /* Co<Expression> debug_value = debug->evaluate(env); */
  /* std::cout << "*debug_value: " << *debug_value << std::endl; */
  /* ExpressionInteger *debugi = dynamic_cast<ExpressionInteger *>(debug_value.GetPointer()); */
  /* std::cout << "debugi->toInteger(): " << debugi->toInteger() << std::endl; */

  for (int i = 0; i < paths.size(); ++i) {
    for (int j = 0; j < paths[i].size(); ++j) {
      std::cout << "paths[" << i << "][" << j << "]: " << paths[i][j].position << std::endl;
    }
    std::cout << "" << std::endl;
  }

  // Find closest pair in paths 0 and 1. Those are our bonders.
  
  vector<QVector3<float>> positions;
  vector<QVector3<float>> colors;
  vector<QVector3<int>> faces;

  for (int pi = 0; pi < paths.size(); ++pi) {
    for (int ni = 0; ni < paths[pi].size(); ++ni) {
      positions.push_back(paths[pi][ni].position);
      colors.push_back(paths[pi][ni].rgb);
    }
  }

  int npre = 0;
  for (int pi = 0; pi < paths.size() - 1; ++pi) {
    int last_a = 0;
    int last_b = 0;
    float least_distance = paths[pi][0].position.GetDistanceTo(paths[pi + 1][0].position);

    int nprea = npre;
    int npreb = npre + paths[pi].size();

    for (int i0 = 0; i0 < paths[pi].size(); ++i0) {
      for (int i1 = 0; i1 < paths[pi + 1].size(); ++i1) {
        float distance = paths[pi][i0].position.GetDistanceTo(paths[pi + 1][i1].position);
        if (distance < least_distance) {
          last_a = i0;
          last_b = i1;
        }
      }
    }

    std::cout << "" << std::endl;
    std::cout << "pi: " << pi << std::endl;
    std::cout << "least_distance: " << least_distance << std::endl;
    std::cout << "last_a: " << last_a << std::endl;
    std::cout << "last_b: " << last_b << std::endl;
    std::cout << "" << std::endl;

    int na = 0;
    int nb = 0;

    while (na < paths[pi].size() || nb < paths[pi + 1].size()) {
      int next_a = (last_a + 1) % paths[pi].size();
      int next_b = (last_b + 1) % paths[pi + 1].size();

      if (na < paths[pi].size() && nb < paths[pi + 1].size()) {
        QVector3<float> ab1 = paths[pi + 1][last_b].position - paths[pi][last_a].position;
        QVector3<float> ab2 = paths[pi + 1][next_b].position - paths[pi][last_a].position;

        QVector3<float> ba1 = paths[pi][last_a].position - paths[pi + 1][last_b].position;
        QVector3<float> ba2 = paths[pi][next_a].position - paths[pi + 1][last_b].position;

        QVector3<float> b2a = paths[pi][last_a].position - paths[pi + 1][last_b].position;
        QVector3<float> b2nexta = paths[pi][next_a].position - paths[pi + 1][last_b].position;
        QVector3<float> a2nextb = paths[pi + 1][next_b].position - paths[pi][last_a].position;

        /* float ab_area = ab1.Cross(ab2).GetLength(); */
        /* float ba_area = ba1.Cross(ba2).GetLength(); */
        /* float ab_area = b2a.Dot(b2nexta); //ab1.Cross(ab2).GetLength(); */
        /* float ba_area = b2a.Dot(b2nextb); //ba1.Cross(ba2).GetLength(); */
        if (a2nextb.GetLength() < b2nexta.GetLength()) {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
          last_b = next_b;
          ++nb;
          std::cout << "b?" << std::endl;
        } else {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
          last_a = next_a;
          ++na;
          std::cout << "a?" << std::endl;
        }
      } else if (na < paths[pi].size()) {
        faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
        last_a = next_a;
        ++na;
        std::cout << "a!" << std::endl;
      } else if (nb < paths[pi + 1].size()) {
        faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
        last_b = next_b;
        ++nb;
        std::cout << "b!" << std::endl;
      }

      npre = npreb;
    }
  }

  for (int i = 0; i < positions.size(); ++i) {
    std::cout << "positions[i]: " << positions[i] << std::endl;
  }
  std::cout << "" << std::endl;

  for (int i = 0; i < faces.size(); ++i) {
    std::cout << "faces[i]: " << faces[i] << std::endl;
  }
  std::cout << "" << std::endl;

  /* faces.erase(faces.begin() + debugi->toInteger(), faces.end()); */

  Co<Trimesh> mesh = Co<Trimesh>(new Trimesh(positions, faces));
  float *vcolor = mesh->AllocateVertexColors();
  for (int i = 0; i < positions.size(); ++i) {
    vcolor[0] = colors[i][0];
    vcolor[1] = colors[i][1];
    vcolor[2] = colors[i][2];
    vcolor += 3;
  }

  return Co<Expression>(new ExpressionMesh(mesh));

  // TODO handle empty paths?

  /* throw MessagedException(paths_closure->getSourceLocation().toAnchor() + ": I expect function loft to be given an array of paths, but that's not what you gave it."); */
}

/* ------------------------------------------------------------------------- */

void ExpressionLoft::write(ostream &out) const {
  out << "(loft path1 path2 ...)";
}

/* ------------------------------------------------------------------------- */

}
