#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

vector<int> generarNivelesVoraz(const char movimientos[]) {
    // Estrategia voraz: elegir el menor nivel que permita
    // completar las bajadas consecutivas de la posicion actual.
    int n = static_cast<int>(strlen(movimientos));

    vector<int> niveles;
    int posicion = 0;
    int menorDisponible = 1;

    // n movimientos requieren n+1 niveles.
    while (posicion <= n) {
        int bajadas = 0;

        // Contar las B consecutivas desde esta posicion.
        while (posicion + bajadas < n
               and movimientos[posicion + bajadas] == 'B') {
            bajadas++;
        }

        // Tomar los bajadas+1 menores niveles disponibles.
        int mayorDelBloque = menorDisponible + bajadas;

        // Escribirlos de mayor a menor para cumplir las B.
        for (int nivel = mayorDelBloque; nivel >= menorDisponible; nivel--) {
            niveles.push_back(nivel);
        }

        // Avanzar al siguiente bloque.
        // Sus niveles seran mayores, cumpliendo la S entre bloques.
        menorDisponible += bajadas + 1;
        posicion += bajadas + 1;
    }

    return niveles;
}

void imprimirNiveles(const vector<int>& niveles) {
    // Mostrar los niveles en el orden de la solucion.
    for (int i = 0; i < static_cast<int>(niveles.size()); i++) {
        if (i > 0) cout << " ";
        cout << niveles[i];
    }
    cout << endl;
}

int main() {
    char movimientos[] = "SBBBSSSSBBBSBSB";

    vector<int> niveles = generarNivelesVoraz(movimientos);

    cout << "Movimientos: " << movimientos << endl;
    cout << "Niveles: ";
    imprimirNiveles(niveles);

    return 0;
}