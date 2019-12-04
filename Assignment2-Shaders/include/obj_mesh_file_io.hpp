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

#include <string>
#include <array>
#include <vector>

#include "obj_mesh.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"

namespace geometry {

bool loadOBJMeshFromFile(std::string const &filePath, geometry::OBJMesh &mesh);

} // namespace geometry
