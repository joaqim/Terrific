#include "SphereDrawable.h"

#include <Magnum/Primitives/Icosphere.h>
#include <Corrade/Utility/Assert.h>

namespace Terrific {
namespace GL {

SphereDrawable::SphereDrawable(
    Object3D& object,
    SceneGraph::DrawableGroup3D* group) : SceneGraph::Drawable3D{object, group} {
}

void SphereDrawable::generateMesh(
    std::vector<Vector3> const &vertices,
    std::vector<Vector3> const &normals,
    std::vector<UnsignedInt> const &indices,
    std::vector<Color3> const &colors,
    std::vector<Vector2> const &uvs) {
#if 0 // Testing icosphereSolid
  const Trade::MeshData3D sphere = Primitives::icosphereSolid(2);

  //_vertexBuffer.setData(MeshTools::interleave(sphere.positions(0), sphere.normals(0)));
  _vertexBuffer.setData(sphere.positions(0), Magnum::GL::BufferUsage::StaticDraw);
  _normalBuffer.setData(sphere.normals(0), Magnum::GL::BufferUsage::StaticDraw);
  Containers::Array<char> indexData;
  MeshIndexType indexType;
  UnsignedInt indexStart, indexEnd;
  std::tie(indexData, indexType, indexStart, indexEnd) =
      MeshTools::compressIndices(sphere.indices());
  _indexBuffer.setData(indexData);
  _mesh.setIndexBuffer(_indexBuffer, 0, indexType, indexStart, indexEnd);
  auto verticesCount = sphere.indices().size();
#else
  _vertexBuffer.setData(vertices, Magnum::GL::BufferUsage::StaticDraw);
  _normalBuffer.setData(normals, Magnum::GL::BufferUsage::StaticDraw);
  _indexBuffer.setData(indices, Magnum::GL::BufferUsage::StaticDraw);
  _uvBuffer.setData(uvs, Magnum::GL::BufferUsage::StaticDraw);

  _mesh.setIndexBuffer(_indexBuffer, 0, Magnum::MeshIndexType::UnsignedInt, 0, _indexBuffer.size());

  auto verticesCount = indices.size(); 
  Containers::Array<char> indexData;
    Magnum::MeshIndexType indexType;
    UnsignedInt indexStart, indexEnd;
    if(indices.size() > 0) {
    std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(indices);
    //std::cout << "Index size:  \t" << indices.size() << std::endl;
    //std::cout << "Index Data size: "<< indexData.size() << std::endl;
    _indexBuffer.setData(indexData, Magnum::GL::BufferUsage::StaticDraw);
    _mesh.setIndexBuffer(_indexBuffer, 0, indexType, indexStart, indexEnd);
  }

  _mesh.addVertexBuffer(_uvBuffer, 0, Shaders::Phong::TextureCoordinates{});
#endif

Magnum::GL::Renderer::setPointSize(5.f);

_mesh
        .setPrimitive( Magnum::GL::MeshPrimitive::Triangles )
        //.setPrimitive( Magnum::GL::MeshPrimitive::Points )
.addVertexBuffer(_normalBuffer, 0,  Shaders::Phong::Normal{})
.addVertexBuffer(_vertexBuffer, 0, Shaders::Phong::Position{});

_mesh.setCount(verticesCount);
  
}

void SphereDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D &camera) {
  using namespace Magnum::Math::Literals;
  _phongShader
      .setDiffuseColor(0xa5c9ea_rgbf)
      .bindDiffuseTexture(*_texture)
      .setLightPosition(camera.cameraMatrix().transformPoint({-17.0f, 17.0f, 17.0f}))
      .setTransformationMatrix(transformationMatrix)
      .setNormalMatrix(transformationMatrix.rotationScaling())
      .setProjectionMatrix(camera.projectionMatrix());

  _mesh.draw(_phongShader);
}

}

}
