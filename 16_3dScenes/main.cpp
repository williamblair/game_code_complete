#include <stdio.h>
#include <GCCMath.h>

void testPlane()
{
    Plane plane;
    plane.Init(Vec3(1,2,-2), Vec3(3,-2,1), Vec3(5,1,-4));
    //plane.Init(Vec3(5,1,-4), Vec3(3,-2,1), Vec3(1,2,2));
    printf("Plane abcd: %f,%f,%f,%f\n", plane.a, plane.b, plane.c, plane.d);
}

int main(int argc, char **argv)
{
    printf("hello world\n");
    testPlane();
    return 0;
}
