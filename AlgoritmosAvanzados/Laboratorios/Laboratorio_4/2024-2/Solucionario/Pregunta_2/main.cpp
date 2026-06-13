#include <algorithm>
#include <iostream>
#include <vector>
#include <ctime>
#include <climits>

#define ITERACIONES 10000
#define ALFA 0.4

using namespace std;

struct Resultado {
    vector<int> puntos;
    int ganancia;
};

struct Nodo {
    int punto;
    int ganancia;
};


void inicializar_candidatos(vector<Nodo> & candidatos, vector<int> & direcciones, vector<int> & ganancias) {
    for (int punto : direcciones) {
        candidatos.push_back({punto, ganancias[punto]});
    }
}

bool compara(Nodo nodo_1, Nodo nodo_2) {
    return nodo_1.ganancia > nodo_2.ganancia;
}


int buscarCantidadRCL(vector<Nodo> & candidatos, int limiteRCL) {
    int cantidadRCL = 0;

    for (auto candidato : candidatos) {
        if (candidato.ganancia >= limiteRCL) {
            cantidadRCL++;
        }
    }

    return cantidadRCL;
}


Resultado construccionGRASP(vector<vector<int>> grafo_rutas, vector<int> ganancias) {
    vector<int> ruta_actual; // Solución vacía (ruta vacía)

    int punto_actual = 0; // Siempre se comienza desde el punto 'A'
    int ganancia_actual = 0;

    while (true) {
        vector<Nodo> candidatos; // Los candidatos disponibles
        inicializar_candidatos(candidatos, grafo_rutas[punto_actual], ganancias);

        if (candidatos.empty()) break;

        // Ordenamiento descendente por ganancia
        sort(candidatos.begin(), candidatos.end(), compara);

        int beta = candidatos[0].ganancia;
        int tau = candidatos[candidatos.size() - 1].ganancia;
        int limiteRCL = beta - ALFA * (beta - tau);
        int cantidadRCL = buscarCantidadRCL(candidatos, limiteRCL);
        int indAleatorio = rand() % cantidadRCL;

        Nodo nodoElegido = candidatos[indAleatorio];

        // Agregar el punto a la ruta actual
        ruta_actual.push_back(nodoElegido.punto);
        ganancia_actual += nodoElegido.ganancia;

        // Actualizar el punto actual
        punto_actual = nodoElegido.punto;
    }
    Resultado resultado_actual;
    resultado_actual.puntos = ruta_actual;
    resultado_actual.ganancia = ganancia_actual;
    return resultado_actual;
}


Resultado graspEntregas(vector<vector<int>> & grafo_rutas, vector<int> & ganancias) {
    Resultado mejor_solucion;           // Mejor solución
    mejor_solucion.ganancia = INT_MIN;  // Mejor valor
    srand(time(NULL));

    // Repetir varias veces
    for (int i = 0; i < ITERACIONES; i++) {
        Resultado actual = construccionGRASP(grafo_rutas, ganancias);

        if (actual.ganancia > mejor_solucion.ganancia) {
            mejor_solucion = actual;
        }
    }

    return mejor_solucion;
}


int main () {
    vector<vector<int>> grafo_rutas = {
        {1,2,3,4,7}, // A -> B C D E H
        {2,3,5,7},   // B -> C D F H
        {5},         // C -> F
        {},          // D -> -
        {2,5,6},     // E -> C F G
        {},          // F -> -
        {},          // G -> -
        {}           // H -> -
    };

    vector<int> ganancias = {
        0,   // A
        20,  // B
        30,  // C
        40,  // D
        40,  // E
        40,  // F
        10,  // G
        50   // H
    };

    Resultado mejor_solucion = graspEntregas(grafo_rutas, ganancias);
    cout << "Ganancia: " << mejor_solucion.ganancia << endl;
    cout << "Puntos seleccionados: ";
    for (int punto : mejor_solucion.puntos) {
        char el_punto = 'A' + punto;
        cout << el_punto << " ";
    }
    return 0;
}