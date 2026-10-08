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

    // El punto de inicio es el punto cero. Toda ruta debe empezar y finalizar en el almacén.
    int punto_actual = 0;

    // Nodos que no pueden ser atendidos por el camión actual debido a su capacidad restante.
    // Se marcan temporalmente como visitados para descartarlos durante esta ruta.
    vector<int> nodos_parcialmente_visitados;

    while (true) {
        vector<Nodo> vecinos;
        Nodo nodo;

        // Si el punto actual es diferente del almacén, entonces el cliente ya fue atendido.
        if (punto_actual != 0) visitados[punto_actual] = true;


        // Verificar los clientes vecinos del punto actual.
        // Se empieza desde 1, porque el nodo 0 es el almacén.
        // El almacén no debe competir contra los clientes en el criterio voraz.
        for (int indice = 1; indice < cant_nodos; indice++) {

            if (matriz_distancias[punto_actual][indice] != 0 and not visitados[indice]) {

                completar_nodo(nodo, matriz_distancias[punto_actual][indice],
                               demandas[indice], indice);
                vecinos.push_back(nodo);
            }
        }

        // Si existen clientes disponibles.
        if (not vecinos.empty()) {

            // CRITERIO VORAZ: Ordenar ascendentemente los vecinos según su distancia.
            // De esta manera se intenta seleccionar primero al cliente más cercano al punto actual.
            sort(vecinos.begin(), vecinos.end(), comparar);


            // Verificar si el cliente más cercano puede ser atendido
            // con la capacidad restante del camión.
            if (capacidad >= vecinos[0].demanda) {

                // El cliente puede ser atendido.
                punto_actual = vecinos[0].punto;
                capacidad -= vecinos[0].demanda;
                una_solucion.ruta.push_back(punto_actual);
                una_solucion.carga_total += vecinos[0].demanda;
                una_solucion.distancia_ruta += vecinos[0].distancia;

            } else {

                // El cliente más cercano no puede ser atendido, porque
                // su demanda supera la capacidad restante del camión.
                // Se marca temporalmente como visitado para intentar
                // con el siguiente cliente más cercano.

                nodos_parcialmente_visitados.push_back(vecinos[0].punto);
                visitados[vecinos[0].punto] = true;
            }
        } else {

            // Si no existen más clientes que este camión pueda atender,
            // entonces debe regresar al almacén.

            if (punto_actual != 0) {
                una_solucion.distancia_ruta += matriz_distancias[punto_actual][0];
                una_solucion.ruta.push_back(0);
                break;
            }

            // Si se está en el almacén y no se pudo seleccionar ningún cliente,
            // entonces el camión no puede realizar una ruta válida.
            hay_tramo_completo = false;
            break;
        }
    }


    // Los nodos descartados temporalmente solamente fueron descartados para el camión actual.
    // Por lo tanto, deben volver al estado "no visitado" para que puedan ser considerados
    // por el siguiente camión.
    if (not nodos_parcialmente_visitados.empty()) {
        for (int punto_nodo = 0; punto_nodo < cant_nodos; punto_nodo++)
            for (int nodo_parcial_visitado : nodos_parcialmente_visitados)
                if (punto_nodo == nodo_parcial_visitado)
                    visitados[punto_nodo] = false;
    }


    return hay_tramo_completo;
}



bool procesar_rutas(vector<vector<int>> & matriz_distancias, int cant_clientes, vector<int> & demandas,
                    int cant_camiones, int capacidad, vector<Solucion> & solucion) {

    // Vector de nodos visitados. El índice 0 representa al almacén.
    vector<bool> visitados(cant_clientes + 1, false);

    bool sePudoRealizarLaRuta;
    bool hay_solucion = true;

    // Para cada camión, realizar una ruta.
    for (int indCamion = 0; indCamion < cant_camiones; indCamion++) {

        // Antes de utilizar otro camión se verifica si todavía
        // existen clientes pendientes.
        bool existen_clientes_pendientes = false;

        for (int i = 1; i <= cant_clientes; i++) {
            if (not visitados[i]) {
                existen_clientes_pendientes = true;
                break;
            }
        }

        // Si todos los clientes ya fueron atendidos, no es necesario utilizar más camiones.
        if (not existen_clientes_pendientes) break;

        // Inicializar la solución parcial. La ruta comienza en el almacén (nodo 0).
        Solucion una_solucion = {{0}, 0, 0};

        sePudoRealizarLaRuta = procesar_tramo(matriz_distancias, cant_clientes + 1,
                                              capacidad, demandas, visitados, una_solucion);

        if (sePudoRealizarLaRuta) {
            // Sí se pudo realizar una ruta completa.
            solucion.push_back(una_solucion);
        } else {
            // El camión no pudo atender ningún cliente pendiente.
            hay_solucion = false;
            break;
        }
    }


    // Después de utilizar los camiones disponibles, verificar
    // que todos los clientes hayan sido atendidos.
    for (int i = 1; i <= cant_clientes; i++) {
        if (not visitados[i]) {
            hay_solucion = false;
            break;
        }
    }

    return hay_solucion;
}


int contar_digitos(int numero) {
    if (numero == 0) return 1;
    int cantidad = 0;
    while (numero > 0) {
        cantidad++;
        numero /= 10;
    }
    return cantidad;
}


int contar_caracteres_ruta(vector<int> & ruta) {
    int cantidad = 0;
    for (int i = 0; i < ruta.size(); i++) {

        // Contar los dígitos del nodo.
        cantidad += contar_digitos(ruta[i]);

        // Contar el guion entre nodos.
        if (i + 1 < ruta.size()) cantidad++;
    }
    return cantidad;
}


void imprimir_solucion(vector<Solucion> & solucion) {
    int ancho_vehiculo = 15;
    int ancho_carga = 26;
    int ancho_distancia = 28;

    // Determinar automáticamente el ancho necesario
    // para mostrar correctamente todas las rutas.
    int ancho_ruta = 15;

    for (int i = 0; i < solucion.size(); i++) {

        int cantidad_caracteres = contar_caracteres_ruta(solucion[i].ruta);

        if (cantidad_caracteres + 4 > ancho_ruta)
            ancho_ruta = cantidad_caracteres + 4;
    }

    int ancho_total = ancho_vehiculo + ancho_ruta + ancho_carga + ancho_distancia;

    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl << left << setw(ancho_vehiculo) << "Vehiculo" << setw(ancho_ruta) << "Ruta";
    cout << setw(ancho_carga) << "Carga total (unidades)" << "Distancia de la ruta (km)" << endl;
    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl;

    int distancia_total = 0;

    for (int i = 0; i < solucion.size(); i++) {
        cout << left << setw(ancho_vehiculo) << i + 1;

        // Contar cuánto ocupará la ruta antes de imprimirla.
        int cantidad_caracteres = contar_caracteres_ruta(solucion[i].ruta);

        // Imprimir la ruta.
        for (int j = 0; j < solucion[i].ruta.size(); j++) {
            cout << solucion[i].ruta[j];
            if (j + 1 < solucion[i].ruta.size()) cout << "-";
        }

        // Completar únicamente el espacio necesario
        // de acuerdo con el ancho calculado para la columna.
        cout << setw(ancho_ruta - cantidad_caracteres) << " ";

        cout << left << setw(ancho_carga) << solucion[i].carga_total;
        cout << solucion[i].distancia_ruta << endl;

        distancia_total += solucion[i].distancia_ruta;
    }

    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl << left << setw(ancho_vehiculo + ancho_ruta + ancho_carga);
    cout << "Distancia total recorrida por la flota" << distancia_total << " km" << endl;
    for (int z = 0; z < ancho_total; z++) cout << "-";
    cout << endl;
}



int main() {
    // Ubicación de los clientes en un grafo que representa la ciudad.
    vector<vector<int>> matriz_distancias = {
        {0, 12, 15, 9, 14, 10, 18},
        {12, 0, 10, 17, 20, 20, 14},
        {15, 10, 0, 11, 19, 22, 22},
        {9, 17, 11, 0, 8, 21, 16},
        {14, 20, 19, 8, 0, 13, 12},
        {10, 20, 22, 21, 13, 0, 16},
        {18, 14, 22, 16, 12, 16, 0}
    };

    // Cantidad de clientes.
    int cant_clientes = matriz_distancias.size() - 1;

    // Cantidad de electrodomésticos a repartir por cliente.
    vector<int> demandas = {0, 30, 40, 25, 50, 20, 35};

    // Cantidad de camiones.
    int cant_camiones = 3;

    // Capacidad de cada camión.
    int capacidad = 100;

    vector<Solucion> solucion;

    if (procesar_rutas(matriz_distancias, cant_clientes, demandas,
                       cant_camiones, capacidad, solucion)) {

        imprimir_solucion(solucion);

    } else {
        cout << "No hay solucion";
    }

    return 0;
}