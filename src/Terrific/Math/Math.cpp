
#include <Terrific/Math/Types.h>
#include <Terrific/Math/Math.h>

namespace Terrific {
namespace  Math {

std::tuple<Vector3, CubeFaceBitSet> Point::cubeCoord() const {
  Vector3 absPos = Magnum::Math::abs(position);
  Vector3 result;
  CubeFaceBitSet faceSet;
  float ratio;
  if (absPos.z() >= fmax(absPos.x(), absPos.y()))
  {
    ratio = 1.0 / absPos.z();
    faceSet |= (position.z() > 0 ? CF_POSZ_BITMASK : CF_NEGZ_BITMASK);
    if (absPos.z() == absPos.x())
    {
      faceSet |= (position.x() > 0 ? CF_POSX_BITMASK : CF_NEGX_BITMASK);
    }
    if (absPos.z() == absPos.y())
    {
      faceSet |= (position.y() > 0 ? CF_POSY_BITMASK : CF_NEGY_BITMASK);
    }
  }
  else if (absPos.y() >= fmax(absPos.x(), absPos.z()))
  {
    ratio = 1.0 / absPos.y();
    faceSet |= (position.y() > 0 ? CF_POSY_BITMASK : CF_NEGY_BITMASK);
    if (absPos.y() == absPos.x())
    {
      faceSet |= (position.x() > 0 ? CF_POSX_BITMASK : CF_NEGX_BITMASK);
    }
    if (absPos.y() == absPos.z())
    {
      faceSet |= (position.z() > 0 ? CF_POSZ_BITMASK : CF_NEGZ_BITMASK);
    }
  }
  else if (absPos.x() >= fmax(absPos.y(), absPos.z()))
  {
    ratio = 1.0 / absPos.x();
    faceSet |= (position.x() > 0 ? CF_POSX_BITMASK : CF_NEGX_BITMASK);
    if (absPos.x() == absPos.y())
    {
      faceSet |= (position.y() > 0 ? CF_POSY_BITMASK : CF_NEGY_BITMASK);
    }
    if (absPos.x() == absPos.z())
    {
      faceSet |= (position.z() > 0 ? CF_POSZ_BITMASK : CF_NEGZ_BITMASK);
    }
  }
  result = position * ratio;
  return std::tie(result, faceSet);
}

Vector3 Point::tangent() const {
  CubeFaceBitSet bitSet;

  Vector3 binormal(1, 0, 0);

  if (bitSet.test(CF_POSX))
  {
    binormal = Vector3(0, 1, 0);
  }
  else if (bitSet.test(CF_NEGX))
  {
    binormal = Vector3(0, 1, 0);
  }
  else if (bitSet.test(CF_POSY))
  {
    binormal = Vector3(1, 0, 0);
  }
  else if (bitSet.test(CF_NEGY))
  {
    binormal = Vector3(-1, 0, 0);
  }
  else if (bitSet.test(CF_POSZ))
  {
    binormal = Vector3(0, 1, 0);
  }
  else if (bitSet.test(CF_NEGZ))
  {
    binormal = Vector3(0, 1, 0);
  }
  else
  {
    assert(false);
  }

  Vector3 normal = position;
  Vector3 result = normalize(cross(binormal, normal));
  return result;
}

Vector3 Point::binormal() const {
  Vector3 t = tangent();
  Vector3 normal = position;
  Vector3 result = normalize(cross(normal, t));
  return result;
}

// http://www.cgafaq.info/wiki/Intersection_of_three_planes
bool threePlanesIntersection(const Plane& planeA, const Plane& planeB, const Plane& planeC, Vector3& result) {
  Vector3 bcCross = cross(planeB.normal(), planeC.normal());
  float denom = dot(planeA.normal(), bcCross);

  if (denom == 0) {
    result = Vector3(0);
    return false;
  }
  else {
    result = (-planeA.distance() * bcCross
              - planeB.distance() * cross(planeC.normal(), planeA.normal())
              - planeC.distance() * cross(planeA.normal(), planeB.normal())) / denom;
    return true;
  }
}

// http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
bool rayAabbIntersection(const Ray& ray, const AABB& aabb) {
  Vector3 n_inv = Vector3(1.0) / ray.direction();

  double tx1 = (aabb.min().x() - ray.origin().x())*n_inv.x();
  double tx2 = (aabb.max().x() - ray.origin().x())*n_inv.x();

  double tmin = fmin(tx1, tx2);
  double tmax = fmax(tx1, tx2);

  double ty1 = (aabb.min().y() - ray.origin().y())*n_inv.y();
  double ty2 = (aabb.max().y() - ray.origin().y())*n_inv.y();

  tmin = fmax(tmin, fmin(ty1, ty2));
  tmax = fmin(tmax, fmax(ty1, ty2));

  double tz1 = (aabb.min().z() - ray.origin().z())*n_inv.z();
  double tz2 = (aabb.max().z() - ray.origin().z())*n_inv.z();

  tmin = fmax(tmin, fmin(tz1, tz2));
  tmax = fmin(tmax, fmax(tz1, tz2));

  return tmax >= fmax(tmin, 0.0);
}

std::vector<Vector3> splitSphericalLineSegment(const Point& start, const Point& end, float deltaAngle) {
  std::vector<Vector3> result;

  assert(start.position != -end.position);

  auto direction = normalize(cross(start.position, end.position));
  float distance = acos(dot(start.position, end.position));

  result.push_back(start.position);

  for (float angle = deltaAngle; angle<distance; angle+=deltaAngle)
  {
    //Mat4 rotation = rotate(Mat4(1.0), angle, direction);
    Matrix4 rotation = Matrix4::rotation(Rad(angle), direction);
    //Vector3 pos = normalize(Vector3(rotation * Vector4(start.position, 1.0)));
    Vector3 pos = (rotation * Vector4(start.position, 1.f)).xyz().normalized(); //NOTE: .xyz() converts Vec4 to Vec3 as per glm semantics.

    result.push_back(pos);
  }

  result.push_back(end.position);

  return result;
}

float lagrangeInterpolate(float x, const std::vector<float>& xArray, const std::vector<float>& yArray) {
  assert(xArray.size() == yArray.size());

  float sum = 0.0;
  for (unsigned int i = 0; i < xArray.size(); ++i)
  {
    float Xi, Yi;
    Xi = xArray[i];
    Yi = yArray[i];
    float factor = 1.0;
    for (unsigned int j = 0; j < xArray.size(); ++j)
    {
      if (i != j)
      {
        float Xj = xArray[j];
        factor *= (x - Xj) / (Xi - Xj);
      }
    }
    sum += factor * Yi;
  }
  return sum;
}

float interpolateSphericalSamples(const Point& p0, const std::vector<Point>& points, const std::vector<float>& values) {
  float totalSqrDistance = std::accumulate(points.begin(), points.end(), 0.0, [p0](float res, const Point& p) {
                                                                                float d = p.sphericalDistance(p0);
                                                                                return res + d * d;
                                                                              });

  float sum = 0.0;
  float weight = 0.0;

  for (size_t i = 0; i < points.size(); ++i)
  {
    const Point& p = points[i];
    float d = p.sphericalDistance(p0);
    float w = (totalSqrDistance - d*d) / totalSqrDistance;
    sum += w * values[i];
    weight += w;
  }
  return sum / weight;
}

float computeTriangleArea(const Vector3& p0, const Vector3& p1, const Vector3& p2) {
  Vector3 v12 = p2 - p1;
  Vector3 v02 = p2 - p0;
  Vector3 v12n = normalize(v12);
  float t = dot(v02, v12n);
  Vector3 c = p2 - v12n * t;
  float d = distance(p0, c);
  float l12 = length(v12);
  return l12 * d * 0.5;
}

void faceAxisDirection(ECubeFace face, Vector3& s_dir, Vector3& t_dir, Vector3& p_dir) {
  switch (face)
  {
    case CF_POSX:
      p_dir = Vector3(1, 0, 0);
      s_dir = Vector3(0, 0, -1);
      t_dir = Vector3(0, 1, 0);
      break;
    case CF_NEGX:
      p_dir = Vector3(-1, 0, 0);
      s_dir = Vector3(0, 0, 1);
      t_dir = Vector3(0, 1, 0);
      break;
    case CF_POSY:
      p_dir = Vector3(0, 1, 0);
      s_dir = Vector3(0, 0, 1);
      t_dir = Vector3(1, 0, 0);
      break;
    case CF_NEGY:
      p_dir = Vector3(0, -1, 0);
      s_dir = Vector3(0, 0, 1);
      t_dir = Vector3(-1, 0, 0);
      break;
    case CF_POSZ:
      p_dir = Vector3(0, 0, 1);
      s_dir = Vector3(1, 0, 0);
      t_dir = Vector3(0, 1, 0);
      break;
    case CF_NEGZ:
      p_dir = Vector3(0, 0, -1);
      s_dir = Vector3(-1, 0, 0);
      t_dir = Vector3(0, 1, 0);
      break;
    default:
      assert(0);
      p_dir = Vector3(1, 0, 0);
      s_dir = Vector3(0, 0, -1);
      t_dir = Vector3(0, 1, 0);
  }
}
}
}




