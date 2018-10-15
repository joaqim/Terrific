
#include <cassert>
#include <ostream>
#include <sstream>
#include <vector>
#include <tuple>
#include <bitset>
#include <numeric> // for std::accumulate

#include <limits>

#define FLT_MAX std::numeric_limits<float>::max()
#define FLT_MIN std::numeric_limits<float>::min()

#include "xs_Float.h"  // http://stereopsis.com/sree/fpu2006.html

#define FLOOR_TO_INT(val)      xs_FloorToInt(val)
#define ROUND_TO_INT(val)      xs_RoundToInt(val)
#define CEIL_TO_INT(val)       xs_CeilToInt(val)

#ifndef _MATH_DEFINES_DEFINED
#define M_PI 3.14159265358979323846
#endif

#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Vector4.h>

#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Functions.h>

#include <Terrific/Math/Types.h>

#include <math.h>


namespace Terrific {
  namespace Math {

    typedef Magnum::Math::Rad<float> Rad;

    inline std::string stringFromVector3(const Vector3 &r3) {
      std::stringstream ss;
      ss << "<" << r3.x() << "," << r3.y() << "," << r3.z() << ">";
      return ss.str();
    }

#if 0
  inline auto atan(float const x, float const y) {
    //return static_cast<float>(Magnum::Math::atan(y / x));
    return ext::atan2(y, x);
  }
  inline float acos(float const &value) {
    //return static_cast<float>(Magnum::Math::acos(value));
    return ext::acos(value);
  }

  inline float asin(float const &value) {
    //return static_cast<float>(Magnum::Math::asin(value));
    return ext::asin(value);
  }

  inline float sin(float const &value) {
    return std::sin(value);
  }

  inline float cos(float const &value) {
    return std::cos(value);
  }

  template<class T>
      inline const T& absT(const T& value) {
    return Magnum::Math::abs(value);
  }

  auto const abs = absT<float>;

  template<class T>
      inline const T& maxT(const T& value, const T &maximum) {
    return Magnum::Math::max(value, maximum);
  }

  template<class T>
      inline const T& minT(const T& value, const T &minimum) {
    return Magnum::Math::min(value, minimum);
  }
  auto const max = maxT<float>;
  auto const min = minT<float>;
#endif

  template<class T> inline
      const T clamp(const T& value, const T& min, const T& max) {
    return Magnum::Math::clamp<T>(value, min, max);
    //return ext::fmin(max, ext::fmax(value, min));
  }

  inline auto length(Vector3 const val) {
    return val.length();
  }

  inline auto normalize(Vector3 const val ) {
    return val.normalized();
  }

  inline float distance(Vector3 const &lhs, Vector3 const &rhs) {

    float x1 = lhs.x();
    float y1 = lhs.y();
    float x2 = rhs.x();
    float y2 = rhs.y();

    auto val1 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    auto val2 = abs(lhs.length() - rhs.length());
    //auto val1 = 1.f;

    assert(val1 == val2);
    return val1;
  }

  class Ray;
  class AABB;
  class Plane;
  class Point;

  inline Vector3 getTransformPosition(const Matrix4& transform) {
    return Vector3(transform[3][0], transform[3][1], transform[3][2]);
  }

  enum ECubeFace
  {
    CF_POSX = 0,
    CF_FIRST = 0,
    CF_NEGX,
    CF_POSY,
    CF_NEGY,
    CF_POSZ,
    CF_NEGZ,

    CF_MAX,

    CF_POSX_BITMASK = (1 << CF_POSX),
    CF_NEGX_BITMASK = (1 << CF_NEGX),
    CF_POSY_BITMASK = (1 << CF_POSY),
    CF_NEGY_BITMASK = (1 << CF_NEGY),
    CF_POSZ_BITMASK = (1 << CF_POSZ),
    CF_NEGZ_BITMASK = (1 << CF_NEGZ),

    CF_INVALID = 0xFF
  };

  typedef std::bitset<CF_MAX> CubeFaceBitSet;

  class Point
  {
 public:
 Point() : theta(0), phi(0) { computePosition(); }

 Point(float theta_, float phi_)
     : theta(theta_), phi(phi_)
    {
      computePosition();
    }

    Point(const Vector3& direction)
    {
      assignDirection(direction);
    }

    Point(float x, float y, float z)
    {
      assignDirection(Vector3(x, y, z));
    }


    void calculateCubeSet() {

    }


    //NOTE: Redefinition when using float instead of double
    /*
      Point(const F3& pos)
      : Point(pos.x(), pos.y(), pos.z)
      {
      }
    */

    float theta;
    float phi;
    Vector3 position;

    std::tuple<Vector3, CubeFaceBitSet> cubeCoord() const;

    Vector3 tangent() const;
    Vector3 binormal() const;

    void assignDirection(const Vector3& direction)
    {
      float r = length(direction);
      //float r = direction.length();
      assert(r > 0);
      theta = acos(Magnum::Math::clamp<float>(direction.z() / r, -1.0, 1.0));
      auto theta2 = acos(clamp<float>(direction.z() / r, -1.0, 1.0));
      assert(theta == theta2);

      phi = atan2(direction.y(), direction.x());
      position = direction / r;
    }

    float sphericalDistance(const Point& p2) const
    {
      float dot = Magnum::Math::dot(position, p2.position);
      float result = acos(clamp<float>(dot, -1.0, 1.0));
      return result;
    }

    bool equalWithEps(const Point& p2, float eps) const
    {
      return std::abs(position.x() - p2.position.x()) < eps &&
          std::abs(position.y() - p2.position.y()) < eps &&
          std::abs(position.z() - p2.position.z()) < eps;
    }

    bool operator < (const Point& p2) const
    {
      return (theta < p2.theta) || (theta == p2.theta && phi < p2.phi);
    }

    friend std::ostream& operator << (std::ostream& stream, const Point& p)
    {
      return stream << p.theta << "," << p.phi;
    }

    template <class Archive>
        void serialize(Archive& ar)
    {
      ar(theta, phi, position.x(), position.y(), position.z());
    }

 private:
    void computePosition()
    {
      position = Vector3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    }
  };

  namespace Util
  {
    extern std::vector<Vector3> splitSphericalLineSegment(const Point& start, const Point& end, float deltaAngle = M_PI / 180.0);
    extern float lagrangeInterpolate(float x, const std::vector<float>& xArray, const std::vector<float>& yArray);
    extern float interpolateSphericalSamples(const Point& p0, const std::vector<Point>& points, const std::vector<float>& values);
    extern float computeTriangleArea(const Vector3& p0, const Vector3& p1, const Vector3& p2);

    extern void faceAxisDirection(ECubeFace face, Vector3& s_dir, Vector3& t_dir, Vector3& p_dir);

    inline float sqrDistance(Vector2 a, Vector2 b)
    {
      auto p = a - b;
      return dot(p, p);
    }
  }

  class SphericalLine
  {
 public:
 SphericalLine() : direction(Vector3(0, 0, 1)), xi(0) {}
 SphericalLine(const Vector3& direction_, float xi_) : direction(normalize(direction_)), xi(xi_) {}

    Vector3 direction;
    float xi;
  };

  class AABB
  {
 public:
    AABB();
    AABB(const Vector3& p);
    AABB(const Vector3& min, const Vector3& max);

    void reset();
    bool isValid() const;
    bool isEmpty() const;

    const Vector3& min() const { assert(isValid()); return m_min; }
    const Vector3& max() const { assert(isValid()); return m_max; }

    Vector3 center() const { assert(isValid()); return (m_min + m_max) * 0.5f; }
    Vector3 size() const { assert(isValid()); return m_max - m_min; }
    Vector3 extent() const { return size() * 0.5f; }

    void getMajorVertices(const Vector3& direction, Vector3& P, Vector3& N) const;

    void unionWith(const Vector3& p);
    void unionWith(const AABB& aabb);
    bool contains(const Vector3& p) const;
    bool contains(const AABB& aabb) const;

    bool operator == (const AABB& aabb) const;
 private:
    Vector3 m_min, m_max;
  };

  class Ray
  {
 public:
    Ray();
    Ray(const Vector3& origin, const Vector3& direction);

    inline const Vector3& origin() const { return m_origin; }
    inline void setOrigin(const Vector3& origin) { m_origin = origin; }
    inline const Vector3& direction() const { return m_direction; }
    inline void setDirection(const Vector3& direction) { m_direction = normalize(direction); }
    inline void setNormalizedDirection(const Vector3& direction) { m_direction = direction; }

 private:
    Vector3 m_origin;
    Vector3 m_direction;
  };

  class Plane
  {
 public:
    Plane();
    Plane(const Plane& other);
    Plane(const Vector3& normal, float distance);
    Plane(const Vector4& vec);
    Plane(const Vector3& a, const Vector3& b, const Vector3& c);

    const Vector3& normal() const { return m_normal; }
    void setNormal(const Vector3& normal) { m_normal = normal; }
    float distance() const { return m_distance; }
    void setDistance(float distance) { m_distance = distance; }

    Plane normalize() const;
    Plane transform(const Matrix4 transform) const;
    float distance(const Vector3& point) const;
    bool pointOnSide(const Vector3& point) const;
    bool lineIntersection(const Vector3& ptA, const Vector3& ptB, Vector3& resultDestination) const;

 private:
    Vector3 m_normal;
    float m_distance;
  };

  bool threePlanesIntersection(const Plane& planeA, const Plane& planeB, const Plane& planeC, Vector3& result);

  bool rayAabbIntersection(const Ray& ray, const AABB& aabb);

  template <typename T>
      class PositionT
  {
 public:
    //using Vec3 = tvec3<T>;
    using Vector3 = Magnum::Math::Vector3<T>;
    PositionT();
    PositionT(ECubeFace face, T s, T t, T p);
    PositionT(ECubeFace face, const Vector3& stp);

    ECubeFace face() const { return m_face; }
    const Vector3& surfacePoint() const { return m_surfacePoint; }
    Vector3 stpCoords() const;
    const Vector3& spacePosition() const { return m_spacePosition; }

 private:
    ECubeFace m_face;
    T m_height;
    Vector3 m_surfacePoint;
    Vector3 m_spacePosition;
  };

  typedef PositionT<float> PositionF;
  typedef PositionT<float> Position;
  typedef PositionT<double> PositionD;
}
}


#include "Math.hpp"
