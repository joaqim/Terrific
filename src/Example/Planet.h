#pragma once
#include <Terrific/Geometry/SphericalVoronoi.h>
#include <Terrific/GL/SphereDrawable.h>
#include <Terrific/Math/Types.h>

#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

namespace Terrific {
  using namespace Magnum::SceneGraph;
    typedef Object<MatrixTransformation3D> Object3D;
    typedef Scene<MatrixTransformation3D> Scene3D;

    class Planet : public Geometry::SphericalVoronoi, public GL::SphereDrawable {
   public:
    explicit Planet(
        Object3D& object,
        DrawableGroup3D* group,
        std::vector<Vector3d> const &points,
        bool debugMode=false);
    };

}
