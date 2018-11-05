#include "Application.h"

#include <iostream>
#include <random>
#include <cstdlib>

#include <Magnum/MeshTools/RemoveDuplicates.h>
#include <Magnum/MeshTools/Duplicate.h>
#include <Magnum/MeshTools/CombineIndexedArrays.h>

#include <Magnum/Primitives/Cube.h>

#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/Assert.h>

#define SEEDED

namespace Magnum {


std::vector<Vector3d> generatePoints(size_t const count) {
  std::vector<Vector3d> points;

  std::random_device const rd;
#ifdef SEEDED
  std::mt19937 rng(145);
#else
  std::mt19937 rng(rd());
  rng.discard(700000);
#endif

  //NOTE: Perform 70000 generations, much slower but improves randomness.
  // https://codereview.stackexchange.com/a/109518



  std::uniform_real_distribution<> heightDistribution(-1.f, 1.f);
  std::uniform_real_distribution<> angleDistribution(0, 2 * M_PI);

  using Magnum::Math::asin;
  using Magnum::Math::cos;
  using Magnum::Math::sin;
  using Magnum::Math::Rad;

  for (size_t i = 0; i < count; i++) {
    double z = heightDistribution(rng);
    auto const phi = Rad<double>(angleDistribution(rng));
    auto theta = asin(z);
    float x = cos(theta) * cos(phi);
    float y = cos(theta) * sin(phi);

    points.push_back(Vector3d(x, y, z));
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

template<class T>
Vector3f CalculateSurfaceNormal (Magnum::Math::Vector3<T> const &p1, Magnum::Math::Vector3<T> const &p2, Magnum::Math::Vector3<T> const &p3) {
  Vector3f const u{p2 - p1};
  Vector3f const v{p3 - p1};

  Vector3f n;
  n.x() = u.y() * v.z() - u.z() * v.y();
  n.y() = u.z() * v.x() - u.x() * v.z();
  n.z() = u.x() * v.y() - u.y() * v.x();
  return n;
}

inline void addVerticesAndNormals(Vector3f const &p1, Vector3f const &p2, Vector3f const &p3, std::vector<Vector3f> &vertices, std::vector<Vector3f> &normals ) {
  vertices.emplace_back(p1);
  vertices.emplace_back(p2);
  vertices.emplace_back(p3);

  std::fill_n(std::back_inserter(normals), 3, CalculateSurfaceNormal(p1, p2, p3));
}


template <class T>
inline Vector2f getUVCoords(Magnum::Math::Vector3<T> const &p) {
  auto const phi = Magnum::Math::atan(p.x() / p.y());
  auto const theta = Magnum::Math::acos(p.normalized().z());

  auto const u = Magnum::Math::sin(theta)*Magnum::Math::cos(phi);
  auto const v = Magnum::Math::sin(theta)*Magnum::Math::sin(phi);
#if 1
  //#ifdef DEBUG
  CORRADE_ASSERT(-1 <= u <= 1, "Error: !(-1 <= u <= 1)", Vector2{});
  CORRADE_ASSERT(-1 <= v <= 1, "Error: !(-1 <= v <= 1)", Vector2{});
  CORRADE_ASSERT(Magnum::Math::pow<2>(u)+Magnum::Math::pow<2>(v) <= 1, "Error: !(u^2 + v^2 <= 1)", Vector2{});
#endif
  return Vector2f{static_cast<float>(u), static_cast<float>(v)};
}

inline void addVerticesAndNormals(Vector3d const &p1, Vector3d const &p2, Vector3d const &p3, std::vector<Vector3f> &vertices, std::vector<Vector3f> &normals, std::vector<Vector2> &uvs) {
  vertices.emplace_back(p1);
  vertices.emplace_back(p2);
  vertices.emplace_back(p3);

  std::fill_n(std::back_inserter(normals), 3, CalculateSurfaceNormal(p1, p2, p3));

  uvs.emplace_back(getUVCoords(p1));
  uvs.emplace_back(getUVCoords(p2));
  uvs.emplace_back(getUVCoords(p3));
}

Application::Application(const Arguments& arguments) :
    Platform::Application{arguments} {
  using namespace Math::Literals;
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  //GL::Renderer::setFrontFace(GL::Renderer::FrontFace::ClockWise);
  {
        _pCameraObject = new Object3D{&_scene};
    _pCameraObject->translate(Vector3f::zAxis(5.0f));
    _pCamera = new SceneGraph::Camera3D(*_pCameraObject);

    _viewMatrix = (Matrix4::lookAt({0.f, 0.f, 5.f}, {0.f, 0.f, 0.f}, Vector3f::zAxis(1.f)));

    _pCamera->setProjectionMatrix(
        Matrix4::perspectiveProjection(
            60.0_degf,
            Vector2{GL::defaultFramebuffer.viewport().size()}.aspectRatio(),
            0.001f,
            100.0f))
        .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setViewport(GL::defaultFramebuffer.viewport().size());

    _pManipulator = new Object3D{&_scene};
    (*_pManipulator)
        .scale(Vector3f{2.0f, 2.0f,2.0f});
#if 0
    (*_pManipulator)
        .translate(Vector3f::yAxis(-0.3f))
        .rotateX(Math::Rad<float>(30.f));
    //.rotateX(30.0_degf);
#endif
  }

}

void Application::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);


//  _pCamera->draw(_drawables);
  swapBuffers();
  redraw(); // NOTE: Always redraw, for now . . .
}

Vector3f Application::positionOnSphere(const Vector2i& position) const {
  const Vector2 positionNormalized = Vector2{position*2.f}/Vector2{_pCamera->viewport()} - Vector2{1.f};
  //const Vector2 positionNormalized = Vector2{position}/Vector2{_pCamera->viewport()} - Vector2{0.5f};
  const Float length = positionNormalized.length();
  Vector3f const result(length > 1.0f
                        ? Vector3f(positionNormalized, 0.0f)
                        : Vector3f(positionNormalized, 1.0f - length));
  //result.y() *= -1.0f;
  return (result*Vector3f::yScale(-1.0f)).normalized();
  //result = Vector3f(result * Vector3f::yScale(-1.0f));
  //return result.normalized();
}

#ifndef CORRADE_TARGET_ANDROID
void Application::viewportEvent(ViewportEvent &event) {
  GL::defaultFramebuffer.setViewport({{}, event.windowSize()});
  _pCamera->setViewport(event.windowSize());
}

void Application::mouseScrollEvent(MouseScrollEvent& event) {
  if(!event.offset().y()) return;

  /* Distance to origin */
  Float distance = _pCameraObject->transformation().translation().z();

  /* Move 15% of the distance back or forward */
  distance *= 1 - (event.offset().y() > 0 ? 1/0.85f : 0.85f);
  _pCameraObject->translate(Vector3f::zAxis(distance));

  redraw();
}

void Application::keyPressEvent(KeyEvent &event) {
  if(event.key() == KeyEvent::Key::Q)
    exit();
}
#endif

void Application::mousePressEvent(MouseEvent& event) {
  if(event.button() == MouseEvent::Button::Left)
    _previousPosition = positionOnSphere(event.position());  
  event.setAccepted();
}

void Application::mouseReleaseEvent(MouseEvent& event) {
  if(event.button() == MouseEvent::Button::Left)
    _previousPosition = Vector3f{0};
  event.setAccepted();
}
void Application::mouseMoveEvent(MouseMoveEvent& event) {
  if(!(event.buttons() & MouseMoveEvent::Button::Left)) return;
  if (_previousPosition.isZero()) return;

  const Vector3f currentPosition = positionOnSphere(event.position());
  const Vector3f axis = Math::cross(_previousPosition, currentPosition);

  if(_previousPosition.length() < 0.001f || axis.length() < 0.001f) return;

  _pManipulator->rotate(Math::angle(_previousPosition, currentPosition), axis.normalized());
  _previousPosition = currentPosition;

  event.setAccepted();
  redraw();
}
} // Magnum
