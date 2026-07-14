#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <algorithm>
#include <iomanip>

#define ITERACIONES 100
#define NUMIND 20
#define TSELECCION 0.5
#define PCASAMIENTO 0.5
#define TMUTACION 0.3
#define TAMRULETA 120

#define NOPERADORES 7
#define NMAQUINAS 5

using namespace std;

/*
 Forma de solución:
 El cromosoma tiene un gen entero por operador. El valor del gen
 esta entre 1 y 5 e indica la máquina asignada a ese operador.
 La aptitud es la suma de ganancias de todas las asignaciones.
*/
int calculafitness(vector<int> cromo,
                   int ganancia[][NMAQUINAS]) {
    int total = 0;

    for(int i=0; i < cromo.size(); i++)
        total += ganancia[i][cromo[i] - 1];

    return total;
}



/*
 Forma de solución:
 Se cuenta cuantos operadores fueron asignados a cada maquina.
 Es aberracion si una maquina queda vacia, si supera su capacidad
 maxima o si algun gen no representa una maquina valida.
*/
bool aberracion(vector<int> ind,int *capacidad,
                int n,int m) {
    vector<int> carga(m+1,0);

    for(int i=0;i<n;i++) {
        int maquina=ind[i];

        if(maquina<1 or maquina>m)
            return true;

        carga[maquina]++;
    }

    for(int j=1;j<=m;j++) {
        if(carga[j]==0)
            return true;

        if(carga[j]>capacidad[j-1])
            return true;
    }

    return false;
}



void muestrapoblacion(vector<vector<int>> poblacion,
                      int ganancia[][NMAQUINAS]) {
    for(int i=0;i<poblacion.size();i++) {
        for(int j=0;j<poblacion[i].size();j++)
            cout<<poblacion[i][j]<<" ";

        cout<<" fitness="
            <<calculafitness(poblacion[i],ganancia)
            <<endl;
    }
}



/*
 Forma de solución:
 Se compactan los genes enteros en una cadena. Como las máquinas
 se numeran del 1 al 5, cada cromosoma distinto produce una clave
 distinta que puede utilizarse para eliminar clones con map.
*/
string clave(vector<int> ind) {
    string s;

    for(int i=0;i<ind.size();i++)
        s+=to_string(ind[i]);

    return s;
}



void mataclon(vector<vector<int>> &poblacion) {
    map<string,vector<int>> unicos;

    for(int i=0;i<poblacion.size();i++)
        unicos[clave(poblacion[i])]=poblacion[i];

    poblacion.clear();

    for(auto &ind:unicos)
        poblacion.push_back(ind.second);
}



/*
 Forma de solución:
 Se generan cromosomas enteros aleatorios con valores de 1 a M.
 Solo se incorporan los individuos que asignan por lo menos un
 operador a cada máquina y respetan todas las capacidades máximas.
*/
void generapoblacion(vector<vector<int>> &poblacion,
                     int *capacidad, int n, int m) {
    int cont=0;

    while(cont < NUMIND) {
        vector<int> individuo;

        for(int i=0;i<n;i++)
            individuo.push_back(1 + rand() % m);

        if(not aberracion(individuo, capacidad, n, m)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}



/*
 Forma de solución:
 La probabilidad de supervivencia es proporcional a la aptitud.
 Los porcentajes se truncan y el residuo se agrega al individuo
 más apto para que la ruleta tenga exactamente cien posiciones.
*/
void calculasupervivencia(vector<vector<int>> poblacion,
                          vector<int> &supervivencia,
                          int ganancia[][NMAQUINAS]) {
    int sumaFitness=0;
    int mejor=0;

    for(int i=0;i<poblacion.size();i++) {
        int fit=calculafitness(poblacion[i],ganancia);
        sumaFitness += fit;

        if(fit>calculafitness(poblacion[mejor],ganancia))
            mejor = i;
    }

    int sumaPorcentajes = 0;

    for(int i=0;i<poblacion.size();i++) {
        int porc=(int)(100.0*
                 calculafitness(poblacion[i],ganancia)/
                 sumaFitness);

        supervivencia.push_back(porc);
        sumaPorcentajes += porc;
    }

    supervivencia[mejor] += 100 - sumaPorcentajes;
}



void cargaruleta(vector<int> supervivencia,int *ruleta) {
    int cont=0;

    for(int i=0;i<supervivencia.size();i++)
        for(int j=0;j<supervivencia[i] and cont<100;j++)
            ruleta[cont++]=i;
}



/*
 Forma de solución:
 Se escoge el cincuenta por ciento de la población y los individuos con mayor
 ganancia ocupan mas posiciones, por lo que tienen mayor probabilidad.
*/
void seleccion(vector<vector<int>> poblacion,
               vector<vector<int>> &padres,
               int ganancia[][NMAQUINAS]) {
    int ruleta[TAMRULETA]{};
    vector<int> supervivencia;

    calculasupervivencia(poblacion,supervivencia,ganancia);
    cargaruleta(supervivencia,ruleta);

    int npadres=round(poblacion.size()*TSELECCION);

    for(int i=0;i<npadres;i++) {
        int ticket=rand()%100;
        padres.push_back(poblacion[ruleta[ticket]]);
    }
}



/*
 Forma de solución:
 El hijo recibe el primer 50 por ciento de genes del padre y los
 genes restantes de la madre. Cada gen conserva un número válido
 de máquina, aunque luego se verifica la factibilidad completa.
*/
void creahijo(vector<int> padre,vector<int> madre,
              vector<int> &hijo) {
    int pos=round(padre.size()*PCASAMIENTO);

    for(int i=0;i<pos;i++)
        hijo.push_back(padre[i]);

    for(int i=pos;i<madre.size();i++)
        hijo.push_back(madre[i]);
}



/*
 Forma de solución:
 Todos los padres seleccionados se combinan con los demás padres.
 Se usa un corte al 50 por ciento y solo se agregan los hijos que
 dejan todas las máquinas activas y no exceden sus capacidades.
*/
void casamiento(vector<vector<int>> &poblacion,
                vector<vector<int>> padres,
                int *capacidad,int n,int m) {
    for(int i=0;i<padres.size();i++)
        for(int j=0;j<padres.size();j++)
            if(i!=j) {
                vector<int> hijo;
                creahijo(padres[i],padres[j],hijo);

                if(not aberracion(hijo,capacidad,n,m))
                    poblacion.push_back(hijo);
            }
}




/*
 Forma de solución:
 Se muta el 30 por ciento de los genes de cada padre seleccionado.
 Cada posición elegida recibe una máquina distinta a la anterior,
 entre 1 y M. Solo se conserva el mutante cuando sigue siendo factible.
*/
void mutacion(vector<vector<int>> &poblacion,
              vector<vector<int>> padres,
              int *capacidad,int n,int m) {
    int nmut=round(n*TMUTACION);

    for(int i=0;i<padres.size();i++) {
        vector<int> usado(n,0);
        int cont=0;

        while(cont<nmut) {
            int gen=rand()%n;

            if(usado[gen]==0) {
                int nueva=1+rand()%m;

                while(nueva==padres[i][gen])
                    nueva=1+rand()%m;

                padres[i][gen]=nueva;
                usado[gen]=1;
                cont++;
            }
        }

        if(not aberracion(padres[i],capacidad,n,m))
            poblacion.push_back(padres[i]);
    }
}



/*
 Forma de solución:
 Se adapta la inversión del codigo entero usando M+1-gen. De esta
 manera 1 cambia con 5, 2 con 4 y 3 permanece igual. El individuo
 invertido solo se agrega cuando mantiene todas las restricciones.
*/
void inversion(vector<vector<int>> &poblacion,
               vector<vector<int>> padres,
               int *capacidad,int n,int m) {
    for(int i=0;i<padres.size();i++) {
        for(int j=0;j<n;j++)
            padres[i][j]=m+1-padres[i][j];

        if(not aberracion(padres[i],capacidad,n,m))
            poblacion.push_back(padres[i]);
    }
}



/*
 Forma de solución:
 Se eliminan clones y se ordena la población de mayor a menor
 ganancia. Luego se conservan como maximo los mejores NUMIND
 cromosomas, aplicando reemplazo elitista entre padres e hijos.
*/
void regenerapoblacion(vector<vector<int>> &poblacion,
                       int ganancia[][NMAQUINAS]) {
    mataclon(poblacion);

    sort(poblacion.begin(),poblacion.end(),
         [ganancia](const vector<int>& a,
                    const vector<int>& b) {
            return calculafitness(a,ganancia)>
                   calculafitness(b,ganancia);
         });

    if(poblacion.size()>NUMIND)
        poblacion.erase(poblacion.begin()+NUMIND,
                        poblacion.end());
}



void muestramejor(vector<vector<int>> poblacion,
                  int ganancia[][NMAQUINAS],
                  int *capacidad,int m) {
    cout<<endl<<"Mejor fitness = "
        <<calculafitness(poblacion[0],ganancia)
        <<endl;

    for(int i=0;i<poblacion[0].size();i++)
        cout<<"Operador "<<i+1
            <<" -> Maquina "<<poblacion[0][i]
            <<endl;

    vector<int> carga(m+1,0);

    for(int maquina:poblacion[0])
        carga[maquina]++;

    cout<<"Carga por maquina: ";

    for(int j=1;j<=m;j++)
        cout<<carga[j]<<"/"<<capacidad[j-1]<<" ";

    cout<<endl;
}



void TupiaCleanAG(int ganancia[][NMAQUINAS], int *capacidad,int n,int m) {
    vector<vector<int>> poblacion;

    srand(time(NULL));
    generapoblacion(poblacion,capacidad,n,m);

    for(int it=0;it<ITERACIONES;it++) {
        vector<vector<int>> padres;

        seleccion(poblacion,padres,ganancia);
        casamiento(poblacion,padres,capacidad,n,m);
        mutacion(poblacion,padres,capacidad,n,m);
        inversion(poblacion,padres,capacidad,n,m);
        regenerapoblacion(poblacion,ganancia);

        cout<<"ITERACION "<<setw(3)<<it+1
            <<" - Mejor ganancia: "
            <<calculafitness(poblacion[0],ganancia)
            <<endl;
    }

    cout<<endl<<"MEJOR SOLUCION FINAL"<<endl;
    muestramejor(poblacion,ganancia,capacidad,m);
}

int main() {
    int ganancia[NOPERADORES][NMAQUINAS]={
        {3, 4, 5, 3, 6},
        {3, 2, 1, 2, 5},
        {1, 8, 5, 3, 1},
        {3, 6, 4, 6, 3},
        {8, 2, 10, 4, 9},
        {7, 6, 3, 2, 3},
        {4, 5, 1, 3, 6}
    };

    int capacidad[NMAQUINAS] = {2, 3, 1, 4, 3};

    TupiaCleanAG(ganancia, capacidad, NOPERADORES,NMAQUINAS);

    return 0;
}
