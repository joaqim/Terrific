#include <Terrific/Geometry/SphericalVoronoi.h>
#include <Magnum/Math/Math.h>
#include <Terrific/Math/Types.h>
#include "../Example/Bitmap.cpp"

#include <random>
#include <cstdlib>

#define SEEDED
std::vector<Vector3d> generatePoints(size_t const count) {
  std::vector<Vector3d> points;

  std::random_device const rd;
#ifdef SEEDED
  std::mt19937 rng(145);
#else
  std::mt19937 rng(rd());
  rng.discard(700000);
#endif

  //NOTE: Perform 70000 generations, much slower but improves randomness.
  // https://codereview.stackexchange.com/a/109518



  std::uniform_real_distribution<> heightDistribution(-1.f, 1.f);
  std::uniform_real_distribution<> angleDistribution(0, 2 * M_PI);

  using Magnum::Math::asin;
  using Magnum::Math::cos;
  using Magnum::Math::sin;
  using Magnum::Math::Rad;

  for (size_t i = 0; i < count; i++) {
    double z = heightDistribution(rng);
    auto const phi = Rad<double>(angleDistribution(rng));
    auto theta = asin(z);
    float x = cos(theta) * cos(phi);
    float y = cos(theta) * sin(phi);

    points.push_back(Vector3d(x, y, z));
  }

  return points;
}

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

  auto points = generatePoints(10); 
  auto sphere = Terrific::Geometry::SphericalVoronoi(points, true);

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
