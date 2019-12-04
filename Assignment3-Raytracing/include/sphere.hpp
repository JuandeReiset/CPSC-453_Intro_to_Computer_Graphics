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

class Sphere {
public:
  Sphere() = default;
  Sphere(math::Vec3f origin, float radius, math::Vec3f color, float refect);

  math::Vec3f origin;
  math::Vec3f color = math::Vec3f(0.0f,0.3f,0.0f);
  float radius = 1.f;
  float refect = 0;
};

} // namespace geometry
