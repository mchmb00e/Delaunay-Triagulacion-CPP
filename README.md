# Intrucciones para compilar y ejecutar.
El archivo _main.cpp_ exporta automáticamente los grafos de Delaunay, Voronoi, Vecino más cercano y Envolvente Convexa.
## Paso 1: Compilar archivo main.cpp
Utilizando g++ en su versión C++17:
### g++ -std=c++17 main.cpp -o main

## Paso 2: Ejecutar nuestro archivo main
### ./main

_Nota:_ La exportación CSV se realiza en el directorio raíz de la forma:
* <b>Triangulación de Delaunay:</b> delaunay.csv
* <b>Diagrama de Voronoi:</b> voronoi.csv
* <b>Envolvente convexa:</b> convexhull.csv
* <b>Grafo del vecino más cercano:</b> nng.csv