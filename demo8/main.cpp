#include <vector>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;
using namespace cml;


typedef boost::shared_ptr<PlainTexturedAlphaModel> pModel_t;

WinIO win;
Renderer& renderer = Renderer::getInstance();
pTexture_t tex0;
pTexture_t tex1;
std::vector<pModel_t> models;
double frameRate;


void quit() {
   renderer.stop();
   win.destroyWindow();
   exit(0);
}

void keyDown(int key) {
   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
      case WinIO::KEY_F:
         cout << "Frame rate (main loop): " << frameRate << "fps\n";
#ifdef DEBUG
         cout << "Frame rate (render loop): " << renderer.getFrameRate() << "fps\n" << flush;
#endif
      break;
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

void drawModels() {
   for (uint_t i = 0; i < models.size(); ++i)
      renderer.draw(models[i].get());
}

void constructModels() {
   float32_t w = 0.1;
   float32_t h = 0.1;

   bool b = false;
   for (float32_t x = 0.f; x < 1.2f; x += w) {
      for (float32_t y = 0.f; y < 0.9f; y += h) {
         b = !b;

         vvvtt_t verts[] = {
            vvvtt_t(x + w,   y,       1.0,    1.0, 0.0),
            vvvtt_t(x + w,   y + h,   1.0,    1.0, 1.0),
            vvvtt_t(x    ,   y,       1.0,    0.0, 0.0),
            vvvtt_t(x + w,   y + h,   1.0,    1.0, 1.0),
            vvvtt_t(x    ,   y + h,   1.0,    0.0, 1.0),
            vvvtt_t(x    ,   y,       1.0,    0.0, 0.0)
         };

         Colour col = b ? Colour(1.0, 0.0, 0.0, 1.0) : Colour(0.0, 0.0, 1.0, 1.0);

         pModel_t model(new PlainTexturedAlphaModel(Renderer::TRIANGLES));
         models.push_back(model);

         model->setVertices(0, verts, 6);
         model->setTextureHandle(b ? tex0->getHandle() : tex1->getHandle());
         model->setColour(col);

//         renderer.bufferModel(model.get());
      }
   }
}

int main() {
   try {
#ifdef DEBUG
      WinIO::dbg_flags |= WinIO::DBG_NO_VSYNC;
#endif
      gInitialise();
      win.init("Demo8 - renderer", 640, 480, false);
      win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
      win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
      win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

      renderer.start();

      pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
      renderer.attachCamera(camera);

      tex0 = pTexture_t(new Texture("0.png"));
      tex1 = pTexture_t(new Texture("1.png"));

      constructModels();

      Range range(0.5, 0.1, 0.4, 0.2);

      while (1) {
         win.doEvents();
         computeFrameRate();
         drawModels();
#ifdef DEBUG
         range.dbg_draw(Colour(0.f, 1.f, 0.f, 0.3f), Colour(0.f, 0.f, 0.f, 1.f), 2, 2);
#endif
         renderer.tick(Colour(1.0, 1.0, 0.0, 1.0));
      }
   }
   catch (Exception& e) {
      e.prepend("An error occurred; ");
      cerr << e.what() << "\n" << flush;

      quit();
   }

   return 0;
}
