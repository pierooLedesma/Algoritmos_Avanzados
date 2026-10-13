#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct Solucion {
    vector<int> ruta;
    int carga_total;
    int distancia_ruta;
    int numero_camion;
    int capacidad_camion;
    int distancia_maxima;
};

struct Nodo {
    int punto;
    int distancia;
    int demanda;
    int prioridad; // 1: prioridad alta; 0: prioridad normal.
};


// Primero mayor prioridad y, a igualdad,
// menor distancia desde el punto actual. Desempate por índice.
bool comparar(Nodo nodo_1, Nodo nodo_2) {
    if (nodo_1.prioridad != nodo_2.prioridad)
        return nodo_1.prioridad > nodo_2.prioridad;

    if (nodo_1.distancia != nodo_2.distancia)
        return nodo_1.distancia < nodo_2.distancia;

    return nodo_1.punto < nodo_2.punto;
}


// Completar los datos de un cliente candidato.
void completar_nodo(Nodo & nodo, int distancia, int demanda, int punto, int prioridad) {
    nodo.demanda = demanda;
    nodo.punto = punto;
    nodo.distancia = distancia;
    nodo.prioridad = prioridad;
}


// Construir UNA ruta eligiendo, en cada paso,
// al vecino FACTIBLE de mayor prioridad y menor distancia.
// Se verifica que el camion pueda volver directamente al almacen
// antes de aceptar cada nuevo cliente.
bool procesar_tramo(vector<vector<int>> &matriz_distancias,
                    int cant_nodos, int capacidad, int distancia_maxima,
                    vector<int> & demandas, vector<int> & prioridades,
                    vector<bool> & visitados, Solucion & una_solucion) {

    int punto_actual = 0;

    while (true) {
        vector<Nodo> vecinos;
        Nodo nodo;

        // Se empieza en 1 para no incluir al almacen como cliente.
        for (int indice = 1; indice < cant_nodos; indice++) {

            // Ignorar lazos, conexiones inexistentes y clientes atendidos.
            if (indice != punto_actual and matriz_distancias[punto_actual][indice] > 0 and
                not visitados[indice] and matriz_distancias[indice][0] > 0) {

                // RESTRICCION 1: la demanda cabe en el camion.
                bool capacidad_suficiente = (demandas[indice] <= capacidad);

                // RESTRICCION 2: se puede visitar al cliente
                // y regresar al almacen sin exceder el limite.
                int distancia_estimada = una_solucion.distancia_ruta
                    + matriz_distancias[punto_actual][indice]
                    + matriz_distancias[indice][0];

                bool distancia_suficiente = (distancia_estimada <= distancia_maxima);

                if (capacidad_suficiente && distancia_suficiente) {
                    completar_nodo(nodo, matriz_distancias[punto_actual][indice],
                                   demandas[indice], indice, prioridades[indice]);
                    vecinos.push_back(nodo);
                }
            }
        }

        // Ya no quedan clientes factibles para el camion actual.
        if (vecinos.empty()) break;

        // CRITERIO VORAZ: prioridad alta primero; luego menor distancia.
        sort(vecinos.begin(), vecinos.end(), comparar);

        // Todos los candidatos ya cumplen las restricciones.
        int siguiente = vecinos[0].punto;
        capacidad -= vecinos[0].demanda;
        una_solucion.carga_total += vecinos[0].demanda;
        una_solucion.distancia_ruta += vecinos[0].distancia;
        una_solucion.ruta.push_back(siguiente);
        visitados[siguiente] = true;
        punto_actual = siguiente;
    }

    // Regresar al almacen. Este retorno ya fue reservado al
    // revisar la factibilidad del ultimo cliente elegido.
    if (punto_actual != 0) {
        una_solucion.distancia_ruta += matriz_distancias[punto_actual][0];
        una_solucion.ruta.push_back(0);
        return true;
    }

    // El camión no pudo atender a ningún cliente.
    return false;
}


// Estrategia voraz: procesar cada camion con SU capacidad y
// SU limite de distancia. Si uno no puede salir, probar el siguiente.
// Al finalizar, verificar que todos los clientes fueron atendidos.
bool procesar_rutas(vector<vector<int>> & matriz_distancias,
                    int cant_clientes, vector<int> & demandas,
                    vector<int> & prioridades, vector<int> &capacidades,
                    vector<int> & distancias_maximas, vector<Solucion> & solucion) {

    int cant_camiones = capacidades.size();

    if (distancias_maximas.size() != cant_camiones or
        demandas.size() != cant_clientes + 1 or prioridades.size() != cant_clientes + 1)
        return false;

    vector<bool> visitados(cant_clientes + 1, false);

    for (int indCamion = 0; indCamion < cant_camiones; indCamion++) {
        bool existen_clientes_pendientes = false;

        for (int i = 1; i <= cant_clientes; i++) {
            if (not visitados[i]) {
                existen_clientes_pendientes = true;
                break;
            }
        }

        if (not existen_clientes_pendientes) break;

        Solucion una_solucion = {
            {0}, 0, 0, indCamion + 1,
            capacidades[indCamion], distancias_maximas[indCamion]};

        bool sePudoRealizarLaRuta = procesar_tramo(
            matriz_distancias, cant_clientes + 1,
            capacidades[indCamion], distancias_maximas[indCamion],
            demandas, prioridades, visitados, una_solucion);

        // Si el camión no puede atender, no se agrega, pero
        // se intenta con los otros camiones de distinta capacidad.
        if (sePudoRealizarLaRuta)
            solucion.push_back(una_solucion);
    }

    for (int i = 1; i <= cant_clientes; i++) {
        if (not visitados[i]) return false;
    }

    return true;
}



// Contar dígitos para alinear las rutas en la tabla.
int contar_digitos(int numero) {
    if (numero == 0) return 1;
    int cantidad = 0;
    while (numero > 0) {
        cantidad++;
        numero /= 10;
    }
    return cantidad;
}


// Contar el ancho de la ruta impresa con guiones.
int contar_caracteres_ruta(vector<int> &ruta) {
    int cantidad = 0;
    for (int i = 0; i < (int)ruta.size(); i++) {
        cantidad += contar_digitos(ruta[i]);
        if (i + 1 < (int)ruta.size()) cantidad++;
    }
    return cantidad;
}


// Mostrar las rutas y el total recorrido por la flota.
void imprimir_solucion(vector<Solucion> &solucion) {
    int ancho_vehiculo = 11, ancho_capacidad = 12, ancho_maximo = 12,
        ancho_carga = 15, ancho_distancia = 18, ancho_ruta = 15;

    for (int i = 0; i < (int)solucion.size(); i++) {
        int caracteres = contar_caracteres_ruta(solucion[i].ruta);
        if (caracteres + 3 > ancho_ruta)
            ancho_ruta = caracteres + 3;
    }

    int ancho_total = ancho_vehiculo + ancho_capacidad + ancho_maximo
                      + ancho_ruta + ancho_carga + ancho_distancia;

    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl << left << setw(ancho_vehiculo) << "Vehiculo";
    cout << setw(ancho_capacidad) << "Capacidad" << setw(ancho_maximo) << "Max km";
    cout << setw(ancho_ruta) << "Ruta" << setw(ancho_carga) << "Carga total";
    cout << "Distancia (km)" << endl;
    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl;

    int distancia_total = 0;

    for (int i = 0; i < (int)solucion.size(); i++) {
        cout << left << setw(ancho_vehiculo) << solucion[i].numero_camion;
        cout << setw(ancho_capacidad) << solucion[i].capacidad_camion;
        cout << setw(ancho_maximo) << solucion[i].distancia_maxima;

        int caracteres = contar_caracteres_ruta(solucion[i].ruta);
        for (int j = 0; j < (int)solucion[i].ruta.size(); j++) {
            cout << solucion[i].ruta[j];
            if (j + 1 < (int)solucion[i].ruta.size()) cout << "-";
        }
        cout << setw(ancho_ruta - caracteres) << " ";
        cout << setw(ancho_carga) << solucion[i].carga_total;
        cout << solucion[i].distancia_ruta << endl;

        distancia_total += solucion[i].distancia_ruta;
    }

    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl << "Distancia total recorrida por la flota: "
         << distancia_total << " km" << endl;
}

int main() {
    // Grafo no dirigido. La diagonal incluye lazos positivos.
    // Estos se ignoran al elegir clientes (indice != punto_actual).
    vector<vector<int>> matriz_distancias = {
        { 5, 12, 15,  9, 14, 10, 18},
        {12,  4, 10, 17, 20, 20, 14},
        {15, 10,  6, 11, 19, 22, 22},
        { 9, 17, 11,  3,  8, 21, 16},
        {14, 20, 19,  8,  2, 13, 12},
        {10, 20, 22, 21, 13,  7, 16},
        {18, 14, 22, 16, 12, 16,  4}
    };

    int cant_clientes = matriz_distancias.size() - 1;
    vector<int> demandas = {0, 30, 40, 25, 50, 20, 35};

    // 1 = alta prioridad; 0 = normal.
    vector<int> prioridades = {0, 0, 1, 0, 0, 1, 0};

    // Un valor por camion, en el mismo orden.
    vector<int> capacidades = {80, 75, 70};
    vector<int> distancias_maximas = {45, 40, 45};

    vector<Solucion> solucion;

    if (procesar_rutas(matriz_distancias, cant_clientes,
                       demandas, prioridades, capacidades,
                       distancias_maximas, solucion)) {
        imprimir_solucion(solucion);
    } else {
        cout << "No hay solucion" << endl;
    }

    return 0;
}
