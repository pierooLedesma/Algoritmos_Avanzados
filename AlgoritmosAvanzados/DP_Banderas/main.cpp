#include <iostream>
#include <vector>
using namespace std;
#define ESTADO 4

/*
 * ==================== ENUNCIADO =======================
 *
 * En el día de la Bandera de Rusia, el dueño de una tienda decidió decorar
 * el escaparate de su establecimiento con franjas textiles de colores
 * blanco, azul y rojo. Desea cumplir las siguientes condiciones:
 *
 *   1) No se pueden colocar franjas del mismo color una al lado de otra.
 *   2) Siempre debe colocarse una franja azul entre una blanca y una roja,
 *      o entre una roja y una blanca.
 *
 * Determinar el número de maneras de cumplir su deseo.
 *
 * Ejemplo: Para N = 3, el resultado es el siguiente:
 *      1) Blanco / Rojo   / Blanco
 *      2) Rojo   / Azul   / Blanco
 *      3) Blanco / Azul   / Rojo
 *      4) Rojo   / Blanco / Rojo
 *
 * Tomar en cuenta que: 'N' es el número de franjas en el que 1 <= N <= 45.
 *
 *
 * ======================= MUESTRAS =========================
 * ╔═══════════════════════╗════════════════════════════════╗
 * ║  Número de franjas    ║  Número de maneras o banderas  ║
 * ║═══════════════════════║════════════════════════════════║
 * ║          1            ║                2               ║
 * ║          2            ║                2               ║
 * ║          3            ║                4               ║
 * ║          4            ║                6               ║
 * ╚═══════════════════════╚════════════════════════════════╝
 */


void imprimir_dp(vector<vector<int>> &dp) {
    cout << "Matriz dp:\n" << endl;

    for (int i = 0; i < dp.size(); i++) {
        for (int j = 0; j < dp[0].size(); j++) {
            cout << dp[i][j] << " ";
        }
        cout << '\n';
    }
    cout << endl << endl;
}

int encontrar_franjas(int franjas) {

    // dp[ESTADO][franjas + 1]
    vector<vector<int>> dp(ESTADO,vector<int>(franjas + 1,0));


    // Para una franjas
    dp[0][1] = 1; // Blanco
    dp[1][1] = 1; // Rojo
    dp[2][1] = 0; // Azul después de blanco
    dp[3][1] = 0; // Azul después de rojo

    for (int j = 2; j <= franjas; j++) {

        // Termina en blanco
        // Antes hay "rojo" o "azul después de rojo"
        dp[0][j] = dp[1][j - 1] + dp[3][j - 1];

        // Termina en rojo
        // Antes hay "blanco" o "azul después de blanco"
        dp[1][j] = dp[0][j - 1] + dp[2][j - 1];

        // Azul después de blanco
        dp[2][j] = dp[0][j - 1];

        // Azul después de rojo
        dp[3][j] = dp[1][j - 1];
    }

    imprimir_dp(dp);

    return dp[0][franjas] + dp[1][franjas];
}

int main() {
    int franjas, cantidad_franjas;
    cout << "Ingrese el valor de franjas:";
    cin >> franjas;

    cantidad_franjas = encontrar_franjas(franjas);
    cout << "Con " << franjas << " franjas se puede(n) formar ";
    cout << cantidad_franjas << " banderas." << endl;

    return 0;
}
