#include <iostream>
#include <algorithm>
#include <iomanip>
#include <climits>
#include <vector>

#define ALFA 0.25
#define X 0.60
#define Y 0.40
#define B 150
#define ITERACIONES 100

using namespace std;


struct Proyecto {
    int id;
    int beneficio;
    int costo;
    string categoria;
    int riesgo;
    int greedy;
};


int metrica_greedy(int beneficio, int costo, int riesgo) {
    /*
     * La razón "beneficio/costo" mide cuánto beneficio económico genera el proyecto por
     * cada unidad de presupuesto utilizada. El término "(10-riesgo)" favorece a los proyectos
     * menos riesgosos. Los pesos 'x' e 'y' equilibran rentabilidad y seguridad, por lo que
     * un mayor valor greedy representa un proyecto más atractivo.
     */
    return X * ((double)beneficio / costo) + Y * (10 - riesgo);
}


void calcular_greedy(vector<Proyecto> &proyectos) {
    for (int i = 0; i < proyectos.size(); i++) {
        proyectos[i].greedy = metrica_greedy(proyectos[i].beneficio, proyectos[i].costo, proyectos[i].riesgo);
    }
}


bool compara(Proyecto &proy1, Proyecto &proy2) {
    return proy1.greedy > proy2.greedy;
}


/*
 * Para reemplazar la selección aleatoria:
 * Si existe un proyecto par dentro de la RCL, entonces se
 * devuelve el primer proyecto de índice par; caso contrario,
 * se devuelve el primer proyecto.
 */
int seleccionaRegla(vector<Proyecto> &rcl) {
    for (int i = 0; i < rcl.size(); i++) {
        if (rcl[i].id % 2 == 0) return i;
    }
    return 0;
}


void construirRCL(vector<Proyecto> &proyectos, vector<Proyecto> &rcl) {
    int beta = proyectos.front().greedy;  // proyectos[0].greedy
    int tau = proyectos.back().greedy;    // proyectos[proyectos.size() - 1].greedy
    int indice_inferior = beta - ALFA * (beta - tau);

    cout << "El valor de greedy minimo (g_min) de los proyectos: " << tau << endl;
    cout << "El valor de greedy maximo (g_max) de los proyectos: " << beta << endl;

    for (int i = 0; i < proyectos.size(); i++) {
        if (proyectos[i].beneficio >= indice_inferior)
            rcl.push_back(proyectos[i]);
    }
}


// Verifica si la categoría del proyecto ya fue seleccionada anteriormente.
bool categoriaRepetida(vector<string> &categorias, string categoria) {
    for (int i = 0; i < categorias.size(); i++) {
        if (categorias[i] == categoria) return true;
    }
    return false;
}


/*
 * Un proyecto es factible cuando su categoria no se repite y
 * su costo no hace superar el presupuesto disponible.
 */
bool esFactible(Proyecto proyecto, int costoAcumulado, vector<string> &categorias) {
    if (categoriaRepetida(categorias, proyecto.categoria)) {
        // La categoría se repite.
        return false;
    }

    if (costoAcumulado + proyecto.costo > B) {
        // El costo del proyecto hace superar el presupuesto diponible 'B'.
        return false;
    }

    return true;
}


int buscar_proyecto(vector<Proyecto> proyectos, int id_proyecto) {
    for (int indice = 0; indice < proyectos.size(); indice++) {
        if (proyectos[indice].id == id_proyecto) return indice;
    }
    return -1;
}


void graspProyectos(Proyecto *proyectos, int cantproy) {
    vector<Proyecto> mejoresProyectos;
    int costoTotal = INT_MAX;

    for (int i = 0; i < ITERACIONES; i++) {
        int costoParcial = 0;
        vector<Proyecto> solucionParcial;
        vector<Proyecto> los_proyectos;
        los_proyectos.insert(los_proyectos.begin(), proyectos, proyectos + cantproy);

        // Calcular el valor "greedy" de cada proyecto.
        calcular_greedy(los_proyectos);

        // Ordenar descendentemente por el valor de "greedy"
        sort(los_proyectos.begin(), los_proyectos.end(), compara);

        while (not los_proyectos.empty()) {
            vector<string> categorias;
            vector<Proyecto> rcl;

            construirRCL(los_proyectos, rcl);

            int idRCL_elegida = seleccionaRegla(rcl);

            if (esFactible(rcl[idRCL_elegida], costoParcial, categorias)) {
                costoParcial += rcl[idRCL_elegida].costo;
                categorias.push_back(rcl[idRCL_elegida].categoria);
                solucionParcial.push_back(rcl[idRCL_elegida]);
            }
            int id_eliminado = buscar_proyecto(los_proyectos, rcl[idRCL_elegida].id);
            los_proyectos.erase(los_proyectos.begin() + id_eliminado);
        }

        if (costoParcial < costoTotal) {
            costoTotal = costoParcial;
            mejoresProyectos.clear();
            mejoresProyectos = solucionParcial;
        }
    }

    cout << endl << endl << endl << "Mejores proyectos" << endl;
    for (int i = 0; i < mejoresProyectos.size(); i++) {
        cout << mejoresProyectos[i].categoria << " / ";
        cout << "beneficio: " << mejoresProyectos[i].beneficio << " / ";
        cout << "costo: " << mejoresProyectos[i].costo << " / ";
        cout << "greedy: " << mejoresProyectos[i].greedy << " / ";
        cout << "riesgo: " << mejoresProyectos[i].riesgo << endl;
    }
}


int main() {
    Proyecto proyectos[] = {
        {0, 80, 40, "Marketing", 8, 0},
        {1, 120, 90, "TI", 5, 0},
        {2, 60, 20, "Produccion", 3, 0},
        {3, 30, 10, "Logistica", 9, 0},
        {4, 200, 150, "RRHH", 2, 0},
        {5, 55, 25, "Marketing", 7, 0},
        {6, 110, 70, "TI", 10, 0},
        {7, 45, 30, "Produccion", 4, 0},
        {8, 75, 50, "Logistica", 6, 0},
        {9, 90, 60, "RRHH", 1, 0}
    };

    int cantProyectos = sizeof(proyectos) / sizeof(proyectos[0]);

    graspProyectos(proyectos, cantProyectos);

    return 0;
}
