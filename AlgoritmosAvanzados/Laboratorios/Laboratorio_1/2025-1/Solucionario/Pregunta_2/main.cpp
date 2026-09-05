#include <iostream>
#include <vector>
using namespace std;

void ingresar_datos(int &filas, int &columnas) {
    cout << "Ingrese la cantidad de filas: ";
    cin >> filas;
    cout << "Ingrese la cantidad de columnas: ";
    cin >> columnas;
}

bool esFactible(vector<vector<int>> &almacen, int fila, int columna) {

    // Obtener las dimensiones actuales del almacén
    int filas = almacen.size();
    int columnas = almacen[0].size();


    // Revisar las 8 posiciones alrededor de la ubicación candidata.
    // Un palet afecta a sus vecinos:
    //
    //      diagonal   arriba   diagonal
    //           \       |       /
    //            \      |      /
    // izquierda ---- PALLET ---- derecha
    //            /      |      \
    //           /       |       \
    //      diagonal  abajo   diagonal
    //
    // Por eso se revisan los movimientos desde -1 hasta 1
    // en filas y columnas.
    for(int i = -1; i <= 1; i++) {

        for(int j = -1; j <= 1; j++) {

            // Se ignora la posición actual porque
            // allí se quiere colocar el nuevo palet.
            if(i == 0 && j == 0)
                continue;

            // Calcular la posición del vecino que se está revisando.
            int nf = fila + i;
            int nc = columna + j;

            // Verificar que el vecino esté dentro de los límites
            // del almacén.
            if(nf >= 0 and nf < filas and nc >= 0 and nc < columnas) {

                // Si existe un palet en alguna posición vecina,
                // no se puede colocar un nuevo palet aquí porque
                // estarían juntos horizontal, vertical o diagonalmente.
                if(almacen[nf][nc] != 0)
                    return false;
               }
        }
    }

    // Si ninguna posición vecina tiene un palet,
    // entonces la ubicación es válida para colocar uno.
    return true;
}


void encontrar_maximo(vector<vector<int>> &almacen, int posicion, vector<vector<int>> &solucion, int &maximo, int contador) {
    int filas = almacen.size();
    int columnas = almacen[0].size();

    // Caso base: Se revisó todas las posiciones
    if (posicion == filas * columnas) {
        if (maximo < contador) {
            maximo = contador;
            solucion = almacen;
        }
        return;
    }

    int fila = posicion / columnas;
    int columna = posicion % columnas;

    if (not (fila==0 and columna==0)) {
        if (esFactible(almacen, fila, columna)) {

            // ELECCIÓN: Colocar palet
            almacen[fila][columna] = contador + 1;

            // EXPLORACIÓN:
            encontrar_maximo(almacen, posicion+1, solucion, maximo, contador + 1);

            // BACKTRACKING: Quitar palet para probar otra alternativa
            almacen[fila][columna] = 0;
        }
    }

    // También probar no colocar palet
    encontrar_maximo(almacen, posicion+1, solucion, maximo, contador);
}


void imprimir_solucion(vector<vector<int>> &solucion, int maximo) {
    cout << "El maximo es: " << maximo << endl;
    for (int i = 0; i < solucion.size(); i++) { // Filas
        for (int j = 0; j < solucion[0].size(); j++) { // Columnas
            j + 1 < solucion[0].size() ? cout << solucion[i][j] << ' ' : cout << solucion[i][j] << endl;
        }
    }
}


int main() {
    int filas, columnas;
    ingresar_datos(filas, columnas);

    // Crear almacén vacío: 0 representa una ubicación sin palet.
    vector<vector<int>> almacen(filas, vector<int>(columnas, 0));

    // Almacenar la mejor distribución encontrada (al inicio almacén y solución son lo mismo).
    vector<vector<int>> solucion = almacen;

    int maximo = 0; // Guarda la cantidad máxima de palets encontrados.
    int contador = 0; // Contador inicial de palets colocados.

    // Iniciar Backtracking desde la primera posición (posicion = 0).
    encontrar_maximo(almacen, 0, solucion, maximo, contador);

    imprimir_solucion(solucion, maximo);

    return 0;
}
