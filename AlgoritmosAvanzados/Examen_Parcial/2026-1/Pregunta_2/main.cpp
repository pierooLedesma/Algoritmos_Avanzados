
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// Imprimir la matriz binaria donde
// dp[i][j] indica si mensaje[i..j] es palindromo.
void imprimir_matriz_solucion(const vector<vector<bool>> & dp, const char mensaje[], int cant_letras) {

    cout << "============================================" << endl;
    cout << "=== IMPRESION DE LA MATRIZ DE SOLUCIONES ===" << endl;
    cout << "============================================" << endl;

    cout << "     ";
    for (int j = 0; j < cant_letras; j++) cout << setw(3) << j;
    cout << endl;

    for (int i = 0; i < cant_letras; i++) {
        cout << setw(2) << i << " " << mensaje[i] << " ";
        for (int j = 0; j < cant_letras; j++) cout << setw(3) << dp[i][j];
        cout << endl;
    }
}


// Recuperar e imprimir el palíndromo
// de mayor longitud encontrado mediante DP.
void imprimir_largo_del_mensaje(const char mensaje[], int inicio_maximo, int largo_maximo) {

    cout << endl << "=== RESPUESTA ===" << endl;
    cout << "Largo del mensaje oculto: " << largo_maximo << endl;
    cout << "Mensaje oculto: ";

    for (int i = inicio_maximo; i < inicio_maximo + largo_maximo; i++) {
        cout << mensaje[i];
    }
    cout << endl;
}


// Completar la matriz de menor a mayor longitud
// usando las soluciones de subcadenas más pequeñas.
void resolver(vector<vector<bool>> & dp, const char mensaje[], int cant_letras) {

    int largo_maximo = 0;
    int inicio_maximo = 0;

    // Evaluar todas las longitudes posibles.
    for (int largo = 1; largo <= cant_letras; largo++) {

        // Evaluar cada subcadena de ese largo.
        for (int i = 0; i + largo <= cant_letras; i++) {

            int j = i + largo - 1;

            // CASO BASE 1:
            // Un caracter siempre es palíndromo.
            if (largo == 1) {
                dp[i][j] = true;
            }

            // CASO BASE 2:
            // Dos caracteres deben ser iguales.
            else if (largo == 2) {
                dp[i][j] = (mensaje[i] == mensaje[j]);
            }

            // CASO GENERAL:
            // Extremos iguales e interior palíndromo.
            else {
                dp[i][j] = (mensaje[i] == mensaje[j]) and dp[i + 1][j - 1];
            }

            // Actualizar la mejor solucion.
            if (dp[i][j] and largo > largo_maximo) {
                largo_maximo = largo;
                inicio_maximo = i;
            }
        }
    }

    imprimir_matriz_solucion(dp, mensaje, cant_letras);
    imprimir_largo_del_mensaje(mensaje, inicio_maximo, largo_maximo);
}


int main() {
    char mensaje[] = "SOLILOLPLALI";
    int cant_letras =sizeof(mensaje) / sizeof(mensaje[0]) - 1;

    // Matriz binaria de soluciones.
    vector<vector<bool>> dp(cant_letras, vector<bool>(cant_letras, false));

    resolver(dp, mensaje, cant_letras);

    return 0;
}
