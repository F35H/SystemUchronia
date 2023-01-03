/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/
Copyright (c) 2000-2013 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

//! [starter]

#include <exception>
#include <iostream>

#include "OGRE/Ogre.h"
#include "OGRE/OgreApplicationContext.h"
#include "OGRE/OgreInput.h"
#include "OGRE/OgreRTShaderSystem.h"
#include "OGRE/OgreCameraMan.h"

using namespace Ogre;
using namespace OgreBites;


struct ShipType {
  std::string MainMesh;

  unsigned char DegreeY;
  unsigned char DegreeZ;

  unsigned char MainEngGear = 0;
  unsigned char MainEngPow;

  unsigned char ScndEngGear = 0;
  unsigned char ScndEngPow;

  unsigned char ShipInt;
}; //ShipType


struct Ship :
  ShipType {
  enum { defaultShip = 0 };

  SceneNode* shipNode;

  Ship(SceneManager* scnMgr, char shTy) {
    switch (shTy) {
    default:
      DegreeZ = 5;
      DegreeY = 5;
      MainEngPow = 1;
      ScndEngPow = 1;

      MainMesh = "Ship.mesh";
      break;
    }; //Switch

    Entity* tShip = scnMgr
      ->createEntity(MainMesh);

    tShip->setCastShadows(true);

    shipNode = scnMgr->getRootSceneNode()
      ->createChildSceneNode();

    shipNode->attachObject(tShip);

    //shipNode->pitch(Degree(90));
  } //Ship

}; //ShipClass


class bitPress {
protected:
  bool A : 1;
  bool W : 1;
  bool D : 1;
  bool S : 1;

  bool Q : 1;
  bool E : 1;

  bool I : 1;
  bool J : 1;
  bool L : 1;
  bool K : 1;

  bool Enter : 1;
  bool Space : 1;

  bitPress() {
    A = 0;
    W = 0;
    D = 0;
    S = 0;

    Q = 0;
    E = 0;
    
    I = 0;
    J = 0;
    L = 0;
    K = 0;
    
    Enter = 0;
    Space = 0;
  }; //bitPressFunc
}; //bitPressClass


class TutorialApplication
  : public ApplicationContext
  , public InputListener
  , bitPress
{
public:
  TutorialApplication();
  virtual ~TutorialApplication();

  Ship* pShip;
  SceneNode* camNode;
  RTShader::ShaderGenerator* shadergen;
  SceneManager* scnMgr;

  void setup();
  bool keyPressed(const KeyboardEvent& evt);
  bool keyReleased(const KeyboardEvent& evt);
  void frameRendered(const FrameEvent& evt);
  bool mouseMoved(const MouseMotionEvent& evt);
}; 


TutorialApplication::TutorialApplication()
  : ApplicationContext("ShipGame")
{
} 


TutorialApplication::~TutorialApplication()
{
  delete pShip;
}


void TutorialApplication::setup()
{
  ApplicationContext::setup();
  addInputListener(this);

  Root* root = getRoot();
  scnMgr = root->createSceneManager();

  shadergen = RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr);

  camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  Camera* cam = scnMgr->createCamera("myCam");

  camNode->setPosition(-50, 0, 0);
  camNode->lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
  camNode->roll(Degree(-90));

  cam->setNearClipDistance(5);
  camNode->attachObject(cam);

  Viewport* vp = getRenderWindow()->addViewport(cam);

  pShip = new Ship(scnMgr, Ship::defaultShip);

  vp->setBackgroundColour(ColourValue(0, 0, 0));

  cam->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

  scnMgr->setAmbientLight(ColourValue(0, 0, 0));
  scnMgr->setShadowTechnique(
    ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

  Light* spotLight = scnMgr->createLight("Sun");

  spotLight->setDiffuseColour(1, 1, 1);
  spotLight->setSpecularColour(1, 1, 1);

  //! [spotlighttype]
  spotLight->setType(Light::LT_POINT);
  //! [spotlighttype]

  //! [spotlightposrot]
  SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  spotLightNode->attachObject(spotLight);
  spotLightNode->setPosition(Vector3(-50, 0, 0));
  //! [spotlightposrot]

  // -- tutorial section end --
}


void TutorialApplication::frameRendered(const FrameEvent& evt) {
  if (bitPress::A) {
    Ogre::Degree d = Degree(
      1 * pShip->DegreeY * evt.timeSinceLastFrame);
    Ogre::Quaternion q = Quaternion(
      Ogre::Radian(d), Ogre::Vector3::UNIT_Y);

    pShip->shipNode->rotate(q);
  }; // IF


  if (bitPress::W) {
    Ogre::Degree d = Degree(
      1 * pShip->DegreeZ * evt.timeSinceLastFrame);
    Ogre::Quaternion q = Quaternion(
      Ogre::Radian(d), Ogre::Vector3::UNIT_Z);

    pShip->shipNode->rotate(q);
  }; // IF


  if (bitPress::S) {
    Ogre::Degree d = Degree(
      -1 * pShip->DegreeZ * evt.timeSinceLastFrame);

    Ogre::Quaternion q = Quaternion(
      Ogre::Radian(d), Ogre::Vector3::UNIT_Z);

    pShip->shipNode->rotate(q);
  }; // IF


  if (bitPress::D) {
    Ogre::Degree d = Degree(
      -1 * pShip->DegreeY * evt.timeSinceLastFrame);
    Ogre::Quaternion q = Quaternion(
      Ogre::Radian(d), Ogre::Vector3::UNIT_Y);

    pShip->shipNode->rotate(q);
  }; // IF
};


bool TutorialApplication::keyPressed(const KeyboardEvent& evt)
{
  switch (evt.keysym.sym) {
    case SDLK_ESCAPE: {
      getRoot()->queueEndRendering();
    } //ESCAPE
    break;

    //XRotate
    case 119: { //W
      bitPress::W = true;
    } //W
    break;

    case 115: { //S
      bitPress::S = true;
    } //S
    break;


    //YRotate
    case 97: { //A
      bitPress::A = true;
    } //A
    break;

    case 100: { //D
      bitPress::D = true;
    } //D
    break;


    //Thrusters
    case 113: { //Q, MainThruster

    } //D
    break;

    case 101: { //E, ScndThruster

    } //D
    break;

    default:
      std::cout << evt.keysym.sym << std::endl;
    break;
  };

  return true;
}


bool TutorialApplication::keyReleased(const KeyboardEvent& evt) {
  switch (evt.keysym.sym) {
  case SDLK_ESCAPE: {
    getRoot()->queueEndRendering();
  } //ESCAPE
  break;

  //XRotate
  case 119: { //W
    bitPress::W = false;
  } //W
  break;

  case 115: { //S
    bitPress::S = false;
  } //S
  break;


          //YRotate
  case 97: { //A
    bitPress::A = false;
  } //A
  break;

  case 100: { //D
    bitPress::D = false;
  } //D
  break;


          //Thrusters
  case 113: { //Q, MainThruster

  } //D
  break;

  case 101: { //E, ScndThruster

  } //D
  break;

  default:
    std::cout << evt.keysym.sym << std::endl;
    break;
  };

  return true;
};


bool TutorialApplication::mouseMoved(const MouseMotionEvent& evt) {

  //x, y 0, 0 top left corner out of resolution. Create of screen to move camera.
  //Make camera class

  std::cout << evt.x << "|" << evt.y << std::endl;

  return true;
};


int main(int argc, char** argv)
{
  try
  {
    TutorialApplication app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error occurred during execution: " << e.what() << '\n';
    return 1;
  }

  return 0;
}

//! [starter]