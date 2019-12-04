#include "menger.hpp"


// don't put 'using namespace' in .cpp files
// exceptions (in main.cpp or function definitions)
using namespace geometry;
using namespace math;

namespace fractal {

	MengerOutput MengerStep(Cube cu) {
		MengerOutput cubes;

		Vec3f origin = cu.a();
		Vec3f ab = cu.b() - origin;
		Vec3f ac = cu.c() - origin;
		Vec3f ae = cu.e() - origin;

		cu = translate(cu, -origin);
		cu = scale(cu, 1.f / 3.f); //play with values

		//bottom layer
		cubes.a = translate(cu, origin);
		cubes.b = translate(cu, origin + ab * 0.33f);
		cubes.c = translate(cu, origin + ab * 0.66f);
		cubes.d = translate(cu, origin + ae * 0.33f);
		cubes.e = translate(cu, origin + ab * 0.66f + ae * 0.33f);
		cubes.f = translate(cu, origin + ae * 0.66f);
		cubes.g = translate(cu, origin + ae * 0.66f + ab * 0.33f);
		cubes.h = translate(cu, origin + ab * 0.66f + ae * 0.66f);

		//middle layer
		cubes.i = translate(cu, origin + ac * 0.33f);
		cubes.j = translate(cu, origin + ab * 0.66f + ac * 0.33f);
		cubes.k = translate(cu, origin + ae * 0.66f + ac * 0.33f);
		cubes.l = translate(cu, origin + ae * 0.66f + ab * 0.66f + ac * 0.33f);

		//upper layer
		cubes.m = translate(cu, origin + ac * 0.66f);
		cubes.n = translate(cu, origin + ab * 0.33f + ac * 0.66f);
		cubes.o = translate(cu, origin + ab * 0.66f + ac * 0.66f);
		cubes.p = translate(cu, origin + ae * 0.33f + ac * 0.66f);
		cubes.q = translate(cu, origin + ab * 0.66f + ae * 0.33f + ac * 0.66f);
		cubes.r = translate(cu, origin + ae * 0.66f + ac * 0.66f);
		cubes.s = translate(cu, origin + ae * 0.66f + ab * 0.33f + ac * 0.66f);
		cubes.t = translate(cu, origin + ae * 0.66f + ab * 0.66f + ac * 0.66f);

		
		return cubes;
	}

	std::vector<Cube> MengerFractal(Cube base, int depth) {
	std::vector<Cube> cubes, tmp;
	cubes.push_back(base);

	for (int i = 0; i < depth; ++i) {
		for (auto const &cu : cubes) {
			auto newCubes = MengerStep(cu);
			tmp.push_back(newCubes.a);
			tmp.push_back(newCubes.b);
			tmp.push_back(newCubes.c);
			tmp.push_back(newCubes.d);
			tmp.push_back(newCubes.e);
			tmp.push_back(newCubes.f);
			tmp.push_back(newCubes.g);
			tmp.push_back(newCubes.h);
			tmp.push_back(newCubes.i);
			tmp.push_back(newCubes.j);
			tmp.push_back(newCubes.k);
			tmp.push_back(newCubes.l);
			tmp.push_back(newCubes.m);
			tmp.push_back(newCubes.n);
			tmp.push_back(newCubes.o);
			tmp.push_back(newCubes.p);
			tmp.push_back(newCubes.q);
			tmp.push_back(newCubes.r);
			tmp.push_back(newCubes.s);
			tmp.push_back(newCubes.t);
			
		}

		cubes.swap(tmp);
		tmp.clear();
	}

		return cubes;
	}

} // namespace fractal
