//CPSC 453 ASSIGNMENT 3


Scene 1: Spheres&Triangles_Shadows+reflection

Settings for Scene 1

 #Camera
 
  #Camera
  Vec3f light{20, 10, 15};
  Vec3f eye{10.f, 5.f, 20.f};
  Vec3f lookat{0.f, 3.f, 0.f};
  Vec3f up{0.f, 2.f, 0.f};

  #Objects
    //Plane p;
  Plane p = Plane(Vec3f(0, 1, 0),Vec3f(0,0,0),Vec3f(0,0.2,0.3),0);
  surfaces.push_back(makeIntersectable(p)); //
  //Sphere s = Sphere();
  //Sphere constructor :: Sphere (positionvector, radius, colorvector, reflectivequality)
  Sphere s = Sphere(Vec3f(-2,2,10),2,Vec3f(0.7,0.1,0.1),0.3) ;
  Sphere s2 = Sphere(Vec3f(5,4,8),1,Vec3f(0.2,0.1,0.5),0) ;
  Sphere s3 = Sphere(Vec3f(1, 5, 0), 4, Vec3f(0.1, 0.7, 0.2), 0.5);
  surfaces.push_back(makeIntersectable(s));
 surfaces.push_back(makeIntersectable(s2));
 surfaces.push_back(makeIntersectable(s3));

Scene2: Reflective plane

Similar settings from scene 1,extra sphere, different angle + reflective plane + no triangles

Scene3: Planet Allignment

Multiple Spheres with different coloring and reflectivity

Scene4: Spheres

Settings
 #Camera
 Vec3f light{0, 10, 15};
  Vec3f eye{0.f, 5.f, 30.f};
  Vec3f lookat{0.f, 3.f, 0.f};
  Vec3f up{0.f, 2.f, 0.f};

#Objects
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

Scene5: Reflection of sphere zoomed in

Scene6: Reflection between Spheres in void

Scene7: Piramid with lighting on top

Scene8: Scene 7 with lighting on the right (+closer)

Scene9: Reflective sphere and Red pyramid
** Eye vector in similar position to light vector

Scene10: