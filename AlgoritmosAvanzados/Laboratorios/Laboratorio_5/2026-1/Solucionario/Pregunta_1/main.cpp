#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <map>
#define NUMIND 20
#define NUMGENERACIONES 1000
#define TASASELECCION 0.3
#define PCASAMIENTO 0.5
using namespace std;


struct Proyecto {
    int id;
    int costo;
    int beneficio;
    vector<int> predecesores;
};

bool cumple_predecesores(vector<int> & cromosoma, int indice, vector<int> & predecesores) {
    // El proyecto no tiene predecesores.
    if (predecesores.empty()) return true; // El proyecto no tiene predecesores.
    // El proyecto tiene predecesores y es el primer gen.
    else if (indice == 0) return false;

    int contador = 0;
    for (int j = 0; j < indice; j++) {
        // Proyecto elegido
        if (cromosoma[j] == 1) {
            // Verificar si el proyecto elegido es
            // parte de los predecesores.
            for (int un_predecesor : predecesores) {
                if (cromosoma[j]*(j+1) == un_predecesor) {
                    contador++;
                    break;
                }
            }
        }
    }
    if (contador == predecesores.size()) return true;
    return false;
}


bool esaberracion(vector<int> & cromosoma, vector<Proyecto> & proyectos, int presupuesto) {
    int suma = 0;
    for (int ind = 0; ind < cromosoma.size(); ind++) {
        // Proyecto elegido
        if (cromosoma[ind] == 1) {
            // Verificar los predecesores (hasta del índice "ind" del cromosoma)
            // del proyecto elegido "proyectos[ind]"
            if (not cumple_predecesores(cromosoma, ind, proyectos[ind].predecesores))
                return true;
            suma += cromosoma[ind] * proyectos[ind].costo;
        }
    }
    if (suma > presupuesto) return true;
    return false;
}


int calculafitness(const vector<int> & cromosoma, const vector<Proyecto> & proyectos) {
    int suma = 0;
    for (int i = 0; i < cromosoma.size(); i++)
        suma += cromosoma[i] * proyectos[i].beneficio;
    return suma;
}


void generapoblacion(vector<vector<int>> & poblacion, vector<Proyecto> & proyectos,
                     int cant_proyectos,int presupuesto) {
    int cont=0;
    srand(time(NULL));
    while (cont<NUMIND) {
        vector<int>cromosoma;
        for (int i = 0; i < cant_proyectos; i++)
            cromosoma.push_back(rand()%2);
        if (not esaberracion(cromosoma, proyectos, presupuesto)) {
            poblacion.push_back(cromosoma);
            cont++;
        }
    }


}

void muestrapoblacion(vector<vector<int>> & poblacion, vector<Proyecto> & proyectos) {
    for (int i = 0; i < poblacion.size(); i++) {
        cout << setw(4) << i+1 << " ";
        for (int j = 0; j < poblacion[i].size(); j++)
            cout << poblacion[i][j] << " ";
        cout << "fo=" << calculafitness(poblacion[i], proyectos) << endl;
    }
}


void calculasupervivencia(vector<vector<int>>poblacion, vector<int>&supervivencia,
                          vector<Proyecto> & proyectos) {
    int sumafitness = 0;

    for (int i = 0; i < poblacion.size(); i++)
        sumafitness += calculafitness(poblacion[i], proyectos);

    int acumulado = 0;

    for (int i = 0; i < poblacion.size(); i++) {
        acumulado += calculafitness(poblacion[i], proyectos);
        int limite =round(100*acumulado/sumafitness);
        supervivencia.push_back(limite);
    }
}


void cargaruleta(vector<int> & supervivencia, int *ruleta) {
    int ini=0;
    for (int i=0;i<supervivencia.size();i++) {
        int fin = supervivencia[i];
        for (int j=ini;j<fin;j++)
            ruleta[j]=i;
        ini=fin;
    }
}


void seleccion(vector<vector<int>> poblacion, vector<vector<int>> & mejores,
               vector<Proyecto> & proyectos) {
    int ruleta[100];
    vector<int>supervivencia;
    calculasupervivencia(poblacion, supervivencia, proyectos);
    int numpadres = round(poblacion.size()*TASASELECCION);
    cargaruleta(supervivencia,ruleta);
    for (int i=0;i<numpadres;i++) {
        int ticket = rand()%100;
        int ind = ruleta[ticket];
        mejores.push_back(poblacion[ind]);
    }
}


void creahijo(vector<int> padre, vector<int> madre, vector<int> & hijo){
        int posi = round(padre.size()*PCASAMIENTO);
        for (int i = 0; i < posi; i++)
            hijo.push_back(padre[i]);
        for (int i = posi; i < madre.size(); i++)
            hijo.push_back(madre[i]);
}


void casamiento(vector<vector<int>> & poblacion, vector<vector<int>> padres,
                vector<Proyecto> & proyectos, int presupuesto) {
    for (int i = 0; i < padres.size(); i++) {
        for (int j = 0; j < padres.size(); j++) {
            if (i != j) {
                vector<int>hijo;
                creahijo(padres[i],padres[j],hijo);
                if (not esaberracion(hijo, proyectos, presupuesto)) {
                    poblacion.push_back(hijo);
                }
            }
        }
    }
}


int decimal(vector<int> cromo) {
    int num=0;
    for (int i=0;i<cromo.size();i++)
        num+=cromo[i]*pow(2,i);

    return num;
}


void mataclon(vector<vector<int>>&poblacion) {
    map<int,vector<int>>municos;
    for (int i=0;i<poblacion.size();i++) {
        int pk = decimal(poblacion[i]);
        municos[pk] = poblacion[i];
    }
    poblacion.clear();
    for (map<int,vector<int>>::iterator it=municos.begin();
        it!=municos.end();it++) {
        poblacion.push_back(it->second);
    }
}


void regenerapoblacion(vector<vector<int>> & poblacion, vector<Proyecto> & proyectos) {
    mataclon(poblacion);
    sort(poblacion.begin(), poblacion.end(),
        [proyectos](const vector<int> & cromo_a, const vector<int> & cromo_b) {
            return calculafitness(cromo_a,proyectos) > calculafitness(cromo_b, proyectos);
        });
    if (poblacion.size() > NUMIND)
        poblacion.erase(poblacion.begin() + NUMIND, poblacion.end());
}


void muestra_mejor(const vector<int> & el_mejor, const vector<Proyecto> & proyectos) {
    cout << endl << endl;
    cout << "Proyecto seleccionado: ";
    for (int i = 0; i < el_mejor.size(); i++) {
        if (el_mejor[i] != 0)
            cout << proyectos[i].id << " ";
    }
    cout << endl;
    cout << "Beneficio total: " << calculafitness(el_mejor, proyectos);
    cout << " (Millones de $)" << endl;
}


void ag_proyectos(vector<Proyecto> & proyectos, int cant_proyectos, int presupuesto) {
    vector<vector<int>> poblacion;
    generapoblacion(poblacion, proyectos,cant_proyectos,presupuesto);
    for (int i = 0; i < NUMGENERACIONES; i++) {
        vector<vector<int>> mejores;
        seleccion(poblacion, mejores, proyectos);
        casamiento(poblacion, mejores, proyectos, presupuesto);
        regenerapoblacion(poblacion, proyectos);
    }
    muestrapoblacion(poblacion, proyectos);
    muestra_mejor(poblacion[0], proyectos);
}



int main() {
    vector<Proyecto> proyectos = {
        {1, 100, 200, {}},
        {2, 50, 300, {1}},
        {3, 150, 300, {1}},
        {4, 50, 400, {}},
        {5, 50, 200, {4}},
        {6, 150, 800, {2, 4}},
        {7, 100, 250, {}}
    };
    int presupuesto = 400;
    int cant_proyectos = proyectos.size();

    ag_proyectos(proyectos, cant_proyectos, presupuesto);

    return 0;
}
