#include "Application.h"

#include <iostream>
#include <random>
#include <cstdlib>

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
    }

    auto const genPoints = generatePoints(10);
    _pSv = new SphericalVoronoi(genPoints);

    //  _pSv->setDebugMode(true);
    _pSv->solve();

    std::vector<Vector3> meshVertices;
    std::vector<UnsignedInt> meshIndices;
    std::vector<Vector3ui> meshFaces;

    std::vector<Vector3> meshNormals;
    std::vector<Color3> meshColors;
    std::vector<Vector2> meshUVs;

}

void Application::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
 

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
