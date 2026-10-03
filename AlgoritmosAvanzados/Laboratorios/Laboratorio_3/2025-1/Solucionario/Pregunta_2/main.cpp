#include <iostream>
#include <vector>
using namespace std;

struct Orden {
    bool tipo; // false : Secar , true: Lavar
    int peso;
};

void mostrar_asignaciones(vector<int> & cantAsignaciones, vector<vector<int>> & asignaciones) {
    cout << "=================================" << endl;
    cout << "Ordenes asignadas a cada lavadora" << endl;
    cout << "=================================" << endl;
    // Recorrer cada lavadora
    for (int i = 0; i < asignaciones.size(); i++) {
        cout << "Lavadora " << i + 1 << " : ";
        // Recorrer cada asignacion de orden
        for (int j = 0; j < cantAsignaciones[i]; j++) {
            cout << asignaciones[i][j] + 1 << (j + 1 < cantAsignaciones[i]  ? ", " : "");
        }
        cout << endl;
    }
    cout << endl;
}


void mostrar_tiempo_por_lavadora(vector<int> & carga) {
    cout << "========================" << endl;
    cout << "Tiempo por cada lavadora" << endl;
    cout << "========================" << endl;
    for (int i = 0; i < carga.size(); i++) { // Recorrer cada lavadora
        cout << "Lavadora " << i + 1 << " : " << carga[i] << endl;
    }
}


void procesar_lavadoras(vector<Orden> & ordenes, int cantLavadoras) {
    // Carga de trabajo (tiempo) por cada lavadora
    vector<int> carga(cantLavadoras, 0);

    // Asignaciones de ordenes a cada lavadora
    vector<vector<int>> asignaciones(cantLavadoras, vector<int>(ordenes.size(), 0));

    // Cantidad de ordenes asignadas a cada lavadora
    vector<int> cantAsignaciones(cantLavadoras, 0);

    /* Para cada orden */
    for (int i = 0; i < ordenes.size(); i++) {

        /* Calcular el tiempo según el tipo */
        int tiempo;
        if (ordenes[i].tipo) tiempo = ordenes[i].peso * 4; // El tipo es "Lavar"
        else tiempo = ordenes[i].peso * 2; // El tipo es "Secar"

        // Encontrar la lavadora que tiene la menor carga de trabajo actual
        int indMin = 0; // Índice de la lavadora con menor tiempo de carga acumulada
        int minTiempoFin = carga[0] + tiempo; // El menor tiempo final (es de la primera lavadora)
        for (int j = 1; j < cantLavadoras; j++) { // Se recorren las demás lavadoras
            if (carga[j] + tiempo < minTiempoFin) {
                minTiempoFin = carga[j] + tiempo; // Actualizar el menor tiempo final
                indMin = j; // Actualizar el índice de lavadora con menor tiempo de carga acumulada
            }
        }

        /* Asignar la orden a la lavadora con menor tiempo acumulado*/
        asignaciones[indMin][cantAsignaciones[indMin]++] = i; // El 'i' es el índice de la orden

        /* Sumar tiempo a la lavadora*/
        carga[indMin] += tiempo;
    }

    mostrar_asignaciones(cantAsignaciones, asignaciones);
    mostrar_tiempo_por_lavadora(carga);
}

int main() {
    vector<Orden> ordenes = {
        {true, 10}, {true, 10}, {false, 8}, {true, 15},
        {false, 9}, {false, 11}, {true, 12}, {false, 15},
        {true, 6}, {false, 10}, {true, 8}, {false, 15},
        {true, 11}, {true, 7}, {true, 7}, {false, 8},
        {false, 9}, {true, 11}, {false, 12}, {true, 15}
    };
    int cantLavadoras = 5;
    procesar_lavadoras(ordenes, cantLavadoras);
    return 0;
}
