#pragma once

#include <array>

#include "vec3f.hpp"

namespace geometry {

//    g-----h
//   /|    /|
//  c-----d |
//  | e---|-f
//  |/    |/
//  a-----b
//
// array layout [a, b, c, d, e, f, g, h]

class Cube {
public:
  enum Vertex { A = 0, B, C, D, E, F, G, H, COUNT };
  using vertices = std::array<math::Vec3f, COUNT>;

public:
  Cube() = default;
  Cube(math::Vec3f a, math::Vec3f b, //
       math::Vec3f c, math::Vec3f d, //
       math::Vec3f e, math::Vec3f f, //
       math::Vec3f g, math::Vec3f h);

  math::Vec3f const &a() const;
  math::Vec3f const &b() const;
  math::Vec3f const &c() const;
  math::Vec3f const &d() const;
  math::Vec3f const &e() const;
  math::Vec3f const &f() const;
  math::Vec3f const &g() const;
  math::Vec3f const &h() const;

  math::Vec3f const &operator[](int index) const;

  math::Vec3f &a();
  math::Vec3f &b();
  math::Vec3f &c();
  math::Vec3f &d();
  math::Vec3f &e();
  math::Vec3f &f();
  math::Vec3f &g();
  math::Vec3f &h();

  math::Vec3f &operator[](int index);

private:
  vertices m_vertices;
};

Cube translate(Cube cube, math::Vec3f v);
Cube scale(Cube cube, float s);

} // namespace geometry
