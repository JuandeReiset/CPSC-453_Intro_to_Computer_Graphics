#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include "vec2f.hpp"
#include "image.hpp"
#include "vec2i.hpp"
#include "grid2.hpp"

using namespace math;
using namespace geometry;
using namespace raster;

// function declaration
using namespace std;

namespace geometry {
class Ray {
public:
  Ray() = default;
  Ray(math::Vec3f origin, math::Vec3f direction)
      : origin(origin), direction(direction) {}

  math::Vec3f origin;
  math::Vec3f direction;
};

class Sphere {
public:
  Sphere() = default;
  Sphere(math::Vec3f origin, float radius)
      : m_origin(origin), m_radius(radius) {}

  float radius() const { return m_radius; }
  math::Vec3f origin() const { return m_origin; }

  float &radius() { return m_radius; }
  math::Vec3f &origin() { return m_origin; }

private:
  math::Vec3f m_origin;
  float m_radius = 1.f;
};

struct Hit {
  using False = Hit;

  explicit operator bool() const { return didIntersect; }
  bool didIntersect = false;
  float rayDepth = std::numeric_limits<float>::max();
};

Hit nearestIntersection(Ray const &ray, Sphere const &sphere) { return Hit(); }

using Screen = geometry::Grid2<raster::RGB>; //renames Grid2D to Screen for easier use

struct ImagePlane {
  Screen screen;
  math::Vec3f center;
  math::Vec3f uDir; 
  math::Vec3f vDir;
  //u and v are two vectors that are orthogonal to each other and will define the plane
  float left;   // right symmetric
  float bottom; // top symmetric

  math::Vec3f coordinate(math::Vec2f pixel) const {
    using std::abs;
    float u = left + (2.f * abs(left)) * (pixel.x + 0.5f) / screen.width();  //l + (r-l)(x+0.5)/nx
    float v = bottom + (2.f * abs(bottom)) * (pixel.y + 0.5f) / screen.height(); //b + (t-b)(y+0.5)/ny

    return center + uDir * u + vDir * v;
  }
};

Hit intersect(Sphere const &s, Ray const &r) {
  Vec2f o(s.origin().x, s.origin().y);
  Vec2f p(r.origin.x, r.origin.y);

  if (distance(o, p) > s.radius())
    return Hit::False();

  Hit hit;

  hit.didIntersect = true;

  // Exercise for tutorial, calculate depth from any vantage point
  hit.rayDepth = -s.origin().z;

  return hit;
}

} // namespace geometry

struct Base {
  virtual ~Base() = default;
  virtual Hit intersectSelf(Ray ray) const = 0; // pure virtual
  virtual Vec3f colour() const = 0;             // pure virtual
};

struct RedSphere : public Base {
  RedSphere(Sphere sphere) : m_sphere(sphere) {}

  RedSphere(math::Vec3f origin, float radius) : m_sphere(origin, radius) {}

  Hit intersectSelf(Ray r) const override { return ::intersect(m_sphere, r); }

  Vec3f colour() const { return {1.f, 0.f, 0.f}; }

  Sphere m_sphere;
};

struct BlueSphere : public Base {
  BlueSphere(Sphere sphere) : m_sphere(sphere) {}

  BlueSphere(math::Vec3f origin, float radius) : m_sphere(origin, radius) {}

  Hit intersectSelf(Ray r) const override { return intersect(m_sphere, r); }

  Vec3f colour() const { return {0.f, 0.f, 1.f}; };

  Sphere m_sphere;
};

int main() {

  // sphere

  using Pointer = std::unique_ptr<Base>;

  std::vector<Pointer> objects;
  objects.push_back(Pointer(new RedSphere({0.f, 0.f, 0.f}, 1.f)));
  objects.push_back(Pointer(new BlueSphere({1.f, 1.f, -0.5f}, 1.f)));
  objects.push_back(Pointer(new RedSphere({1.f, 0.f, -1.f}, 2.f)));
  objects.push_back(Pointer(new BlueSphere({-2.f, 0.f, -2.f}, 5.f)));

  ImagePlane imagePlane;
  imagePlane.screen = Screen(1000, 1000);
  imagePlane.center = {0.f, 0.f, 0.f};
  imagePlane.uDir = {1.f, 0.f, 0.f}; // normalized
  imagePlane.vDir = {0.f, 1.f, 0.f}; // normalized
  imagePlane.left = -10.f;
  imagePlane.bottom = -10.f;

  for (int32_t y = 0; y < imagePlane.screen.height(); ++y) {
    for (int32_t x = 0; x < imagePlane.screen.width(); ++x) {
      math::Vec2f pixel(x, y);
      Vec3f colorOut;

      auto origin = imagePlane.coordinate(pixel);

      auto direction = math::Vec3f(0.f, 0.f, -1.f);
      Ray ray(origin, direction);

      Hit previousHit;
      for (auto const &object : objects) {    ///abstract class for handling different objects
        auto hit = object->intersectSelf(ray); //boolean, determine if the object is being overview
        if (hit && hit.rayDepth < previousHit.rayDepth) {   //Is the object closer than the last object read
          previousHit = hit;
          colorOut = object->colour();
        }
      }

      imagePlane.screen({x, y}) = raster::convertToRGB(colorOut);
    }
  }

  raster::write_screen_to_file("./test.png", imagePlane.screen);

  return EXIT_SUCCESS;
}
