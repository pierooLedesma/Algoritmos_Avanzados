#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <climits>

#define ITERACIONES 10000
#define ALFA 0.38

using namespace std;

struct Resultado {
    vector<vector<int>> tarea_asignada_maquina;  // S[j] = tareas asignadas a la máquina j
    vector<int> tiempo_acumulado_maquina;        // A[j] = tiempo acumulado de la máquina j
    int makespan;                                // máximo tiempo acumulado entre máquinas
};

Resultado construccionGRASP(const vector<vector<int>>& matriz_de_tiempos) {
    int cant_tareas = matriz_de_tiempos.size();       // número de tareas
    int cant_maquinas = matriz_de_tiempos[0].size();  // número de máquinas

    vector<int> tiempos_acumulados(cant_maquinas, 0);  // tiempos acumulados por máquina
    vector<vector<int>> tareas_asignadas(cant_maquinas);    // tareas asignadas a cada máquina
    vector<int> programadas;                                // tareas ya programadas

    for (int i = 0; i < cant_tareas; i++) {
        int beta = INT_MAX;
        int tau = INT_MIN;

        vector<int> valor(cant_maquinas);

        // 3.1 y 3.2: calcular beta y tau
        for (int j = 0; j < cant_maquinas; j++) {
            valor[j] = tiempos_acumulados[j] + matriz_de_tiempos[i][j];

            if (valor[j] < beta) {
                beta = valor[j];
            }

            if (valor[j] > tau) {
                tau = valor[j];
            }
        }

        // 3.3: construir la RCL
        double limiteRCL = beta + ALFA * (tau - beta);

        vector<int> RCL;

        for (int j = 0; j < cant_maquinas; j++) {
            if (valor[j] >= beta and valor[j] <= limiteRCL) {
                RCL.push_back(j);
            }
        }

        // 3.4: seleccionar una máquina aleatoria dentro de la RCL
        int indiceAleatorio = rand() % RCL.size();
        int maquina = RCL[indiceAleatorio];

        // 3.5: actualizar la carga acumulada de la máquina
        tiempos_acumulados[maquina] = tiempos_acumulados[maquina] + matriz_de_tiempos[i][maquina];

        // 3.6: marcar tarea como programada
        programadas.push_back(i);

        // 3.7: asignar tarea a la máquina seleccionada
        tareas_asignadas[maquina].push_back(i);
    }

    // 4. makespan = máximo valor de A[j]
    int makespan = *max_element(tiempos_acumulados.begin(), tiempos_acumulados.end());

    Resultado el_resultado;
    el_resultado.tarea_asignada_maquina = tareas_asignadas;
    el_resultado.tiempo_acumulado_maquina = tiempos_acumulados;
    el_resultado.makespan = makespan;

    return el_resultado;
}

Resultado graspProgramacionTareas(const vector<vector<int>>& matriz_de_tiempos) {
    Resultado mejor;
    mejor.makespan = INT_MAX;
    srand(time(NULL));

    for (int i = 0; i < ITERACIONES; i++) {
        Resultado actual = construccionGRASP(matriz_de_tiempos);

        if (actual.makespan < mejor.makespan) {
            mejor = actual;
        }
    }

    return mejor;
}

void imprimirResultado(const Resultado & resultados) {
    cout << "Mejor solucion encontrada:" << endl;

    for (int j = 0; j < resultados.tarea_asignada_maquina.size(); j++) {
        cout << "Maquina " << j + 1 << ": ";

        for (int tarea : resultados.tarea_asignada_maquina[j]) {
            cout << "T" << tarea + 1 << " ";
        }

        cout << "| Tiempo acumulado: " << resultados.tiempo_acumulado_maquina[j] << endl;
    }

    cout << "Makespan: " << resultados.makespan << endl;
}

int main() {
    /*
        Matriz T:
        Filas    = tareas
        Columnas = máquinas

        matriz_de_tiempos[i][j] = tiempo que demora la tarea i en la máquina j
    */

    vector<vector<int>> matriz_de_tiempos = {
        {4, 6, 5},
        {7, 3, 8},
        {5, 4, 6},
        {9, 7, 4},
        {6, 5, 3},
        {8, 6, 7}
    };

    Resultado mejor = graspProgramacionTareas(matriz_de_tiempos);

    imprimirResultado(mejor);

    return 0;
}