#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionLoft.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionUnit.h"
#include "twodee/MessagedException.h"
#include "twodee/Plane.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector2.h"

#include "igl/fit_plane.h"

using namespace td;

namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionLoft::ExpressionLoft() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

float norm_atan2f(float y, float x) {
  float theta = atan2(y, x);
  if (theta < 0) {
    theta += 2 * acos(-1);
  }
  return theta;
}

float theta_diff(float from, float to) {
  float diff = to - from;
  if (diff < 0) {
    diff += 360;
  }
  return diff;
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
      std::vector<Node> &path = paths[paths.size() - 1];

      // If path is explicitly closed (first and last are nearly the same), remove duplicate.
      if (path.size() > 1 && (path[0].position - path[path.size() - 1].position).GetLength() < 1.0e3f) {
        path.erase(path.begin() + path.size() - 1);
      }
    } else {
      throw MessagedException(element->getSourceLocation().toAnchor() + ": I expect function loft to be given an array of paths, but that's not what you gave it.");
    }
  }

  vector<QVector3<float>> positions;
  vector<QVector3<float>> colors;
  vector<QVector3<int>> faces;
  vector<vector<QVector2<float>>> contours;

  for (unsigned int pi = 0; pi < paths.size(); ++pi) {
    for (unsigned int ni = 0; ni < paths[pi].size(); ++ni) {
      positions.push_back(paths[pi][ni].position);
      colors.push_back(paths[pi][ni].rgb);
    }

    if (paths[pi].size() == 1) {
      vector<QVector2<float>> flattened_positions;
      flattened_positions.push_back(QVector2<float>(0.0f)); 
      contours.push_back(flattened_positions);
    } else {
      // Fit a plane to pass through this contour's point cloud.
      Eigen::MatrixXd cloud(paths[pi].size(), 3);;
      for (unsigned int vi = 0; vi < paths[pi].size(); ++vi) {
        QVector3<float> &position = paths[pi][vi].position;
        cloud(vi, 0) = position[0];
        cloud(vi, 1) = position[1];
        cloud(vi, 2) = position[2];
      }

      Eigen::RowVector3d eigen_normal;
      Eigen::RowVector3d eigen_centroid;
      igl::fit_plane(cloud, eigen_normal, eigen_centroid);
      QVector3<float> normal(eigen_normal[0], eigen_normal[1], eigen_normal[2]);
      std::cout << "normal: " << normal << std::endl;
      Plane<float, 3> plane(QVector3<float>(eigen_centroid[0], eigen_centroid[1], eigen_centroid[2]), normal);
      QMatrix4<float> align = QMatrix4<float>::GetRotate(normal, QVector3<float>(0, 0, 1));
      std::cout << "align: " << align << std::endl;

      // Project each point onto the plane, align it with the Z axis, and then
      // throw away Z.
      vector<QVector2<float>> flattened_positions;
      for (unsigned int vi = 0; vi < paths[pi].size(); ++vi) {
        QVector3<float> &position = paths[pi][vi].position;
        std::cout << "position: " << position << std::endl;
        std::cout << "plane.GetPoint(): " << plane.GetPoint() << std::endl;
        QVector3<float> projected = plane.Project(position) - plane.GetPoint();
        std::cout << "projected: " << projected << std::endl;
        QVector3<float> z_aligned = align * projected;
        std::cout << "z_aligned: " << z_aligned << std::endl;
        QVector2<float> only_xy = QVector2<float>(z_aligned[0], z_aligned[1]);
        only_xy.Normalize();
        std::cout << "only_xy: " << only_xy << std::endl;
        flattened_positions.push_back(only_xy);
      }
      contours.push_back(flattened_positions);
    }

    std::cout << "" << std::endl;
  }

  int npre = 0;
  for (unsigned int pi = 0; pi < paths.size() - 1; ++pi) {
    int first_a = 0;
    int first_b = 0;
    float max_dot = -1;

    // Find closest-angle pair in paths n and n + 1. Those are our bonders.
    for (unsigned int i0 = 0; i0 < paths[pi].size(); ++i0) {
      for (unsigned int i1 = 0; i1 < paths[pi + 1].size(); ++i1) {
        float dot = contours[pi][i0].Dot(contours[pi][i1]);
        if (dot > max_dot) {
          first_a = i0;
          first_b = i1;
          max_dot = dot;
        }
      }
    }

    float theta_a = norm_atan2f(contours[pi][first_a][1], contours[pi][first_a][0]);
    float theta_b = norm_atan2f(contours[pi][first_b][1], contours[pi][first_b][0]);
    float theta_so_far = theta_a < theta_b ? theta_a : theta_b;
    std::cout << "first_a: " << first_a << std::endl;
    std::cout << "first_b: " << first_b << std::endl;

    /* float theta_a = norm_atan2f(contours[pi][first_a][1], contours[pi][first_a][0]); */
    /* float theta_b = norm_atan2f(contours[pi][first_b][1], contours[pi][first_b][0]); */
    /* std::cout << "theta_a: " << theta_a << std::endl; */
    /* std::cout << "theta_b: " << theta_b << std::endl; */

    int nprea = npre;
    int npreb = npre + paths[pi].size();
    int last_a = first_a;
    int last_b = first_b;
    unsigned int na = 0;
    unsigned int nb = 0;

    do {
      int next_a = (last_a + 1) % paths[pi].size();
      int next_b = (last_b + 1) % paths[pi + 1].size();

      if (na < paths[pi].size() && nb < paths[pi + 1].size() && paths[pi].size() > 1 && paths[pi + 1].size() > 1) {
        float theta_a = norm_atan2f(contours[pi][next_a][1], contours[pi][next_a][0]);
        float theta_b = norm_atan2f(contours[pi][next_b][1], contours[pi][next_b][0]);
        if (theta_diff(theta_so_far, theta_a) <= theta_diff(theta_so_far, theta_b)) {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
          last_a = next_a;
          ++na;
          theta_so_far = theta_a;
          std::cout << "picked a" << std::endl;
        } else {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
          last_b = next_b;
          ++nb;
          theta_so_far = theta_b;
          std::cout << "picked b" << std::endl;
        }
      } else if (last_a != next_a && na < paths[pi].size() && paths[pi].size() > 1) {
        std::cout << "forced a" << std::endl;
        faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
        last_a = next_a;
        ++na;
      } else if (paths[pi + 1].size() > 1) {
        std::cout << "forced b" << std::endl;
        faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
        last_b = next_b;
        ++nb;
      } else {
        throw new UnlocatedException("uh oh bad loft");
      }

      npre = npreb;

      /* if (na == paths[pi].size() && nb == paths[pi + 1].size()) { */
        /* break; */
      /* } */
      if (last_a == first_a && last_b == first_b) {
        break;
      }
    } while (true);
  }

#if 0
  int npre = 0;
  for (int pi = 0; pi < paths.size() - 1; ++pi) {
    int first_a = 0;
    int first_b = 0;
    float least_distance = paths[pi][0].position.GetDistanceTo(paths[pi + 1][0].position);

    int nprea = npre;
    int npreb = npre + paths[pi].size();

    // Find closest pair in paths n and n + 1. Those are our bonders.
    for (int i0 = 0; i0 < paths[pi].size(); ++i0) {
      for (int i1 = 0; i1 < paths[pi + 1].size(); ++i1) {
        float distance = paths[pi][i0].position.GetDistanceTo(paths[pi + 1][i1].position);
        std::cout << "distance from a[" << i0 << "] to b[" << i1 << "] is " << distance << std::endl;
        if (distance < least_distance) {
          first_a = i0;
          first_b = i1;
          least_distance = distance;
        }
      }
    }

    std::cout << "" << std::endl;
    std::cout << "pi: " << pi << std::endl;
    std::cout << "least_distance: " << least_distance << std::endl;
    std::cout << "first_a: " << first_a << std::endl;
    std::cout << "first_b: " << first_b << std::endl;
    std::cout << "" << std::endl;

    int last_a = first_a;
    int last_b = first_b;
    int na = 0;
    int nb = 0;

    do {
      /* std::cout << "pi: " << pi << std::endl; */
      /* std::cout << "paths[pi].size(): " << paths[pi].size() << std::endl; */
      int next_a = (last_a + 1) % paths[pi].size();
      int next_b = (last_b + 1) % paths[pi + 1].size();
      // TODO last_a == next_a && last_b == next_b
      /* std::cout << "next_a: " << next_a << std::endl; */
      /* std::cout << "next_b: " << next_b << std::endl; */

      if (na < paths[pi].size() && nb < paths[pi + 1].size()) {
        /* QVector3<float> ab1 = paths[pi + 1][last_b].position - paths[pi][last_a].position; */
        /* QVector3<float> ab2 = paths[pi + 1][next_b].position - paths[pi][last_a].position; */

        /* QVector3<float> ba1 = paths[pi][last_a].position - paths[pi + 1][last_b].position; */
        /* QVector3<float> ba2 = paths[pi][next_a].position - paths[pi + 1][last_b].position; */

        /* QVector3<float> b2a = paths[pi][last_a].position - paths[pi + 1][last_b].position; */
#if 0
        QVector3<float> b2nexta = paths[pi][next_a].position - paths[pi + 1][last_b].position;
        QVector3<float> a2nextb = paths[pi + 1][next_b].position - paths[pi][last_a].position;

        std::cout << "a or b" << std::endl;
        if (last_b != next_b && (last_a == next_a || a2nextb.GetLength() < b2nexta.GetLength())) {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
          last_b = next_b;
          ++nb;
        } else {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
          last_a = next_a;
          ++na;
        }
#elif true
        QVector3<float> last_diff_a = plane.Project(paths[pi][last_a].position) - plane.GetPoint();
        last_diff_a.Normalize();
        QVector3<float> next_diff_a = plane.Project(paths[pi][next_a].position) - plane.GetPoint();
        next_diff_a.Normalize();
        float dot_a = last_diff_a.Dot(next_diff_a);

        QVector3<float> last_diff_b = plane.Project(paths[pi][last_b].position) - plane.GetPoint();
        last_diff_b.Normalize();
        QVector3<float> next_diff_b = plane.Project(paths[pi][next_b].position) - plane.GetPoint();
        next_diff_b.Normalize();
        float dot_b = last_diff_b.Dot(next_diff_b);

        if (dot_a > dot_b) {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
          last_a = next_a;
          ++na;
          std::cout << "picked a" << std::endl;
        } else {
          faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
          last_b = next_b;
          ++nb;
          std::cout << "picked b" << std::endl;
        }

#else

        // If this is the first pick, just choose the point that yields the shortest diagonal.
        if (na == 0 && nb == 0) {
          QVector3<float> b2nexta = paths[pi][next_a].position - paths[pi + 1][last_b].position;
          QVector3<float> a2nextb = paths[pi + 1][next_b].position - paths[pi][last_a].position;

          /* std::cout << "a or b" << std::endl; */
          if (last_b != next_b && (last_a == next_a || a2nextb.GetLength() < b2nexta.GetLength())) {
            faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
            last_b = next_b;
            ++nb;
            std::cout << "picked b" << std::endl;
          } else {
            faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
            last_a = next_a;
            ++na;
            std::cout << "picked a" << std::endl;
          }
        }

        // Otherwise, let's try to find a face that bends the least.
        else {
          /* std::cout << "find normals" << std::endl; */

          // Find previous face's normal.
          const QVector3<int> &prev_face = faces[faces.size() - 1];
          QVector3<float> v0 = positions[prev_face[0]];
          QVector3<float> v1 = positions[prev_face[1]];
          QVector3<float> v2 = positions[prev_face[2]];
          QVector3<float> diff01 = v1 - v0;
          QVector3<float> diff02 = v2 - v0;
          QVector3<float> prev_normal = diff01.Cross(diff02);
          prev_normal.Normalize();
          /* std::cout << "prev_normal: " << prev_normal << std::endl; */

          // Find normal of aba face.
          v0 = paths[pi][last_a].position;
          v1 = paths[pi + 1][last_b].position;
          v2 = paths[pi][next_a].position;
          diff01 = v1 - v0;
          diff02 = v2 - v0;
          QVector3<float> aba_normal = diff01.Cross(diff02);
          aba_normal.Normalize();
          /* std::cout << "aba_normal: " << aba_normal << std::endl; */
 
          // Find normal of bab face.
          v0 = paths[pi + 1][last_b].position;
          v1 = paths[pi][last_a].position;
          v2 = paths[pi + 1][next_b].position;
          diff01 = v1 - v0;
          diff02 = v2 - v0;
          QVector3<float> bab_normal = diff01.Cross(diff02);
          bab_normal.Normalize();
          /* std::cout << "bab_normal: " << bab_normal << std::endl; */
 
          // Pick the one that when abs-dotted with prev normal is closest to 1.
          float dot_aba = aba_normal.Dot(prev_normal);
          float dot_bab = bab_normal.Dot(prev_normal);
          /* std::cout << "dot_aba: " << dot_aba << std::endl; */
          /* std::cout << "dot_bab: " << dot_bab << std::endl; */
          if (fabs(dot_bab) > fabs(dot_aba)) {
            /* std::cout << "went with b" << std::endl; */
            faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
            last_b = next_b;
            ++nb;
          } else {
            /* std::cout << "went with a" << std::endl; */
            faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
            last_a = next_a;
            ++na;
          }
          /* std::cout << "" << std::endl; */
        }
#endif
      } else if (last_a != next_a && na < paths[pi].size()) {
        faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, nprea + next_a));
        last_a = next_a;
        ++na;
      } else {
        faces.push_back(QVector3<int>(nprea + last_a, npreb + last_b, npreb + next_b));
        last_b = next_b;
        ++nb;
      }

      npre = npreb;

      if (last_a == first_a && last_b == first_b) {
        break;
      }
    } while (true);
  }
#endif

  /* for (int i = 0; i < positions.size(); ++i) { */
    /* std::cout << "positions[i]: " << positions[i] << std::endl; */
  /* } */
  /* std::cout << "" << std::endl; */

  /* for (int i = 0; i < faces.size(); ++i) { */
    /* std::cout << "faces[i]: " << faces[i] << std::endl; */
  /* } */
  /* std::cout << "" << std::endl; */

  /* faces.erase(faces.begin() + debugi->toInteger(), faces.end()); */

  Co<Trimesh> mesh = Co<Trimesh>(new Trimesh(positions, faces));
  float *vcolor = mesh->AllocateVertexColors();
  for (unsigned int i = 0; i < positions.size(); ++i) {
    vcolor[0] = colors[i][0];
    vcolor[1] = colors[i][1];
    vcolor[2] = colors[i][2];
    vcolor += 3;
  }

  return Co<Expression>(new ExpressionMesh(mesh));
  /* return ExpressionUnit::getSingleton(); */

  // TODO handle empty paths?

  /* throw MessagedException(paths_closure->getSourceLocation().toAnchor() + ": I expect function loft to be given an array of paths, but that's not what you gave it."); */
}

/* ------------------------------------------------------------------------- */

void ExpressionLoft::write(ostream &out) const {
  out << "(loft path1 path2 ...)";
}

/* ------------------------------------------------------------------------- */

}
