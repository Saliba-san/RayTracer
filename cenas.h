#include "vec3.h"
#include "color.h"
//objetos
#include "esfera.h"
#include "esferaMovendo.h"
#include "box.h"

#include "bvh.h"

#include "listaObjetos.h"
#include "material.h"
#include "meioConstante.h"

listaObjetos terra(){
  auto terra_texture = make_shared<imagem_texture>("im_tex/julio.png");
  auto terra_surface = make_shared<lambertian>(terra_texture);
  auto bola1 = make_shared<esfera>(point3(0,0,6), 3, terra_surface);
  auto bola2 = make_shared<esfera>(point3(0,0,0), 3, terra_surface);
  auto globo = make_shared<esfera>(point3(0,0,-6), 3, terra_surface);
  listaObjetos world;
  world.add(bola1);
  world.add(bola2);
  world.add(globo);
  return world;
}

listaObjetos random_scene() {
    listaObjetos world;

    auto pertext = make_shared<noise>(1.72);

    world.add(make_shared<esfera>(point3(0,-1000,0), 1000, make_shared<lambertian>(make_shared<solidColor>(0.3,0.7,0.2))));


    for (int a = -14; a < 14; a++) {
        for (int b = -14; b < 14; b++) {
            auto choose_mat = randDouble();
            point3 center(a + 0.9*randDouble(), 0.2, b + 0.9*randDouble());

            if ((center - vec3(4, 0.2, 0)).length() > 0.7) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.65) {
                    // diffuse
                    auto albedo = make_shared<solidColor>(color::random() * color::random());
                    sphere_material = make_shared<lambertian>(albedo);
                    //auto center2 = center + point3(0, randDouble(0, 0.5), 0);
                    world.add(make_shared<esfera>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.75) {
                    auto albedo = make_shared<quadriculado>(
                        make_shared<solidColor>(color::random() * color::random()),
                        make_shared<solidColor>(0.9, 0.9, 0.9)
                    );
                    // diffuse
                    sphere_material = make_shared<lambertian>(albedo);
                    //auto center2 = center + point3(0, randDouble(0, 0.5), 0);
                    world.add(make_shared<esfera>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<esfera>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dieletric>(1.5, 1.0);
                    world.add(make_shared<esfera>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dieletric>(1.5, 1.0);
    world.add(make_shared<esfera>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(make_shared<solidColor>(0.7, 0.2, 0.2));
    world.add(make_shared<esfera>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<esfera>(point3(4, 1, 0), 1.0, material3));

    return world;
}

listaObjetos two_spheres() {
  listaObjetos objects;

  auto checker = make_shared<quadriculado>(
      make_shared<solidColor>(0.2, 0.3, 0.1),
      make_shared<solidColor>(0.9, 0.9, 0.9)
  );

  objects.add(make_shared<esfera>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
  objects.add(make_shared<esfera>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

  return objects;
}

listaObjetos luzSimples() {
  listaObjetos mundo;

  auto pertext = make_shared<noise>(4);
  auto checker = make_shared<quadriculado>(
      make_shared<solidColor>(0.2, 0.3, 0.1),
      make_shared<solidColor>(0.9, 0.9, 0.9)
      );
  mundo.add(make_shared<esfera>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));
  mundo.add(make_shared<esfera>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

  auto difflight = make_shared<fonteDifusa>(make_shared<solidColor>(4,4, 4));
  mundo.add(make_shared<xyRect>(3,5,1,4,-2,difflight));
  mundo.add(make_shared<esfera>(point3(0,7,0), 2, difflight));
  return mundo;
}

listaObjetos cornellBox() {
  listaObjetos mundo;

  auto red = make_shared<lambertian>(make_shared<solidColor>(.65, .05, .05));
  auto white = make_shared<lambertian>(make_shared<solidColor>(.73, .73, .73)); 
  auto green = make_shared<lambertian>(make_shared<solidColor>(.12, .45, .15));
  auto light = make_shared<fonteDifusa>(make_shared<solidColor>(15, 15, 15));

  mundo.add(make_shared<flipFace>(make_shared<yzRect>(0, 555, 0, 555, 555, green)));
  mundo.add(make_shared<yzRect>(0, 555, 0, 555, 0, red));
  mundo.add(make_shared<xzRect>(213, 343, 227, 332, 554, light));
  mundo.add(make_shared<xzRect>(0, 555, 0, 555, 0, white));
  mundo.add(make_shared<flipFace>(make_shared<xzRect>(0, 555, 0, 555, 555, white)));
  mundo.add(make_shared<xyRect>(0, 555, 0, 555, 555, white));

  shared_ptr<objeto> box1 = make_shared<box>(point3(0,0,0), point3(165, 330, 165), white);
  box1 = make_shared<rotateY>(box1, 15);
  box1 = make_shared<translate>(box1, vec3(265, 0, 295));
  mundo.add(box1);

  shared_ptr<objeto> box2 = make_shared<box>(point3(0,0,0), point3(165, 165, 165), white);
  box2 = make_shared<rotateY>(box2, -18);
  box2 = make_shared<translate>(box2, vec3(130, 0, 65));
  mundo.add(box2);

  return mundo;
};

listaObjetos nextWeekFinalScene() {
  listaObjetos boxes1;
  auto ground = make_shared<lambertian>(make_shared<solidColor>(0.48, 0.83, 0.53));

  const int boxes_per_side = 20;
  for(int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i*w;
      auto z0 = -1000.0 + j*w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = randDouble(1, 101);
      auto z1 = z0 +w;

      boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
    }
  }
  
  listaObjetos mundo;

  mundo.add(make_shared<nodeBVH>(boxes1,0,1));

  auto light = make_shared<fonteDifusa>(make_shared<solidColor>(7,7,7));
  mundo.add(make_shared<xzRect>(123, 423, 147, 412, 554, light));

  auto center1 = point3(400, 400, 200);
  auto center2 = center1 + vec3(0,-50,0);
  auto movingSphereMaterial = 
    make_shared<lambertian>(make_shared<solidColor>(0.7,0.3,0.1));
  mundo.add(make_shared<esferaMovendo>(center1, center2, 0, 1, 50, movingSphereMaterial));

  mundo.add(make_shared<esfera>(point3(260, 150, 45), 50, make_shared<dieletric>(1.5, 1.0)));

  mundo.add(make_shared<esfera>(
        point3(0,150,145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 0.7)
  ));

  auto boundry = make_shared<esfera>(point3(360, 150, 145), 70, make_shared<dieletric>(1.5, 1.0));
  mundo.add(boundry);
  mundo.add(make_shared<meioConstante>(
        boundry, .0001, make_shared<solidColor>(0.3,0.3,0.8)));

  auto emat = make_shared<lambertian>(make_shared<imagem_texture>("im_tex/earthmap.jpg"));
  mundo.add(make_shared<esfera>(point3(400,200,400), 100, emat));

  auto pertext = make_shared<noise>(100);
  mundo.add(make_shared<esfera>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

  listaObjetos boxes2;
  auto white = make_shared<lambertian>(make_shared<solidColor>(.73, .73, .73));
  int ns = 1000;
  for(int j = 0; j < ns; j++) {
    boxes2.add(make_shared<esfera>(point3::random(0,165), 10, white));
  }
  
  mundo.add(make_shared<translate>(
        make_shared<rotateY>(
          make_shared<nodeBVH>(boxes2, 0.0, 1.0),
          15),
          vec3(-100,270,395)
        )
      );

  return mundo;
};


      
