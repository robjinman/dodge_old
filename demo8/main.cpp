#include <map>
#include <vector>
#include <dodge/dodge.hpp>
#include <iostream>


using namespace std;
using namespace Dodge;
using namespace cml;


WinIO win;
map<int, bool> keyState;
double frameRate;
Renderer renderer;
Graphics2d graphics2d;
std::vector<boost::shared_ptr<Renderer::Model> > models;

boost::shared_ptr<RenderBrush> brush1;
boost::shared_ptr<RenderBrush> brush2;
boost::shared_ptr<RenderBrush> brush3;

matrix44f_c I;


pTexture_t tex0;
pTexture_t tex1;


void quit() {
   exit(0);
}

void keyDown(int key) {
   switch (key) {
      case WinIO::KEY_ESCAPE: quit(); break;
      case WinIO::KEY_F: cout << "Frame rate: " << frameRate << "fps\n" << flush; break;
   }

   keyState[key] = true;
}

void keyUp(int key) { keyState[key] = false; }

void draw() {
   for (uint_t i = 0; i < models.size(); ++i) {
      renderer.attachBrush(i % 2 == 0 ? brush1 : brush3);

      renderer.stageModel(models[i]);
   }

   renderer.render();
}

void keyboard() {

}

void computeFrameRate() {
   static Timer timer;
   static long i = 0;
   i++;

   if (i % 10 == 0) {
      frameRate = 10.0 / timer.getTime();
      timer.reset();
   }
}

void onWindowResize(int w, int h) {
   Renderer renderer;
   renderer.onWindowResize(w, h);
   graphics2d.getCamera()->setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

void constructModels() {
   float32_t w = 0.02;
   float32_t h = 0.02;
//   float32_t w = 0.1;
//   float32_t h = 0.1;

   bool b = false;
   for (float32_t x = 0.f; x < 1.2f; x += w) {
      for (float32_t y = 0.f; y < 1.0f; y += h) {
         b = !b;

         Renderer::vvvtt_t verts[] = {
            {x + w,   y,       1.0,    1.0, 0.0/*,    1.0, 0.0, 0.0, 1.0*/},
            {x + w,   y + h,   1.0,    1.0, 1.0/*,    0.0, 1.0, 0.0, 1.0*/},
            {x    ,   y,       1.0,    0.0, 0.0/*,    0.0, 0.0, 1.0, 1.0*/},
            {x + w,   y + h,   1.0,    1.0, 1.0/*,    1.0, 0.0, 0.0, 1.0*/},
            {x    ,   y + h,   1.0,    0.0, 1.0/*,    0.0, 1.0, 0.0, 1.0*/},
            {x    ,   y,       1.0,    0.0, 0.0/*,    0.0, 0.0, 1.0, 1.0*/}
         };

         boost::shared_ptr<Renderer::Model> model(new Renderer::Model(Renderer::TEXTURED_ALPHA, false));

         model->primitiveType = Renderer::TRIANGLES;
         model->texHandle = b ? tex0->getHandle() : tex1->getHandle();
         model->verts = new Renderer::vvvtt_t[6];
         model->n = 6;

         memcpy(model->verts, verts, 6 * sizeof(Renderer::vvvtt_t));
         memcpy(model->matrix, I.data(), 16 * sizeof(Renderer::matrixElement_t));

         renderer.bufferModel(model);

         models.push_back(model);
      }
   }
}

int main() {
   win.init("Demo8 - renderer", 640, 480, false);
   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(keyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(keyUp));
   win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(onWindowResize));

   graphics2d.init(640, 480);

   identity_transform(I);

   tex0 = pTexture_t(new Texture("0.png"));
   tex1 = pTexture_t(new Texture("1.png"));

   brush1 = boost::shared_ptr<RenderBrush>(new RenderBrush(Colour(1.0, 0.0, 0.0, 1.0), Colour(0.0, 0.0, 0.0, 0.0), 0));
   brush2 = boost::shared_ptr<RenderBrush>(new RenderBrush(Colour(0.0, 0.0, 1.0, 1.0), Colour(0.0, 0.0, 0.0, 0.0), 0));
   brush3 = boost::shared_ptr<RenderBrush>(new RenderBrush(Colour(0.0, 1.0, 0.0, 1.0), Colour(0.0, 0.0, 0.0, 0.0), 0));

   constructModels();

   while (1) {
      renderer.attachBrush(brush2);
      renderer.clear();

      win.doEvents();
      keyboard();
      draw();
      computeFrameRate();
      win.swapBuffers();
   }

   return 0;
}
