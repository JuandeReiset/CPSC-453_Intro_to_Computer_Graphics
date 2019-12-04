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


#include "plane.hpp"

namespace geometry {
Plane::Plane(math::Vec3f normal, math::Vec3f origin, math::Vec3f color, float refect)
    : normal(normal), origin(origin), color(color), refect(refect) {}
}
