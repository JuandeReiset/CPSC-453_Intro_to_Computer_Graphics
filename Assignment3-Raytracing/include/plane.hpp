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

class Plane {
public:
  Plane() = default;
  Plane(math::Vec3f normal, math::Vec3f origin, math::Vec3f color,float refect);

  math::Vec3f normal = {0.f, 1.f, 0.f};
  math::Vec3f origin = {0.f, 0.f, 0.f};
  math::Vec3f color = {0.f, 0.5f, 0.0f};
  float refect = 0;
};

} // namespace geometry
