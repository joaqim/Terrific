#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/DefaultFramebuffer.h>

#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Trade/MeshData3D.h>

#include <Magnum/Shaders/Phong.h>

#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/Camera.h>

#include <Magnum/GL/Renderer.h>

#include <Terrific/Geometry/SphericalVoronoi.h>

namespace Magnum {

  typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
  typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;


  class SphereDrawable: public SceneGraph::Drawable3D {
 public:
    explicit SphereDrawable(
        Object3D& object,
        SceneGraph::DrawableGroup3D* group,
        std::vector<Vector3> const &vertices,
        std::vector<Vector3> const &normals,
        std::vector<UnsignedInt> const &indices,
        std::vector<Color3> const &colors);
 private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D &camera) override;
    GL::Mesh _mesh;
    Shaders::Phong _phongShader;

    GL::Buffer _vertexBuffer;
    GL::Buffer _indexBuffer;
    GL::Buffer _normalBuffer;
    GL::Buffer _colorBuffer;
  };
}
