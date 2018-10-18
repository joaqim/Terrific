#pragma once

#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif
#include <Magnum/Trade/ImageData.h>

#include <Magnum/GL/ImageFormat.h>
#include <Magnum/GL/PixelFormat.h>
#include <Magnum/GL/BufferImage.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/Trade/ImageData.h>
#include <Corrade/Containers/Optional.h>
#include <Corrade/Containers/Array.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

#include <Magnum/Trade/AbstractImporter.h>

#include <Terrific/Geometry/SphericalVoronoi.h>
#include <Terrific/GL/SphereDrawable.h>

#include <Corrade/Utility/Resource.h>

#include "Bitmap.h"
#include "Planet.h"
#include <FastNoiseSIMD.h>

namespace Magnum {

  using Terrific::Geometry::SphericalVoronoi;
  typedef SceneGraph::Object<SceneGraph::MatrixTransformation3D> Object3D;
  typedef SceneGraph::Scene<SceneGraph::MatrixTransformation3D> Scene3D;

  class Application: public Platform::Application {
 public:
    explicit Application(const Arguments& arguments);

 private:
    void drawEvent() override;

 private:
    Vector3f positionOnSphere(const Vector2i& position) const;
#ifndef CORRADE_TARGET_ANDROID
    void viewportEvent(ViewportEvent &event) override;
    void mouseScrollEvent(MouseScrollEvent& event);
    void keyPressEvent(KeyEvent &event) override;
#endif

    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;
    void mouseMoveEvent(MouseMoveEvent& event) override;

 private:
    void CreateColors(std::size_t const count);

 private:
    //SphericalVoronoi *_pSv;
    Terrific::Planet *_pSv;
    Terrific::GL::SphereDrawable *sphere;

    std::vector<Color3> _colors;
 private:
    Containers::Array<Containers::Optional<GL::Texture2D>> _textures;
    GL::Texture2D _texture;
 private:
    Scene3D _scene;
    SceneGraph::DrawableGroup3D _drawables;
    SceneGraph::Camera3D *_pCamera;
    Object3D *_pCameraObject;
    Object3D *_pManipulator;
    Matrix4 _viewMatrix;


    Vector3f _mainCameraVelocity;
    Vector3f _previousPosition;
  };
}
