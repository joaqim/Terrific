#include "Application.h"

#include <iostream>

#include <Corrade/PluginManager/PluginManager.h>
#include <Corrade/Interconnect/Receiver.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/GL/TextureFormat.h>

namespace Magnum {

typedef PluginManager::Manager<Trade::AbstractImporter> PluginManagerImporter;

class ImageLoader : public AbstractResourceLoader<Trade::ImageData2D> {
 public:
  std::unique_ptr<Trade::AbstractImporter> _tgaImporter;

  ImageLoader(PluginManagerImporter &pluginManager) {
    _tgaImporter = pluginManager.loadAndInstantiate("TgaImporter");
  }

  void doLoad(ResourceKey key) override {
    Debug{} << "ImageLoader::doLoad()";

    const Utility::Resource rs{"data-textures"};

    if(!_tgaImporter->openData(rs.getRaw("stone.tga"))) {
      setNotFound(key);
      Debug{} << "Image not found.";
      return;
    }
    Containers::Optional<Trade::ImageData2D> image = _tgaImporter->image2D(0);

    CORRADE_INTERNAL_ASSERT(image);
    //set(key, *image, ResourceDataState::Final, ResourcePolicy::Manual);
  }
};

class ImageManager : public ResourceManager<Trade::ImageData2D>, public Interconnect::Receiver {
 public:
  ImageManager() {

  }

  void onFileChanged(const std::string &filename) {
    Debug{} << filename << " changed!";

    //openFile(filename);
  }
};

Application::Application(const Arguments& arguments) :
    Platform::Application{arguments}
{
  {
    ImageManager imageManager;

    PluginManagerImporter pluginManagerImporter;
    ImageLoader imageLoader{pluginManagerImporter};
     imageManager.setLoader(&imageLoader);

  }
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
