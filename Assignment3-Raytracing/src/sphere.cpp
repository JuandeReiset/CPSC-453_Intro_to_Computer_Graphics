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

#include "sphere.hpp"

namespace geometry {
Sphere::Sphere(math::Vec3f origin, float radius, math::Vec3f color,float refect)
    : origin(origin), radius(radius), color(color), refect(refect) {}
}
