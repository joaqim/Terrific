#include "SphereDrawable.h"
#include <iostream>
namespace Magnum {

SphereDrawable::SphereDrawable(
    Object3D& object,
    SceneGraph::DrawableGroup3D* group,
    std::vector<Vector3> const &vertices,
    std::vector<Vector3> const &normals,
    std::vector<UnsignedInt> const &indices,
    std::vector<Color3> const &colors) : SceneGraph::Drawable3D{object, group} {

  _vertexBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
#define WITH_NORMALS
#ifdef WITH_NORMALS
  _normalsBuffer.setData(normals, GL::BufferUsage::StaticDraw);
#endif
  _indexBuffer.setData(indices, GL::BufferUsage::StaticDraw);

  _mesh.setIndexBuffer(_indexBuffer, 0, MeshIndexType::UnsignedInt, 0, _indexBuffer.size());

  auto verticesCount = indices.size();
  {
    Containers::Array<char> indexData;
    MeshIndexType indexType;
    UnsignedInt indexStart, indexEnd;
    if(indices.size() > 0) {
      std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(indices);
      std::cout << "Index size:  \t" << indices.size() << std::endl;
      std::cout << "Index Data size: "<< indexData.size() << std::endl;
      _indexBuffer.setData(indexData, GL::BufferUsage::StaticDraw);
      _mesh.setIndexBuffer(_indexBuffer, 0, indexType, indexStart, indexEnd);
    }
    GL::Renderer::setPointSize(5.f);

    _mesh
        .setPrimitive( MeshPrimitive::Triangles )
#ifdef WITH_NORMALS
        .addVertexBuffer(_normalsBuffer, 0,  Shaders::Phong::Normal{})
#endif
        .addVertexBuffer(_vertexBuffer, 0, Shaders::Phong::Position{});


    _mesh.setCount(verticesCount);
  }
}

void SphereDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D &camera) {
  using namespace Math::Literals;
  _phongShader
      .setDiffuseColor(0xa5c9ea_rgbf)
      .setLightPosition(camera.cameraMatrix().transformPoint({-17.0f, 17.0f, 17.0f}))
      .setTransformationMatrix(transformationMatrix)
      .setNormalMatrix(transformationMatrix.rotationScaling())
      .setProjectionMatrix(camera.projectionMatrix());

  _mesh.draw(_phongShader);
}

}


