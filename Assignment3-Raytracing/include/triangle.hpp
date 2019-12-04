/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief
 */

#pragma once

#include "vec3f.hpp"

namespace geometry {

template <typename T> class Triangle_ {
private:
  enum Index : unsigned char { A = 0, B, C, COUNT };

public:
  Triangle_() = default;
  Triangle_(T a, T b, T c);
  //Triangle_(T a, T b, T c, math::Vec3f color, float refect);

  T const &a() const;
  T const &b() const;
  T const &c() const;
  //math::Vec3f const &color() const;
  //float const &refect() const;

  T const &operator[](int index) const;

  T &a();
  T &b();
  T &c();
  math::Vec3f color=math::Vec3f(1.0,0,0);
  float reflection=0;



  T &operator[](int index);

private:
  T m_vertices[Index::COUNT];
 // T m_color;
  //float m_refect;
};

using Triangle = Triangle_<math::Vec3f>;
using IndexTriangle = Triangle_<unsigned int>;

Triangle scale(Triangle t, float s);

Triangle translate(Triangle t, math::Vec3f const &v);

math::Vec3f normal(Triangle const &t);

} // namespace geometry

#include "triangle.tpp"
