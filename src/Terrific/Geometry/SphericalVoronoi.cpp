#include "SphericalVoronoi.h"

#include <iostream>

namespace Terrific {
namespace Geometry {

using std::cout;
using std::endl;

Cell::Cell (uint32_t i, Point const &p) : index(i), point(p) {}
void Cell::reset() {
  halfEdges.clear();
}

Vertex::Vertex(Point const &p, Cell_ptr c0, Cell_ptr c1) : point(p) {
  cells.insert(c0);
  cells.insert(c1);
}
Vertex::Vertex(Point const &p, Cell_ptr c0, Cell_ptr c1, Cell_ptr c2) : point(p) {
  cells.insert(c0);
  cells.insert(c1);
  cells.insert(c2);
}

void Vertex::reset() {
  halfEdges.clear();
  cells.clear();
}


HalfEdge::HalfEdge(std::shared_ptr<Vertex> s, std::shared_ptr<Vertex> e)
    : start(s), end(e) {}
void HalfEdge::reset() {
  pCell.reset();
  start.reset();
  end.reset();
  prev.reset();
  next.reset();
  twin.reset();
}



BeachArc::BeachArc(std::shared_ptr<Cell> Cell_) : pCell(Cell_) {}

std::shared_ptr<Cell> pCell;

std::shared_ptr<CircleEvent> circleEvent;      // the related circle event

std::shared_ptr<Vertex> startVertex;

bool BeachArc::operator< (const BeachArc& ba) const {
  return pCell->point.theta < ba.pCell->point.phi; // Theta and Phi
}

SiteEvent::SiteEvent(std::shared_ptr<Cell> Cell_)
    : pCell(Cell_)
  {
    theta = pCell->point.theta;
    phi = pCell->point.phi;
  }

bool SiteEvent::operator< (const SiteEvent& right) const
{
  return (theta < right.theta) || (theta == right.theta && phi < right.phi);
}

bool SiteEvent::operator>=(const SiteEvent& right) const {
  return (theta > right.theta) || (theta == right.theta && phi >= right.phi);
}

CircleEvent::CircleEvent(const BeachArc_ptr& arc_i_, const BeachArc_ptr& arc_j_, const BeachArc_ptr& arc_k_)
    : arc_i(arc_i_), arc_j(arc_j_), arc_k(arc_k_) {
  auto pij = cell_i()->point.position - cell_j()->point.position;
  auto pkj = cell_k()->point.position - cell_j()->point.position;
  auto direction = cross(pij, pkj);
  circle_center = Point(direction);
  circle_radius = acos(dot(circle_center.position, cell_i()->point.position));
  theta = acos(circle_center.position.z()) + circle_radius;
}

bool CircleEvent::operator< (const CircleEvent& ce) const {
  return theta < ce.theta;
}

bool compare_CircleEvent_priority::operator()(const std::shared_ptr<CircleEvent>& left, const std::shared_ptr<CircleEvent>& right) const
{
  //          return *left < *right; //NOTE: Not sure
    return *right < *left;
  }

void SphericalVoronoi::setDebugMode(bool debugMode) { this->debugMode = debugMode; }

//const std::vector<HalfEdge_ptr>& SphericalVoronoi::getHalfEdges() const { return halfEdges; }
//const std::vector<Vertex_ptr>& SphericalVoronoi::getVertices() const { return vertices; }
//const std::vector<Cell_ptr>& SphericalVoronoi::getCells() const { return cells; }

beach_type::const_iterator SphericalVoronoi::getPrevArcOnBeach(beach_type::const_iterator it) const
{
  if (it != beach.begin())
  {
    return std::prev(it);
  }
  else
  {
    return std::prev(beach.end());
  }
}

beach_type::const_iterator SphericalVoronoi::getNextArcOnBeach(beach_type::const_iterator it) const
{
  auto next = std::next(it);
  if (next == beach.end())
  {
    next = beach.begin();
  }
  return next;
}


bool SphericalVoronoi::isArcOnBeach(const BeachArc_ptr& arc) const
{
  return std::find(beach.begin(), beach.end(), arc) != beach.end();
}


void SphericalVoronoi::addNewSiteEvent(const SiteEvent& event)
{
  /*
    using namespace std;
    auto it = lower_bound(siteEventQueue.begin(), siteEventQueue.end(), event);
    siteEventQueue.insert(it, event);
  */
  siteEventQueue.push_back(event);
  std::push_heap(siteEventQueue.begin(), siteEventQueue.end(), std::greater_equal<>{});
}

void SphericalVoronoi::addNewCircleEvent(const std::shared_ptr<CircleEvent>& event)
{
  /*
    using namespace std;
    auto it = lower_bound(circleEventQueue.begin(), circleEventQueue.end(), event, compare_CircleEvent_priority());
    circleEventQueue.insert(it, event);
  */
  circleEventQueue.push_back(event);
  std::push_heap(circleEventQueue.begin(), circleEventQueue.end(), compare_CircleEvent_priority{});
}

void SphericalVoronoi::removeCircleEvent(const std::shared_ptr<CircleEvent>& event)
{
  /*
    using namespace std;
    auto it = find(circleEventQueue.begin(), circleEventQueue.end(), event);
    assert(it != circleEventQueue.end());
    circleEventQueue.erase(it);
  */
  circleEventDeletedEvents.insert(event);
}

std::shared_ptr<CircleEvent> SphericalVoronoi::getCircleEvent() {
  while (circleEventQueue.size()) {
    auto it = circleEventDeletedEvents.find(circleEventQueue[0]);
    if (it != circleEventDeletedEvents.end()) {
      // We have an event that is meant to be deleted, remove it
      std::pop_heap(circleEventQueue.begin(), circleEventQueue.end(), compare_CircleEvent_priority{});
      circleEventQueue.pop_back();
      circleEventDeletedEvents.erase(it);
    } else {
      return circleEventQueue[0];
    }
  }
  return nullptr;
}
void SphericalVoronoi::popCircleEvent() {
  while (circleEventQueue.size()) {
    auto it = circleEventDeletedEvents.find(circleEventQueue[0]);
    std::pop_heap(circleEventQueue.begin(), circleEventQueue.end(), compare_CircleEvent_priority{});
    circleEventQueue.pop_back();
    if (it != circleEventDeletedEvents.end()) {
      // We have an event that is meant to be deleted, remove it and repeat
      circleEventDeletedEvents.erase(it);
    } else {
      return;
    }
  }
}

size_t SphericalVoronoi::getCircleEventQueueSize() const {
  return circleEventQueue.size() - circleEventDeletedEvents.size();
}

#define SV_DEBUG(...)   do { if (debugMode) { __VA_ARGS__; } } while(0)

bool comparePhi(float const lhs, float const rhs) {
  return(static_cast<int>(lhs*1000.0) == static_cast<int>(rhs*1000.0));
}

namespace
{
struct IndexedDirection
{
  IndexedDirection(const Vector3& dir, int id)
      : direction(dir)
      , index(id)
  {
  }

  Vector3 direction;
  int index;
};
}


SphericalVoronoi::SphericalVoronoi(const std::vector<Vector3>& directions)
    : scanLine(), nbSteps(0), debugMode(false)
{
  using namespace std;

  std::vector<std::pair<int, Vector3>> sortedDirections;
  for (auto& dir : directions)
  {
    sortedDirections.push_back(std::pair<int, Vector3>(sortedDirections.size(), dir));
  }

  sort(sortedDirections.begin(), sortedDirections.end(), [](const std::pair<int, Vector3>& a, const std::pair<int, Vector3>& b) { return a.second.z() > b.second.z(); });
  for (size_t i=0; i<sortedDirections.size(); ++i)
  {
    auto& d = sortedDirections[i].second;
    if (Math::length(d) < eps) continue;
    Point p(d);
    //SV_DEBUG(std::cout << "Position: ("<< d.x() << ", " << d.y() << ", "<< d.z() << ")\n");
    for (auto& cell : cells)
    {
      if (cell->point.equalWithEps(p, eps))
      {
        continue;
      }
#if 0 //TODO: Remove this after debugging
      if(cell->index == 54) {
        //assert(cell->point.phi == 0.775055f);
        //assert(comparePhi(cell->point.phi, 0.775055f));
        std::cout << cell->point.phi << std::endl;
        assert(!comparePhi(cell->point.phi, 0.795741f));
        //assert(cell->point.phi != 0.795741f);
      }
#endif
    }

    auto c = shared_ptr<Cell>(new Cell(sortedDirections[i].first, p));
    //SV_DEBUG(std::cout << "\t index: " << c->index << ",\t phi: " << c->point.phi << "\n");

    //std::cout << "\t position: "  << c->point.position.y() << "\n";
    cells.push_back(c);

    addNewSiteEvent(SiteEvent(c));
  }
}

bool SphericalVoronoi::isFinished() const
{
  //return siteEventQueue.size() == 0 && circleEventQueue.size() == 0;
  return siteEventQueue.size() == 0 && getCircleEventQueueSize() == 0;
}

void SphericalVoronoi::dumpBeachState(std::ostream& stream)
{
  using namespace std;

  stream << "  beach [";
  for (auto itArc=beach.begin(); itArc!=beach.end(); ++itArc)
  {
    auto arc = *itArc;
    stream << arc->pCell->index;
    if (arc->circleEvent)
    {
      auto itPrevArc = getPrevArcOnBeach(itArc);
      auto itNextArc = getNextArcOnBeach(itArc);
      assert(arc->circleEvent->arc_j == arc);
      assert(arc->circleEvent->arc_i == *itPrevArc);
      assert(arc->circleEvent->arc_k == *itNextArc);
      stream << "*";
    }
    stream << ",";
  }
  stream << "]" << endl;
}

void SphericalVoronoi::step(float maxDeltaXi)
{
  using namespace std;

  if (!isFinished())
  {
    float nextXi = scanLine.xi + maxDeltaXi;

    SV_DEBUG(cout << "step " << nbSteps << " " << scanLine.xi);

    //if (siteEventQueue.size() > 0 && (circleEventQueue.size() == 0 || siteEventQueue[0].theta < circleEventQueue[0]->theta))
    if (siteEventQueue.size() > 0 && (getCircleEventQueueSize() == 0 || siteEventQueue[0].theta < getCircleEvent()->theta))
    {
      //auto se = siteEventQueue[0];
      std::pop_heap(siteEventQueue.begin(), siteEventQueue.end(), std::greater_equal<>{});
      auto se = siteEventQueue.back();
      if (se.theta <= nextXi)
      {
        scanLine.xi = se.theta;
        SV_DEBUG(cout << " -> " << scanLine.xi << endl);
        SV_DEBUG(dumpBeachState(cout));
        handleSiteEvent(se);
        //siteEventQueue.erase(siteEventQueue.begin());
        siteEventQueue.pop_back();
        SV_DEBUG(dumpBeachState(cout));
      }
      else
      {
        scanLine.xi = nextXi;
        SV_DEBUG(cout << " -> " << scanLine.xi << endl);
      }
    }
    //else if (circleEventQueue.size() > 0)
    else if (getCircleEventQueueSize() > 0)
    {
      // auto ce = circleEventQueue[0];
      auto ce = getCircleEvent();
      if (ce->theta <= nextXi)
      {
        scanLine.xi = ce->theta;
        SV_DEBUG(cout << " -> " << scanLine.xi << endl);
        SV_DEBUG(dumpBeachState(cout));
        handleCircleEvent(ce);
        //circleEventQueue.erase(circleEventQueue.begin());
        popCircleEvent();
        SV_DEBUG(dumpBeachState(cout));
      }
      else
      {
        scanLine.xi = nextXi;
        SV_DEBUG(cout << " -> " << scanLine.xi << endl);
      }
    }
    else
    {
      scanLine.xi = nextXi;
      SV_DEBUG(cout << " -> " << scanLine.xi << endl);
    }

    if (isFinished())
    {
      finializeGraph();
    }

    if (debugMode)
    {
      SV_DEBUG(cout << "=================" << endl);
    }

    ++ nbSteps;
  }
}

void SphericalVoronoi::solve(std::function<void(int)> cb)
{
  int nbSteps = 0;
  while (!isFinished())
  {
    step(M_PI * 4);
    ++nbSteps;
    if (cb)
    {
      cb(nbSteps);
    }
  }
}

void SphericalVoronoi::finializeGraph()
{
  using namespace std;

  SV_DEBUG(cout << "Finalize graph" << endl);

  for (auto& edge: halfEdges)
  {
    auto v0 = edge->start;
    auto v1 = edge->end;
    v0->halfEdges.push_back(edge);
  }

  cleanupMiddleVertices();
  duplicateHalfEdges();
  bindHalfEdgesToCells();

  for (size_t i=0; i<halfEdges.size(); ++i)
  {
    halfEdges[i]->index = uint32_t(i);
  }

  for (size_t i=0; i<vertices.size(); ++i)
  {
    vertices[i]->index = uint32_t(i);
  }
}

void SphericalVoronoi::cleanupMiddleVertices()
{
  using namespace std;

  SV_DEBUG(cout << "cleanupMiddleVertices" << endl);

  std::vector<Vertex_ptr> deleteVertices;
  std::vector<HalfEdge_ptr> deleteHalfEdges;

  for (auto& v0 : vertices)
  {
    if (v0->cells.size() == 2)
    {
      assert(v0->halfEdges.size() == 2);
      auto v1 = v0->halfEdges[0]->end;
      auto v2 = v0->halfEdges[1]->end;

      auto newEdge = make_shared<HalfEdge>(v1, v2);
      v1->halfEdges.push_back(newEdge);
      halfEdges.push_back(newEdge);

      deleteVertices.push_back(v0);
      deleteHalfEdges.push_back(v0->halfEdges[0]);
      deleteHalfEdges.push_back(v0->halfEdges[1]);
    }
  }

  SV_DEBUG(cout << "Vertices to delete : " << deleteVertices.size() << endl);
  SV_DEBUG(cout << "Half edges to delete : " << deleteHalfEdges.size() << endl);

  for (auto& v : deleteVertices)
  {
    vertices.erase(find(vertices.begin(), vertices.end(), v));
  }

  for (auto& edge : deleteHalfEdges)
  {
    halfEdges.erase(find(halfEdges.begin(), halfEdges.end(), edge));
  }
}

void SphericalVoronoi::duplicateHalfEdges()
{
  using namespace std;

  SV_DEBUG(cout << "duplicateHalfEdges" << endl);

  vector<HalfEdge_ptr> newHalfEdges;

  for (auto& edge: halfEdges)
  {
    auto newEdge = make_shared<HalfEdge>(edge->end, edge->start);
    edge->twin = newEdge;
    newEdge->twin = edge;
    edge->end->halfEdges.push_back(newEdge);
    newHalfEdges.push_back(newEdge);
  }

  copy(newHalfEdges.begin(), newHalfEdges.end(), back_inserter(halfEdges));
}

void SphericalVoronoi::bindHalfEdgesToCells()
{
  using namespace std;

  SV_DEBUG(cout << "bindHalfEdgesToCells" << endl);

  for (auto& e : halfEdges)
  {
    e->pCell.reset();
    e->next.reset();
    e->prev.reset();
  }

  for (auto& e : halfEdges)
  {
    vector<Cell_ptr> common;
    std::set_intersection(e->start->cells.begin(), e->start->cells.end(), e->end->cells.begin(), e->end->cells.end(), back_inserter(common));
    assert(common.size() == 2);
    //if(common.size() == 2) common.pop_back();
    Cell_ptr c0 = common[0];
    Vector3 d0 = e->start->point.position - c0->point.position;
    Vector3 d1 = e->end->point.position - c0->point.position;
    Vector3 n = cross(d0, d1);
    Cell_ptr c = nullptr;
    if (dot(n, c0->point.position) > 0)
    {
      c = c0;
    }
    else
    {
      c = common[1];
    }

    e->pCell = c;
    c->halfEdges.push_back(e);
  }

  for (auto& c : cells)
  {
    vector<HalfEdge_ptr> potentialEdges(c->halfEdges);
    c->halfEdges.clear();

    for (auto& e : potentialEdges)
    {
      for (auto& e1 : potentialEdges)
      {
        if (e1 == e) continue;

        if (e->end == e1->start)
        {
          e->next = e1;
          e1->prev = e;
        }
      }
    }

    auto e = potentialEdges[0];
    while (potentialEdges.size() > 0)
    {
      c->halfEdges.push_back(e);
      potentialEdges.erase(find(potentialEdges.begin(), potentialEdges.end(), e));
      e = e->next;
    }

    assert(e == c->halfEdges[0]);
  }

  for (auto& e : halfEdges)
  {
    e->otherCell = e->twin->pCell;
  }
}



bool SphericalVoronoi::arcsIntersection(const BeachArc& arc1, const BeachArc& arc2, float xi, Point& oPoint)
{
  float theta1 = arc1.pCell->point.theta;
  float phi1 = arc1.pCell->point.phi;

  float theta2 = arc2.pCell->point.theta;
  float phi2 = arc2.pCell->point.phi;

  if (theta1 >= xi)
  {
    if (theta2 >= xi)
    {
      return false;
    }
    else
    {
      Point pt = phiToPoint(phi1, xi, theta2, phi2);
      oPoint = pt;
      return true;
    }
  }
  if (theta2 >= xi)
  {
    if (theta1 >= xi)
    {
      return false;
    }
    else
    {
      Point pt = phiToPoint(phi2, xi, theta1, phi1);
      oPoint = pt;
      return true;
    }
  }

  float cos_xi = cos(xi);
  float sin_xi = sin(xi);
  float cos_theta1 = cos(theta1);
  float sin_theta1 = sin(theta1);
  float cos_theta2 = cos(theta2);
  float sin_theta2 = sin(theta2);
  float cos_phi1 = cos(phi1);
  float sin_phi1 = sin(phi1);
  float cos_phi2 = cos(phi2);
  float sin_phi2 = sin(phi2);
  float a1 = (cos_xi - cos_theta2) * sin_theta1 * cos_phi1;
  float a2 = (cos_xi - cos_theta1) * sin_theta2 * cos_phi2;
  float a = a1 - a2;
  float b1 = (cos_xi - cos_theta2) * sin_theta1 * sin_phi1;
  float b2 = (cos_xi - cos_theta1) * sin_theta2 * sin_phi2;
  float b = b1 - b2;
  float c = (cos_theta1 - cos_theta2) * sin_xi;
  float l = sqrt(a*a + b*b);
  if (abs(a) > l || abs(c) > l)
  {
    return false;
  }
  else
  {
    auto gamma = atan2(a, b);
    auto sin_phi_int_plus_gamma_1 = c / l;

    float phi_int_plus_gamma_1 = asin(sin_phi_int_plus_gamma_1);

    auto pA = phi_int_plus_gamma_1 - gamma;

    Point ptA_1 = phiToPoint(pA, xi, theta1, phi1);

    oPoint = ptA_1;
    if (oPoint.phi > M_PI)
    {
      oPoint.phi -= M_PI * 2;
    }
        if (oPoint.phi < -M_PI)
        {
          oPoint.phi += M_PI * 2;
        }
        return true;
      }
    }

    bool SphericalVoronoi::intersectWithNextArc(beach_type::const_iterator itArc, float xi, Point& oPoint) const
    {
        auto itNextArc = getNextArcOnBeach(itArc);
        if (itNextArc == itArc)
        {
            return false;
        }
        bool result = arcsIntersection(**itArc, **itNextArc, xi, oPoint);
        return result;
    }

    bool SphericalVoronoi::intersectWithPrevArc(beach_type::const_iterator itArc, float xi, Point& oPoint) const
    {
        auto itPrevArc = getPrevArcOnBeach(itArc);
        if (itPrevArc == itArc)
        {
            return false;
        }
        bool result = arcsIntersection(**itPrevArc, **itArc, xi, oPoint);
        return result;
    }

void SphericalVoronoi::handleSiteEvent(SiteEvent& event)
{
  using namespace std;

  SV_DEBUG(cout << "HandleSiteEvent " << event << endl);

  if (beach.size() == 0)
  {
    beach.emplace_back(make_shared<BeachArc>(event.pCell));
  }
  else if (beach.size() == 1)
  {
    beach.emplace_back(make_shared<BeachArc>(event.pCell));
    auto arc = beach[0];
    auto newArc = beach[1];
    Point p = phiToPoint(event.phi, scanLine.xi, arc->pCell->point.theta, arc->pCell->point.phi);
    std::shared_ptr<Vertex> newVertex = std::shared_ptr<Vertex>(new Vertex(p, event.pCell, arc->pCell));
    vertices.push_back(newVertex);
    arc->startVertex = newVertex;
    newArc->startVertex = newVertex;
  }
  else
  {
    bool intersectFound = false;
    for (beach_type::const_iterator itArc=beach.begin(); itArc!=beach.end(); ++itArc)
    {
      auto arc = *itArc;
      beach_type::const_iterator itPrevArc = getPrevArcOnBeach(itArc);
      auto prevArc = *itPrevArc;
      beach_type::const_iterator itNextArc = getNextArcOnBeach(itArc);
      auto nextArc = *itNextArc;
      /*auto& nextArc = */*itNextArc;

      Point pointPrev, pointNext;
      bool intPrev, intNext;
      //intPrev = intersectWithPrevArc(itArc, scanLine.xi, pointPrev);
      //intNext = intersectWithNextArc(itArc, scanLine.xi, pointNext);
      assert(itArc != itPrevArc);
      assert(itArc != itNextArc);
      intPrev = arcsIntersection(**itPrevArc, **itArc, scanLine.xi, pointPrev);
      intNext = arcsIntersection(**itArc, **itNextArc, scanLine.xi, pointNext);

      float phi_start = arc->pCell->point.phi - M_PI;
      if (intPrev)
      {
        phi_start = pointPrev.phi;
      }

      float phi_end = arc->pCell->point.phi + M_PI;
      if (intNext)
      {
        phi_end = pointNext.phi;
      }

      if (phi_start <= phi_end)
      {
        intersectFound = phi_start <= event.phi && event.phi <= phi_end;
      }
      else
      {
        intersectFound = event.phi < phi_end || event.phi > phi_start;
      }
      if (intersectFound)
      {
        //TODO: handle event.phi == phi_start or phi_end
        auto vertex1 = prevArc->startVertex;
        auto vertex2 = arc->startVertex;

        SV_DEBUG(cout << "Intersect with arc: " << *arc << endl);

        if (arc->circleEvent)
        {
          SV_DEBUG(cout << "  clear circleEvent " << *arc->circleEvent << endl);
          removeCircleEvent(arc->circleEvent);
          arc->circleEvent.reset();
        }
        beach_type::const_iterator itArc2 = beach.insert(itArc, make_shared<BeachArc>(arc->pCell));
        itArc = std::next(itArc2);
        auto arc2 = *itArc2;
        beach_type::const_iterator itNewArc = beach.insert(itArc, make_shared<BeachArc>(event.pCell));
        itArc = std::next(itNewArc);
        auto newArc = *itNewArc;

        Point p = phiToPoint(event.phi, scanLine.xi, arc->pCell->point.theta, arc->pCell->point.phi);
        std::shared_ptr<Vertex> newVertex = std::shared_ptr<Vertex>(new Vertex(p, event.pCell, arc->pCell));
        vertices.push_back(newVertex);

        arc2->startVertex = newArc->startVertex = newVertex;

        SV_DEBUG(cout << "  after insert arc: {" << *arc2 << "}, {" << *newArc << "}, {" << *arc << "}, {" << *nextArc << "}" << endl);

        // refresh all iterators
        itPrevArc = find(beach.begin(), beach.end(), prevArc);
        itArc2 = getNextArcOnBeach(itPrevArc);
        itNewArc = getNextArcOnBeach(itArc2);
        itArc = getNextArcOnBeach(itNewArc);
        //itNextArc = getNextArcOnBeach(itNextArc);
        itNextArc = getNextArcOnBeach(itArc);

        auto ce1 = std::make_shared<CircleEvent>(prevArc, arc2, newArc);
        if (ce1->theta >= event.theta)
        {
                        arc2->circleEvent = ce1;
                        //itArc2->startVertex = newVertex;
                        SV_DEBUG(cout << "  create new circleEvent " << *ce1 << endl);
                        addNewCircleEvent(ce1);
                    }

        auto ce2 = std::make_shared<CircleEvent>(newArc, arc, nextArc);
        if (ce2->theta >= event.theta)
        {
          arc->circleEvent = ce2;
          //itArc->startVertex = newVertex;
          SV_DEBUG(cout << "  create new circleEvent " << *ce2 << endl);
          addNewCircleEvent(ce2);
        }

        if (prevArc->circleEvent)
        {
          SV_DEBUG(cout << "  refresh circleEvent " << *prevArc->circleEvent << endl);
          removeCircleEvent(prevArc->circleEvent);
          prevArc->circleEvent.reset();
          auto itPrevPrevArc = getPrevArcOnBeach(itPrevArc);
          auto ce = std::make_shared<CircleEvent>(*itPrevPrevArc, *itPrevArc, *itArc2);
          addNewCircleEvent(ce);
          prevArc->circleEvent = ce;
        }

        break;
                }
            }
            assert(intersectFound);
        }
    }

void SphericalVoronoi::handleCircleEvent(const CircleEvent_ptr& event)
{
  using namespace std;

  SV_DEBUG(cout << "HandleCircleEvent " << *event << endl);

  auto arc_j = event->arc_j;
  auto arc_i = event->arc_i;
  auto arc_k = event->arc_k;

  //        assert(isArcOnBeach(arc_j));
  //        assert(isArcOnBeach(arc_i));
  //        assert(isArcOnBeach(arc_k));

  assert(arc_j->circleEvent == event);
  arc_j->circleEvent.reset();
  if (arc_i->circleEvent)
  {
    SV_DEBUG(cout << "remove circleEvent " << *arc_i->circleEvent << " from arcI " << *arc_i << endl);
    removeCircleEvent(arc_i->circleEvent);
    arc_i->circleEvent.reset();
  }
  if (arc_k->circleEvent)
  {
    SV_DEBUG(cout << "remove circleEvent " << *arc_k->circleEvent << " from arcK " << *arc_k << endl);
    removeCircleEvent(arc_k->circleEvent);
    arc_k->circleEvent.reset();
  }

  auto newVertex = make_shared<Vertex>(event->circle_center, arc_i->pCell, arc_j->pCell, arc_k->pCell);
  vertices.push_back(newVertex);

  if (arc_i->startVertex)
  {
    auto edge = make_shared<HalfEdge>(arc_i->startVertex, newVertex);
    SV_DEBUG(cout << "  create half_edge for arcI " << *arc_i << "[" << *edge << "]" << endl);
    halfEdges.push_back(edge);
  }

  if (arc_j->startVertex)
  {
    auto edge = make_shared<HalfEdge>(arc_j->startVertex, newVertex);
    SV_DEBUG(cout << "  create half_edge for arcJ " << *arc_j << "[" << *edge << "]" << endl);
    halfEdges.push_back(edge);
  }
  //itArcK->startVertex = newVertex;

  {
    auto it = find(beach.begin(), beach.end(), arc_j);
    assert(it != beach.end());
    SV_DEBUG(cout << "  arc " << *arc_j << " removed from beach");
    beach.erase(it);
  }

  auto itArcI = find(beach.begin(), beach.end(), arc_i);
  //        assert(itArcI != beach.end());
  auto itArcK = find(beach.begin(), beach.end(), arc_k);
  //        assert(itArcK != beach.end());

  if (getPrevArcOnBeach(itArcI) == itArcK)
  {
    if (arc_k->startVertex)
    {
      auto edge = make_shared<HalfEdge>(arc_k->startVertex, newVertex);
      SV_DEBUG(cout << "  create half_edge for arcK " << *arc_k << "[" << *edge << "]" << endl);
      halfEdges.push_back(edge);
    }
    SV_DEBUG(cout << "  arc " << *arc_i << " removed from beach");
    beach.erase(itArcI); itArcI = beach.end();

    itArcK = find(beach.begin(), beach.end(), arc_k);
    //            assert(itArcK != beach.end());
    SV_DEBUG(cout << "  arc " << *arc_k << " removed from beach");
    beach.erase(itArcK); itArcK = beach.end();
  }
  else
  {
    auto itArc1 = getPrevArcOnBeach(itArcI);
    auto itArc2 = getNextArcOnBeach(itArcK);
    auto& arc_1 = *itArc1;
    auto& arc_2 = *itArc2;

    if (arc_1->pCell->index != arc_i->pCell->index && arc_i->pCell->index != arc_k->pCell->index && arc_1->pCell->index != arc_k->pCell->index)
    {
      //                assert(isArcOnBeach(arc_1));
      //                assert(isArcOnBeach(arc_i));
      //                assert(isArcOnBeach(arc_k));

      auto ceI = std::make_shared<CircleEvent>(arc_1, arc_i, arc_k);
      if (ceI->theta >= scanLine.xi)
      {
        SV_DEBUG(cout << "  create new circleEvent " << *ceI << endl);
        arc_i->circleEvent = ceI;
        arc_i->startVertex = newVertex;
        addNewCircleEvent(ceI);
      }
    }

    if (arc_i->pCell->index != arc_k->pCell->index && arc_k->pCell->index != arc_2->pCell->index && arc_i->pCell->index != arc_2->pCell->index)
    {
      //                assert(isArcOnBeach(arc_i));
      //                assert(isArcOnBeach(arc_k));
      //                assert(isArcOnBeach(arc_2));

      auto ceK = std::make_shared<CircleEvent>(arc_i, arc_k, arc_2);
      if (ceK->theta >= scanLine.xi)
      {
        SV_DEBUG(cout << "  create new circleEvent " << *ceK << endl);
        arc_k->circleEvent = ceK;
        //itArcK->startVertex = newVertex;
        addNewCircleEvent(ceK);
      }
    }
  }
}

Point SphericalVoronoi::thetaToPoint(float theta, bool positive, float xi, float theta1, float phi1)
{
  float delta_phi = 0;

  if (theta != 0)
  {
    auto theta_p = theta1;
    auto cos_delta_phi = ((cos(xi) - cos(theta_p)) / tan(theta) + sin(xi)) / sin(theta_p);
    cos_delta_phi = Magnum::Math::clamp<float>(cos_delta_phi, -1.0, 1.0);
    delta_phi = acos(cos_delta_phi);
  }
  float s = positive ? 1 : -1;
  Point p(theta, phi1 + delta_phi * s);
  return p;
}

Point SphericalVoronoi::phiToPoint(float phi, float xi, float theta1, float phi1)
{
  if (theta1 >= xi)
  {
    assert(0);
    return Point(xi, phi);      // could be any point on the line segment
  }
  else
  {
    auto a = - (sin(theta1) * cos(phi - phi1) - sin(xi));
    auto b = - (cos(xi) - cos(theta1));
    auto theta = atan2(b, a);
    return Point(theta, phi);
  }
}





}
}
