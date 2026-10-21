#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <climits>

#define ITERACIONES 10000
#define ALFA 0.38

using namespace std;

struct Resultado {
    vector<int> ruta;    // S = ruta de ciudades visitadas
    int distancia_total; // distancia total del recorrido
    bool factible;       // indica si se completó el recorrido
};


Resultado construccionGRASP(const vector<vector<int>> & matriz_de_distancias, int ciudad_inicial) {
    int cant_ciudades = matriz_de_distancias.size();

    vector<int> visitadas;  // V = ciudades ya visitadas
    vector<int> ruta;       // S = ruta construida

    // 1. Inicializar V y S con la ciudad inicial
    visitadas.push_back(ciudad_inicial);
    ruta.push_back(ciudad_inicial);

    // 2. Inicializar la distancia total
    int distancia_total = 0;

    // 3. Establecer la ciudad actual
    int ciudad = ciudad_inicial;

    // 4. Repetir mientras existan ciudades sin visitar
    while (visitadas.size() < cant_ciudades) {

        // 4.1: formar CONEC con ciudades conectadas y no visitadas
        vector<int> CONEC;

        for (int i = 0; i < cant_ciudades; i++) {
            bool fue_visitada = find(visitadas.begin(), visitadas.end(), i) != visitadas.end();

            if (not fue_visitada and matriz_de_distancias[ciudad][i] > 0) {
                CONEC.push_back(i);
            }
        }

        // Si no existen ciudades disponibles
        if (CONEC.empty()) {
            Resultado no_factible;
            no_factible.distancia_total = INT_MAX;
            no_factible.factible = false;
            return no_factible;
        }

        int beta = INT_MAX;
        int tau = INT_MIN;

        // Guarda las distancias desde la ciudad actual hasta una ciudad candidata
        vector<int> valor;

        // 4.2 y 4.3: calcular beta y tau
        for (int candidata : CONEC) {
            valor.push_back(matriz_de_distancias[ciudad][candidata]);
            beta = min(beta, valor.back());
            tau = max(tau, valor.back());
        }

        // 4.4: construir la RCL
        double limiteRCL = beta + ALFA * (tau - beta);

        vector<int> RCL;

        sort(valor.begin(), valor.end()); // Ordenamiento ascendente de distancias

        for (int ind=0; ind < valor.size(); ind++) {
            if (valor[ind] <= limiteRCL) RCL.push_back(CONEC[ind]);
            else break;
        }

        // 4.5: seleccionar una ciudad aleatoria de la RCL
        int indiceAleatorio = rand() % RCL.size();

        int seleccionada = RCL[indiceAleatorio];

        // 4.6: marcar la ciudad como visitada
        visitadas.push_back(seleccionada);
        ruta.push_back(seleccionada);

        // 4.7: actualizar la distancia total
        distancia_total += matriz_de_distancias[ciudad][seleccionada];

        // 4.8: actualizar la ciudad actual
        ciudad = seleccionada;
    }

    // 5. Regresar a la ciudad inicial
    if (matriz_de_distancias[ciudad][ciudad_inicial] < 0) {
        Resultado no_factible;
        no_factible.distancia_total = INT_MAX;
        no_factible.factible = false;
        return no_factible;
    }

    distancia_total += matriz_de_distancias[ciudad][ciudad_inicial];

    ruta.push_back(ciudad_inicial);

    Resultado el_resultado;
    el_resultado.ruta = ruta;
    el_resultado.distancia_total = distancia_total;
    el_resultado.factible = true;

    return el_resultado;
}


Resultado graspAgenteViajero(const vector<vector<int>> & matriz_de_distancias, int ciudad_inicial) {
    Resultado mejor;
    mejor.distancia_total = INT_MAX;
    mejor.factible = false;

    srand(time(NULL));

    for (int i = 0; i < ITERACIONES; i++) {
        Resultado actual = construccionGRASP(matriz_de_distancias, ciudad_inicial);

        if (actual.factible and
            actual.distancia_total < mejor.distancia_total) {

            mejor = actual;
        }
    }

    return mejor;
}


void imprimirResultado(const Resultado & resultado) {
    if (not resultado.factible) {
        cout << "No se encontro una ruta factible." << endl;
        return;
    }

    cout << "Mejor solucion encontrada:" << endl;
    cout << "Ruta: ";

    for (int i = 0; i < resultado.ruta.size(); i++) {

        cout << "C" << resultado.ruta[i] + 1;

        if (i + 1 < resultado.ruta.size()) {
            cout << " -> ";
        }
    }

    cout << endl;
    cout << "Distancia total: " << resultado.distancia_total<< endl;
}

int main() {
    /*
        Matriz D:

        Filas    = ciudad de origen
        Columnas = ciudad de destino

        matriz_de_distancias[i][j]
        representa la distancia desde
        la ciudad i hasta la ciudad j.

        Se utiliza -1 cuando no existe
        conexión entre dos ciudades.
    */

    vector<vector<int>> matriz_de_distancias = {
        /* C1 */ {0,  10, 15, 20, 12},
        /* C2 */ {10, 0,  35, 25, 17},
        /* C3 */ {15, 35, 0,  30, 28},
        /* C4 */ {20, 25, 30, 0,  11},
        /* C5 */ {12, 17, 28, 11, 0}
    };

    int ciudad_inicial = 0;  // Ciudad C1

    Resultado mejor = graspAgenteViajero(matriz_de_distancias, ciudad_inicial);

    imprimirResultado(mejor);

    return 0;
}