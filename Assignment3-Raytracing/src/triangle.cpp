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

#include "triangle.hpp"

namespace geometry {

Triangle scale(Triangle t, float s) {
  t.a() *= s;
  t.b() *= s;
  t.c() *= s;

  return t;
}

Triangle translate(Triangle t, math::Vec3f const &v) {
  t.a() += v;
  t.b() += v;
  t.c() += v;

  return t;
}

math::Vec3f normal(Triangle const &t) {
  return normalized(math::cross(t.b() - t.a(), t.c() - t.a()));
}

} // namespace
