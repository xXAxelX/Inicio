#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <conio.h> // Para detectar la tecla presionada
#include <windows.h> // Para Sleep y detectar clics del mouse
#include <stdbool.h> // Para usar variables booleanas

using namespace std;

void procesarProductos(int numeroP, int QuantumSO, int productoDañado, string productos[], int quantum[]) {
    bool terminarEjecucion = false;
    bool interrupcionPorUsuario = false;
    std::map<std::string, std::map<int, std::string>> chequeos = {
        {"Atún", {
            {1, "Peso"}, {2, "Etiqueta"}, {3, "Vencimiento"}, {4, "Proteínas"}, {5, "Sodio"},
            {6, "Grasa"}, {7, "Grasa saturada"}, {8, "Valor nutricional"}, {9, "Evaluación física del producto"}, {10, "Certificación vigente"}
        }},
        {"Aceite de soya", {
            {1, "Calidad"}, {2, "Ácido Graso"}, {3, "Vencimiento"}, {4, "Proteínas"}, {5, "Sodio"},
            {6, "Grasa"}, {7, "Grasa saturada"}, {8, "Tipo de semilla tierna"}, {9, "Envase"}, {10, "Certificación vigente"}
        }},
        {"Arroz", {
            {1, "Peso"}, {2, "Etiqueta"}, {3, "Vencimiento"}, {4, "Inocuidad"}, {5, "Limpieza"},
            {6, "Grano 99%"}, {7, "Dietético 95%"}, {8, "Secado"}, {9, "Evaluación física del producto"}, {10, "Certificación vigente"}
        }},
        {"Manteca", {
            {1, "Peso"}, {2, "Alto contenido de grasa láctea"}, {3, "Vencimiento"}, {4, "Contenidos máximos de agua del 16%"},
            {5, "Textura suave"}, {6, "Determinación del % de humedad: Máximo 16%"}, {7, "Grasa 2%"}, {8, "Valor nutricional"},
            {9, "Consistencia"}, {10, "Control de ausencia de antisépticos"}
        }},
        // Para productos genéricos
        {"Genérico", {
            {1, "Libre de impurezas"}, {2, "Orgánicos"}, {3, "Vencimiento"}, {4, "Desinfección"},
            {5, "Control Fitosanitarias"}, {6, "Control de Plagas"}, {7, "Certificación Ambiental"},
            {8, "Valor nutricional"}, {9, "Empaque"}, {10, "Certificación"}
        }}
    };

    while (true) {
        bool hayQuantumPendiente = false;

        for (int i = 0; i < numeroP; i++) {
            if (quantum[i] <= 0) continue;

            printf("\rProcesando el producto No. %d: %s", i + 1, productos[i].c_str());
            fflush(stdout);
            std::string productoActual = productos[i];
            std::map<int, std::string> chequeoActual;

            if (chequeos.find(productoActual) != chequeos.end()) {
                chequeoActual = chequeos[productoActual];
            } else {
                chequeoActual = chequeos["Genérico"];
            }

            printf("\rQuantum del producto %d: %d", i + 1, quantum[i]);
            fflush(stdout);

            char letra = _getch();  
            if (i + 1 == productoDañado || letra == 'p') {
                printf("\r\nInterrupción aplicada al producto No. %d...", i + 1);
                fflush(stdout);
                Sleep(1000);
                printf("\033[F\033[K");
            } else if (letra == 'A' || letra == 'a') {
                int vueltas = 1;
                while (vueltas <= quantum[i] || vueltas <= QuantumSO) {  // Asegurar que no sobrepase quantum
                    printf("\rChequeo número %d: %s", vueltas, chequeoActual[vueltas].c_str());
                    fflush(stdout);

                    if (_kbhit()) {
                        char tecla = _getch();
                        if (tecla == 'C' || tecla == 'c') {
                            printf("\nInterrupción aplicada al producto No. %d...", i + 1);
                            Sleep(1000);
                            printf("\033[F\033[K");
                            interrupcionPorUsuario = true;
                            break;
                        } else if (tecla == 'F' || tecla == 'f') {
                            terminarEjecucion = true;
                            break;
                        }
                    }
                    vueltas++;
                    Sleep(1000);
                }

                // Reducir el quantum solo después de procesar los chequeos
                quantum[i] -= QuantumSO;
                if (quantum[i] < 0) quantum[i] = 0;
            }

            if (terminarEjecucion) {
                printf("\033[E\033[K");
                printf("\nProceso interrumpido en el producto No. %d\n", i + 1);
                return;
            }
        }

        // **Verificar si queda quantum por procesar**
        for (int i = 0; i < numeroP; i++) {
            if (quantum[i] > 0) {
                hayQuantumPendiente = true;
                break;
            }
        }

        if (!hayQuantumPendiente) break;
    }

    if (!interrupcionPorUsuario && !terminarEjecucion) {
        printf("\n\t\t\tFin, proceso completado con éxito\n");
    }
    if (interrupcionPorUsuario) {
        printf("\n\t\t\tFin, proceso completado con interrupciones\n");
    }
}


int main() {
    string productos[] = {"Atún", "Aceite de soya", "Arroz", "Manteca", "Manteca", "Tomate", "Papas", "Sandias", "Melones", "Papayas"};
    int quantum[] = {5, 12, 4, 8, 2, 6, 1, 7, 9, 10};

    int numeroP = 0;
    int QuantumSO = 0;
    bool QSO = false;
    bool CantidadC = false;
    bool Malo = false;
    int productoDañado;

    while (!QSO) {
        cout << "Ingresa el quantum del SO entre 5 y 10: ";
        cin >> QuantumSO;
        if (cin.fail() || QuantumSO < 5 || QuantumSO > 10) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Debes ingresar un número válido entre 5 y 10.\n";
        } else {
            QSO = true;
        }
    }

    while (!CantidadC) {
        cout << "Ingresa un numero de productos entre 5 y 10: ";
        cin >> numeroP;
        if (cin.fail() || numeroP < 5 || numeroP > 10) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Debes ingresar un número válido entre 5 y 10.\n";
        } else {
            CantidadC = true;
        }
    }

    while (Malo == false){
        cout << "Ingresa el producto dañado (número de producto entre 1 y " << numeroP << "): ";
        cin >> productoDañado;
        if (cin.fail()) {
            cin.clear();  // Limpia el estado de error de cin
            cin.ignore(10000, '\n');  // Descarta la entrada incorrecta
            cout << "Error: No puedes ingresar letras, solo numeros.\n";
            continue;
        }
        if (productoDañado <= numeroP || productoDañado == 0){
            Malo = true;
        }
        else {
            Malo = false;
        }
    }

    cout << "\t\t\tLa cantidad de productos es: " << numeroP << endl;
    cout << "\t\t\tEl producto dañado es el número: " << productoDañado << endl;
    cout << "\tPresione la tecla A para continuar" 
         << "\t\tPresione F para terminar el programa." 
         << "\t\tPresione la tecla C para interrumpir un producto" << endl;

    procesarProductos(numeroP, QuantumSO, productoDañado, productos, quantum); 
    
    return 0;
}
