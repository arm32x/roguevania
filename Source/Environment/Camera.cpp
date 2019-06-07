#include "Camera.hpp"

#include <cmath>

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

void Camera::update(float delta, const Vector2f& focalPoint) {
    view.setCenter(view.getCenter() + Vector2f((focalPoint.x - view.getCenter().x) * 1.0f / 16.0f, 0.0f));
    view.setCenter(view.getCenter() + Vector2f(0.0f, (focalPoint.y - view.getCenter().y) * 1.0f / 8.0f));
    
    previousFocalPoint = focalPoint;
}

Vector2f Camera::alignCenter() {
    Vector2f oldCenter = view.getCenter();
    view.setCenter(std::round(view.getCenter().x), std::round(view.getCenter().y));
    return oldCenter;
}
