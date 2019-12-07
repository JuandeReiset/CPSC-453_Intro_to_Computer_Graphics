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

#include "obj_mesh_file_io.hpp"

#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <fstream>


namespace geometry {

bool loadOBJMeshFromFile(std::string const &filePath, OBJMesh &meshOut) {

  std::ifstream in(filePath.c_str());

  if (!in) {
    std::cerr << "[Error] could not open OBJ file " << filePath << '\n';
    return false;
  }

  std::vector<IndicesTriangle> triangles;
  std::vector<math::Vec3f> vertices;
  std::vector<math::Vec2f> textureCoords;
  std::vector<math::Vec3f> normals;

  std::string s;

  while (getline(in, s)) {
    std::stringstream line(s);
    std::string token;

    line >> token; // get first

    if (token == "mtllib") {
      std::cerr << "[Log] Ignoring mtlib\n";
    } else if (token == "v") {
      math::Vec3f vec;
      line >> vec; // this calls the istream operator >> defined in vec3f.h/cpp

      vertices.push_back(vec);
    } else if (token == "vt") {
      math::Vec2f vec;
      line >> vec; // this calls the istream operator >> defined in vec3f.h/cpp

      textureCoords.push_back(vec);
    } else if (token == "vn") {
      math::Vec3f vec;
      line >> vec; // this calls the istream operator >> defined in vec3f.h/cpp

      normals.push_back(vec);
    } else if (token == "f") {

      std::vector<Indices> tmpIndices;
      tmpIndices.reserve(4);

      while (line >> s) {
        std::stringstream item(s);
        Indices tmpIndex;

        if (getline(item, s, '/')) {
          std::stringstream index(s);
          index >> tmpIndex.vertexID();
          tmpIndex.vertexID() -= 1;
        }

        if (getline(item, s, '/')) {
          std::stringstream index(s);
          index >> tmpIndex.textureCoordID();
          tmpIndex.textureCoordID() -= 1;
        }

        if (getline(item, s, '/')) {
          std::stringstream index(s);
          index >> tmpIndex.normalID();
          tmpIndex.normalID() -= 1;
        }

        tmpIndices.push_back(tmpIndex);
      }

      if (tmpIndices.size() == 3) {
        triangles.push_back({tmpIndices[0], // a
                             tmpIndices[1], // b
                             tmpIndices[2]} // c
                            );

      } else {
        std::cerr << "[Error] not triangulated mesh: " << filePath << '\n';
        return false;
      }
    }
  }

  meshOut = {triangles, vertices, textureCoords, normals};
  return true;
}

std::string Convert(float number) {
	std::ostringstream buff;
	buff << number;
	return buff.str();
}

void write2OBJ(geometry::OBJMesh &mesh) {
	//std::string input = "Creating object \n";
	std::string input = "";

	std::ofstream out("./pottery.obj");

	for (auto v : mesh.vertices) {
		out << "v " << (v.x) << " "<< (v.y) << " " << (v.z) << "\n";
	}

	//input =input + "\n \n";

	for (auto p : mesh.textureCoords) {
		out << "vt " << p.x << " " << p.y << "\n";
		/*input = input + "vt " + Convert(p.x) + " ";
		input = input + Convert(p.y) + " ";
		input = input + "[0]" + "\n";
		*/
	}

	//input = input + "\n \n";

	for (auto v : mesh.normals) {
		out << "vn " << (v.x) << " " << (v.y) << " " << (v.z) << "\n";
		/*
		input = input + "vn " + Convert(v.x) + " ";
		input = input + Convert(v.y) + " ";
		input = input + Convert(v.z) + " "+ "\n";
	   */
	}

	//input = input + "\n \n";
	for (auto f : mesh.triangles) {

		out << "f " << f[0][0] + 1 << " " << f[1][0] + 1 << " " << f[2][0] + 1 << "\n";
		
	}


	out.close();
	
}

} // namespace geometry
