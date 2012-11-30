#include <vector>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;
using namespace cml;


typedef Renderer::Model Model;
typedef boost::shared_ptr<Model> pModel_t;

WinIO win;
Renderer renderer;
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
         cout << "Frame rate (render loop): " << renderer.getFrameRate() << "fps\n" << flush;
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
   Renderer renderer;
   renderer.onWindowResize(w, h);
   renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

void constructModels() {
   float32_t w = 0.1;
   float32_t h = 0.1;

   matrix44f_c I;
   identity_transform(I);

   bool b = false;
   for (float32_t x = 0.f; x < 1.2f; x += w) {
      for (float32_t y = 0.f; y < 0.9f; y += h) {
         b = !b;

         Renderer::vvvtt_t verts[] = {
            {x + w,   y,       1.0,    1.0, 0.0},
            {x + w,   y + h,   1.0,    1.0, 1.0},
            {x    ,   y,       1.0,    0.0, 0.0},
            {x + w,   y + h,   1.0,    1.0, 1.0},
            {x    ,   y + h,   1.0,    0.0, 1.0},
            {x    ,   y,       1.0,    0.0, 0.0}
         };

         Colour col = b ? Colour(1.0, 0.0, 0.0, 1.0) : Colour(0.0, 0.0, 1.0, 1.0);

         pModel_t model(new Model(Renderer::TEXTURED_ALPHA, false));
         models.push_back(model);

         model->setVerts(Renderer::TRIANGLES, verts, 6, sizeof(Renderer::vvvtt_t));
         model->texHandle = b ? tex0->getHandle() : tex1->getHandle();
         model->colour = col;
         model->setMatrix(I.data());

         renderer.bufferModel(model.get());
         renderer.stageModel(model.get());
      }
   }
}

int main() {
   try {
      win.init("Demo8 - renderer", 640, 480, false);
      win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
      win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
      win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

      renderer.start();

      pCamera_t camera(new Camera(640.0 / 480.0, 1.f));
      renderer.attachCamera(camera);

      renderer.setBgColour(Colour(1.0, 1.0, 0.0, 1.0));

      tex0 = pTexture_t(new Texture("0.png"));
      tex1 = pTexture_t(new Texture("1.png"));

      constructModels();

      while (1) {
         win.doEvents();
         computeFrameRate();
         renderer.checkForErrors();
      }
   }
   catch (Exception& e) {
      e.prepend("An error occurred; ");
      cerr << e.what() << flush;

      quit();
   }

   return 0;
}
