#pragma once

#include <vector>
#include "cube.hpp"
#include "vec3f.hpp"

namespace fractal {

	struct MengerOutput {
		geometry::Cube a, b, c, d, e, f, g, h,i,j,k,l,m,n,o,p,q,r,s,t;
	};

	MengerOutput MengerStep(geometry::Cube c);

	std::vector<geometry::Cube> MengerFractal(geometry::Cube base,
		int depth);
} // namespace fractal
