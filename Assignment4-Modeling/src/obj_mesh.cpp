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

#include "obj_mesh.hpp"

#include <algorithm>
#include <cassert>

using namespace math;

namespace geometry {

std::vector<math::Vec3f>
calculateTriangleNormals(std::vector<IndicesTriangle> const &indexTriangles,
                         std::vector<math::Vec3f> const &vertices) {
  std::vector<Vec3f> tmp;
  for (int i = 0; i < indexTriangles.size(); i++) {
	  Vec3f a = vertices[indexTriangles[i][0][0]];
	  Vec3f b = vertices[indexTriangles[i][1][0]];
	  Vec3f c = vertices[indexTriangles[i][2][0]];

	  Vec3f ba = b - a;
	  Vec3f ca = c - a;

	  Vec3f n = (ba) ^ (ca);

	  tmp.push_back(normalized(n));
  }
  return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices) {
	std::vector<math::Vec3f> face_normals = calculateTriangleNormals(indexTriangles, vertices);


	std::vector<Vec3f> tmp;
	for (int i = 0; i < vertices.size(); i++) {
		tmp.push_back({ 0,0,0 });
	}

	for (int i = 0; i < indexTriangles.size(); i++) {
		Vec3f n = face_normals[i];

		tmp[indexTriangles[i][0][0]] += n;
		tmp[indexTriangles[i][1][0]] += n;
		tmp[indexTriangles[i][2][0]] += n;


	}
	for (int i = 0; i < tmp.size(); i++) {
		tmp[i] = normalized(tmp[i]);

	}

	return tmp;
}

std::vector<math::Vec3f>
calculateVertexNormals(std::vector<IndicesTriangle> const &indexTriangles,
                       std::vector<math::Vec3f> const &vertices,
                       std::vector<math::Vec3f> const &triangleNormals) {
	std::vector<Vec3f> tmp;
	//TODO
	return tmp;
}

} // namespace geometry
