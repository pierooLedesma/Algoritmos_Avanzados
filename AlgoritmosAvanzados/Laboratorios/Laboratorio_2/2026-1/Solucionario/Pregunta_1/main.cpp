// Nombre y Apellido: Alessandro Piero Ledesma Guerra
// Código: 20217340
// Fecha: 25 de abril del 2026

#include <iostream>
using namespace std;

void ordenar_por_tiempo_de_fin(int datos[][6],int eventos) {
    for (int i = 0; i < eventos - 1; i++) {
        for (int j = i + 1; j < eventos; j++) {
            if (datos[1][i] > datos[1][j]) {
                for (int k=0; k < 3; k++) { // 3 atributos: Inicio, Fin, Pago
                    int temporal = datos[k][i];
                    datos[k][i] = datos[k][j];
                    datos[k][j] = temporal;
                }
            }
        }
    }
}


int programacion_dinamica(int datos[][6], int eventos) {
    int dp[eventos], pago_actual, pago_dp_actual, nuevo_pago;
    dp[0] = datos[2][0];
    for (int i = 1; i <= eventos; i++) {
        pago_actual = datos[2][i-1];
        pago_dp_actual = dp[i-1];
        nuevo_pago = 0;

        // Verificar si el "Inicio" del evento actual es mayor
        // que el "Fin" del evento anterior.
        if (datos[0][i - 1] > datos[1][i - 2]) {
            nuevo_pago = dp[i-1];

            // Si cumplen exactamente una hora de diferencia entre
            // el "Inicio" del evento actual con el "Fin" del evento anterior.
            if (datos[0][i - 1] - datos[1][i - 2] == 1) {
                nuevo_pago += 15;
            }
        }
        nuevo_pago = nuevo_pago + pago_actual;
        dp[i] = max(pago_dp_actual, nuevo_pago);
    }
    for (int p=0; p<eventos; p++) cout << dp[p] << ' ';
    cout << endl << endl;
    return dp[eventos];
}


int main() {
    int eventos = 6;
    int datos[3][6] = {
        {1, 4, 6, 6, 5, 8},
        {3, 5, 8, 8, 9, 12},
        {30, 10, 60, 20, 50, 40}
    };

    // A pesar que está ordenado en el problema, lo ordeno de tal forma se pueda
    // ingresar datos que no estén ordenados para otros casos.
    ordenar_por_tiempo_de_fin(datos, eventos);

    cout << "Ganancia maxima: " << programacion_dinamica(datos, eventos);
    return 0;
}
