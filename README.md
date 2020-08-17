# Basic Ray Tracer

This was a small project just to learn how Ray Tracing works. It's based on Peter Shirley's Ray Tracing in One Weekend book series, and I must say the code is not optimized, and runs slowly (as I said: it's for learning purposes).

## Modifying the Code

### Parameters

The most interesting parameters to modify are the first variables in main(), "comp" is the width of the image; "alt" is the height; "samplesPorPixel" (this is an intersting one) is the number of rays shot per pixel of the image, increasing this number makes the program take longer to execute, but, it makes the quality better, although with lower samples per pixel you can get really cool and interesting images; and "profundidade" is how many bounces a ray can do.
![High samplesPorPixel](https://github.com/Saliba-san/RayTracer/tree/master/progress/marblessphere.png?raw=true)
![Low samplesPorPixel](https://github.com/Saliba-san/RayTracer/tree/master/progress/doido2.png?raw=true)


### Camera Parameters

You can also change the camera parameters: lookat(x,y,z) is the point that the camera is looking at; lookfrom(x,y,z) is the point where the camera is located; vup(x,y,z) is the vector that defines which direction is "up" to the camera; dist_to_focus is the focal distance of the camera; aperture is the time the lid of the camera stays open; and finally, vfov, is the field of view of the camera, to decrease it is to zoom-in the camera.

### Scenes

I have some scenes commented in the "Mainmp.cpp" file:
```c++
//auto world = random_scene();
//auto world = terra();
//auto world = two_spheres();
//auto world = luzSimples();
//auto world = CornelBox();
```
All of these configurations are stored in "cenas.h" file and you can just uncomment one of them if you want that scene, but you can easily create new ones if you want.

## Compiling and Executing

The project is completely self contained, and does not need any outside libraries, so just use your favorite compiler to compile the "Mainmp.cpp" file. I will say, though, that to use parallelization in your CPU cores, you will need to give to your compiler the flag "-fopenmp", as shown bellow, for that you will need OpenMP installed (libomp-dev in Debian/Ubuntu).

```bash
g++ -o RayTracer Mainmp.cpp -fopenmp
```
Above is a compilation using g++ to a file called RayTracer using multiple CPUs via OpenMP, but if you're lazy you can just use the compilated file already in the Repo. As you execute the program it will output rows of numbers that you will need to output it into a "*.ppm" image file, to do this in bash with the executable "RayTracer" and "image.ppm" as the output image, it would be like:

```bash
./RayTracer > image.ppm
```

