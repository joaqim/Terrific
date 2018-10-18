#pragma once
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
#include <Terrific/Math/Types.h>

namespace Terrific {
  namespace GL {

    using namespace Magnum;
    typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
    typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;


  class SphereDrawable: public SceneGraph::Drawable3D {
 public:
    explicit SphereDrawable(
        Object3D& object,
        SceneGraph::DrawableGroup3D* group);

    void generateMesh(
        std::vector<Vector3> const &vertices,
        std::vector<Vector3> const &normals,
        std::vector<UnsignedInt> const &indices,
        std::vector<Color3> const &colors,
        std::vector<Vector2> const &uvs);

    void setTexture(Magnum::GL::Texture2D &texture) {
      _texture = &texture;
      _phongShader.bindDiffuseTexture(texture);
    }
 private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D &camera) override;
    Magnum::GL::Mesh _mesh;
    Shaders::Phong _phongShader{Shaders::Phong::Flag::DiffuseTexture};
    Magnum::GL::Texture2D *_texture;

    Magnum::GL::Buffer _vertexBuffer;
    Magnum::GL::Buffer _indexBuffer;
    Magnum::GL::Buffer _normalBuffer;
    Magnum::GL::Buffer _colorBuffer;
    Magnum::GL::Buffer _uvBuffer;
  };
}
}
