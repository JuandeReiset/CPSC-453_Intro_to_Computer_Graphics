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

#include <limits>
#include <cmath>

#include "ray.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "triangle.hpp"

namespace geometry {
struct Hit {
  // just so we can use it as a bool in if statements, syntactic-sugar
  explicit operator bool() const;

  bool didIntersect = false;
  float rayDepth = std::numeric_limits<float>::max();
  math::Vec3f color = math::Vec3f(0.f,0.3f,0.f);
};

Hit intersect(Ray const &r, Sphere const &sphere);

Hit intersect(Ray const &ray, Triangle const &triangle);

Hit intersect(Ray const &r, Plane const &p);

} // namespace geometry
