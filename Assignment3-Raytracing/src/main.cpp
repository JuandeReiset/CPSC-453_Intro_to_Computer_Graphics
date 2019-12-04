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
 * @edited by : Juan Luis de Reiset, Computer science student at Ucalgary
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert> //assert
#include <random>
  
#include "mat3f.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "image.hpp"
#include "vec2i.hpp"
#include "grid2.hpp"
#include "timer.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "ray.hpp"
#include "ray_intersect.hpp"


// only do this kinda stuff in main.cpp or *.cpp; never *.h
using namespace math;
using namespace geometry;
using namespace std;

namespace raytracing {

Vec3f normalAt(Vec3f const &p, Sphere const &s) {
  Vec3f n ;
  n = (p-s.origin)*(1/s.radius);
  return n;
}

Vec3f normalAt(Vec3f const &p, Triangle const &t) {
	/*
	Vec3f A = t.a();
	Vec3f B = t.b();
	Vec3f C = t.c();
	float TriangleArea = (0.5) * (A.x*B.y + B.x * C.y + C.x*A.y - A.x*C.y -B.x* A.y - C.x - C.x*B.y );

	
	Vec3f vertexNormalA = Vec3f(0, 0, 0);
	Vec3f vertexNormalB = Vec3f(0, 2, 0);
	Vec3f vertexNormalC = Vec3f(0, 4, 0);

	vertexNormalA = normalized(vertexNormalA + normal(t));
	vertexNormalA = normalized(vertexNormalA + normal(t));
	vertexNormalA = normalized(vertexNormalA + normal(t));

	
	vertexNormalB = normalized(vertexNormalB + normal(t));
	vertexNormalB = normalized(vertexNormalB + normal(t));
	vertexNormalB = normalized(vertexNormalB + normal(t));

	vertexNormalC = normalized(vertexNormalC + normal(t));
	vertexNormalC = normalized(vertexNormalC + normal(t));
	vertexNormalC = normalized(vertexNormalC + normal(t));
	

	float beta = (norm(cross((C - A), (p - C))) / 2) / TriangleArea;
	float gamma = (norm(cross((B - A),(p - B))) / 2) / TriangleArea;

	return normalized((1 - beta - gamma) * vertexNormalA + beta * vertexNormalB + gamma * vertexNormalC );
	//return normal(t);
	*/
	
	Vec3f edgeAB = t.b() - t.a();
	Vec3f edgeAC = t.c() - t.a();

	return normalized(edgeAB^edgeAC);
	
}

Vec3f normalAt(Vec3f const & /*p*/, Plane const &p) {
  return p.normal;
}

float reflectAt(Vec3f const & /*p*/, Plane const &p) {
	return p.refect;
}

float reflectAt(Vec3f const & /*p*/, Sphere const &s) {
	return s.refect;
}

float reflectAt(Vec3f const & /*p*/, Triangle const &t) {
	return t.reflection;
}


struct Surface {
	virtual ~Surface() = default;
	virtual Hit intersectSelf(Ray const &ray) const = 0;
	virtual Vec3f normalAtSelf(Vec3f const &p) const = 0;
	virtual float reflectAtSelf(Vec3f const &p) const = 0;
};

// helper class/function to make, e.g., class Sphere : public Surface
// Wrapping the geometry (e.g., sphere) in a class for intersection
// does not 'pollute' the geometry with inheritence
template <class T> struct Intersect_ : public Surface {
  template <typename... Args>
  Intersect_(Args... args)
      : m_self(std::forward<Args>(args)...) {}

  Hit intersectSelf(Ray const &ray) const { return intersect(ray, m_self); }
  Vec3f normalAtSelf(Vec3f const &p) const { return normalAt(p, m_self); }
  float reflectAtSelf(Vec3f const &p) const { return reflectAt(p, m_self); }

  T m_self;
};
//T is any figure / object


template <typename T> std::unique_ptr<Intersect_<T>> makeIntersectable(T t) {
  return std::unique_ptr<Intersect_<T>>(new Intersect_<T>(t));
}

struct ImagePlane {
  using Screen = geometry::Grid2<raster::RGB>;

  Screen screen;
  math::Vec3f origin;
  math::Vec3f u;
  math::Vec3f v;
  float left;   // right symmetric
  float bottom; // top symmetric

  ImagePlane &resolution(uint32_t width, uint32_t height) {
    screen = Screen(width, height);
    return *this;
  }
  ImagePlane &center(Vec3f center) {
    origin = center;
    return *this;
  }
  ImagePlane &uvAxes(Vec3f up, Vec3f right) {
    u = up;
    v = right;
    return *this;
  }
  ImagePlane &dimensions(float width, float height) {
    left = -(0.5f * width);
    bottom = -(0.5f * height);
    return *this;
  }

  math::Vec3f pixelTo3D(math::Vec2f pixel) const {
    using std::abs;

    // shift to center
    pixel += {0.5f, 0.5f};

    float u_x = left + (2.f * abs(left)) * (pixel.x) / screen.width();
    float v_y = bottom + (2.f * abs(bottom)) * (pixel.y) / screen.height();

    return origin + u_x * u + v_y * v;
  }
};

ImagePlane makeImagePlane(Vec3f const &eye, Vec3f const &lookatPosition,
                          Vec3f const &canonicalUp, int xResolution,
                          int yResolution, float width, float height,
                          float nearPlaneDistanace) {
  // make orthonormal basis around eye
  auto gaze = normalized(lookatPosition - eye);

  auto u = gaze ^ canonicalUp;
  u.normalize();

  auto v = u ^ gaze;
  v.normalize();

  // image plane is orthogonal to camera gaze so use u,v for image plane
  ImagePlane imagePlane;
  // using method chaining to have named parameter/configuation)
  imagePlane.resolution(xResolution, yResolution)
      .center(eye + gaze * nearPlaneDistanace)
      .dimensions(width, height)
      .uvAxes(u, v);

  return imagePlane;
}

Vec3f reflect(Vec3f k, Vec3f n) {
	return k - 2 * (dot(k, n))*n;
}

using s_ptr = std::unique_ptr<Surface>;

Vec3f get_reflec_dir(Vec3f raydir, Vec3f n) {
	return raydir - 2 * raydir*n*n;
}

//calculate standard lighting
Vec3f calculate_lighting(Vec3f color, Vec3f p, Vec3f n, Ray ray, Vec3f light) {


	Vec3f lightColor = Vec3f(1, 1, 1.f);

	float ambientStrength = 0.9;
	Vec3f ambient = ambientStrength * lightColor;

	// diffuse
	Vec3f norm = normalized(n);
	Vec3f lightDir = normalized(light - p);
	float diff = max(dot(norm, lightDir), 0.0f);
	Vec3f diffuse = diff * lightColor;


	// specular
	float specularStrength = 1;
	Vec3f viewDir = normalized(ray.origin - p);
	Vec3f reflectDir = ((2.0f * (dot(lightDir, n))) * n - lightDir);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 256);
	Vec3f specular = specularStrength * spec * lightColor;




	Vec3f lightc = diffuse + ambient + specular;
	
	return Vec3f(lightc.x* color.x, lightc.y*color.y, lightc.z*color.z);

}

//calculate standard lighting
Vec3f calculate_ambientonly(Vec3f color, Vec3f p, Vec3f n, Ray ray, Vec3f light) {


	Vec3f lightColor = Vec3f(1, 1, 1.f);

	float ambientStrength = 0.9;
	Vec3f ambient = ambientStrength * lightColor;

	Vec3f lightc = ambient;

	return Vec3f(lightc.x* color.x, lightc.y*color.y, lightc.z*color.z);

}

Vec3f castRay(Ray ray,
              math::Vec3f eye,                    //
              math::Vec3f light,                  //
              std::vector<s_ptr> const &surfaces, 
	          int depth)                          {
	
	constexpr float ambientIntensity = 0.1f;
  // background color
  Vec3f colorOut(0.1f, 0.1f, 0.1f);
  if (depth > 1000) {
	  return colorOut;
  }
  
  // find closed object, if any
  Hit closest;
  // pointer to closest object
  Surface const *surface = nullptr;
  for (auto const &s : surfaces) {
    auto hit = s->intersectSelf(ray);
    if (hit && (hit.rayDepth < closest.rayDepth)) {
      closest = hit;
      surface = s.get();
    }
  }

  // if hit get point
  if (surface != nullptr) {
      // CALCULATE SHADOWS
	  Vec3f c = Vec3f(0.3,0.3,0.1);
      Vec3f d = ray.direction;
      Vec3f p = ray.origin + d * closest.rayDepth;
	  auto bias = 1e-4f;
	  auto p2 = pointOnLne(p, light - p, bias);
	  Ray shadowRay = Ray(p2, light-p);
	  bool shadowed = false;
	  Hit shadow_intersect;

	  for (auto const &s : surfaces) {
		  auto intersect = s->intersectSelf(shadowRay);

		  if (intersect.rayDepth < 0) {
			  continue;
		  }
		  if (intersect && (intersect.rayDepth < closest.rayDepth)) {
			  shadow_intersect = intersect;
			  shadowed = true;
			  break;
		  }
	  }

      Vec3f n = surface->normalAtSelf(p);
	  float reflect = surface->reflectAtSelf(p);

	  if (reflect > 0) {
		  if (shadowed && (reflect < 1)) {
			  Vec3f reflect_dir = get_reflec_dir(ray.direction, n);
			  Ray reflectionray = Ray(pointOnLne(p, n, bias), reflect_dir);
			 // colorOut = colorOut + reflect * castRay(reflectionray, eye, light, surfaces, depth += 1) + calculate_lighting(closest.color, p, n, ray, light)-Vec3f(0.1f, 0.1f, 0.1f);
			  colorOut = colorOut +  calculate_lighting(closest.color, p, n, ray, light) - Vec3f(0.1f, 0.1f, 0.1f);
		  }
		  else {
			  
			  Vec3f reflect_dir = get_reflec_dir(ray.direction, n);
			  Ray reflectionray = Ray(pointOnLne(p, n, bias), reflect_dir);
			  colorOut = colorOut + reflect * castRay(reflectionray, eye, light, surfaces, depth += 1) + calculate_lighting(closest.color, p, n, ray, light);
			 
			 
		  }
	  }
	  else {
		  if (shadowed) {
			  colorOut= calculate_lighting(closest.color, p, n, ray, light);
			  return colorOut-=Vec3f(0.1f, 0.1f, 0.1f);
		  }
		  else if (reflect <= 0) {
			  colorOut = calculate_lighting(closest.color, p, n, ray, light);

			  return  colorOut;
		  }
	  }
    
  }
  return colorOut;
}

void render(ImagePlane &imagePlane, //
            math::Vec3f eye,        // all below could be in 'scene' object
            math::Vec3f light,      //
            std::vector<s_ptr> const &surfaces) {

  // Standard mersenne_twister_engine seeded
  thread_local std::mt19937 gen(0);
  auto sampleRange = [](float a, float b) {
    using distrubution = std::uniform_real_distribution<>;
    return distrubution(a, b)(gen);
  };

  // question? if I switch the order here of rows vs. coloumns is it faster?
  // slower? why?
  for (int32_t y = 0; y < imagePlane.screen.height(); ++y) {
    for (int32_t x = 0; x < imagePlane.screen.width(); ++x) {

      math::Vec2f pixel(x, y);
      auto pixel3D = imagePlane.pixelTo3D(pixel);
      auto direction = normalized(pixel3D - eye);
      auto bias = 1e-4f;
      auto p = pointOnLne(eye, direction, bias);
      Ray r(p, direction);

      auto colorOut = castRay(r, eye, light, surfaces,0);

      // correct to quantiezed error
      // (i.e., removes banded aliasing when converting to 8bit RGB)
      constexpr float halfStep = 1.f / 512;
      colorOut = raster::quantizedErrorCorrection(
          colorOut, sampleRange(-halfStep, halfStep));

      imagePlane.screen({x, y}) = raster::convertToRGB(colorOut);
    }
  }
}
} // namespace

int main() {
  // setup camera and imagePlane
  Vec3f light{20, 10, 15};
  Vec3f eye{10.f, 5.f, 20.f};
  Vec3f lookat{0.f, 3.f, 0.f};
  Vec3f up{0.f, 2.f, 0.f};
  int resolutionX = 1000;
  int resolutionY = 1000;
  float planeWidth = 50.f;
  float planeHeight = 50.f;
  float focalDist = 50.f;

  using namespace raytracing;

  auto imagePlane = makeImagePlane(eye, lookat, up,          //
                                   resolutionX, resolutionY, //
                                   planeWidth, planeHeight,  //
                                   focalDist);

  // setup scene
  std::vector<s_ptr> surfaces;

  //Provided angles of the camera and light settings are found in the README.md, but different settings may be used as long as the camera looks at origin in direction to the horizon
  //Scene 1
   /*
  //Plane p;
  Plane p = Plane(Vec3f(0, 1, 0),Vec3f(0,0,0),Vec3f(0,0.2,0.3),0);
  Plane p2 = Plane(Vec3f(0, 0, 1), Vec3f(0, 0, -20), Vec3f(1, 1, 1),0);
  surfaces.push_back(makeIntersectable(p)); //
  //surfaces.push_back(makeIntersectable(p2));

  //Sphere s = Sphere();
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
  Sphere s = Sphere(Vec3f(-2,2,10),2,Vec3f(0.7,0.1,0.1),0.3) ;
  Sphere s2 = Sphere(Vec3f(5,4,8),1,Vec3f(0.2,0.1,0.5),0) ;
  Sphere s3 = Sphere(Vec3f(1, 5, 0), 4, Vec3f(0.1, 0.7, 0.2), 0.99);
  surfaces.push_back(makeIntersectable(s));
 surfaces.push_back(makeIntersectable(s2));
 surfaces.push_back(makeIntersectable(s3));
 

 //Triangle constructor :: Triangle (vertexa,vertexb,vertexc, color, reflectiviequality)
 Triangle test({ 0, 1, 6 }, { 3, 1, 5 }, { 3, 4, 5 });
 test.color= Vec3f(0.1, 0.1, 0.8);
 test.reflection = 0;
 Triangle test2({ -2, 5, 12 } ,{ 1, 5, 12 } , { -2, 9, 12 });
 test2.color = Vec3f( 0.1,0.1,0.8);
 test2.reflection = 0;
 surfaces.push_back(makeIntersectable(test));
 surfaces.push_back(makeIntersectable(test2));
 */
 
  //Scene 2
  /*
  //Plane p;
  Plane p = Plane(Vec3f(0, 1, 0), Vec3f(0, 0, 0), Vec3f(0, 0.2, 0.3), 0.1);
 // Plane p2 = Plane(Vec3f(0, 0, 1), Vec3f(0, 0, -20), Vec3f(1, 1, 1), 0);
  surfaces.push_back(makeIntersectable(p)); //
  //surfaces.push_back(makeIntersectable(p2));

  //Sphere s = Sphere();
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
  Sphere s = Sphere(Vec3f(-2, 2, 10), 2, Vec3f(0.7, 0.1, 0.1), 0.3);
  Sphere s2 = Sphere(Vec3f(5, 4, 8), 1, Vec3f(0.2, 0.1, 0.5), 0);
  Sphere s3 = Sphere(Vec3f(1, 5, 0), 4, Vec3f(0.1, 0.7, 0.2), 0.99);
 
  Sphere s4 = Sphere(Vec3f(2, 5, -2), 1, Vec3f(0.1, 0.7, 0.2), 0.2);

  Sphere s6 = Sphere(Vec3f(3, 5, -10), 2, Vec3f(0.7, 0.2, 0.2), 0.6);
  surfaces.push_back(makeIntersectable(s));
  surfaces.push_back(makeIntersectable(s2));
  surfaces.push_back(makeIntersectable(s3));
  surfaces.push_back(makeIntersectable(s4));
  surfaces.push_back(makeIntersectable(s6));
  */


  //Scene 3
  //Sphere s = Sphere();
  /*
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
  Sphere s = Sphere(Vec3f(0, -10, 0), 1, Vec3f(0.5, 0.5, 0.5), 0.1);
  Sphere s2 = Sphere(Vec3f(0, -4, 0), 3, Vec3f(1, 0.6, 0.0), 0);
  Sphere s3 = Sphere(Vec3f(0, 2, 0), 2, Vec3f(0, 0, 1), 0.7);
  Sphere s4 = Sphere(Vec3f(0, 6, 0), 1, Vec3f(0.8, 0.4, 0.2), 0.2);
  Sphere s5 = Sphere(Vec3f(0, 10, 0), 2, Vec3f(0.8, 0.8, 0.8), 0);
  Sphere s6 = Sphere(Vec3f(0, 16, 0), 2, Vec3f(0.5, 0.5, 0.7), 0.8);


  surfaces.push_back(makeIntersectable(s));
  surfaces.push_back(makeIntersectable(s2));
  surfaces.push_back(makeIntersectable(s3));
  surfaces.push_back(makeIntersectable(s4));
  surfaces.push_back(makeIntersectable(s5));
  surfaces.push_back(makeIntersectable(s6));
  */

  //Scene 4 
  /*
   //Plane p;
  Plane p = Plane(Vec3f(0, 1, 0), Vec3f(0, 0, 0), Vec3f(0.7, 0.5, 0.3), 0);
  
  surfaces.push_back(makeIntersectable(p)); //

   //Sphere s = Sphere();
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
  Sphere s = Sphere(Vec3f(-8, 3, 0), 3, Vec3f(0.7, 0.1, 0.5), 0.5);
  Sphere s2 = Sphere(Vec3f(8, 3, 0), 2, Vec3f(0, 0.1, 0.7), 0.5);
  Sphere s3 = Sphere(Vec3f(8, 10, 0), 3, Vec3f(0.1, 0.7, 0.5), 0.5);
  Sphere s4 = Sphere(Vec3f(-8, 10, 0), 2, Vec3f(0.7, 0.5, 0.1), 0.5);
  surfaces.push_back(makeIntersectable(s));
  surfaces.push_back(makeIntersectable(s2));
  surfaces.push_back(makeIntersectable(s3));
  surfaces.push_back(makeIntersectable(s4));
  */

  //Scene 5
  ///*
  //Plane p;
  Plane p = Plane(Vec3f(0, 1, 0), Vec3f(0, 0, 0), Vec3f(0.7, 0.5, 0.3), 0);
  surfaces.push_back(makeIntersectable(p));

  //Sphere s = Sphere();
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
  Sphere s = Sphere(Vec3f(0, 3, 5), 3, Vec3f(0.2, 0.1, 0.3), 0.1);
  surfaces.push_back(makeIntersectable(s));
  //*/
  
  //Scene 6
/*
  //Sphere s = Sphere();
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
	Sphere s = Sphere(Vec3f(-3, 3, 5), 3, Vec3f(0.7, 0, 0.5), 0.7);
	Sphere s2 = Sphere(Vec3f(3, 3, 5), 3, Vec3f(0.5, 0.3, 0.7), 0.7);
	surfaces.push_back(makeIntersectable(s));
	surfaces.push_back(makeIntersectable(s2));
*/
  //Scene 7
  /*
	 //Plane p;
	Plane p = Plane(Vec3f(0, 1, 0), Vec3f(0, 0, 0), Vec3f(0.7, 0.5, 0.3), 0);
	surfaces.push_back(makeIntersectable(p));

	 //Triangle constructor :: Triangle (vertexa,vertexb,vertexc, color, reflectiviequality)
	Triangle t1({ -7, 0, 10 }, { 3, 0, 10 }, { -2, 5, 5 } );
	t1.color = Vec3f(0.1, 0.1, 0.8);
	//t1.reflection = 0.5;
	Triangle t2({ -7, 0, -10 }, { -7, 0, 10 }, { -2, 5, 5 });
	t2.color = Vec3f(0.1, 0.1, 0.8);
	Triangle t3({ 3, 0, 10 }, { 3, 0, -10 }, { -2, 5, 5 } );
	t3.color = Vec3f(0.1, 0.1, 0.8);
	Triangle t4({ -7, 0, -10 }, { 3, 0, -10 }, { -2, 5, 5 });
	t4.color = Vec3f(0.1, 0.1, 0.8);
	
	surfaces.push_back(makeIntersectable(t1));
	surfaces.push_back(makeIntersectable(t2));
	surfaces.push_back(makeIntersectable(t3));
	surfaces.push_back(makeIntersectable(t4));
	*/
  //Scene 8
/*
   //Plane p;
Plane p = Plane(Vec3f(0, 1, 0), Vec3f(0, 0, 0), Vec3f(0.7, 0.5, 0.3), 0);
surfaces.push_back(makeIntersectable(p));

//Sphere s = Sphere();
//Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
Sphere s = Sphere(Vec3f(0, 7, 5), 3, Vec3f(0.7, 0.1, 0.5), 1);
surfaces.push_back(makeIntersectable(s));

//Triangle constructor :: Triangle (vertexa,vertexb,vertexc, color, reflectiviequality)
Triangle t1({ -7, 0, 10 }, { 3, 0, 10 }, { -2, 5, 5 });
t1.color = Vec3f(0.8, 0.1, 0.1);
//t1.reflection = 0.5;
Triangle t2({ -7, 0, -10 }, { -7, 0, 10 }, { -2, 5, 5 });
t2.color = Vec3f(0.8, 0.1, 0.1);
Triangle t3({ 3, 0, 10 }, { 3, 0, -10 }, { -2, 5, 5 });
t3.color = Vec3f(0.8, 0.1, 0.1);
Triangle t4({ -7, 0, -10 }, { 3, 0, -10 }, { -2, 5, 5 });
t4.color = Vec3f(0.8, 0.1, 0.1);

surfaces.push_back(makeIntersectable(t1));
surfaces.push_back(makeIntersectable(t2));
surfaces.push_back(makeIntersectable(t3));
surfaces.push_back(makeIntersectable(t4));

*/
  //Scene 9
/*
Plane p = Plane(Vec3f(0, 1, 0), Vec3f(0, 0, 0), Vec3f(0.7, 0.5, 0.3), 0.2);
surfaces.push_back(makeIntersectable(p));
  //Triangle constructor :: Triangle (vertexa,vertexb,vertexc, color, reflectiviequality)
Triangle t1({ -7, 0, 10 }, { 3, 0, 10 }, { -2, 5, 5 });
t1.color = Vec3f(0.8, 0.1, 0.1);
//t1.reflection = 0.1;
surfaces.push_back(makeIntersectable(t1));
*/
 

  // render that thing...
  temporal::Timer timer(true);

  render(imagePlane, eye, light, surfaces);

  std::cout << "Time elapsed: " << std::fixed << timer.minutes() << " min.\n";

  raster::write_screen_to_file("./test.png", imagePlane.screen); //result of rasterization

  return EXIT_SUCCESS;
}
