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

#include "ray.hpp"

namespace geometry {

Ray::Ray(math::Vec3f origin, math::Vec3f direction)
    : origin(origin), direction(direction) {}

math::Vec3f pointOnLne(math::Vec3f const &origin, math::Vec3f const &direction,
                       float t) {
  return origin + direction * t;
}

math::Vec3f evaluate(Ray const &ray, float t) {
  return pointOnLne(ray.origin, ray.direction, t);
}

} // namespace geometry