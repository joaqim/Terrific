#include "GuiGfxElement.h"

namespace Magnum {

// ANIMATION

void GuiGfxAnimation::addFrame(int num)
{
  _frames.push_back(num);
}
void GuiGfxAnimation::setType(GuiGfxAnimationType type)
{
  _type = type;
}
void GuiGfxAnimation::setFrameLength(float length)
{
  _frameLength = length;
}

// CONSTRUCTORS

GuiGfxElement::GuiGfxElement(Gui* apGui)
{
  mpGui = apGui;

  // Set up vertices

  for(int i = 0; i < 4; i++) {
    mvVtx[i] = {{0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 0.f}};
  }


  mvImageSize = Vector2{0, 0}; 
  mvOffset = Vector3{0, 0, 0};

  mvActiveSize = Vector2{0, 0}; 

  ////////////////////////////
  //Set up textures
  for(int i=0; i<kMaxGuiTextures; ++i)
  {
    mvTextures[i] = NULL;
    //mvImages[i] = NULL;
  }

  mlTextureNum =0;
  mlCurrentAnimation = 0;
  mfCurrentFrame =0;
  mbForwardAnim = true;
  mlActiveImage =0;
  mbAnimationPaused = false;

  mbFlushed = false;
}

//---------------------------------------------------

GuiGfxElement::~GuiGfxElement()
{
  mvAnimations.clear(); 

  ////////////////////////////////
  // Delete all textures / Images
  // TODO: Shouldn't be needed with Magnum::Resource<>
  /*
    if(mvImageBufferVec.size()>0)
    {
    for(int i=0; i< (int)mvImageBufferVec.size(); ++i)
    {
    //mpGui->GetResources()->GetImageManager()->Destroy(mvImageBufferVec[i]);
    }
    }
    else
    {
    for(int i=0; i<mlTextureNum; ++i)
    {
    if(mvImages[i])
    {
    //Skip for now, memory might be fucked..
    //mpGui->GetResources()->GetImageManager()->Destroy(mvImages[i]);
    }
    else if(mvTextures[i])
    {
    mpGui->GetResources()->GetTextureManager()->Destroy(mvTextures[i]);
    }
    }
    }
  */
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void GuiGfxElement::update(float afTimeStep)
{
  /*
//////////////////////////////////////
  //Update image animations
  if(mvAnimations.empty() || mbAnimationPaused) return;
  if(mlCurrentAnimation >= (int)mvAnimations.size()) return;

  GuiGfxAnimation* pAnim = mvAnimations[mlCurrentAnimation];

  int lFrame = 0;
  //////////////////////////////////
  // Non random animation update

  if(pAnim->_type != GuiGfxAnimationType_RANDOM)
  {
  mfCurrentFrame += afTimeStep * (1.0f/pAnim->mfFrameLength);
  lFrame = (int) mfCurrentFrame;
  if(lFrame >= (int)mvImageBufferVec.size())
  {
  //Log("Over max!\n");
  //Stop at end
  if(pAnim->mType == eGuiGfxAnimationType_StopAtEnd)
  {
  lFrame = (int)mvImageBufferVec.size()-1;
  mfCurrentFrame = (float)lFrame;
  }
  //Loop
  else if(pAnim->mType == eGuiGfxAnimationType_Loop)
  {
  lFrame =0;
  mfCurrentFrame = 0;
  }
  //Oscillate
  else if(pAnim->mType == eGuiGfxAnimationType_Oscillate)
  {
  lFrame =1;
  mfCurrentFrame = 1;
  mbForwardAnim = !mbForwardAnim;
  }
  }
  //Log("Frame %d %f actual_frame: %d size: %d\n",lFrame,mfCurrentFrame,((int)mvImageBufferVec.size()-1) - lFrame,mvImageBufferVec.size());

  //Oscillate fix
  if(mbForwardAnim== false && pAnim->mType == eGuiGfxAnimationType_Oscillate)
  {
  lFrame = ((int)mvImageBufferVec.size()-1) - lFrame;
  }
  }
  //////////////////////////////////
  // Random animation update
  else if(mvImageBufferVec.size() > 1)
  {
  float fPrev = mfCurrentFrame;
  mfCurrentFrame += afTimeStep * (1.0f/pAnim->mfFrameLength);
  lFrame = (int) mfCurrentFrame;
  if((int)mfCurrentFrame != (int)fPrev)
  {
  int lPrev = (int)fPrev;
  do
  {
  lFrame = cMath::RandRectl(0, (int)mvImageBufferVec.size()-1);
  }
  while(lFrame == lPrev);

  mfCurrentFrame = (float)lFrame;
  }
  }
  //////////////////////////////////
  // Set new image
  if(lFrame != mlActiveImage)
  {
  mlActiveImage = lFrame;
  SetImage(mvImageBufferVec[mlActiveImage],0);
  }
  */
}

//-----------------------------------------------------------------------

void GuiGfxElement::addImage(GL::BufferImage2D aImage) {

  setImage(aImage, mlTextureNum);

  mvActiveSize = getImageSize();

  ++mlTextureNum;
}

//---------------------------------------------------

void GuiGfxElement::addTexture(GL::Texture2D aTexture) {
}

//---------------------------------------------------

void GuiGfxElement::addImageToBuffer(GL::BufferImage2D aImage)
{
  if(mvImageBufferVec.size()==0)
  {
    setImage(aImage,0);
  }

  mvImageBufferVec.push_back(aImage);
}

//---------------------------------------------------

GuiGfxAnimation* GuiGfxElement::createAnimtion(const std::string& asName)
{
  GuiGfxAnimation *pAnimation = new GuiGfxAnimation();

  pAnimation->_name = asName;

  mvAnimations.push_back(pAnimation);

  return pAnimation;
}

//---------------------------------------------------

void GuiGfxElement::playAnimation(int alNum)
{
  if(mlCurrentAnimation == alNum) return;

  mlCurrentAnimation = alNum;

  mfCurrentFrame =0;
  mbForwardAnim = true;
  mlActiveImage =0;
  setImage(mvImageBufferVec[mlActiveImage],0);
}

void GuiGfxElement::setAnimationTime(float afTime)
{
  if(mlCurrentAnimation>=0)
    mfCurrentFrame = afTime / mvAnimations[mlCurrentAnimation]->_frameLength;
  else
    mfCurrentFrame = afTime;
}

//---------------------------------------------------

void GuiGfxElement::setMaterial(GuiMaterial *apMat)
{
  mpMaterial = apMat;
}

//---------------------------------------------------

void GuiGfxElement::setColor(const Color3 &aColor)
{
  for(int i=0; i<4; ++i)	mvVtx[i].col = aColor;
}

//-----------------------------------------------------------------------

Vector2 GuiGfxElement::getImageSize()
{
  return mvImageSize;
}

//-----------------------------------------------------------------------

void GuiGfxElement::flush()
{
  if(mbFlushed) return;

  /*
    for(int i=0; i<mlTextureNum; ++i)
    {
    if(mvImages[i]) mvImages[i]->getFrameBitmap()->flushToTexture();
    }
    for(size_t i=0; i < mvImageBufferVec.size(); ++i)
    {
    if(mvImageBufferVec[i]) mvImageBufferVec[i]->getFrameBitmap()->FlushToTexture();
    }
  */

  mbFlushed = true;
}

//-----------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// PRIVATE METHODS
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------

void GuiGfxElement::setImage(GL::BufferImage2D apImage, int alNum) {
  //Set image and texture (for sorting)
  //mvImages[alNum] = apImage;
  //mvTextures[alNum] = apImage->GetTexture();

  /*
      //Get texture coords
      const tVertexVec& vImageVtx = apImage->GetVertexVec();
      for(int i=0; i<4; ++i) mvVtx[i].tex = vImageVtx[i].tex;

      if(alNum==0)
      {
      mvImageSize.x = (float)apImage->GetWidth();
      mvImageSize.y = (float)apImage->GetHeight();
      }
      */
      }
};




