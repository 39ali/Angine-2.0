#pragma once
#include "../math/CommonMath.h"
#include "../math/Quaternion.h"
#include "../math/Vector2f.h"
#include "../math/Vector3f.h"
#include "../renderer/Window.h"
namespace Angine {
using namespace Renderer;
class CameraFP {
 public:
  CameraFP(Window& _win, const vec3f& _pos, const vec3f& _target,
           const vec3f& _up)
      : pos(_pos), target(_target.normalize()), up(_up.normalize()), win(_win) {
    mousePos.x = win.getWidth() / 2.0f;
    mousePos.y = win.getHeight() / 2.f;
    vec3f t(target.x, 0.00, target.z);
    t = t.normalize();

    if (t.z >= 0.0f) {
      if (t.x >= 0.0f)
        angleH = 360.0f - toDegrees(asin(t.z));
      else
        angleH = 180.0f + toDegrees(asin(t.z));
    } else {
      if (target.x >= 0.0f)
        angleH = toDegrees(asin(-target.z));
      else
        angleH = 180.0f - toDegrees(asin(-target.z));
    }

    angleV = -toDegrees(asin(target.y));
  };
  vec3f& getPos() { return pos; }
  vec3f& getTarget() { return target; }
  vec3f& getUp() { return up; }

  void update() {


	  float mx, my;
	  win.getMousePos(mx, my);
	  const double deltax = mx - mousePos.x;
	  const double deltay = my - mousePos.y;

	  mousePos.x = mx;
	  mousePos.y = my;

	  angleH += (float)deltax / 20.0f;
	  angleV += (float)deltay / 20.f;


	  vec3f view(1.0f, 0.0f, 0.0f);
	  //angleH = 312.399994f;
	  //angleV = -25.8500004;
	  view = Quaternion::rotate(angleH, vec3f(0.0f, 1.0f, 0.0f), view).normalize();

	  vec3f haxis = vec3f(0.0f, 1.0f, 0.0f).cross(view).normalize();

	  view = Quaternion::rotate(angleV, haxis, view);

	  target = view.normalize();

	  up = target.cross(haxis).normalize();


    if (win.isKeyPressed(GLFW_KEY_W)) {
      pos += (target * stepSize);
    }
    if (win.isKeyPressed(GLFW_KEY_S)) {
      pos -= (target * stepSize);
    }
    if (win.isKeyPressed(GLFW_KEY_A)) {
      const vec3f left = target.cross(up).normalize() * stepSize;
      pos += left;
    }
    if (win.isKeyPressed(GLFW_KEY_D)) {
      const vec3f right = up.cross(target).normalize() * stepSize;
      pos += right;
    }

   
  }

 private:
  vec3f pos;
  vec3f target;
  vec3f up;
  Window& win;
  const float stepSize = 0.20f;
  vec2f mousePos;
  float angleH, angleV;
};

}  // namespace Angine
