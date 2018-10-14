#include <vector>
#include <Magnum/GL/Texture.h> 
#include <Magnum/GL/BufferImage.h> 
#include <Magnum/Image.h> 
#include <Magnum/Resource.h>

#include <Magnum/Math/Color.h> 
#include <Magnum/Magnum.h> 

#include "GuiTypes.h"

namespace Magnum {

  class GuiMaterial;
  class ResourceImage;
  class Gui;

  struct Vertex {
    Vector2 pos;
    Vector2 tex;
    Color3 col; 
  };

#define kMaxGuiTextures (4)

  //------------------------------------------

  enum GuiGfxAnimationType
  {
    GuiGfxAnimationType_LOOP,
    GuiGfxAnimationType_STOP_AT_END,
    GuiGfxAnimationType_OSCILLATE,
    GuiGfxAnimationType_RANDOM,
    GuiGfxAnimationType_LASTENUM
  };

  class GuiGfxAnimation
  {
    friend class GuiGfxElement;
 public:
 GuiGfxAnimation(): _frameLength(1), _type(GuiGfxAnimationType_LOOP){}

    void addFrame(int num);
    void setType(GuiGfxAnimationType type);
    void setFrameLength(float length);

 private:
    std::string _name;
    std::vector<int> _frames;
    float _frameLength;
    GuiGfxAnimationType _type;
  };

  //------------------------------------------

  class GuiGfxElement
  {
    friend class GuiSet;
    friend class GuiRenderObjectCompare;
 public:
    GuiGfxElement(Gui* apGui);
    ~GuiGfxElement();

    void update(float afTimeStep);

    void addImage(GL::BufferImage2D aImage);
    void addTexture(GL::Texture2D apTexture);

    void addImageToBuffer(GL::BufferImage2D aImage);

    void setOffset(const Vector3& avOffset){mvOffset = avOffset;}
    const Vector3& getOffset()const { return mvOffset;}

    void setActiveSize(const Vector2& avSize){mvActiveSize = avSize;}
    const Vector2& getActiveSize(){ return mvActiveSize;}

    GuiGfxAnimation* createAnimtion(const std::string& asName);
    void playAnimation(int alNum);
    GuiGfxAnimation* getAnimation(int alIdx){return mvAnimations[alIdx];}
    void setAnimationTime(float afTime);

    void setAnimationPaused(bool abX){mbAnimationPaused = abX;}
    bool gSetAnimationPaused(){ return mbAnimationPaused;}

    void setMaterial(GuiMaterial *apMat);

    void setColor(const Color3 &aColor);

    Vector2 getImageSize();

    void flush();
 private:
    void setImage(GL::BufferImage2D apImage, int alNum);

    Vector2 mvImageSize;
    Gui *mpGui;
    //std::vector<Vertex> mvVtx;
    //Vertex mvVtx[3]; 
    Containers::ArrayView<Vertex> mvVtx{0, 3}; 

    Vector3 mvOffset;
    Vector2 mvActiveSize;

    GuiMaterial *mpMaterial;
    GL::Texture2D* mvTextures[kMaxGuiTextures]; 
    GL::BufferImage2D mvImages[kMaxGuiTextures];

    std::vector<GL::BufferImage2D> mvImageBufferVec; 

    std::vector<GuiGfxAnimation*> mvAnimations;
    int mlCurrentAnimation;
    float mfCurrentFrame;
    int mlActiveImage;
    bool mbForwardAnim;
    bool mbAnimationPaused;

    int mlTextureNum;

    bool mbFlushed;
  };
};

