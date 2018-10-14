#pragma once

#include <Magnum/GL/DefaultFramebuffer.h>
#ifndef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/Sdl2Application.h>
#else
#include <Magnum/Platform/AndroidApplication.h>
#endif

#include <Magnum/GL/Texture.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Shaders/VertexColor.h>

#include <Magnum/Text/Alignment.h>

#include <Magnum/ResourceManager.h>

namespace Magnum {

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

  };

}
