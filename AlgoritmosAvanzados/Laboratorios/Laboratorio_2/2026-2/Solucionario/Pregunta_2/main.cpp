#include <iostream>
#include <vector>
using namespace std;

void imprimir_terreno(const vector<vector<char>> & terreno) {
    int filas = terreno.size();
    int columnas = terreno[0].size();

    for (int j = 0; j < columnas * 2; j++) cout << "=";
    cout << endl;

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << terreno[i][j] << " ";
        }
        cout << endl;
    }

    for (int j = 0; j < columnas * 2; j++) cout << "=";

    cout << endl << endl;
}


void imprimir_solucion(vector<vector<bool>> & matriz) {
    cout << endl << "=== MATRIZ DP ===" << endl;
    for (int i = 0; i < matriz.size(); i++) {
        for (int j = 0; j < matriz[0].size(); j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


// Determinar si cada casilla libre es alcanzable
// desde la posición superior o izquierda.
bool buscar_escombros(const vector<vector<char>> & terreno, int filas,
                      int columnas, vector<vector<bool>> & dp) {

    // CASO BASE 1: Posicion inicial.
    dp[0][0] = (terreno[0][0] != '*');

    // CASO BASE 2: Primera fila.
    // Solamente se puede llegar desde la izquierda.
    for (int j = 1; j < columnas; j++)
        dp[0][j] = (terreno[0][j] != '*') and dp[0][j - 1];

    // CASO BASE 3: Primera columna.
    // Solamente se puede llegar desde arriba.
    for (int i = 1; i < filas; i++)
        dp[i][0] = (terreno[i][0] != '*') and dp[i - 1][0];

    // CASO GENERAL: Posiciones internas.
    for (int i = 1; i < filas; i++) {
        for (int j = 1; j < columnas; j++) {

            // Si hay escombros, entonces no es alcanzable.
            if (terreno[i][j] == '*') {
                dp[i][j] = false;
            }
            else {
                // Alcanzable desde arriba o izquierda.
                dp[i][j] = dp[i - 1][j] or dp[i][j - 1];
            }
        }
    }

    imprimir_solucion(dp);

    // Verificar la posicion final.
    return dp[filas - 1][columnas - 1];
}


// Reconstruir una ruta valida utilizando
// la matriz DP, desde el destino hasta el origen.
// No se crea ninguna matriz auxiliar.
void marcar_ruta(vector<vector<char>> & terreno, const vector<vector<bool>> & dp) {

    int i = terreno.size() - 1;
    int j = terreno[0].size() - 1;

    if (not dp[i][j]) return;

    while (true) {

        // Colocar las letras A, B, C...
        int paso = i + j;

        // "26" porque del 0 al 25 son los índices de las letras del abecedario.
        terreno[i][j] = (paso < 26) ? char('A' + paso) : '.';

        // Se llego al origen.
        if (i == 0 and j == 0) break;

        // Retroceder a una casilla alcanzable.
        if (i > 0 and dp[i - 1][j]) {
            i--;
        }
        else {
            j--;
        }
    }
}


int main() {
    vector<vector<char>> terreno = {
        {' ', ' ', ' ', '*', '*'},
        {' ', ' ', ' ', '*', ' '},
        {' ', ' ', '*', ' ', ' '},
        {' ', ' ', ' ', ' ', ' '},
        {' ', '*', '*', ' ', '*'},
        {' ', '*', '*', ' ', '*'},
        {' ', '*', ' ', ' ', ' '},
        {'*', '*', '*', '*', ' '},
        {' ', ' ', ' ', ' ', ' '}
    };

    // vector<vector<char>> terreno = {
    //     {' ', ' ', ' ', '*', '*'},
    //     {' ', ' ', ' ', '*', ' '},
    //     {' ', ' ', '*', ' ', ' '},
    //     {' ', ' ', ' ', ' ', ' '},
    //     {' ', '*', '*', ' ', '*'},
    //     {' ', '*', '*', ' ', '*'},
    //     {' ', '*', ' ', '*', ' '},
    //     {'*', '*', '*', '*', ' '},
    //     {' ', ' ', ' ', ' ', ' '}
    // };

    int filas = terreno.size();
    int columnas = terreno[0].size();

    cout << "Se recibe como datos de entrada: n=";
    cout << filas << " y m=" << columnas << endl;

    imprimir_terreno(terreno);

    // Solo dos matrices: terreno y dp.
    vector<vector<bool>> dp(filas, vector<bool>(columnas, false));

    bool hay_salida = buscar_escombros(terreno, filas, columnas, dp);

    cout << "La solucion para llegar desde (0,0) " << "hasta (" << filas - 1 << ",";
    cout << columnas - 1 << ") es:" << endl;
    cout << (hay_salida ? "Si" : "No") << " se encontro una salida." << endl;

    if (hay_salida) {
        marcar_ruta(terreno, dp);
        cout << endl << "Una ruta valida:" << endl;
        imprimir_terreno(terreno);
    }

    return 0;
}
