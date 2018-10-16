#include "Application.h"

#include <iostream>
#include <random>
#include <cstdlib>

#include <Magnum/MeshTools/RemoveDuplicates.h>
#include <Magnum/MeshTools/Duplicate.h>
#include <Magnum/MeshTools/CombineIndexedArrays.h>

#include <Magnum/Primitives/Cube.h>

namespace Magnum {

std::vector<Vector3> generatePoints(size_t count) {
  std::vector<Vector3> points;

  std::random_device rd;
#ifdef SEEDED
  std::mt19937 rng(145);
#else
  std::mt19937 rng(rd());
  rng.discard(700000);
#endif

  //NOTE: Perform 70000 generations, much slower but improves randomness.
  // https://codereview.stackexchange.com/a/109518



  std::uniform_real_distribution<> heightDistribution(-1, 1);
  std::uniform_real_distribution<> angleDistribution(0, 2 * M_PI);

  for (size_t i = 0; i < count; i++) {
    double z = heightDistribution(rng);
    double phi = angleDistribution(rng);
    double theta = asin(z);
    double x = cos(theta) * cos(phi);
    double y = cos(theta) * sin(phi);

    points.push_back(Vector3(x, y, z));
  }

  return points;
}

void Application::CreateColors(std::size_t const count) {
  srand (time(NULL));
  for (size_t i = 0; i < count; i++) {
    _colors.emplace_back(rand() % 200 + 50, rand() % 200 + 55, rand() % 200 + 50);}
  //std::cout << rand() % 200 + 50 << ", " << rand() % 200 + 55 << ", " << rand() % 200 + 50 << "\n";
  //std::cout << rand() % 200 + 50 << ", " << rand() % 200 + 55 << ", " << rand() % 200 + 50 << "\n";
}
Vector3 CalculateSurfaceNormal (Vector3 const &p1, Vector3 const &p2, Vector3 const &p3) {
  Vector3 const u{p2 - p1};
  Vector3 const v{p3 - p1};

  Vector3 n;
  n.x() = u.y() * v.z() - u.z() * v.y();
  n.y() = u.z() * v.x() - u.x() * v.z();
  n.z() = u.x() * v.y() - u.y() * v.x();
  return n;
}
using Terrific::Geometry::HalfEdge_ptr;
using Terrific::Geometry::Cell_ptr;
using Terrific::Geometry::Vertex_ptr;
using Terrific::Geometry::Point;

struct compare_edges {// : public std::binary_function<sv::half_edge_ptr, sv::half_edge_ptr, Vector3> {
  Vector3 operator() (Vector3 &result, HalfEdge_ptr const &edge) {
    //return result + edge->end->point.position + edge->twin->end->point.position;
    return result + edge->end->point.position;
  }
};


void smoothLaplacian(std::vector<Cell_ptr> &cells, std::vector<Vertex_ptr> &vertices, std::size_t const count=1) {
  for(std::size_t i{0};i<count;i++) {
    for(auto &c : cells) {
      c->point.position = std::accumulate(c->halfEdges.begin(),c->halfEdges.end(), Vector3{0}, compare_edges())/c->halfEdges.size();
    }
    for(auto &v : vertices) {
      Vector3 const p1 = v->point.position;
      Vector3 const sum_of_adj_verts = ((std::accumulate(v->halfEdges.begin(),v->halfEdges.end() , Vector3{0}, compare_edges())));
      v->point.position = sum_of_adj_verts/float(v->halfEdges.size());
      /*
        if(p1 == v->point.position) {
        break;
        }
      */
    }
  }
}
inline void addVerticesAndNormals(Vector3 const &p1, Vector3 const &p2, Vector3 const &p3, std::vector<Vector3> &vertices, std::vector<Vector3> &normals ) {
  vertices.emplace_back(p1);
  vertices.emplace_back(p2);
  vertices.emplace_back(p3);

  std::fill_n(std::back_inserter(normals), 3, CalculateSurfaceNormal(p1, p2, p3));
}

inline void addVerticesAndNormals(Vector3 const &p1, Vector3 const &p2, Vector3 const &p3, std::vector<Vector3> &vertices, std::vector<Vector3> &normals, std::vector<Vector2> &uvs) {
  vertices.emplace_back(p1);
  vertices.emplace_back(p2);
  vertices.emplace_back(p3);

  std::fill_n(std::back_inserter(normals), 3, CalculateSurfaceNormal(p1, p2, p3));

  //uvs.emplace_back(getUVCoords(p1));
  //uvs.emplace_back(getUVCoords(p2));
  //uvs.emplace_back(getUVCoords(p3));
}


void fillVectors(
    std::vector<HalfEdge_ptr> const &edges,
    std::vector<Vector3> &meshVertices,
    std::vector<Vector3> &meshNormals,
    std::vector<UnsignedInt> &meshIndices,
    std::vector<Color3> &meshColors,
    std::vector<Vector2> &meshUVs,
    std::vector<Color3> &colors)
{
  auto count = UnsignedInt{0};
  for(auto const &e : edges) {

    addVerticesAndNormals(e->pCell->point.position, e->start->point.position, e->end->point.position, meshVertices, meshNormals, meshUVs);

    auto te = e->twin;
#define USE_INDICES
#ifdef USE_INDICES
    meshIndices.push_back(count++);
    meshIndices.push_back(count++);
    meshIndices.push_back(count++);

    std::fill_n(std::back_inserter(meshColors),4, colors[e->end->index%255]);
#else
    std::fill_n(std::back_inserter(meshColors),3, colors[e->end->index%255]);
    std::fill_n(std::back_inserter(meshColors),3, colors[te->end->index%255]);
#endif

#ifdef USE_INDICES
    meshVertices.push_back(te->pCell->point.position);
    meshNormals.push_back(CalculateSurfaceNormal(te->pCell->point.position, te->start->point.position, te->end->point.position));

    //meshVertices.push_back(te->start->point.position);
    //meshVertices.push_back(te->end->point.position);
    meshIndices.push_back(count++);
    meshIndices.push_back(count - 2);
    meshIndices.push_back(count - 3);
#else
    addVerticesAndNormals(te->pCell->point.position, te->end->point.position, te->start->point.position, meshVertices, meshNormals);
#endif
  }
  //break;
#define REMOVE_DUPS
#ifdef REMOVE_DUPS
  {
    //#define TEST_CUBE
#ifdef TEST_CUBE
    {
      const Trade::MeshData3D cube = Primitives::cubeSolid();
      meshIndices = cube.indices();
      meshNormals = cube.normals(0);
      meshVertices = cube.positions(0);
      meshColors.clear();
      UnsignedInt i{0};
      for(auto const &v : meshVertices) {
        meshColors.push_back(colors[i++]);
      }
      CORRADE_ASSERT(meshIndices.size() == 36,"TEST_CUBE failed",);
      CORRADE_ASSERT(meshNormals.size() == 24,"TEST_CUBE failed",);
      CORRADE_ASSERT(meshVertices.size() == 24,"TEST_CUBE failed",);
      CORRADE_ASSERT(meshColors.size() == 24,"TEST_CUBE failed",);
    }
#endif
#if 1
    std::cout <<"\t Indices: "<< meshIndices.size() << std::endl;
    std::cout <<"\t Normals: "<< meshNormals.size() << std::endl;
    std::cout <<"\t Vertices: "<< meshVertices.size() << std::endl;
    std::cout <<"\t Colors: "<< meshColors.size() << std::endl;
    std::cout <<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl;
    //std::cout <<
#endif

#if 0
    meshIndices = MeshTools::duplicate(meshIndices, MeshTools::removeDuplicates(meshVertices));
    //meshIndices = MeshTools::removeDuplicates(meshVertices);
#else

    //      auto meshVerticesIndices = MeshTools::removeDuplicates(meshVertices);
    //auto meshVerticesIndices = MeshTools::duplicate( meshIndices, MeshTools::removeDuplicates(meshVertices));
#ifdef USE_INDICES // Convert back to non-indexed array
    meshVertices = MeshTools::duplicate(meshIndices, meshVertices);
    meshNormals = MeshTools::duplicate(meshIndices, meshNormals);
    meshColors = MeshTools::duplicate(meshIndices, meshColors);
#endif
    auto meshVerticesIndices = MeshTools::removeDuplicates(meshVertices);
    auto meshNormalsIndices = MeshTools::removeDuplicates(meshNormals);
    auto meshColorsIndices = MeshTools::removeDuplicates(meshColors);

#if 0
    std::cout << "\tVerticesIndices Min:" << meshVerticesIndices.size() << std::endl;
    std::cout << "\tNormalIndices Min:" << meshNormalsIndices.size() << std::endl;
    std::cout << "\tColorIndices Min:" << meshColors.size() << std::endl;
#endif

#ifdef TEST_CUBE
    CORRADE_ASSERT(meshVerticesIndices.size() == 36,"TEST_CUBE failed",);
    CORRADE_ASSERT(meshNormalsIndices.size() == 36,"TEST_CUBE failed",);
#endif


    //std::vector<UnsignedInt>
#if 1
    //      meshIndices.clear();
    meshIndices =
        //MeshTools::duplicate(meshIndices,
        MeshTools::combineIndexedArrays(
            std::make_pair(std::cref(meshVerticesIndices), std::ref(meshVertices)),
            std::make_pair(std::cref(meshNormalsIndices), std::ref(meshNormals)),
            std::make_pair(std::cref(meshColorsIndices), std::ref(meshColors))
            //)
                                        );
#endif
#if 0
    std::cout <<"\t Indices: "<< meshIndices.size() << std::endl;
    std::cout <<"\t Normals: "<< meshNormals.size() << std::endl;
    std::cout <<"\t Vertices: "<< meshVertices.size() << std::endl;
    std::cout <<"\t Colors: "<< meshColors.size() << std::endl;
    std::cout <<"\t Colors Indices: "<< meshColorsIndices.size() << std::endl;
#endif


    //meshIndices = Magnum::MeshTools::removeDuplicates(meshVertices);
    //std::cout << meshIndices.size() << std::endl;
#endif
  }
#endif

}


Application::Application(const Arguments& arguments) :
    Platform::Application{arguments} {

  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  {
    _pCameraObject = new Object3D{&_scene};
    _pCameraObject->translate(Vector3::zAxis(5.0f));
    _pCamera = new SceneGraph::Camera3D(*_pCameraObject);

    _viewMatrix = (Matrix4::lookAt({0.f, 0.f, 5.f}, {0.f, 0.f, 0.f}, Vector3::zAxis(1.f)));

    _pCamera->setProjectionMatrix(
        //Matrix4::perspectiveProjection(Magnum::Math::Deg<float>(60.0),
        Matrix4::perspectiveProjection(Magnum::Math::Deg<float>(60.0f),//60.0_degf,
                                       Vector2{GL::defaultFramebuffer.viewport().size()}.aspectRatio(), 0.001f, 100.0f))
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    _pManipulator = new Object3D{&_scene};
#if 1
    (*_pManipulator)
        .translate(Vector3::yAxis(-0.3f))
        .rotateX(Math::Rad<float>(30.f));
    //.rotateX(30.0_degf);
#endif
  }

  auto const genPoints = generatePoints(100);
  _pSv = new SphericalVoronoi(genPoints);
  CreateColors(256);

  auto cells = _pSv->getCells();
  auto vertices = _pSv->getVertices();
  auto edges = _pSv->getHalfEdges();

  //  _pSv->setDebugMode(true);
  _pSv->solve();

  smoothLaplacian(cells, vertices, 2);

  std::vector<Vector3> meshVertices;
  std::vector<UnsignedInt> meshIndices;
  std::vector<Vector3ui> meshFaces;

  std::vector<Vector3> meshNormals;
  std::vector<Color3> meshColors;
  std::vector<Vector2> meshUVs;

#if 1
  fillVectors(
      _pSv->getHalfEdges(),
      meshVertices,
      meshNormals,
      meshIndices,
      meshColors,
      meshUVs,
      _colors);
#endif
  //std::vector<Vector3> &meshVertices,
  //std::vector<Vector3> &meshNormals,
  //std::vector<UnsignedInt> &meshIndices,
  //std::vector<Color3> &meshColors,
  //std::vector<Vector2> &meshUVs)


#if 1
  new SphereDrawable{*_pManipulator, &_drawables,
        meshVertices,
        meshNormals,
        meshIndices,
        meshColors,
        };
#endif


}

void Application::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);


  _pCamera->draw(_drawables);
  swapBuffers();
  redraw(); // NOTE: Always redraw, for now . . .
}

#ifndef CORRADE_TARGET_ANDROID
void Application::keyPressEvent(KeyEvent &event) {
  if(event.key() == KeyEvent::Key::Q)
    exit();
}
#endif

void Application::mousePressEvent(MouseEvent& event) {
}
void Application::mouseReleaseEvent(MouseEvent& event) {
}
void Application::mouseMoveEvent(MouseMoveEvent& event) {
}
} // Magnum
