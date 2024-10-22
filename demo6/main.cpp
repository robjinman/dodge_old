#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;


WinIO win;
Renderer& renderer = Renderer::getInstance();
double frameRate = 1;
Polygon shape;
pFont_t font1;
pTextEntity_t txtFpsLbl, txtMainFrLbl, txtRenderFrLbl, txtMainFr, txtRenderFr;


void quit() {
   renderer.stop();
   win.destroyWindow();
   exit(0);
}

void keyDown(int key) {
   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
   }
}

void computeFrameRate() {
   static Timer timer;
   static long i = 0;
   i++;

   if (i % 1000 == 0) {
      frameRate = 1000.0 / timer.getTime();
      timer.reset();
   }
}

void onWindowResize(int w, int h) {
   renderer.onWindowResize(w, h);
   renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

void drawFrameRates() {
   stringstream fr;
   fr << static_cast<long>(frameRate);
   txtMainFr->setText(fr.str());

   fr.str("");
   fr << static_cast<long>(renderer.getFrameRate());
   txtRenderFr->setText(fr.str());
}

int main() {
   try {
      WinIO::dbg_flags |= WinIO::DBG_NO_VSYNC;
      win.init("Rotating Square", 240, 240, false);
      gInitialise();

      win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
      win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
      win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

      pCamera_t camera(new Camera(240.0 / 240.0, 1.f));
      renderer.attachCamera(camera);

      renderer.start();

      shape.addVertex(0.3, 0.3);
      shape.addVertex(0.7, 0.3);
      shape.addVertex(0.7, 0.7);
      shape.addVertex(0.3, 0.7);

      shape.setLineWidth(3);
      shape.setFillColour(Colour(0.0, 0.2, 0.9, 1.0));
      shape.setLineColour(Colour(0.7, 0.1, 0.1, 1.0));

      shape.setRenderTransform(0.f, -0.1f, 1);

      pTexture_t tex(new Texture("font1.png"));
      font1 = pFont_t(new Dodge::Font(tex, 0, 64, 192, 192, 16, 24));

      Colour col(1, 1, 0, 1);

      txtFpsLbl = pTextEntity_t(new TextEntity(internString("lblFps"), internString("txtLabel"), font1, "fps", Vec2f(0.05, 0.05)));
      txtMainFrLbl = pTextEntity_t(new TextEntity(internString("lblMainLoopFR"), internString("txtLabel"), font1, "Main:", Vec2f(0.05, 0.05)));
      txtRenderFrLbl = pTextEntity_t(new TextEntity(internString("lblRenderLoopFR"), internString("txtLabel"), font1, "Renderer:", Vec2f(0.05, 0.05)));
      txtMainFr = pTextEntity_t(new TextEntity(internString("mainLoopFR"), internString("txtFR"), font1, "0", Vec2f(0.05, 0.05)));
      txtRenderFr = pTextEntity_t(new TextEntity(internString("renderLoopFR"), internString("txtFR"), font1, "0", Vec2f(0.05, 0.05)));

      unique_ptr<Quad> quad(new Quad(Vec2f(0.9f, 0.3f)));
      pEntity_t box(new Entity(internString("frameRates"), internString("box")));
      box->setShape(move(quad));
      box->setFillColour(Colour(0.f, 0.f, 0.f, 0.5f));
      box->setZ(2);

      box->addChild(txtFpsLbl);
      box->addChild(txtMainFrLbl);
      box->addChild(txtMainFr);
      box->addChild(txtRenderFrLbl);
      box->addChild(txtRenderFr);

      txtFpsLbl->setTranslation(0.05f, 0.2f);
      txtFpsLbl->setFillColour(col);
      txtFpsLbl->setZ(3);

      txtMainFrLbl->setTranslation(0.05f, 0.1f);
      txtMainFrLbl->setFillColour(col);
      txtMainFrLbl->setZ(3);

      txtRenderFrLbl->setTranslation(0.05f, 0.04f);
      txtRenderFrLbl->setFillColour(col);
      txtRenderFrLbl->setZ(3);

      txtMainFr->setTranslation(0.55f, 0.1f);
      txtMainFr->setFillColour(col);
      txtMainFr->setZ(3);

      txtRenderFr->setTranslation(0.55f, 0.04f);
      txtRenderFr->setFillColour(col);
      txtRenderFr->setZ(3);

      box->setTranslation(0.05f, 0.65f);

      Timer timer;
      while (1) {
         win.doEvents();
         computeFrameRate();

         shape.rotate((180.0) / frameRate, Vec2f(0.5f, 0.5f));

         if (timer.getTime() > 0.5) {
            drawFrameRates();
            timer.reset();
         }

         shape.draw();
         box->draw();
         txtFpsLbl->draw();
         txtMainFrLbl->draw();
         txtRenderFrLbl->draw();
         txtMainFr->draw();
         txtRenderFr->draw();

         renderer.tick(Colour(0.2, 0.9, 0.2, 1.0));
      }
   }
   catch (Exception& e) {
      e.prepend("An error occurred; ");
      cerr << e.what() << "\n" << flush;

      quit();
   }

   return 0;
}
