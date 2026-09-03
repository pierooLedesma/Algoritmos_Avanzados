#include <iostream>
#include <vector>
using namespace  std;

void imprimir_solucion(vector<int> &solucion) {
    cout << "{";
    for (int i = 0; i < solucion.size(); i++)
        i + 1 < solucion.size() ? cout << solucion[i] << "," : cout << solucion[i] << "}" << endl;
}


void agrupar_backtracking(int ind_ingreso, int ingreso[], int cant_ingresos,
                          int capacidad[], int cant_ubicaciones, vector<int> &rack,
                          bool &solucion_encontrada) {

    // Si ya se encontró una solución válida, entonces se detiene la búsqueda.
    if (solucion_encontrada) return;

    // Caso base: Si todos los grupos de productos fueron ubicados,
    //            significa que se encontró una distribución válida.
    if (ind_ingreso == cant_ingresos) {
        imprimir_solucion(rack);
        solucion_encontrada = true;
        return;
    }


    // Se prueban todas las ubicaciones posibles del rack para colocar el producto actual.
    for (int ind_capacidad = 0; ind_capacidad < cant_ubicaciones; ind_capacidad++) {

        // El producto solo puede colocarse si la suma de los
        // productos almacenado actualmente más el nuevo producto
        // no supera la capacidad máxima de la ubicación.
        if (ingreso[ind_ingreso] + rack[ind_capacidad] <= capacidad[ind_capacidad]) {

            // ELECCIÓN: Se coloca temporalmente el producto actual
            //           en la ubicación seleccionada.
            rack[ind_capacidad] += ingreso[ind_ingreso];

            // EXPLORACIÓN: Se continúa con el siguiente producto
            agrupar_backtracking(ind_ingreso + 1, ingreso, cant_ingresos, capacidad,
                                 cant_ubicaciones, rack, solucion_encontrada);

            // Deshacer elección (ocurre aquí el BACKTRACKING):
            // Se deshace la elección realizada anteriormente
            // para probar otra ubicación diferente.
            rack[ind_capacidad] -= ingreso[ind_ingreso];
        }
    }
}

int main() {
    int ingreso[] = {2,3,1,4};     // Grupos de productos que deben ser almacenados.
    int capacidad[] = {3,2,5,1,3}; // Capacidades máximas de cada ubicación del rack.

    // Inicialmente todas las ubicaciones en el "rack" están vacías.
    vector<int> rack(sizeof(capacidad)/sizeof(capacidad[0]), 0);

    // Se utiliza para detener la búsqueda después de encontrar la primera solución válida.
    bool solucion_encontrada = false;

    agrupar_backtracking(0, ingreso, sizeof(ingreso)/sizeof(ingreso[0]), capacidad,
           rack.size(), rack, solucion_encontrada);
    return 0;
}
