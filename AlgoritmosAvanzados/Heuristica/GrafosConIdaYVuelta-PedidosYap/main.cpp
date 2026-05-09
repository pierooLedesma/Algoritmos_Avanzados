#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Nodo {
    int punto;
    int distancia;
};

bool comparar(Nodo primerNodo, Nodo segundoNodo) {
    return primerNodo.distancia < segundoNodo.distancia;
}

int rutaMinima(int puntoInicio, int puntoFin, vector<vector<int>> mapa) {
    int puntoActual = puntoInicio;
    int distanciaTotal = 0;
    bool visitados[mapa.size()];
    for (int i = 1; i < mapa.size(); i++) visitados[i] = false;

    while (true) {
        vector<Nodo> vecinos;
        Nodo nodo;
        if (puntoActual != puntoInicio) visitados[puntoActual] = true;
        for (int indice = 0; indice < mapa.size(); indice++) {
            if (mapa[puntoActual][indice]!=0 and not visitados[indice]) {
                nodo.punto = indice;
                nodo.distancia = mapa[puntoActual][indice];
                vecinos.push_back(nodo);
            }
        }
        if (not vecinos.empty()) {
            sort(vecinos.begin(), vecinos.end(), comparar);
            puntoActual = vecinos[0].punto;
            distanciaTotal += vecinos[0].distancia;
        } else {
            cout << "No hay solucion: ";
            distanciaTotal = 0;
            break;
        }
        if (puntoActual == puntoFin) break;
    }
    return distanciaTotal;
}

int main() {
    vector<vector<int>> mapa = {
        {0,4,5,6,0,0,0,0},
        {4,0,0,0,2,0,0,0},
        {5,0,0,0,0,0,0,3},
        {6,0,0,0,0,3,0,0},
        {0,2,0,0,0,0,10,0},
        {0,0,0,3,0,0,2,0},
        {0,0,0,0,10,2,0,0},
        {0,0,3,0,0,0,0,0},
    };
    cout << "Tiempo de recorrido: "
    << rutaMinima(0, 2, mapa) << " minuto(s)." << endl;
    return 0;
}
