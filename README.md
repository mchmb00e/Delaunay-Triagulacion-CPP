## Prerequisitos
* El archivo _CSV_ que contiene las coordenadas debe llevar un formato x,y,z en la primera línea del documento (El programa ignora la coordenada _Z_)
* El archivo _CSV_ debe contener valores _X_ e _Y_ pertenecientes a [-300, 300]. (Leer anexo al final de este documento para rangos mayores)

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

### Anexo para valores fuera del rango [-300, 300]
En la función _delaunay()_ perteneciente a la clase _Plane_, modificar los puntos del supertriángulo inicial _SP_ de tal manera que todos los puntos estén contenidos en el supertriángulo.
_Puede usar Geogebra importando el archivo _CSV_ para graficar un supertriángulo._

### Recursos
* https://www.geogebra.org/?lang=es
* https://es.wikipedia.org/wiki/Triangulaci%C3%B3n_de_Delaunay
* https://www.youtube.com/watch?v=GctAunEuHt4&t=109s
* https://www.youtube.com/watch?v=V1dwbYlfLIw
