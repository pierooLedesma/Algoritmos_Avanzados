#include <iostream>
#include <vector>
using namespace std;

bool validacion_de_la_orden(int variable_peso, int maximaDiferenciaPeso, vector<int> orden) {
    if (orden.empty())  return true;
    for (auto carne : orden) {
        if (abs(carne - variable_peso) <= maximaDiferenciaPeso) return true;
    }
    return false;
}


bool construir_orden(int indice, int peso_pedido, int maximaDiferenciaPeso, vector<int> pesos_pedidos,
                     vector<int> &orden, vector<int> &indices) {

    // Caso base : Se alcanzó exactamente el peso pedido.
    if (peso_pedido == 0) return true;

    // Caso base: No hay más elementos.
    if (indice >= pesos_pedidos.size()) return false;

    int variable_peso = pesos_pedidos[indice];

    // Si el peso actual es demasiado grande, entonces lo ignoro.
    if (variable_peso > peso_pedido) {
        return construir_orden(indice + 1, peso_pedido, maximaDiferenciaPeso, pesos_pedidos, orden, indices);
    }

    if (validacion_de_la_orden(variable_peso, maximaDiferenciaPeso, orden)) {

        // Se incluye el actual pedazo de carne en la orden.
        orden.push_back(variable_peso);
        indices.push_back(indice);
        if (construir_orden(indice + 1, peso_pedido - variable_peso, maximaDiferenciaPeso, pesos_pedidos, orden, indices)) {
            return true;
        }
        orden.pop_back();
        indices.pop_back();
    }

    // No se incluye el actual pedazo de carne en la orden.
    // Aquí no se necesita agregar este pedazo de carne en la orden ni decrementar
    // el peso pedido en la siguiente llamada recursiva.
    if (construir_orden(indice + 1, peso_pedido, maximaDiferenciaPeso, pesos_pedidos, orden, indices)) {
        return true;
    }

    return false;
}


void solucionar_pedido(int peso_pedido, int maximaDiferenciaPeso, vector<int> pesos_pedidos, vector<vector<int>> &ordenes) {
    while (true) {
        vector<int> orden;
        vector<int> indices;

        if (construir_orden(0, peso_pedido, maximaDiferenciaPeso, pesos_pedidos, orden, indices)) {
            ordenes.push_back(orden);
            for (int ind = indices.size() - 1; ind >= 0; ind--)
                pesos_pedidos.erase(pesos_pedidos.begin() + indices[ind]);
        } else {
            break;
        }
    }
}


void imprimir_solucion(vector<vector<int>> ordenes, int peso_pedido) {
    cout << "Numero de pedidos que se puede atender de " << peso_pedido << " kg: " << ordenes.size() << endl;
    cout << "Peso de los cortes de cada pedido: ";
    if (not ordenes.empty()) {
        int contador_ordenes = ordenes.size();
        for (auto una_orden : ordenes) {
            cout << "{";
            for (int i = 0; i < una_orden.size(); i++) {
                cout << una_orden[i] << (i + 1 != una_orden.size() ? ", " : "}");
            }
            contador_ordenes--;
            cout << (contador_ordenes > 0 ? " " : "\n");
        }
    } else {
        cout << "No se puedo atender ningun pedido" << endl;
    }
}


int main() {
    int peso_pedido = 15;
    int maximaDiferenciaPeso = 4;
    vector<int> pesos_pedidos = {2, 8, 9, 6, 7, 6};
    //vector<int> pesos_pedidos = {12, 3, 7};
    //vector<int> pesos_pedidos = {20, 7, 8};
    vector<vector<int>> ordenes;
    solucionar_pedido(peso_pedido, maximaDiferenciaPeso, pesos_pedidos, ordenes);
    imprimir_solucion(ordenes, peso_pedido);
    return 0;
}
