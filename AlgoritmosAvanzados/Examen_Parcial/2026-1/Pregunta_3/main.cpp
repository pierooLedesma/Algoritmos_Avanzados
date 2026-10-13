#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
using namespace std;

struct Paciente {
    int indice;
    int tiempo;
};

struct Asignacion {
    int tiempo_inicial;
    int tiempo_final;
    int maquina;
    int cargaTrabajoActual;
};


// CRITERIO VORAZ: ordenar por tiempo de procesamiento descendente.
void ordenamiento_previo(vector<vector<Paciente>> & maquinas) {
    // Para cada máquina
    for (int maquina = 0; maquina < maquinas.size(); maquina++) {

        // Ordenar descendentemente los pacientes por el tiempo de procesamiento.
        for (int p1 = 0; p1 < maquinas[maquina].size() - 1; p1++) {
            for (int p2 = p1 + 1; p2 < maquinas[maquina].size(); p2++) {
                if (maquinas[maquina][p1].tiempo < maquinas[maquina][p2].tiempo) {
                    struct Paciente una_persona = maquinas[maquina][p1];
                    maquinas[maquina][p1] = maquinas[maquina][p2];
                    maquinas[maquina][p2] = una_persona;
                }
            }
        }
    }
}


void completar_asignacion(struct Asignacion & una_asignacion, int tiempoInicio,
                          int tiempoFinal, int ind_maquina) {
    una_asignacion.tiempo_inicial = tiempoInicio;
    una_asignacion.tiempo_final = tiempoFinal;
    una_asignacion.maquina = ind_maquina;
    una_asignacion.cargaTrabajoActual = tiempoFinal;
}



bool asignacion_iterativa(vector<vector<Paciente>> & maquinas,
                          vector<vector<Asignacion>> & asignaciones, int & makespan) {
    int cant_maquinas = maquinas.size();
    int cant_pacientes = maquinas[0].size();

    // Cuando estará libre cada máquina.
    vector<int> cargaTrabajo(cant_maquinas, 0);

    // Cuando estará libre cada paciente.
    vector<int> disponiblePaciente(cant_pacientes, 0);

    // Controlar los exámenes ya realizados.
    // realizado[m][p]: el paciente 'p' ya hizo su examen en máquina 'm'.
    vector<vector<bool>> realizado(cant_maquinas, vector<bool>(cant_pacientes, false));

    int tiempoActual = 0;
    int totalAsignaciones = 0;
    int totalExamenes = cant_maquinas * cant_pacientes;

    while (totalAsignaciones < totalExamenes) {

        // Revisar todas las máquinas.
        for (int maquina = 0; maquina < cant_maquinas; maquina++) {

            // Si está ocupada, entonces no puede atender.
            if (cargaTrabajo[maquina] > tiempoActual) continue;

            // Recorrer su lista previamente ordenada.
            for (int posicion = 0; posicion < cant_pacientes; posicion++) {

                int paciente = maquinas[maquina][posicion].indice;
                int duracion = maquinas[maquina][posicion].tiempo;

                // Si ya hizo este examen, descartarlo.
                if (realizado[maquina][paciente - 1]) continue;

                // Si está en otra máquina, descartarlo.
                if (disponiblePaciente[paciente - 1] > tiempoActual) continue;

                int tiempoFinal = tiempoActual + duracion;

                // Registrar el examen.
                Asignacion una_asignacion;
                completar_asignacion(una_asignacion, tiempoActual, tiempoFinal,maquina);
                asignaciones[paciente - 1].push_back(una_asignacion);

                // Actualizar los estados.
                realizado[maquina][paciente - 1] = true;
                disponiblePaciente[paciente - 1] = tiempoFinal;
                cargaTrabajo[maquina] = tiempoFinal;

                totalAsignaciones++;

                break; // La máquina ya tiene un paciente.
            }
        }

        if (totalAsignaciones == totalExamenes) break;

        // Buscar el siguiente instante en el que
        // termina un examen en alguna máquina.
        int siguienteTiempo = INT_MAX;

        for (int maquina = 0; maquina < cant_maquinas; maquina++) {

            if (cargaTrabajo[maquina] > tiempoActual and
                cargaTrabajo[maquina] < siguienteTiempo) {

                siguienteTiempo = cargaTrabajo[maquina];
            }
        }

        if (siguienteTiempo == INT_MAX) return false;

        tiempoActual = siguienteTiempo;
    }

    makespan = *max_element(cargaTrabajo.begin(), cargaTrabajo.end());

    return true;
}



// Cota inferior: al menos el máximo entre la carga de cada máquina
// y el tiempo total de exámenes de cada paciente.
int calcular_cota_inferior(vector<vector<Paciente>> & maquinas) {
    int cant_pacientes = maquinas[0].size();
    vector<int> tiempoPaciente(cant_pacientes, 0);
    int cota = 0;

    for (int maquina = 0; maquina < maquinas.size(); maquina++) {
        int sumaMaquina = 0;
        for (int j = 0; j < maquinas[maquina].size(); j++) {
            Paciente persona = maquinas[maquina][j];
            sumaMaquina += persona.tiempo;
            tiempoPaciente[persona.indice - 1] += persona.tiempo;
        }
        cota = max(cota, sumaMaquina);
    }
    for (int paciente = 0; paciente < cant_pacientes; paciente++) {
        cota = max(cota, tiempoPaciente[paciente]);
    }
    return cota;
}


void mostrar_pacientes_por_maquinas(vector<vector<Paciente>> & maquinas) {
    for (int maquina = 0; maquina < maquinas.size(); maquina++) {
        cout << " - ";
        if (maquina == 0) cout << " Rayos X";
        else if (maquina == 1) cout << " Sangre";
        else cout << " Ecografia";
        cout << " (M" << maquina + 1 << "): ";
        for (int persona = 0; persona < maquinas[maquina].size(); persona++) {
            cout << "P" << maquinas[maquina][persona].indice;
            cout << "(" << maquinas[maquina][persona].tiempo << ")";
            cout << (persona + 1 >= maquinas[maquina].size() ? "\n" : " > ");
        }
    }
}



// Las asignaciones ya están en orden cronológico, porque se guardaron
// según el tiempo de la simulación.
void mostrar_resumen_por_paciente(vector<vector<Asignacion>> & asignaciones) {
    cout << left << setw(12) << "Paciente";
    cout << setw(50) << "Orden de visita" << "Termina en" << endl;
    cout << string(73, '-') << endl;

    for (int paciente = 1; paciente < asignaciones.size(); paciente++) {
        cout << left << setw(12) << ("P" + to_string(paciente));
        int tiempoFinalPaciente = 0;
        for (int j = 0; j < asignaciones[paciente].size(); j++) {
            Asignacion a = asignaciones[paciente][j];
            cout << "M" << a.maquina + 1;
            cout << "(" << a.tiempo_inicial << "-" << a.tiempo_final << ")";
            if (j + 1 < asignaciones[paciente].size()) cout << " -> ";
            tiempoFinalPaciente = max(tiempoFinalPaciente, a.tiempo_final);
        }
        cout << "   t=" << tiempoFinalPaciente << endl;
    }
}



void mostrar_solucion(vector<vector<Paciente>> & maquinas,
                      vector<vector<Asignacion>> & asignaciones, int makespan) {
    cout << "Listas de pacientes ordenadas por maquina:" << endl;
    mostrar_pacientes_por_maquinas(maquinas);
    cout << endl << "Resumen por paciente:" << endl;
    mostrar_resumen_por_paciente(asignaciones);

    int cota = calcular_cota_inferior(maquinas);
    cout << endl << "Cota inferior teorica: " << cota << " minutos" << endl;
    cout << "Tiempo de procesamiento final acumulado o makespan: ";
    cout << makespan << " minutos (";
    cout << fixed << setprecision(2) << (double)makespan / 60.0 << " horas)" << endl;
}



void hospital_voraz(vector<vector<Paciente>> & maquinas) {
    int makespan = 0;
    int cant_pacientes = maquinas[0].size();
    vector<vector<Asignacion>> asignaciones(cant_pacientes);

    ordenamiento_previo(maquinas);
    if (asignacion_iterativa(maquinas, asignaciones, makespan)) {
        mostrar_solucion(maquinas, asignaciones, makespan);
    } else {
        cout << "No se pudo completar la asignacion" << endl;
    }
}


int main() {

    // Las filas son las máquinas [Rayos X (M1); Análisis de Sangre (M2); Ecografía (M3)]
    // y las columnas son los pacientes (conformados por "struct Paciente" que tiene su índice y tiempo).
    vector<vector<Paciente>> maquinas = {
        {{1, 10}, {2,8}, {3,15}, {4,10}, {5,8}, {6,15}, {7,10}, {8,8}, {9,15}, {10,10}},
        {{1,12}, {2,8}, {3,5},  {4,12}, {5,5}, {6,8},  {7,5},  {8,12}, {9,8}, {10,5}},
        {{1,15}, {2,7}, {3,12}, {4,7}, {5,15}, {6,12}, {7,15}, {8,7}, {9,12}, {10,7}},
    };

    hospital_voraz(maquinas);
    return 0;
}