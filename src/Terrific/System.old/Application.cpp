#include "Application.h"

#include <Magnum/MeshTools/RemoveDuplicates.h>
#include <Magnum/MeshTools/Duplicate.h>
#include <Magnum/MeshTools/CombineIndexedArrays.h>

#include <Magnum/Primitives/Cube.h>

#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/Assert.h>

namespace Magnum {


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
