#include <iostream>
#include "point.h"
#include <vector>

using namespace std;


class Edge {
private:
    Point p1, p2;
public:
    Edge(Point p1, Point p2);
    Point getP1();
    Point getP2();
    void setP1(Point p);
    void setP2(Point p);
    bool operator==(Edge e) const;
    bool operator!=(Edge e) const;
    bool contain(Point p); // Comprueba si existe determinado punto en la arista
};

class Triangle {
private:
    Point p1, p2, p3;
public:
    Triangle(Point p1, Point p2, Point p3);
    Point getP1() const;
    Point getP2() const;
    Point getP3() const;
    void setP1(Point p);
    void setP2(Point p);
    void setP3(Point p);
    void print(int number); // Realiza un cout de los puntos del triangulo con un indice
    bool operator==(Triangle t) const;
    bool operator!=(Triangle t) const;
    bool in_circumference(Point D); // Comprueba si un punto D está dentro de la circunferencia
    bool contain(Point p); // Comprueba si un punto p pertenece a los puntos del triangulo
    bool contain_edge(Edge e); // Comprueba si una arista pertenece al triangulo
    double det(); // Determinante que comprueba si el sentido del triangulo es horario o antihorario
    Triangle counterclockwise(); // Retorna el triangulo de manera antihoraria
    vector<Edge> edges(); // Retorna un vector de aristas del triangulo
    Point circumcenter(); // Retorna el circuncentro del triangulo
};

// Proceso de HASH para los unorderes_set de Triangle y Edge
namespace std {
    template<>
    struct hash<Triangle> {
        size_t operator()(Triangle t) const {
            size_t h1 = hash<Point>()(t.getP1());
            size_t h2 = hash<Point>()(t.getP2());
            size_t h3 = hash<Point>()(t.getP3());
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
    template<>
    struct hash<Edge> {
        size_t operator()(Edge e) const {
            auto h1 = hash<Point>{}(e.getP1());
            auto h2 = hash<Point>{}(e.getP2());
            return h1 ^ (h2 << 1);
        }
    };
};

// Calcula un determinante de una matriz de orden 4
double determinant4x4(double m[4][4]) {
    double det = m[0][0]*(m[1][1]*m[2][2]*m[3][3] + m[1][2]*m[2][3]*m[3][1] + m[1][3]*m[2][1]*m[3][2]
                            - m[1][3]*m[2][2]*m[3][1] - m[1][2]*m[2][1]*m[3][3] - m[1][1]*m[2][3]*m[3][2]);
    det -= m[0][1]*(m[1][0]*m[2][2]*m[3][3] + m[1][2]*m[2][3]*m[3][0] + m[1][3]*m[2][0]*m[3][2]
                 - m[1][3]*m[2][2]*m[3][0] - m[1][2]*m[2][0]*m[3][3] - m[1][0]*m[2][3]*m[3][2]);
    det += m[0][2]*(m[1][0]*m[2][1]*m[3][3] + m[1][1]*m[2][3]*m[3][0] + m[1][3]*m[2][0]*m[3][1]
                 - m[1][3]*m[2][1]*m[3][0] - m[1][1]*m[2][0]*m[3][3] - m[1][0]*m[2][3]*m[3][1]);
    det -= m[0][3]*(m[1][0]*m[2][1]*m[3][2] + m[1][1]*m[2][2]*m[3][0] + m[1][2]*m[2][0]*m[3][1]
                 - m[1][2]*m[2][1]*m[3][0] - m[1][1]*m[2][0]*m[3][2] - m[1][0]*m[2][2]*m[3][1]);
    return det;
}

// Retorna el punto de intersección de las mediatrices de dos aristas
Point intersection(Edge e1, Edge e2) {
    Point p1 = e1.getP1();
    Point p2 = e1.getP2();
    Point p3 = e2.getP1();
    Point p4 = e2.getP2();

    double x1 = p1.getX(), y1 = p1.getY();
    double x2 = p2.getX(), y2 = p2.getY();
    double x3 = p3.getX(), y3 = p3.getY();
    double x4 = p4.getX(), y4 = p4.getY();

    double mx1 = (x1 + x2) / 2.0;
    double my1 = (y1 + y2) / 2.0;
    double mx2 = (x3 + x4) / 2.0;
    double my2 = (y3 + y4) / 2.0;

    double slope1 = (x2 - x1) != 0 ? (y2 - y1) / (x2 - x1) : 0;
    double slope2 = (x4 - x3) != 0 ? (y4 - y3) / (x4 - x3) : 0;

    double perpSlope1 = (slope1 == 0) ? 0 : -1 / slope1;
    double perpSlope2 = (slope2 == 0) ? 0 : -1 / slope2;

    double c1 = my1 - perpSlope1 * mx1;
    double c2 = my2 - perpSlope2 * mx2;

    double xIntersection = (c2 - c1) / (perpSlope1 - perpSlope2);
    double yIntersection = perpSlope1 * xIntersection + c1;

    return Point(xIntersection, yIntersection);
}

Triangle::Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {}
Point Triangle::getP1() const { return p1; }
Point Triangle::getP2() const { return p2; }
Point Triangle::getP3() const { return p3; }
void Triangle::setP1(Point p) { p1 = p; }
void Triangle::setP2(Point p) { p2 = p; }
void Triangle::setP3(Point p) { p3 = p; }
bool Triangle::operator==(Triangle t) const {
    // Obtenemos los puntos del triángulo a comparar
    Point t_p1 = t.getP1();
    Point t_p2 = t.getP2();
    Point t_p3 = t.getP3();

    // Comparamos todos los puntos en cualquier orden
    return (p1 == t_p1 && p2 == t_p2 && p3 == t_p3) ||
           (p1 == t_p1 && p2 == t_p3 && p3 == t_p2) ||
           (p1 == t_p2 && p2 == t_p1 && p3 == t_p3) ||
           (p1 == t_p2 && p2 == t_p3 && p3 == t_p1) ||
           (p1 == t_p3 && p2 == t_p1 && p3 == t_p2) ||
           (p1 == t_p3 && p2 == t_p2 && p3 == t_p1);
}
bool Triangle::operator!=(Triangle t) const {
    return !(*this == t);
}
bool Triangle::in_circumference(Point D) {

    // Calculamos a través de un especifico determinante.
    // https://es.wikipedia.org/wiki/Triangulaci%C3%B3n_de_Delaunay
    double matrix[4][4] = {
        { p1.getX(), p1.getY(), p1.getX()*p1.getX() + p1.getY()*p1.getY(), 1 },
        { p2.getX(), p2.getY(), p2.getX()*p2.getX() + p2.getY()*p2.getY(), 1 },
        { p3.getX(), p3.getY(), p3.getX()*p3.getX() + p3.getY()*p3.getY(), 1 },
        { D.getX(), D.getY(), D.getX()*D.getX() + D.getY()*D.getY(), 1 }
    };

    double det = determinant4x4(matrix);

    return det > 0; // Si el determinante finalmente es mayor a 0, entonces está dentro
}
double Triangle::det() {
    // Calcula el determinante de la forma
    // | x2-x1 y2-y1 |
    // | x3-x1 y3-y1 |
    return (p2.getX() - p1.getX()) * (p3.getY() - p1.getY()) - (p2.getY() - p1.getY()) * (p3.getX() - p1.getX());
}
Triangle Triangle::counterclockwise() {
    if (det() < 0) { // Si el determinante es menor a 0 entonces se realiza una rotación de vértices
        return {p1, p3, p2};
    } else {
        return {p1, p2, p3};
    }
}
vector<Edge> Triangle::edges() {
    return {{p1, p2}, {p2, p3}, {p3, p1}};
}
void Triangle::print(int number = 0) {
    if (number != 0)
        cout << "Triangle " << number << endl;
    cout << "(" << p1.getX() << ", " << p1.getY() << ")" << endl;
    cout << "(" << p2.getX() << ", " << p2.getY() << ")" << endl;
    cout << "(" << p3.getX() << ", " << p3.getY() << ")" << endl;
}
bool Triangle::contain(Point p) {
    return (p == p1 || p == p2 || p == p3);
}
bool Triangle::contain_edge(Edge e) {
    bool status = false;
    for (Edge edge : edges()) {
        if (edge == e)
            status = true;
    }
    return status;
}

Point Triangle::circumcenter() {
    // Evaluamos dos aristas que no sean paralelas a algun eje.
    // Esto para evitar mediatrices con pendiente infinita o pendiente igual a 0
    if ((p1.getX() == p2.getX()) || (p1.getY() == p2.getY())) {
        return intersection({p3, p1}, {p3, p2});
    } else if ((p1.getX() == p3.getX()) || (p1.getY() == p3.getY())) {
        return intersection({p2, p1}, {p2, p3});
    } else if ((p2.getX() == p3.getX()) || (p2.getY() == p3.getY())) {
        return intersection({p1, p2}, {p1, p3});
    } else {
        return intersection({p1, p2}, {p1, p3});
    }
}

Edge::Edge(Point p1, Point p2) : p1(p1), p2(p2) {
    this->p1 = p1;
    this->p2 = p2;
};
Point Edge::getP1() { return p1; }
Point Edge::getP2() { return p2; }
void Edge::setP1(Point p) { p1 = p; }
void Edge::setP2(Point p) { p2 = p; }
bool Edge::operator==(Edge e) const {
    return (p1 == e.getP1() && p2 == e.getP2()) || (p1 == e.getP2() && p2 == e.getP1());
}
bool Edge::operator!=(Edge e) const {
    return !(*this == e);
}
bool Edge::contain(Point p) {
    return (p == p1 || p == p2);
}

