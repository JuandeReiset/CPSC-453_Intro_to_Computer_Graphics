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

#include "ray_intersect.hpp"
#include<algorithm>

namespace geometry {

Hit::operator bool() const { return didIntersect; }

//Work on this two functions
Hit intersect(Ray const &ray, Triangle const &triangle) {
  Hit hit;
 
  /*
  |a d g| | beta|    |j|
  |b e h| |gamma|  = |k|
  |c f i| |  t  |    |l|
  
  */
  float a = (triangle.a().x) - (triangle.b().x);
  float b = (triangle.a().y) - (triangle.b().y);
  float c = (triangle.a().z) - (triangle.b().z);
  float d = (triangle.a().x) - (triangle.c().x);
  float e = (triangle.a().y) - (triangle.c().y);
  float f = (triangle.a().z) - (triangle.c().z);
  float g = ray.direction.x;
  float h = ray.direction.y;
  float i = ray.direction.z;
  float j = (triangle.a().x) - ray.origin.x;
  float k = (triangle.a().y) - ray.origin.y;
  float l = (triangle.a().z) - ray.origin.z;

  float M =  a *(e*i-h*f) + b*(g*f-d*i) + c*(d*h - e*g) ;

  float t = -(f*(a*k - b*j) + e * (c*j - a*l) + d * (b*l - c*k)) / M;
  float gamma = (i*(a*k - b*j) + h * (c*j - a*l) + g * (b*l - c*k)) / M;
  float  beta = (j*(e*i - h * f) + k*(g*f - d * i) + l*(d*h - e * g)) / M;

  float t0 = 0;
  float t1 = 12;
  /*
  if(t < t0 || t > t1){
	  return hit;
  }
  */
  if (gamma < 0 || gamma > 1) {
	  return hit;
  }
  if (beta < 0 || beta > (1 - gamma)) {
	  return hit;
  }
  
  hit.didIntersect = true;
  hit.color = triangle.color;
  hit.rayDepth = t;
  return hit;
}

Hit intersect(Ray const &ray, Sphere const &sphere) {
  Hit hit;
  auto CE = ray.origin - sphere.origin;
      float A = dot(ray.direction, ray.direction);
      float B = 2*dot(ray.direction, (ray.origin-sphere.origin));
      float C = (dot((ray.origin-sphere.origin), (ray.origin-sphere.origin))-pow(sphere.radius,2));

      float quad = pow(B,2)-(4*A*C);
      if (quad < 0 || quad ==0) {
          return hit;
      }
      else if(quad > 0){
          hit.didIntersect = true;
          hit.color = sphere.color;
          double t1 =  (-B + sqrt(quad))/(2*A);
          double t2 =  (-B - sqrt(quad))/(2*A);

          if(t1>0 &&(t1 < t2 || t2 < 0)){
              hit.rayDepth = t1;
          }
          else if(t2 > 0){
              hit.rayDepth=t2;
          }
      }

	  
      return hit;
}


Hit intersect(Ray const &r, Plane const &p) {
  Hit hit;

  auto denom = r.direction * p.normal;
  if (std::abs(denom) < 1e-5f)
    return hit;

  auto t = ((p.origin - r.origin) * p.normal) / denom;

  if (t < 0.f)
    return hit;

  hit.didIntersect = true;
  hit.color = p.color;
  hit.rayDepth = t;

  return hit;
}

} // namespace geometry
