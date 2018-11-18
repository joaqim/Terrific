#include <Terrific/Geometry/SphericalVoronoi.cpp>
#include <Magnum/Math/Math.h>
#include "../Example/Bitmap.cpp"

#include <Corrade/Utility/Assert.h>
#include <iostream>

#include <Magnum/Math/Vector.h>
#include <Magnum/Math/Color.h>
#include <Terrific/Math/Types.h>

namespace Terrific {
  namespace GL {
    struct Mesh {
      std::vector<Vector3f> vertices; 
      std::vector<UnsignedInt> indices; 
      std::vector<Vector3f> normals; 
      std::vector<Color3> colors; 
      std::vector<Vector2f> uvs; 
    };
  }
}


int main() {
  using namespace Terrific::Geometry;

  auto points = SphericalVoronoi::generatePoints(10);
  auto sphere = SphericalVoronoi(points, true);
  sphere.solve();

  int width{512}, height{512};
  auto bitmap = Bitmap(width, height);
  for(int i{0};i<width;i++) {
    for(int j{0};j<height;j++) {
      bitmap.SetPixel(i, j);
    }
  }
  CORRADE_ASSERT(bitmap.SaveBitmap("test.bmp"),"Failed to write bitmap file test.bmp", 1);

  return 0;
};
