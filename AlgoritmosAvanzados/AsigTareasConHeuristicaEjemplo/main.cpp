#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Orden {
    int id;
    int cantidad;
    int tiempoTotal;
};

// Ordenar de mayor a menor
bool comparar(Orden a, Orden b) {
    return a.tiempoTotal > b.tiempoTotal;
}

int main() {

    vector<int> botellas = {
        103,58,88,126,195,
        90,54,195,124,113,
        193,55,97,97,169,
        50,167,74,79,109
    };

    vector<Orden> ordenes;

    // Crear órdenes
    for(int i=0; i<20; i++) {

        Orden o;

        o.id = i + 1;
        o.cantidad = botellas[i];

        // 6 minutos por botella
        o.tiempoTotal = botellas[i] * 6;

        ordenes.push_back(o);
    }

    // Ordenar descendentemente
    sort(ordenes.begin(),
         ordenes.end(),
         comparar);

    // 5 líneas de producción
    vector<int> carga(5,0);

    vector<string> asignadas[5];

    // Algoritmo voraz
    for(auto orden : ordenes) {

        int mejorLinea = 0;

        // Buscar línea menos cargada
        for(int i=1; i<5; i++) {

            if(carga[i] < carga[mejorLinea]) {
                mejorLinea = i;
            }
        }

        // Asignar orden
        carga[mejorLinea] +=
            orden.tiempoTotal;

        asignadas[mejorLinea]
            .push_back(
                "Orden" +
                to_string(orden.id)
            );
    }

    // Mostrar resultados
    cout << "ASIGNACION FINAL\n\n";

    for(int i=0; i<5; i++) {

        cout << "Linea "
             << i+1 << ": ";

        for(string s : asignadas[i]) {
            cout << s << " ";
        }

        cout << "\nCarga total: "
             << carga[i]
             << " minutos\n\n";
    }

    // Makespan
    int makespan =
        *max_element(
            carga.begin(),
            carga.end()
        );

    cout << "Makespan = "
         << makespan
         << " minutos\n";

    return 0;
}
