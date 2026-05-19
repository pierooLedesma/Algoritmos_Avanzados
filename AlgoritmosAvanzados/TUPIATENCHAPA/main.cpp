#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    int trabajos[] = {5,10,30,20,15};
    int cant_trabajos = sizeof(trabajos)/sizeof(trabajos[0]);
    int cant_robots = 2;
    int pref[cant_trabajos + 1];

    pref[0] = 0;

    // Sumas acumuladas
    for(int i = 1; i <= cant_trabajos; i++) {
        pref[i] = pref[i-1] + trabajos[i-1];
    }

    int dp[cant_robots][cant_trabajos];

    // Inicializar en 0
    for(int i = 0; i < cant_robots; i++) {
        for(int j = 0; j < cant_trabajos; j++) {
            dp[i][j] = 0;
        }
    }

    // Caso base: 1 robot
    for(int j = 1; j <= cant_trabajos; j++) {
        dp[1][j] = pref[j];
    }

    // Programación dinámica
    for(int i = 2; i <= cant_robots; i++){
        for(int j = 1; j <= cant_trabajos; j++){
            int minimo = 999999;

            for(int p = 1; p < j; p++) {
                int ultimoRobot = pref[j] - pref[p];

                int actual = max(dp[i-1][p], ultimoRobot);
                if(actual < minimo) {
                    minimo = actual;
                }
            }

            // Caso especial
            if(j == 1) dp[i][j] = trabajos[0];
            else dp[i][j] = minimo;
        }
    }
    
    // Mostrar matriz DP
    cout << "MATRIZ DP\n\n";
    for(int i = 1; i <= cant_robots; i++) {
        for(int j = 1; j <= cant_trabajos; j++) {
            cout << setw(5) << dp[i][j];
        }

        cout << endl;
    }
    cout << "\nTiempo representativo minimo: " << dp[cant_robots][cant_trabajos] << endl;

    return 0;
}
