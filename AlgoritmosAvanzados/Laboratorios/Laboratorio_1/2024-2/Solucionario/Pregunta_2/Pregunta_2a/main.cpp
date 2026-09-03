#include <iostream>
#include <vector>
using namespace  std;

void imprimir_solucion(vector<int> &solucion) {
    cout << "{";
    for (int i = 0; i < solucion.size(); i++)
        i + 1 < solucion.size() ? cout << solucion[i] << "," : cout << solucion[i] << "}" << endl;
}

void agrupar_backtracking(int ind_ingreso, int ingreso[], int cant_ingresos, int capacidad[],
                          int cant_ubicaciones, vector<int> &rack, bool &solucion_encontrada) {
    if (solucion_encontrada) return;
    if (ind_ingreso == cant_ingresos) {
        imprimir_solucion(rack);
        solucion_encontrada = true;
        return;
    }
    for (int ind_capacidad = 0; ind_capacidad < cant_ubicaciones; ind_capacidad++) {
        if (ingreso[ind_ingreso] + rack[ind_capacidad] <= capacidad[ind_capacidad]) {
            rack[ind_capacidad] += ingreso[ind_ingreso];
            agrupar_backtracking(ind_ingreso + 1, ingreso, cant_ingresos, capacidad, cant_ubicaciones, rack, solucion_encontrada);
            rack[ind_capacidad] -= ingreso[ind_ingreso];
        }
    }
}

int main() {
    int ingreso[] = {2,3,1,4};
    int capacidad[] = {3,2,5,1,3};
    vector<int> rack(sizeof(capacidad)/sizeof(capacidad[0]), 0);
    bool solucion_encontrada = false;
    agrupar_backtracking(0, ingreso, sizeof(ingreso)/sizeof(ingreso[0]), capacidad, rack.size(), rack, solucion_encontrada);
    return 0;
}
