#include <functional>
#include <math.h>

using namespace std;

class Point {
private:
  double x, y;
public:
  Point(double x, double y);
  double getX();
  double getY();
  void setX(double x);
  void setY(double y);
  bool operator==(Point p) const;
  bool operator!=(Point p) const;
};

// Proceso de HASH para los unordered_set en la clase Point
namespace std {
  template<>
  struct hash<Point> {
    size_t operator()(Point p) const {
      return hash<double>()(p.getX()) ^ hash<double>()(p.getY());
    }
  };
}

bool compare_points(Point a, Point b);

Point::Point(double x, double y) {
  this->x = x;
  this->y = y;
}

double Point::getX() { return x; }
double Point::getY() { return y; }

void Point::setX(double x) { this->x = x; }
void Point::setY(double y) { this->y = y; }

bool Point::operator==(Point p) const {
  return x == p.getX() && y == p.getY();
}
bool Point::operator!=(Point p) const {
  return !(*this == p);
}

bool compare_points(Point a, Point b) { // Compara puntos con respecto a X y luego con respecto a Y
  if (a.getX() != b.getX()) {
    return a.getX() < b.getX();
  } else {
    return a.getY() < b.getY();
  }
}

// Comprueba la distancia entre dos puntos en el plano
double distance(Point a, Point b) {
  return sqrt(pow(b.getX() - a.getX(), 2) + pow(b.getY() - a.getY(), 2));
}