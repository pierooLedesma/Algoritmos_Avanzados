#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Tarea {
    int tiempo;
    int peso;
    char letra;
};


struct Solucion {
    struct Tarea tarea;
    double ratio;
    double tiempoAcumulado;
    double costoPonderado;
};


bool compara(struct Tarea uno, struct Tarea dos) {
    return (double)uno.peso/uno.tiempo > (double)dos.peso/dos.tiempo or
           ((double)uno.peso/uno.tiempo == (double)dos.peso/dos.tiempo and uno.peso < dos.peso);
}


void imprimirSolucion(vector<Solucion> conjuntoDeSolucion) {
    double costoTotalPonderado = 0.0;
    cout << fixed << setprecision(2);
    cout << "============================================" << endl;
    cout << "ORDENAMIENTO FINAL SEGUN LA REGLA DE SMITH" << endl;
    cout << "============================================" << endl;
    for (int i = 0; i < conjuntoDeSolucion.size(); i++) {
        cout << "Tarea: " << conjuntoDeSolucion[i].tarea.letra << endl;
        cout << "Tiempo procesamiento: " << (double)conjuntoDeSolucion[i].tarea.tiempo << endl;
        cout << "Peso: " << (double)conjuntoDeSolucion[i].tarea.peso << endl;
        cout << "Completion time: " << conjuntoDeSolucion[i].tiempoAcumulado << endl;
        cout << "Costo ponderado: " << conjuntoDeSolucion[i].costoPonderado << endl;
        cout << "--------------------------------------------" << endl;
        costoTotalPonderado += conjuntoDeSolucion[i].costoPonderado;
    }
    cout << "COSTO TOTAL PONDERADO: " << costoTotalPonderado << endl;
}


void minimizar(vector<Tarea> conjuntoDeTareas) {

    // Inicializar el vector "conjuntoDeSolucion" para cada elemento de dicho vector.
    vector<Solucion> conjuntoDeSolucion(conjuntoDeTareas.size(), {0,0,0,0,0,0});

    // Ordenar descendentemente el vector "conjuntoDeTareas" por el ratio (peso / tiempo), como primera condición,
    // y ordenar por peso de forma ascendente (como segunda condición).
    sort(conjuntoDeTareas.begin(), conjuntoDeTareas.end(), compara);

    conjuntoDeSolucion[0].tarea = conjuntoDeTareas[0];
    conjuntoDeSolucion[0].ratio = (double)conjuntoDeTareas[0].peso / conjuntoDeTareas[0].tiempo;
    conjuntoDeSolucion[0].tiempoAcumulado += conjuntoDeTareas[0].tiempo;
    conjuntoDeSolucion[0].costoPonderado += conjuntoDeSolucion[0].tiempoAcumulado * conjuntoDeSolucion[0].tarea.peso;
    for (int ind = 1; ind < conjuntoDeTareas.size(); ind++) {
        conjuntoDeSolucion[ind].tarea = conjuntoDeTareas[ind];
        conjuntoDeSolucion[ind].ratio = (double)conjuntoDeTareas[ind].peso / conjuntoDeTareas[ind].tiempo;
        conjuntoDeSolucion[ind].tiempoAcumulado = conjuntoDeSolucion[ind - 1].tiempoAcumulado + conjuntoDeTareas[ind].tiempo;
        conjuntoDeSolucion[ind].costoPonderado += conjuntoDeSolucion[ind].tiempoAcumulado * conjuntoDeSolucion[ind].tarea.peso;
    }

    imprimirSolucion(conjuntoDeSolucion);
}


int main() {
    vector<Tarea> conjuntoDeTareas = {
        {4, 20, 'A'},
        {2, 10, 'B'},
        {5, 15, 'C'},
        {3, 18, 'D'}
    };
    minimizar(conjuntoDeTareas);
    return 0;
}
