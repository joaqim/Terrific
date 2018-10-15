#pragma once

#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif

#include <Terrific/Geometry/SphericalVoronoi.h>
#include <Terrific/GL/SphereDrawable.h>

namespace Magnum {

  using Terrific::Geometry::SphericalVoronoi;

  class Application: public Platform::Application {
 public:
    explicit Application(const Arguments& arguments);

 private:
    void drawEvent() override;

#ifndef CORRADE_TARGET_ANDROID
    void keyPressEvent(KeyEvent &event) override;
#endif

    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
 private:
    SphericalVoronoi *_pSv;
 private:
    Scene3D _scene;
    SceneGraph::DrawableGroup3D _drawables;
    SceneGraph::Camera3D *_pCamera;
    Object3D *_pCameraObject;
    Object3D *_pManipulator;
    Matrix4 _viewMatrix;


    Vector3 _mainCameraVelocity;
    Vector3 _previousPosition;
  };
}
