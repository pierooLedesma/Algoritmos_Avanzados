#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Vertice {
    int grado;
    int indice;
    vector<int> vecinos; // Se guardan los índices de los vecinos
};


void calcularGrados(Vertice *vertices, int cantidadVertices, vector<vector<int>> grafo) {
    for (int ind = 0; ind < cantidadVertices; ind++) {
        int cantidadDeRelaciones = 0; // Inicializar la cantidad de relaciones (grado) de un vértice.

        // Calcular la cantidad de relaciones (grado) de un vértice
        for (int indVecino = 0; indVecino < cantidadVertices; indVecino++) {
            if (grafo[ind][indVecino] == 1) {
                vertices[ind].vecinos.push_back(indVecino);
                cantidadDeRelaciones++;
            }
        }

        vertices[ind].grado = cantidadDeRelaciones;
        vertices[ind].indice = ind;
    }
}


void ordenarVerticesDescendente(Vertice *vertices, int cantidadVertices) {
    for (int i=0; i < cantidadVertices - 1; i++) {
        for (int j = i + 1; j < cantidadVertices; j++) {
            if (vertices[i].grado < vertices[j].grado) {
                Vertice auxiliar = vertices[i];
                vertices[i] = vertices[j];
                vertices[j] = auxiliar;
            }
        }
    }
}


void agregarColoresProhibidos(int indVerticeActual, Vertice *vertices,
                              int *colores, vector<int> &coloresProhibidos) {
    int indice_vecino;
    for (int j = 0; j < vertices[indVerticeActual].vecinos.size(); j++) {
        indice_vecino = vertices[indVerticeActual].vecinos[j];
        if (colores[indice_vecino] != -1) coloresProhibidos[j] = colores[indice_vecino];
    }
}


void asignarMenorColorDisponible(vector<int> coloresProhibidos, int *colores, int indVerticeActual) {
    int color = 0, indiceIterativo = 0;
    while (color == coloresProhibidos[indiceIterativo]) {
        color++;
        indiceIterativo++;
    }
    colores[indVerticeActual] = color;
}


// bool compara(Vertice a, Vertice b) {
//     return a.grado > b.grado;
// }


int colorearGrafoVoraz(vector<vector<int>> grafo) {
    int cantidadVertices = grafo.size();
    Vertice vertices[cantidadVertices];

    int colores[cantidadVertices];
    fill(colores, colores + cantidadVertices, -1); // Inicializar colores en -1

    calcularGrados(vertices, cantidadVertices, grafo);
    ordenarVerticesDescendente(vertices, cantidadVertices);

    // Usando "sort" para un orden descendente sería la siguiente instrucción:
    //sort(vertices, vertices + cantidadVertices, compara);

    for (int v = 0; v < cantidadVertices; v++) {
        vector<int> coloresProhibidos(vertices[v].vecinos.size(), -1);
        // En la línea 53, el primer argumento del "()" es la cantidad de vecinos del vértice actual.
        // En la línea 53, el segundo argumento del "()" es la inicialización en -1 para todos los elementos del vector.

        agregarColoresProhibidos(v, vertices, colores, coloresProhibidos);
        asignarMenorColorDisponible(coloresProhibidos, colores, v);
    }

    int cantColores = *max_element(colores, colores + cantidadVertices) + 1;

    return cantColores;
}


int main() {
    vector<vector<int>> grafo = {
        {0, 1, 1, 1, 0, 0}, // A
        {1, 0, 1, 0, 1, 0}, // B
        {1, 1, 0, 1, 0, 1}, // C
        {1, 0, 1, 0, 0, 1}, // D
        {0, 1, 0, 0, 0, 1}, // E
        {0, 0, 1, 1, 1, 0}  // F
    };

    cout << "Se crearon " << colorearGrafoVoraz(grafo) << " color(es)." << endl;
}
