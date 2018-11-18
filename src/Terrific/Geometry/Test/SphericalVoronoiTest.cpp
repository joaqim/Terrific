#include <Terrific/Utility/Log.h>
//#include <Terrific/Geometry/SphericalVoronoi.h>
#include <Terrific/Geometry/SphericalVoronoi.cpp>

#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Assert.h>

#include <Terrific/Math/Types.h>

#include <list>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif
#if 0
// https://stackoverflow.com/a/1861337

UnsignedLong GetTimeMs64() {
#ifdef _WIN32
  /* Windows */
  FILETIME ft;
  LARGE_INTEGER li;

  /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
   * to a LARGE_INTEGER structure. */
  GetSystemTimeAsFileTime(&ft);
  li.LowPart = ft.dwLowDateTime;
  li.HighPart = ft.dwHighDateTime;

  uint64 ret = li.QuadPart;
  ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
  ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

  return ret;
#else
  /* Linux */
  struct timeval tv;

  gettimeofday(&tv, NULL);

  UnsignedLong ret = tv.tv_usec;
  /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
   ret /= 1000;

   /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
   ret += (tv.tv_sec * 1000);

   return ret;
#endif
}
#endif

namespace Terrific {
namespace Geometry {
namespace Test {
class Unregistered
{ };


struct SphericalVoronoiTest : Corrade::TestSuite::Tester {
  explicit SphericalVoronoiTest() {
    addTests({
        &SphericalVoronoiTest::initialize
      });
    addBenchmarks(
        {&SphericalVoronoiTest::genPoints}, 1,BenchmarkType::CpuTime);

    addBenchmarks(
        {&SphericalVoronoiTest::createSphere}, 1,BenchmarkType::CpuTime);

    addBenchmarks(
        {&SphericalVoronoiTest::stepUntilFinished}, 1,BenchmarkType::CpuTime
                  );
  }

  void initialize() {
    CORRADE_ASSERT(Terrific::Utility::Log::init() == EXIT_SUCCESS, "Failed to initialize Terrific::Utility::Log",);
    TERRIFIC_INFO("Application starting up {0}", 1);
    CORRADE_VERIFY(true);
  }

  void genPoints() {
    //points = Terrific::Geometry::SphericalVoronoi::generatePoints(std::size_t{180});
    TERRIFIC_INFO("Generating {0} random points on a Sphere of radius 1 for {1} iterations", _kPointSize, _kIterations);
    CORRADE_BENCHMARK(_kIterations)
        _lPoints.emplace_back(SphericalVoronoi::generatePoints(_kPointSize));
  }

  void createSphere() {
    TERRIFIC_INFO("Initializing Points as Cells on Sphere for {1} iterations", NULL, _kIterations);
    CORRADE_BENCHMARK(_kIterations) {
      //TERRIFIC_INFO("{0}, {1}", ,_lPoints.size());
      auto sphere = SphericalVoronoi(_lPoints.back(), false);
      _lPoints.pop_back();
      _vSpheres.push_back(std::move(sphere));
    }
    //_vSpheres.emplace_back(_lPoints[testCaseRepeatId()], false);
  }


  void stepUntilFinished() {
    TERRIFIC_INFO("Generating a Voronoi Sphere of radius 1 for {1} iterations", NULL, _kIterations);
    CORRADE_BENCHMARK(_kIterations) {
      auto &sphere = _vSpheres.back();
      while(!sphere.isFinished()) {
        //sphere->solve([](int i){TERRIFIC_INFO("{0} \n", i)});
        sphere.solve();
      }
      _vSpheres.pop_back();
    }
    CORRADE_VERIFY(true);
  }

  //std::unique_ptr<SphericalVoronoi> sphere;
  //std::vector<std::unique_ptr<SphericalVoronoi>> _vSpheres;
  std::vector<SphericalVoronoi> _vSpheres;
  typedef std::vector<Vector3d> Points;
  std::vector<Points> _lPoints;


  UnsignedLong oldTime{0}, currentTime{0}, deltaTime;

  UnsignedLong const _kIterations{100};
  std::size_t const _kPointSize{280};

};
} // namespace Test
} // namespace Geometry
} // namespace Terrific

CORRADE_TEST_MAIN(Terrific::Geometry::Test::SphericalVoronoiTest)
