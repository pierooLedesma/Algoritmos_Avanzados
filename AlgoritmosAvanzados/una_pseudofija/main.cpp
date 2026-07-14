#include <algorithm>
#include <iostream>
#include <vector>
#define ITERACIONES 1000
#define ALFA 0.3
#include <iomanip>
using namespace std;

struct Ciudad {
    string ciudad;
    int costo;
};

struct Motorizado {
    int id;
    vector<pair<string, int> > servicio;
};

struct Asignacion {
    string ciudad;
    int motoId;
};

bool comparaCiudad(Ciudad c1, Ciudad c2) {
    return c1.costo > c2.costo;
}

void construirRcl1(vector<Ciudad> poblacion, vector<Ciudad> &rcl1, double limiteInferior) {
    for (int i = 0; i < poblacion.size(); i++) {
        if (poblacion[i].costo >= limiteInferior) {
            rcl1.push_back(poblacion[i]);
        }
    }
}


int CalcularPosicion(Ciudad ciud) {
    string nombre = ciud.ciudad;
    if (nombre == "A") return 0;
    else if (nombre == "B") return 1;
    else if (nombre == "C") return 2;
    else if (nombre == "D") return 3;
    else return -1;
}

void construirRcl2(vector<Motorizado> poblacionServicio, vector<Motorizado> &Rcl2, double limSuper2, int posCiudad) {
    for (int i = 0; i < poblacionServicio.size(); i++) {
        if (poblacionServicio[i].servicio.at(posCiudad).second <= limSuper2) {
            Rcl2.push_back(poblacionServicio[i]);
        }
    }
}

int buscarIndice(vector<Ciudad> ciudades, Ciudad ciud) {
    for (int i = 0; i < ciudades.size(); i++) {
        if (ciudades[i].ciudad == ciud.ciudad)
            return i;
    }
    return -1;
}

void Grasp(vector<Ciudad> ciudades, vector<Motorizado> servicioMotorizado, int k) {
    vector<Asignacion> solucionFinal;
    int costoTotal = INT_MAX;
    srand(time(NULL));
    for (int i = 0; i < ITERACIONES; i++) {
        vector<Asignacion> solucionParcial;
        int costoParcial = 0;
        vector<Ciudad> poblacion = ciudades;
        vector<Motorizado> poblacionServicio = servicioMotorizado;
        while (not poblacion.empty()) {
            int precioMax = k;

            //primerRCL ordenamos primero la ciudad con mayor costo ya en un
            //escenario donde no se podria repetir repartidor ,serian las que
            //primero se deberian poder asignar ya que custan mucho
            sort(poblacion.begin(), poblacion.end(), comparaCiudad);
            int beta1 = poblacion.front().costo;
            int tau1 = poblacion.back().costo;
            double limiteInferior = beta1 - ALFA * (beta1 - tau1);
            vector<Ciudad> Rcl1;
            construirRcl1(poblacion, Rcl1, limiteInferior);
            Ciudad ciudad = Rcl1[rand() % Rcl1.size()];

            /*
             * Para el Segundo Rcl tenemos que para la ciudad elegida tenemos que asignarle el
             * repartidor con menor costo
             */
            int posCiudad = CalcularPosicion(ciudad);
            sort(poblacionServicio.begin(), poblacionServicio.end(), [posCiudad](Motorizado m1, Motorizado m2) {
                return m1.servicio[posCiudad].second < m2.servicio[posCiudad].second;
            });
            int beta2 = poblacionServicio.front().servicio[posCiudad].second;
            int tau2 = poblacionServicio.back().servicio[posCiudad].second;
            double limSuper2 = beta2 + ALFA * (tau2 - beta2);
            vector<Motorizado> Rcl2;
            construirRcl2(poblacionServicio, Rcl2, limSuper2, posCiudad);
            int idempotencia = rand() % Rcl2.size();
            Motorizado motorizado = Rcl2[rand() % Rcl2.size()];
            int costoAEvaluar=ciudad.costo+motorizado.servicio[posCiudad].second;
            if (costoAEvaluar<= costoTotal) {
                Asignacion asi={ciudad.ciudad,motorizado.id};
                solucionParcial.push_back(asi);
                costoParcial+=costoAEvaluar;
            }
            int indiceEliminar = buscarIndice(poblacion, ciudad);
            if (indiceEliminar != -1) poblacion.erase(poblacion.begin() + indiceEliminar);
        }

        if (costoParcial < costoTotal) {
            costoTotal = costoParcial;
            solucionFinal.clear();
            solucionFinal = solucionParcial;
        }
    }
    cout<<"Respuesta"<<left<<endl;
    cout<<setw(15)<<"Ciudad"<<"Motorizado"<<endl;
    sort(solucionFinal.begin(),solucionFinal.end(),[](Asignacion m1, Asignacion m2) {return m1.ciudad<m2.ciudad;});
    for (auto &asi: solucionFinal) {
        cout<<setw(15)<<asi.ciudad<<" "<<asi.motoId<<endl;
    }

}

int main() {
    vector<Ciudad> ciudades = {
        {"A", 16},
        {"B", 15},
        {"C", 12},
        {"D", 18},
    };
    vector<Motorizado> servicioMotorizado = {
        {1, {{"A", 3}, {"B", 4}, {"C", 5}, {"D", 6}}},
        {2, {{"A", 5}, {"B", 2}, {"C", 3}, {"D", 4}}},
        {3, {{"A", 7}, {"B", 5}, {"C", 4}, {"D", 2}}},
        {4, {{"A", 8}, {"B", 7}, {"C", 5}, {"D", 1}}},
        {5, {{"A", 4}, {"B", 3}, {"C", 7}, {"D", 5}}},
    };
    int k = 20;
    Grasp(ciudades, servicioMotorizado, k);
    return 0;
}