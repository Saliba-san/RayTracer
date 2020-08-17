# Basic Ray Tracer

This was a small project just to learn how Ray Tracing works. It's based on Peter Shirley's Ray Tracing in One Weekend book series, and I must say the code is not optimized, and runs slowly (as I said: it's for learning purposes).

## Compiling and Executing

The project is completely self contained, and does not need any outside libraries, so just use your favorite compiler to compile the "Mainmp.cpp" file. I will say, though, that to use parallelization in your CPU cores, you will need to give to your compiler the flag "-fopenmp", as shown bellow, for that you will need OpenMP installed (libomp-dev in Debian/Ubuntu).

```bash
g++ -o RayTracer Mainmp.cpp -fopenmp
```
Above is a compilation using g++ to a file called RayTracer using multiple CPUs via OpenMP, but if you're lazy you can just use the compilated file already in the Repo. As you execute the program it will output rows of numbers that you will need to output it into a "*.ppm" image file, to do this in bash with the executable "RayTracer" and "image.ppm" as the output image, it would be like:

```bash
./RayTracer > image.ppm
```

