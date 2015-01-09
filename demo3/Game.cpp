#include <cstdio>
#include <sys/time.h>
#include <iostream>
#include <sstream>
#include "Game.hpp"


using namespace Dodge;
using namespace std;


void Game::quit() {
   exit(0);
}

void Game::keyUp(int code) {
   m_keys[code] = false;

   // dirKeyStack is the list of currently depressed directional
   // keys in the order they were pressed.
   for (uint_t i = 0; i < m_dirKeyStack.size(); ++i) {
      if (m_keys[m_dirKeyStack[i]] == false) {
         m_dirKeyStack.erase(m_dirKeyStack.begin() + i);
         --i;
      }
   }
}

void Game::keyDown(int code) {
   if (code == WinIO::KEY_LEFT || code == WinIO::KEY_RIGHT
      || code == WinIO::KEY_UP || code == WinIO::KEY_DOWN) {

      m_dirKeyStack.push_back(code);
   }

   m_keys[code] = true;

   switch (code) {
      case WinIO::KEY_ESCAPE: quit(); break;
   }
}

void Game::keyboard() {
   static long strMoveDown = internString("moveDown");
   static long strMoveLeft = internString("moveLeft");
   static long strMoveUp = internString("moveUp");
   static long strMoveRight = internString("moveRight");

   static int missedKeypress = -1;

   // Attempt to move in direction of missed keypress
   if (missedKeypress != -1) {
      switch (missedKeypress) {
         case WinIO::KEY_RIGHT:
            if (m_player->playAnimation(strMoveRight)) {
               m_player->finishTransformation(strMoveRight);
               m_player->playTransformation(strMoveRight);
               missedKeypress = -1;
            }
         break;
         case WinIO::KEY_LEFT:
            if (m_player->playAnimation(strMoveLeft)) {
               m_player->finishTransformation(strMoveLeft);
               m_player->playTransformation(strMoveLeft);
               missedKeypress = -1;
            }
         break;
         case WinIO::KEY_UP:
            if (m_player->playAnimation(strMoveUp)) {
               m_player->finishTransformation(strMoveUp);
               m_player->playTransformation(strMoveUp);
               missedKeypress = -1;
            }
         break;
         case WinIO::KEY_DOWN:
            if (m_player->playAnimation(strMoveDown)) {
               m_player->finishTransformation(strMoveDown);
               m_player->playTransformation(strMoveDown);
               missedKeypress = -1;
            }
         break;
      }
   }
   else {
      // Move in the direction of most recently pressed key
      if (m_dirKeyStack.size() > 0) {
         switch (m_dirKeyStack.back()) {
            case WinIO::KEY_RIGHT:
               if (m_player->playAnimation(strMoveRight)) {
                  m_player->finishTransformation(strMoveRight);
                  m_player->playTransformation(strMoveRight);
               }
               else {   // If the player is already moving remember this keypress
                  if (m_player->getAnimName() != strMoveRight)
                     missedKeypress = WinIO::KEY_RIGHT;
               }
            break;
            case WinIO::KEY_LEFT:
               if (m_player->playAnimation(strMoveLeft)) {
                  m_player->finishTransformation(strMoveLeft);
                  m_player->playTransformation(strMoveLeft);
               }
               else {
                  if (m_player->getAnimName() != strMoveLeft)
                     missedKeypress = WinIO::KEY_LEFT;
               }
            break;
            case WinIO::KEY_UP:
               if (m_player->playAnimation(strMoveUp)) {
                  m_player->finishTransformation(strMoveUp);
                  m_player->playTransformation(strMoveUp);
               }
               else {
                  if (m_player->getAnimName() != strMoveUp)
                     missedKeypress = WinIO::KEY_UP;
               }
            break;
            case WinIO::KEY_DOWN:
               if (m_player->playAnimation(strMoveDown)) {
                  m_player->finishTransformation(strMoveDown);
                  m_player->playTransformation(strMoveDown);
               }
               else {
                  if (m_player->getAnimName() != strMoveDown)
                     missedKeypress = WinIO::KEY_DOWN;
               }
            break;
         }
      }
   }
}

void Game::computeFrameRate() {
   static long i = 0;
   ++i;

   if (i % 100 == 0) {
      static timeval now;
      static timeval prev;

      gettimeofday(&now, NULL);

      double t = now.tv_sec - prev.tv_sec;
      t += static_cast<double>(now.tv_usec - prev.tv_usec) / 1000000.0;

      if (i > 100)
         m_frameRate = 100.0 / t;

      prev = now;
   }
}

void Game::playerSetup() {
   float32_t w = 32.f * gGetPixelSize().x;
   float32_t h = 32.f * gGetPixelSize().y;

   pTexture_t texMan(new Texture("data/textures/man.png"));
   m_player = pSprite_t(new Sprite(internString("m_player"), internString("man"), texMan));
   m_player->setOnScreenSize(w, h);

   vector<AnimFrame> aFrames;
   aFrames.push_back(AnimFrame(Vec2f(0.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(0.f, 32.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(0.f, 64.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(0.f, 96.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t animMoveDown(new Animation(internString("moveDown"), 0.25f, aFrames));

   aFrames.clear();
   aFrames.push_back(AnimFrame(Vec2f(32.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 32.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 64.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 96.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t animMoveLeft(new Animation(internString("moveLeft"), 0.25f, aFrames));

   aFrames.clear();
   aFrames.push_back(AnimFrame(Vec2f(64.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(64.f, 32.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(64.f, 64.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(64.f, 96.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t animMoveUp(new Animation(internString("moveUp"), 0.25f, aFrames));

   aFrames.clear();
   aFrames.push_back(AnimFrame(Vec2f(96.f, 0.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(96.f, 32.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(96.f, 64.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(96.f, 96.f), Vec2f(32.f, 32.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t animMoveRight(new Animation(internString("moveRight"), 0.25f, aFrames));

   m_player->addAnimation(animMoveDown);
   m_player->addAnimation(animMoveLeft);
   m_player->addAnimation(animMoveUp);
   m_player->addAnimation(animMoveRight);

   vector<TransPart> fFrames;
   fFrames.push_back(TransPart(0.25f, Vec2f(0.f, -h), 0.f, Vec2f(1.f, 1.f)));
   pTransformation_t transMoveDown(new Transformation(internString("moveDown"), fFrames));

   fFrames.clear();
   fFrames.push_back(TransPart(0.25f, Vec2f(-w, 0.f), 0.f, Vec2f(1.f, 1.f)));
   pTransformation_t transMoveLeft(new Transformation(internString("moveLeft"), fFrames));

   fFrames.clear();
   fFrames.push_back(TransPart(0.25f, Vec2f(0.f, h), 0.f, Vec2f(1.f, 1.f)));
   pTransformation_t transMoveUp(new Transformation(internString("moveUp"), fFrames));

   fFrames.clear();
   fFrames.push_back(TransPart(0.25f, Vec2f(w, 0.f), 0.f, Vec2f(1.f, 1.f)));
   pTransformation_t transMoveRight(new Transformation(internString("moveRight"), fFrames));

   m_player->addTransformation(transMoveDown);
   m_player->addTransformation(transMoveLeft);
   m_player->addTransformation(transMoveUp);
   m_player->addTransformation(transMoveRight);

   m_player->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.f, 0.f), Vec2f(w, 0.f), Vec2f(w, h), Vec2f(0.f, h))));

   m_player->setTranslation(w * 7.f, h * 5.f);
   m_player->setZ(2);
   m_player->setFillColour(Colour(1.f, 1.f, 1.f, 1.f));
   m_player->addToWorld();

   m_worldSpace.trackEntity(m_player);
}

void Game::buttonPressHandler(pEntity_t entity) {
   std::cout << "Button Pressed!\n";
}

void Game::buttonReleaseHandler(pEntity_t entity) {
   std::cout << "Button Released!\n";
}

void Game::uiSetup() {
   float32_t w = 16.f * gGetPixelSize().x;
   float32_t h = 16.f * gGetPixelSize().y;

   pTexture_t tex(new Texture("data/textures/ss8x8squares128x128y.png"));
   pUiButton_t btn(new UiButton(internString("button1"), tex));
   btn->setOnScreenSize(w, h);
   btn->setFillColour(Colour(1.f, 1.f, 1.f, 1.f));

   vector<AnimFrame> aFrames;
   aFrames.push_back(AnimFrame(Vec2f(0.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(16.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(48.f, 0.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t btnPress(new Animation(internString("press"), 0.25f, aFrames));

   aFrames.clear();
   aFrames.push_back(AnimFrame(Vec2f(0.f, 16.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(16.f, 16.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 16.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(48.f, 16.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t btnRelease(new Animation(internString("release"), 0.25f, aFrames));

   aFrames.clear();
   aFrames.push_back(AnimFrame(Vec2f(0.f, 32.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(16.f, 32.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 32.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(48.f, 32.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t hoverOn(new Animation(internString("hoverOn"), 0.25f, aFrames));

   aFrames.clear();
   aFrames.push_back(AnimFrame(Vec2f(0.f, 48.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(16.f, 48.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(32.f, 48.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   aFrames.push_back(AnimFrame(Vec2f(48.f, 48.f), Vec2f(16.f, 16.f), Colour(1.0, 1.0, 1.0, 1.0)));
   pAnimation_t hoverOff(new Animation(internString("hoverOff"), 0.25f, aFrames));

   btn->addAnimation(btnPress);
   btn->addAnimation(btnRelease);
   btn->addAnimation(hoverOn);
   btn->addAnimation(hoverOff);

   btn->setShape(unique_ptr<Shape>(new Quad(Vec2f(0.f, 0.f), Vec2f(w, 0.f), Vec2f(w, h), Vec2f(0.f, h))));
   btn->setTranslation(0.1f, 0.15f);

   btn->setOnPressHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &Game::buttonPressHandler));
   btn->setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)>(this, &Game::buttonReleaseHandler));

   btn->addToWorld();

   m_entities.push_back(btn);
}

void Game::init() {
   gInitialise();

   m_win.init("Shit Game", 640, 480, false);
   m_win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Game::quit));
   m_win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Game::keyDown));
   m_win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Game::keyUp));

   Functor<void, TYPELIST_0()> fMakeContext(&m_win, &WinIO::createGLContext);
   Functor<void, TYPELIST_0()> fSwap(&m_win, &WinIO::swapBuffers);

   m_renderer.start(fMakeContext, fSwap);

   pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
   m_renderer.attachCamera(camera);

   m_worldSpace.init(unique_ptr<Quadtree<pEntity_t> >(new Quadtree<pEntity_t>(1, Range(0.f, 0.f, 64.f / 48.f, 1.f))));

   playerSetup();
   uiSetup();

   pTexture_t texFont1(new Texture("data/textures/font2.png"));
   m_font1 = pFont_t(new Dodge::Font(texFont1, 0, 0, 852, 792, 71, 98));
}

void Game::launch() {
   init();

   while (1) {
      LOOP_START;

      m_win.doEvents();
      keyboard();
      computeFrameRate();

      m_worldSpace.dbg_draw(Colour(1.f, 0.f, 0.f, 1.f), 1, 5.f);

      for (uint_t i = 0; i < m_entities.size(); ++i) {
         m_entities[i]->update();
         m_entities[i]->draw();
      }

      m_player->update();
      m_player->draw();

//      stringstream strFr;
//      strFr << "Frame Rate: " << m_frameRate << "fps";
//      m_graphics2d.setFillColour(Colour(1.f, 0.f, 0.f, 1.f));
//      m_graphics2d.drawText(*m_font1, Vec2f(0.03f, 0.05f), strFr.str(), 0.03f, 0.9f, 5);

      m_eventManager.doEvents();

      m_renderer.tick(Colour(0.5, 0.6, 0.8, 1.0));

      LOOP_END;
   }
}
