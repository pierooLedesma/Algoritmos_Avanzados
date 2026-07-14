#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <string>

#define IND 20
#define NUMITERACIONES 50
#define TSELECCION 0.5
#define TCASAMIENTO 0.5
#define NRULETA 110

#define NCLIENTES 20
#define NVEHICULOS 8
#define NNODOS 60
#define DEPOSITO 0

#define COSTOKM 1.0
#define COSTOEXCESOTERMICO 200.0
#define COSTOEXCESOJORNADA 200.0

using namespace std;

struct Tcliente {
    int nodo;
    double volumen;
    double inicioVentana;
    double finVentana;
    double penalizacion;
};

struct Tvehiculo {
    double capacidad;
    double limiteTermico;
    double maxOperacion;
};


/*
 * Forma de solución:
 * El cromosoma se divide en 20 bloques de 8 bits. El bloque 'i'
 * representa al cliente 'i' y contiene exactamente un bit igual a 1.
 * La posicion del '1' indica el vehículo asignado a dicho cliente.
 */
int vehiculoAsignado(vector<int> cromo,int cliente) {
    for (int j=0;j<NVEHICULOS;j++)
        if (cromo[cliente*NVEHICULOS+j]==1)
            return j;
    return -1;
}



/*
 * Forma de solución:
 * Una solución es aberración cuando un cliente no tiene exactamente
 * un vehículo asignado o cuando el volumen total asignado a algún
 * vehículo supera su capacidad máxima de carga.
 */
bool aberracion(vector<int> cromo,Tcliente *clientes,Tvehiculo *vehiculos) {
    double carga[NVEHICULOS]{0};

    for (int i=0;i<NCLIENTES;i++) {
        int cantidad=0;
        int vehiculo=-1;

        for (int j=0;j<NVEHICULOS;j++) {
            if (cromo[i*NVEHICULOS+j]==1) {
                cantidad++;
                vehiculo=j;
            }
        }

        if (cantidad!=1) return true;

        carga[vehiculo]+=clientes[i].volumen;
    }

    for (int j=0;j<NVEHICULOS;j++)
        if (carga[j]>vehiculos[j].capacidad) return true;

    return false;
}

/*
 * Forma de solución:
 * Se genera un individuo asignando cada cliente a un vehículo que
 * todavía tenga capacidad. Se procesa a los clientes en un orden
 * aleatorio para producir individuos diferentes en la población.
*/
bool generaindividuo(vector<int> &individuo,Tcliente *clientes, Tvehiculo *vehiculos) {
    individuo.assign(NCLIENTES * NVEHICULOS, 0);

    double carga[NVEHICULOS]{};
    vector<int> orden;

    for (int i = 0; i < NCLIENTES; i++)
        orden.push_back(i);

    for (int i = 0; i < NCLIENTES; i++) {
        int pos = rand() % NCLIENTES;
        int aux = orden[i];
        orden[i] = orden[pos];
        orden[pos] = aux;
    }

    for (int k = 0; k < NCLIENTES; k++) {
        int cliente = orden[k];
        vector<int> factibles;

        for (int j = 0; j < NVEHICULOS; j++)
            if (carga[j] + clientes[cliente].volumen <= vehiculos[j].capacidad)
                factibles.push_back(j);

        if (factibles.empty())
            return false;

        int vehiculo = factibles[rand()%factibles.size()];
        individuo[cliente * NVEHICULOS + vehiculo] = 1;
        carga[vehiculo] += clientes[cliente].volumen;
    }

    return true;
}



void generapoblacioninicial(vector<vector<int>> &poblacion,
                            Tcliente *clientes,Tvehiculo *vehiculos) {
    int cont = 0;

    while (cont < IND) {
        vector<int> individuo;

        if (generaindividuo(individuo, clientes, vehiculos)) {
            poblacion.push_back(individuo);
            cont++;
        }
    }
}



/*
 * Forma de solución:
 * Para evaluar una asignación binaria se construye la ruta de cada
 * vehículo mediante vecino más cercano. El costo suma distancia,
 * penalizaciones por incumplir ventanas y costos por exceder los
 * límites térmico y de jornada del vehículo.
*/
double calculacosto(vector<int> cromo,Tcliente *clientes, Tvehiculo *vehiculos,
                    double distancia[][NNODOS], double tiempo[][NNODOS]) {
    if (aberracion(cromo,clientes,vehiculos))
        return 1000000000.0;

    double costoTotal = 0;

    for (int v=0; v < NVEHICULOS; v++) {
        vector<int> visitado(NCLIENTES,0);
        int nodoActual=DEPOSITO;
        double reloj = 0;
        double distanciaRuta = 0;

        while (true) {
            int siguiente = -1;
            double mejorTiempo = 1000000000.0;

            for (int i=0; i<NCLIENTES; i++) {
                if (not visitado[i] and vehiculoAsignado(cromo,i) == v) {
                    double t = tiempo[nodoActual][clientes[i].nodo];

                    if (t<mejorTiempo) {
                        mejorTiempo = t;
                        siguiente = i;
                    }
                }
            }

            if (siguiente == -1) break;

            distanciaRuta += distancia[nodoActual][clientes[siguiente].nodo];
            double llegada = reloj + tiempo[nodoActual][clientes[siguiente].nodo];

            if (llegada > clientes[siguiente].finVentana)
                costoTotal += clientes[siguiente].penalizacion;

            if (llegada < clientes[siguiente].inicioVentana)
                reloj = clientes[siguiente].inicioVentana;
            else
                reloj = llegada;

            nodoActual = clientes[siguiente].nodo;
            visitado[siguiente] = 1;
        }

        if (nodoActual != DEPOSITO) {
            distanciaRuta += distancia[nodoActual][DEPOSITO];
            reloj += tiempo[nodoActual][DEPOSITO];
        }

        costoTotal += distanciaRuta*COSTOKM;

        if (reloj > vehiculos[v].limiteTermico)
            costoTotal += (reloj - vehiculos[v].limiteTermico) * COSTOEXCESOTERMICO;

        if (reloj > vehiculos[v].maxOperacion)
            costoTotal += (reloj - vehiculos[v].maxOperacion) * COSTOEXCESOJORNADA;
    }

    return costoTotal;
}



/*
 * Forma de solución:
 * Como el objetivo original es minimizar el costo, se transforma
 * a una aptitud de maximización. Un costo menor produce un fitness
 * mayor, lo que permite mantener la ruleta del código base.
*/
int calculafitness(vector<int> cromo,Tcliente *clientes, Tvehiculo *vehiculos,
                   double distancia[][NNODOS], double tiempo[][NNODOS]) {

    double costo = calculacosto(cromo,clientes,vehiculos,distancia,tiempo);
    int fitness = round(1000000.0 / (1.0 + costo));

    if (fitness < 1) fitness = 1;

    return fitness;
}



void calculasupervivencia(vector<vector<int>> poblacion, vector<int> &supervivencia,
                          Tcliente *clientes,Tvehiculo *vehiculos,
                          double distancia[][NNODOS], double tiempo[][NNODOS]) {
    int sumafitness = 0;
    int mejor = 0;

    for (int i = 0; i < poblacion.size(); i++) {
        int fitness = calculafitness(poblacion[i],clientes,vehiculos, distancia,tiempo);
        sumafitness += fitness;

        if (fitness > calculafitness(poblacion[mejor], clientes, vehiculos, distancia,tiempo))
            mejor = i;
    }

    int sumaPorcentajes = 0;

    for (int i=0; i < poblacion.size(); i++) {
        int supfit = (int)(100.0 *
                     calculafitness(poblacion[i],clientes,
                                    vehiculos, distancia, tiempo) / sumafitness);
        supervivencia.push_back(supfit);
        sumaPorcentajes += supfit;
    }

    supervivencia[mejor] += 100 - sumaPorcentajes;
}



void cargaruleta(vector<int> supervivencia,int *ruleta) {
    int cont = 0;

    for (int i=0; i < supervivencia.size(); i++)
        for (int j = 0; j < supervivencia[i] and cont < 100; j++)
            ruleta[cont++] = i;
}



/*
 * Forma de solución:
 * Se usa selección por ruleta como en el código base. Se selecciona
 * el 50% de la población para que participe en casamiento.
*/
void seleccionar(vector<vector<int>> poblacion, vector<vector<int>> &seleccion,
                 Tcliente *clientes,Tvehiculo *vehiculos,
                 double distancia[][NNODOS],
                 double tiempo[][NNODOS]) {
    vector<int> supervivencia;
    int ruleta[NRULETA]{};

    calculasupervivencia(poblacion,supervivencia, clientes, vehiculos, distancia, tiempo);
    cargaruleta(supervivencia, ruleta);

    int nselec = round(poblacion.size() * TSELECCION);

    for (int i = 0; i < nselec; i++) {
        int ganador = rand() % 100;
        int ind = ruleta[ganador];
        seleccion.push_back(poblacion[ind]);
    }
}


void copiabloque(vector<int> origen,vector<int> &destino,int cliente) {
    for (int j = 0; j < NVEHICULOS; j++)
        destino[cliente * NVEHICULOS + j] = origen[cliente * NVEHICULOS + j];
}



/*
 * Forma de solución:
 * El bloque heredado se selecciona aleatoriamente entre dos índices
 * de clientes. Se copian bloques completos de 8 bits para conservar
 * que cada cliente tenga exactamente un vehículo asignado.
 */
void creahijos(vector<int> padre,vector<int> madre,
               vector<int> &hijo1,vector<int> &hijo2) {
    hijo1.assign(NCLIENTES*NVEHICULOS,0);
    hijo2.assign(NCLIENTES*NVEHICULOS,0);

    int inicio = rand() % NCLIENTES;
    int fin = rand() % NCLIENTES;

    if (inicio > fin) {
        int aux = inicio;
        inicio = fin;
        fin = aux;
    }

    for (int i = 0; i < NCLIENTES; i++) {
        if (i >= inicio and i <= fin) {
            copiabloque(padre,hijo1, i);
            copiabloque(madre,hijo2, i);
        }
        else {
            copiabloque(madre,hijo1, i);
            copiabloque(padre,hijo2, i);
        }
    }
}



/*
 * Forma de solución:
 * Los padres seleccionados se emparejan de dos en dos. Cada pareja
 * produce dos hijos mediante un bloque aleatorio. Solo se insertan
 * hijos que respeten asignación única y capacidad de volumen.
*/
void casamiento(vector<vector<int>> &poblacion,
                vector<vector<int>> seleccion,
                Tcliente *clientes,Tvehiculo *vehiculos) {
    for (int i = 0; i + 1 < seleccion.size(); i += 2) {
        vector<int> hijo1, hijo2;
        creahijos(seleccion[i],seleccion[i+1],hijo1,hijo2);

        if (not aberracion(hijo1, clientes, vehiculos))
            poblacion.push_back(hijo1);

        if (not aberracion(hijo2, clientes, vehiculos))
            poblacion.push_back(hijo2);
    }
}



string clave(vector<int> cromo) {
    string numero;

    for (int i = 0; i < cromo.size(); i++)
        numero += char('0' + cromo[i]);

    return numero;
}


void mataclon(vector<vector<int>> &poblacion) {
    map<string,vector<int>> municos;

    for (int i=0;i<poblacion.size();i++)
        municos[clave(poblacion[i])] = poblacion[i];

    poblacion.clear();

    for (map<string,vector<int>>::iterator it = municos.begin(); it != municos.end();it++)
        poblacion.push_back(it->second);
}


void regenerapoblacion(vector<vector<int>> &poblacion,
                       Tcliente *clientes,Tvehiculo *vehiculos,
                       double distancia[][NNODOS],
                       double tiempo[][NNODOS]) {
    mataclon(poblacion);

    sort(poblacion.begin(),poblacion.end(),
         [clientes, vehiculos, distancia, tiempo]
         (const vector<int> &a,const vector<int> &b) {
             return calculafitness(a, clientes, vehiculos, distancia, tiempo)>
                    calculafitness(b, clientes, vehiculos, distancia, tiempo);
         });

    if (poblacion.size() > IND)
        poblacion.erase(poblacion.begin() + IND,poblacion.end());

    while (poblacion.size() < IND) {
        vector<int> individuo;
        if (generaindividuo(individuo, clientes, vehiculos))
            poblacion.push_back(individuo);
    }

    mataclon(poblacion);

    while (poblacion.size() < IND) {
        vector<int> individuo;
        if (generaindividuo(individuo, clientes, vehiculos))
            poblacion.push_back(individuo);
    }
}



void muestramejor(vector<vector<int>> &poblacion, Tcliente *clientes,Tvehiculo *vehiculos,
                  double distancia[][NNODOS], double tiempo[][NNODOS]) {
    cout<<fixed<<setprecision(2);
    cout<<"Costo mejor = "
        <<calculacosto(poblacion[0],clientes,vehiculos,distancia,tiempo)
        <<endl;

    for (int i = 0; i < NCLIENTES; i++)
        cout << "Cliente " << i + 1 << " -> Vehiculo "
            <<vehiculoAsignado(poblacion[0], i) + 1<<endl;
}



void FrioExpressAG(Tcliente *clientes,Tvehiculo *vehiculos,
                   double distancia[][NNODOS], double tiempo[][NNODOS]) {
    vector<vector<int>> poblacion;

    srand(time(NULL));
    generapoblacioninicial(poblacion, clientes, vehiculos);

    for (int i = 0; i < NUMITERACIONES; i++) {
        vector<vector<int>> seleccion;

        seleccionar(poblacion,seleccion, clientes, vehiculos, distancia, tiempo);
        casamiento(poblacion,seleccion, clientes, vehiculos);
        regenerapoblacion(poblacion, clientes, vehiculos, distancia, tiempo);

        cout<<"Generacion "<<i+1<<" - ";
        cout<<"mejor costo: "<<fixed<<setprecision(2)
            <<calculacosto(poblacion[0],clientes,vehiculos,
                           distancia,tiempo)<<endl;
    }

    cout<<endl<<"MEJOR SOLUCION FINAL"<<endl;
    muestramejor(poblacion,clientes,vehiculos,distancia,tiempo);
}




// Los siguientes datos son solamente una instancia de prueba.
void cargardatosprueba(Tcliente *clientes,Tvehiculo *vehiculos,
                       double distancia[][NNODOS], double tiempo[][NNODOS]) {
    for (int i=0;i<NCLIENTES;i++) {
        clientes[i].nodo = i + 1;
        clientes[i].volumen = 1.0 + (i % 5) * 0.5;
        clientes[i].inicioVentana = 1.0 + (i % 4) * 2.0;
        clientes[i].finVentana = clientes[i].inicioVentana + 4.0;
        clientes[i].penalizacion = 50.0 + i * 5.0;
    }

    for (int i = 0; i < NVEHICULOS; i++) {
        vehiculos[i].capacidad = 12.0;
        vehiculos[i].limiteTermico = 12.0;
        vehiculos[i].maxOperacion = 10.0;
    }

    for (int i=0; i < NNODOS; i++)
        for (int j = 0; j < NNODOS; j++) {
            if (i == j) {
                distancia[i][j] = 0;
                tiempo[i][j] = 0;
            }
            else {
                int diferencia = i - j;
                if (diferencia < 0) diferencia -= diferencia;
                distancia[i][j] = 1 + (diferencia % 15);
                tiempo[i][j] = distancia[i][j] / 20.0;
            }
        }
}


int main() {
    Tcliente clientes[NCLIENTES];
    Tvehiculo vehiculos[NVEHICULOS];
    double distancia[NNODOS][NNODOS];
    double tiempo[NNODOS][NNODOS];

    cargardatosprueba(clientes,vehiculos,distancia,tiempo);
    FrioExpressAG(clientes,vehiculos,distancia,tiempo);

    return 0;
}