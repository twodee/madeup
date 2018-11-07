#include "madeup/ExpressionArray.h"
#include "madeup/ExpressionUtilities.h"
#include "madeup/ExpressionClosure.h"
#include "madeup/ExpressionNetwork.h"
#include "madeup/ExpressionMesh.h"
#include "madeup/ExpressionNodes.h"
#include "madeup/ExpressionUnit.h"
#include "madeup/MeshBoolean.h"
#include "twodee/MessagedException.h"
#include "twodee/Plane.h"
#include "twodee/QMatrix4.h"
#include "twodee/QVector2.h"

using namespace td;


namespace madeup {

/* ------------------------------------------------------------------------- */

ExpressionNetwork::ExpressionNetwork() :
  Expression() {
}

/* ------------------------------------------------------------------------- */

struct compareQVector3Int {
  bool operator()(const QVector3<int> &a, const QVector3<int> &b) const {
    return a[0] < b[0] || (a[0] == b[0] && (a[1] < b[1] || (a[1] == b[1] && a[2] < b[2])));
  }
};

/* ------------------------------------------------------------------------- */

typedef map<QVector3<int>, int, compareQVector3Int> index_map_t;

int cache(vector<QVector3<float>> &positions,
          index_map_t &index_map,
          int precision,
          QVector3<float> position) {
  QVector3<int> iposition(position * (float) precision + 0.5f);
  index_map_t::iterator match = index_map.find(iposition);
  if (match == index_map.end()) {
    int index = index_map.size();
    index_map[iposition] = index;
    positions.push_back(position);
    return index;
  } else {
    return match->second;
  }
}

/* ------------------------------------------------------------------------- */

Co<Expression> ExpressionNetwork::evaluate(Environment &env) const {
  Co<ExpressionInteger> precision_value = ExpressionUtilities::lookup<ExpressionInteger>("precision", "integer", "network", env);
  Co<ExpressionNumber> radius_value = ExpressionUtilities::lookup<ExpressionNumber>("radius", "number", "network", env);
  Co<ExpressionArrayReference> paths_ref = ExpressionUtilities::lookup<ExpressionArrayReference>("paths", "array", "network", env);
  Co<ExpressionArray> paths_array = paths_ref->getArray();

  Co<Expression> nsides_value = env["nsides"]->evaluate(env);
  int nsides = dynamic_cast<ExpressionInteger *>(nsides_value.GetPointer())->toInteger();

  int precision = precision_value->toInteger();
  float radius = radius_value->toReal();

  vector<vector<Node>> paths;
  for (int i = 0; i < paths_array->getSize(); ++i) {
    Co<Expression> element = (*paths_array)[i];
    ExpressionNodes *nodes = dynamic_cast<ExpressionNodes *>(element.GetPointer());
    if (nodes) {
      paths.push_back(nodes->getPath());
    } else {
      throw MessagedException(element->getSourceLocation().toAnchor() + ": I expect function network to be given an array of paths, but that's not what you gave it.");
    }
  }

  vector<QVector3<float>> positions;
  vector<QVector2<int>> edges;
  map<QVector3<int>, int, compareQVector3Int> index_map;

  for (vector<Node> path : paths) {
    int index_pre = cache(positions, index_map, precision, path[0].position);
    for (int i = 1; i < path.size(); ++i) {
      int index_post = cache(positions, index_map, precision, path[i].position);
      edges.push_back(QVector2<int>(index_pre, index_post));
      index_pre = index_post;
    }
  }

  Co<Trimesh> mesh(MeshBoolean::network(positions, edges, radius, nsides));
  float *vcolor = mesh->AllocateVertexColors();
  if (paths.size() > 0 && paths[0].size()) {
    QVector3<float> rgb = paths[0][0].rgb;
    for (unsigned int i = 0; i < mesh->GetVertexCount(); ++i) {
      vcolor[0] = rgb[0];
      vcolor[1] = rgb[1];
      vcolor[2] = rgb[2];
      vcolor += 3;
    }
  }

  return Co<Expression>(new ExpressionMesh(mesh));
}

/* ------------------------------------------------------------------------- */

void ExpressionNetwork::write(ostream &out) const {
  out << "(network path1 path2 ...)";
}

/* ------------------------------------------------------------------------- */

}
