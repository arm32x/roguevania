#include "Camera.hpp"

using namespace Roguevania;
using namespace Roguevania::Environment;
using namespace sf;

Camera::Camera(float x, float y)
    : view(Vector2f(x, y), Vector2f(640, 360)), previousFocalPoint(x, y) {
    
}

Camera::Camera(const Vector2f& center)
    : Camera(center.x, center.y) {
    
}

Camera::Camera()
    : Camera(0, 0) {
    
}

void Camera::update(float delta, Vector2f focalPoint) {
    view.setCenter(view.getCenter() + (focalPoint - view.getCenter()) * 1.0f / 64.0f);
    
    previousFocalPoint = focalPoint;
}
