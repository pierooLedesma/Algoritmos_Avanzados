#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct Solucion {
    vector<int> ruta;
    int carga_total;
    int distancia_ruta;
};


struct Nodo {
    int punto;
    int distancia;
    int demanda;
};


bool comparar(struct Nodo nodo_1, struct Nodo nodo_2) {
    return nodo_1.distancia < nodo_2.distancia;
}


void completar_nodo(struct Nodo & nodo, int distancia, int demanda, int punto) {
    nodo.demanda = demanda;
    nodo.punto = punto;
    nodo.distancia = distancia;
}



bool procesar_tramo(vector<vector<int>> & matriz_distancias, int cant_nodos, int capacidad,
                    vector<int> & demandas, vector<bool> & visitados, Solucion & una_solucion) {

    bool hay_tramo_completo = true;

    // El punto de inicio es el punto cero. En el problema, el punto de inicio debe ser
    // igual que el punto final, ya que toda ruta debe empezar y finalizar en el punto 0.
    int punto_actual = 0;

    // Nodos no visitados, pero que se coloca como "visitado" para descartarlo en la siguiente iteración.
    vector<int> nodos_parcialmente_visitados;

    // En la variable "una_solucion" ingreso el punto 0 para que la ruta esté el punto de inicio.
    una_solucion.ruta.push_back(punto_actual);

    while (true) {
        vector<Nodo> vecinos;
        Nodo nodo;

        // Si el punto actual es diferente al punto final (el punto o nodo igual a cero),
        // entonces colocarlo como visitado.
        if (punto_actual != 0) visitados[punto_actual] = true;

        // Verificar cada punto vecino o nodo vecino del punto actual.
        for (int indice = 0; indice < cant_nodos; indice++) {
            if (matriz_distancias[punto_actual][indice] != 0 and not visitados[indice]) {
                completar_nodo(nodo, matriz_distancias[punto_actual][indice],
                               demandas[indice], indice);
                vecinos.push_back(nodo);
            }
        }

        // Verificar que hayan vecinos.
        if (not vecinos.empty()) {

            // Ordenar descendentemente los vecinos por distancia para elegir
            // el nodo vecino de mayor distancia (criterio voraz).
            sort(vecinos.begin(), vecinos.end(), comparar);

            if (capacidad >= capacidad - vecinos[0].demanda and capacidad - vecinos[0].demanda >= 0) {

                // Es posible viajar por el nodo "vecino[0]", porque la capacidad del camión actual
                // aún tiene espacio para la demanda del nodo "vecino[0]".

                punto_actual = vecinos[0].punto;
                capacidad -= vecinos[0].demanda;
                una_solucion.ruta.push_back(punto_actual);
                una_solucion.carga_total += vecinos[0].demanda;
                una_solucion.distancia_ruta += vecinos[0].distancia;

            } else if (capacidad - vecinos[0].demanda < 0) {
                // La demanda del nodo "vecino[0]" supera la capacidad del camión actualmente.
                // Colocar como visitado este punto, para que en la siguiente iteración no se cuente.
                nodos_parcialmente_visitados.push_back(vecinos[0].punto);
                visitados[vecinos[0].punto] = true;
            } else {
                hay_tramo_completo = false;
                break;
            }
        }

        // En esta línea del algoritmo, si "punto_actual == 0", entonces estoy en el punto final o nodo final y
        // se interpreta que "punto_actual" es el punto final; caso contrario, continuar con el siguiente punto.
        if (punto_actual == 0) break;
    }

    // Colocar los "nodos paracialmente visitados" a su estado correcto (no visitado)
    // si es que hubo estos tipos de nodo visitados.
    if (not nodos_parcialmente_visitados.empty())
        for (int punto_nodo = 0; punto_nodo < cant_nodos; punto_nodo++)
            for (int nodo_parcial_visitado : nodos_parcialmente_visitados)
                if (punto_nodo == nodo_parcial_visitado)
                    visitados[punto_nodo] = false;

    return hay_tramo_completo;
}



bool procesar_rutas(vector<vector<int>> & matriz_distancias, int cant_clientes,
                    vector<int> & demandas, int cant_camiones, int capacidad, vector<Solucion> & solucion) {
    // Vector de nodos visitados
    vector<bool> visitados(cant_clientes + 1, false);

    int indCamion;
    bool sePudoRealizarLaRuta, hay_solucion = true;

    // Para cada camión, realizar la ruta.
    for (indCamion = 0; indCamion < cant_camiones; indCamion++) {

        // Inicializar la solución parcial de la ruta del camión con inicialización en cero en todos los campos.
        Solucion una_solucion = {{}, 0, 0};

        sePudoRealizarLaRuta = procesar_tramo(matriz_distancias, cant_clientes + 1, capacidad,
                                              demandas, visitados, una_solucion);

        if (sePudoRealizarLaRuta) {
            // Sí se pudo realizar una ruta completa.
            solucion.push_back(una_solucion);
        } else {
            // No se pudo realizar una ruta completa.
            hay_solucion = false;
            break;
        }
    }
    return hay_solucion;
}



void imprimir_solucion(vector<Solucion> & solucion) {
    for (int z = 0; z < 94; z++) cout << "-";
    cout << endl << left << setw(26) << "Vehiculo" << setw(15) << "Ruta" << setw(26);
    cout << "Carga total (unidades)" << "Distancia de la ruta (km)" << endl;
    for (int z = 0; z < 94; z++) cout << "-";
    cout << endl;

    int distancia_total = 0;
    int contador_caracteres_ruta;
    for (int i = 0; i < solucion.size(); i++) {
        cout << setw(26) << i + 1;
        contador_caracteres_ruta = 0;
        for (int j = 0; j < solucion[i].ruta.size(); j++) {
            contador_caracteres_ruta++;
            cout << solucion[i].ruta[j] << (j + 1 == solucion[i].ruta.size() ? "" : "-");
            contador_caracteres_ruta++;
        }
        cout << setw(11-contador_caracteres_ruta) << " ";
        cout << right << setw(8) << solucion[i].carga_total;
        cout << setw(26) << solucion[i].distancia_ruta << left << endl;
        distancia_total += solucion[i].distancia_ruta;
    }

    for (int z = 0; z < 94; z++) cout << "-";
    cout << endl << setw(68) << "Distancia total recorrida" << distancia_total << " km" << endl;
    cout << "por la flota" << endl;
    for (int z = 0; z < 94; z++) cout << "-";
    cout << endl;
}


int main() {
    // Ubicación de los clientes en un grafo que represente la ciudad
    vector<vector<int>> matriz_distancias = {
        {0, 12, 15, 9, 14, 10, 18},
        {12, 0, 10, 17, 20, 20, 14},
        {15, 10, 0, 11, 19, 22, 22},
        {9, 17, 11, 0, 8, 21, 16},
        {14, 20, 19, 8, 0, 13, 12},
        {10, 20, 22, 21, 13, 0, 16},
        {18, 14, 22, 16, 12, 16, 0}
    };

    // Cantidad de clientes
    int cant_clientes = matriz_distancias.size() - 1;

    // Cantidad de electrodomésticos a repartir por cliente
    vector<int> demandas = {0, 30, 40, 25, 50, 20, 35};

    int cant_camiones = 3; // Cantidad de camiones
    int capacidad = 100; // Capacidad de cada camión

    vector<Solucion> solucion;
    if (procesar_rutas(matriz_distancias, cant_clientes, demandas, cant_camiones, capacidad, solucion)) {
        imprimir_solucion(solucion);
    } else {
        cout << "No hay solucion";
    }
    return 0;
}
