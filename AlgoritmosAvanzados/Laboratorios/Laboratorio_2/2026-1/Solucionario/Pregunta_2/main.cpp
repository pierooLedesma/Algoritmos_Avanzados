#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

void ingreso_de_actas(int &actas_lima, int &actas_extranjero) {
    cout << "Ingrese las actas de Lima: ";
    cin >> actas_lima;
    cout << "Ingrese las actas del extranjero: ";
    cin >> actas_extranjero;
}


void completar_matriz_de_soluciones(int *regiones, int cant_regiones, vector<vector<bool>> &matriz_de_soluciones, int suma_maxima) {
    // Ordenar ascendentemente las actas de las regiones
    sort(regiones, regiones + cant_regiones);

    // Completar con 1's la primera columna de la matríz de soluciones (el "dp").
    for (int i = 0; i < cant_regiones; i++) matriz_de_soluciones[i][0] = true;

    // Completar la matriz de soluciones
    for (int i = 1; i <= cant_regiones; i++) {
        for (int j = 1; j <= suma_maxima; j++) {
            matriz_de_soluciones[i][j] = matriz_de_soluciones[i - 1][j];
            if (j >= regiones[i - 1]) {
                matriz_de_soluciones[i][j] =
                    matriz_de_soluciones[i][j] or matriz_de_soluciones[i - 1][j - regiones[i - 1]];
            }
        }
    }

    // Impresión de la matriz de soluciones
    cout << "IMPRESIÓN DE LA MATRIZ DE SOLUCIONES:" << endl;
    for (int i = 0; i < cant_regiones; i++)
        for (int j = 0; j < suma_maxima; j++)
            cout << matriz_de_soluciones[i][j] << " ";
}


void validar_actas(vector<vector<bool>> matriz_de_soluciones, int *regiones, int cant_regiones, int suma_maxima) {
    cout << endl << endl << endl << endl << endl << endl;
    cout << "Las validaciones dieron como resultado:" << endl;

    cout << "Cantidad total de actas ";
    cout << (matriz_de_soluciones[cant_regiones][suma_maxima] ? "correcta." : "incorrecta.") << endl;

    // Se ordenó anteriormente las "regiones" y se conoce que las actas del extranjero y del oriente
    // son las que tienen menos actas; por ende, estas dos regiones son las dos primeras en "regiones".
    cout << "Suma de actas del oriente y del extranjero ";
    cout << (matriz_de_soluciones[2][7000] ? "correcta." : "incorrecta.") << endl;

    cout << "Diferencia de actas entre los dos grupos ";
    int suma_actas = 0; // Total de actas sin contabilizar las actas de Lima y Sierra Sur.
    int grupo_1; // Suma de actas del Extranjero, Oriente, Costa Sur y Sierra Centro.
    int grupo_2; // Suma de actas de la Costa Norte y Sierra Norte.
    for (int i = 0; i < cant_regiones - 2; i++) suma_actas += regiones[i];

    // Encontrar la mayor suma del "grupo_1"
    for (grupo_1 = suma_actas; grupo_1 > 0; grupo_1--)
        if (matriz_de_soluciones[4][grupo_1]) break;

    grupo_2 = suma_actas - grupo_1;
    int diferencia = grupo_2 - grupo_1;
    cout << (diferencia == 3000 ? "correcta." : "incorrecta.") << endl << endl;

    cout << "Por tal motivo se declara cantidades ";
    cout << (matriz_de_soluciones[cant_regiones][suma_maxima] and matriz_de_soluciones[2][7000]
             and diferencia == 3000 ? "correcta." : "incorrecta.") << endl;
}


int main() {
    int actas_lima, actas_extranjero;
    ingreso_de_actas(actas_lima, actas_extranjero);
    int regiones[] = {actas_lima, 6000, 12000, 8000, 12000, 15000, actas_extranjero, 4500};
    int cant_regiones = sizeof(regiones) / sizeof(regiones[0]);
    int suma_maxima = 95000;
    vector<vector<bool>> matriz_de_soluciones(cant_regiones + 1, vector<bool>(suma_maxima + 1, false));
    completar_matriz_de_soluciones(regiones, cant_regiones, matriz_de_soluciones, suma_maxima);
    validar_actas(matriz_de_soluciones, regiones, cant_regiones, suma_maxima);
}