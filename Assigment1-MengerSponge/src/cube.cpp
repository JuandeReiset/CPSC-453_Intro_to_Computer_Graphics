#include "cube.hpp"

using namespace math;

namespace geometry {

Cube::Cube(Vec3f a, Vec3f b, //
           Vec3f c, Vec3f d, //
           Vec3f e, Vec3f f, //
           Vec3f g, Vec3f h)
    : m_vertices({{a, b, c, d, e, f, g, h}}) {}

Vec3f const &Cube::a() const { return m_vertices[A]; }
Vec3f const &Cube::b() const { return m_vertices[B]; }
Vec3f const &Cube::c() const { return m_vertices[C]; }
Vec3f const &Cube::d() const { return m_vertices[D]; }
Vec3f const &Cube::e() const { return m_vertices[E]; }
Vec3f const &Cube::f() const { return m_vertices[F]; }
Vec3f const &Cube::g() const { return m_vertices[G]; }
Vec3f const &Cube::h() const { return m_vertices[H]; }

Vec3f const &Cube::operator[](int index) const { return m_vertices[index]; }

Vec3f &Cube::a() { return m_vertices[A]; }
Vec3f &Cube::b() { return m_vertices[B]; }
Vec3f &Cube::c() { return m_vertices[C]; }
Vec3f &Cube::d() { return m_vertices[D]; }
Vec3f &Cube::e() { return m_vertices[E]; }
Vec3f &Cube::f() { return m_vertices[F]; }
Vec3f &Cube::g() { return m_vertices[G]; }
Vec3f &Cube::h() { return m_vertices[H]; }

Vec3f &Cube::operator[](int index) { return m_vertices[index]; }

Cube translate(Cube cube, Vec3f v) {
  for (int idx = 0; idx < Cube::Vertex::COUNT; ++idx) {
    cube[idx] += v;
  }

  return cube;
}

Cube scale(Cube cube, float s) {
  for (int idx = 0; idx < Cube::Vertex::COUNT; ++idx) {
    cube[idx] *= s;
  }

  return cube;
}

} // namespace geometry
