/* Trabajo 2 - Paradigmas de Programación
 * Fecha: 26 - 08 - 2024
 * Integrantes:
 *  > Miguel Chamorro
 *  > Maximiliano Correa
 *  > Matías Mora
 *
 *  Profesor: Francisco Moreno
 *
 */

#include "triangle.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Comprueba si existe una arista en un vector de aristas
bool exists(vector<Edge> edges, Edge e);

// Cuenta la cantidad de veces que se repite una arista en un vector de triangulos
int count(vector<Triangle> triangles, Edge e);

// Cuenta la cantidad de veces que se repite una arista en un vector de aristas
int count_edges(vector<Edge> edges, Edge e);

// Clase principal que organiza los distintos grafos
class Plane {
private:
    unordered_set<Triangle> triangles; // Almacena los triangulos de Delaunay
    vector<Point> points; // Almacena los puntos ordenados con respecto a X y con respecto a Y
public:
    void read_csv(string file_path); // Lee un archivo csv y obtiene sus puntos
    // Busca un triangulo vecino al ingresado con respecto a la arista e
    // de no encontrarse retorna el triangulo ingresado.
    Triangle search_neighbor(Triangle t, Edge e);
    void delaunay(); // Genera la triangulación de Delaunay y exporta un archivo csv con sus aristas
    void convex_hull(); // Obtiene el borde externo de Delaunay y exporta un archivo csv con sus aristas
    void voronoi(); // Genera el diagrama de voronoi y exporta un archivo csv con sus aristas
    void nearest_neighbor(); // Genera el grafo del vecino más cercano y exporta un archivo csv con sus aristas
};

int main() {
    Plane plane;
    plane.read_csv("puntos.csv");
    cout << "Exportando Delaunay" << endl;
    plane.delaunay();
    cout << "Exportando Voronoi" << endl;
    plane.voronoi();
    cout << "Exportando Convex Hull" << endl;
    plane.convex_hull();
    cout << "Exportando Nearest Neighbor" << endl;
    plane.nearest_neighbor();
    cout << "Proceso finalizado." << endl;

    return 0;
}

bool exists(vector<Edge> edges, Edge e) {
    for (Edge edge : edges) {
        if ((edge.getP1() == e.getP1() && edge.getP2() == e.getP2()) ||
            (edge.getP1() == e.getP2() && edge.getP2() == e.getP1())) {
            return true;
            }
    }
    return false;
}
int count(vector<Triangle> triangles, Edge e) {
    int count = 0;
    for (Triangle triangle : triangles) {
        for (Edge triangle_edge : triangle.edges()) {
            if (triangle_edge == e) {
                count++;
            }
        }
    }
    return count;
}
int count_edges(vector<Edge> edges, Edge e) {
    int count = 0;
    for (Edge edge : edges) {
        if (edge == e)
            count++;
    }
    return count;
}
void Plane::read_csv(string file_path) {
    double aux[3];     // Array auxiliar para almacenar los valores x, y, z
    ifstream file(file_path); // Abrir el archivo CSV
    string line;              // Cadena para almacenar cada línea leída

    getline(file, line); // Ignorar la primera línea (encabezado "x,y,z")

    while (getline(file, line)) {
        char delimiter;         // Variable para gestionar el delimitador de comas
        istringstream ss(line); // Stream para procesar la línea
        ss >> aux[0] >> delimiter >> aux[1] >> delimiter >>
            aux[2];                         // Leer los valores de x, y, z
        points.push_back(Point(aux[0], aux[1])); // Agregar el punto al vector
    }
    sort(points.begin(), points.end(), compare_points);
}
void Plane::delaunay() {
    Point SP1 = {-800, -400}, SP2 = {1000, -400}, SP3 = {0, 1000}; // Definimos puntos para un supertriangulo
    Triangle SP = {SP1, SP2, SP3}; // Definimos el supertriangulo
    triangles.insert(SP); // Insertamos el supertriangulo como triangulo base
    vector<Edge> convex; // Vector que almacena los bordes externos de los triángulos malos
    vector<Triangle> bad_triangles; // Vector que almacena los triangulos malos por iteración de puntos

    for (Point p : points) { // Iteramos en cada punto
        if (!bad_triangles.empty()) // Vaciamos ambos vectores si no lo estan
            bad_triangles.clear();
        if (!convex.empty())
            convex.clear();

        for (Triangle t : triangles) { //  Iteramos en los triangulos actuales
            // Comprobamos si el punto p está dentro de la circunferencia del triangulo
            // ordenado en un sentido antihorario
            if (t.counterclockwise().in_circumference(p))
                bad_triangles.push_back(t); // Añadimos el triangulo a los triangulos malos
        }

        for (Triangle bt : bad_triangles) { // Iteramos en los triangulos malos
            for (Edge e : bt.edges()) { // Iteramos en las aristas de los triangulos malos
                if (count(bad_triangles, e) == 1) { // Si la arista es igual a 1 entonces es una arista externa
                    convex.push_back(e); // Añadimos la arista a los bordes externos
                }
            }
            triangles.erase(bt); // Borramos los triangulos malos a medida que los utilizamos
        }
        for (Edge e : convex) { // Iteramos en el vector de aristas externas
            triangles.insert({p, e.getP1(), e.getP2()}); // Añadimos los nuevos triangulos conectados al nuevo punto
        }
    }
    unordered_set<Triangle> triangles_aux = triangles; // Creamos un set auxiliar
    for (Triangle t : triangles_aux) { // Iteramos en este
        if (t.contain(SP.getP1()) || t.contain(SP.getP2()) || t.contain(SP.getP3()))
            // Eliminamos los triangulos que tengan puntos asociados al supertriangulo inicial
            triangles.erase(t);
    }
    vector<Edge> edge_aux; // Definimos un vector de aristas auxiliar
    for (Triangle t : triangles) { // Iteramos en los triangulos
        for (Edge e : t.edges()) {
            if (!exists(edge_aux, e)) // Almacenamos las aristas evitando duplicados
                edge_aux.push_back(e);
        }
    }
    ofstream file("delaunay.csv"); // Creamos una variable de exportación de archivo
    file << "x1,y1,x2,y2\n"; // Fijamos la primera linea de texto
    for (Edge e : edge_aux)
        // Exportamos nuestras aristas
        file << e.getP1().getX() << "," << e.getP1().getY() << "," << e.getP2().getX() << "," <<e.getP2().getY()<< endl;
    file.close();
}
void Plane::convex_hull() {
    if (!triangles.empty()) { // Verificamos que la triangulación de delaunay fue realizada
        vector<Edge> edges; // Definimos un vector para almacenar bordes
        vector<Edge> convex; // Definimos un vector para almacenar bordes externos
        for (Triangle t : triangles) { // Iteramos en los triangulos guardando todos los bordes existentes
            for (Edge e : t.edges()) {
                edges.push_back(e);
            }
        }
        for (Edge e : edges) { // Almacenamos en convex los bordes externos que existen en un solo triangulo
            if (count_edges(edges, e) == 1)
                convex.push_back(e);
        }
        ofstream file("convexhull.csv"); // Exportamos los bordes a un archivo csv
        file << "x1,y1,x2,y2\n";
        for (Edge e : convex)
            file << e.getP1().getX() << "," << e.getP1().getY() << "," << e.getP2().getX() << "," <<e.getP2().getY()<< endl;
        file.close();

    } else {
        delaunay();
        convex_hull();
    }
}
Triangle Plane::search_neighbor(Triangle t, Edge e) {
    for (Triangle t0 : triangles) { // Iteramos en los triangulos de delaunay
        for (Edge e0 : t0.edges()) { // Iteramos en las aristas de los triangulos de delaunay
            if (e0 == e && t0 != t ) // Comprobamos si es un vecino del triangulo ingresado
                return t0;
        }
    }
    return t;
}
void Plane::voronoi() {
    if (!triangles.empty()) { // Verificamos que se realizó la triangulación de Delaunay
        Triangle neighbor = {{0, 0}, {0, 0}, {0, 0}};
        Point center = {0, 0};
        Point neighbor_center = {0, 0};
        vector<Edge> edges;
        for (Triangle t : triangles) { // Iteramos en los triangulos asociados
            for (Edge e : t.edges()) { // Itemamos en los bordes del triangulo
                neighbor = search_neighbor(t, e); // Buscamos un vecino en determinada arista
                if (neighbor != t) { // De encontrarse, creamos una arista entre sus circuncentros
                    center = t.circumcenter();
                    neighbor_center = neighbor.circumcenter();
                    edges.push_back({center, neighbor_center});
                }
            }
        }
        vector<Edge> edges_aux;
        for (Edge e : edges) { // Eliminamos duplicados
            if (!exists(edges_aux, e))
                edges_aux.push_back(e);
        }
        ofstream file("voronoi.csv"); // Exportamos nuestros bordes del diagrama de Voronoi
        file << "x1,y1,x2,y2\n";
        for (Edge e : edges_aux)
            file << e.getP1().getX() << "," << e.getP1().getY() << "," << e.getP2().getX() << "," <<e.getP2().getY()<< endl;
        file.close();
    } else {
        delaunay();
        voronoi();
    }
}
void Plane::nearest_neighbor() {
    vector<Edge> edges;
    double min_distance;
    double d;
    Point min_point = {0, 0};
    for (Point p : points) { // Iteramos en los puntos
        min_distance = 999999; // Definimos una distancia inicial grande
        for (Point q : points) { // Comparamos las distancias con todos los puntos
            d = distance(p, q);
            if (d < min_distance && p != q) { // Comprobamos si la distancia es menor que la minima y si es distinto al punto en iteracion
                min_distance = d; // Asignamos el nuevo minimo
                min_point = q; // Almacenamos el punto
            }
        }
        edges.push_back({p, min_point}); // Almacenamos la arista
    }
    vector<Edge> edges_aux;
    for (Edge e : edges) { // Limpiamos duplicados
        if (!exists(edges_aux, e))
            edges_aux.push_back(e);
    }
    ofstream file("nng.csv"); // Exportamos las aristas a un archivo csv
    file << "x1,y1,x2,y2\n";
    for (Edge e : edges_aux)
        file << e.getP1().getX() << "," << e.getP1().getY() << "," << e.getP2().getX() << "," <<e.getP2().getY()<< endl;
    file.close();
}