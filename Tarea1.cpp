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

    // Se ejecutará mientras haya al menos un producto con quantum > 0
    while (true) {
        bool hayQuantumPendiente = false;

        for (int i = 0; i < numeroP; i++) {
            if (quantum[i] <= 0) continue; // Saltar productos ya terminados

            // Limpia la línea anterior antes de imprimir
            printf("\r\t\t\t\t\t\t\t                                                                   ");
            fflush(stdout);
            printf("\rProcesando el producto No. %d: %s", i + 1, productos[i].c_str());
            fflush(stdout);
            printf("\r\t\t\t\t\t\t\tQuantum del producto %d: %d", i + 1, quantum[i]);
            fflush(stdout);

            int nuevoQuantum = quantum[i] - QuantumSO;
            if (nuevoQuantum > 0) {
                quantum[i] = nuevoQuantum;
            }
            else {
                quantum[i] = 0;
            }

            char letra = _getch();
            if (i + 1 == productoDañado || letra == 'p') { 
                printf("\r\nInterrupción aplicada, para el producto No. %d...", i + 1);
                fflush(stdout);
                Sleep(1000);
                printf("\033[F\033[K");  
            } else if (letra == 'A' || letra == 'a') {
                int vueltas = 1;
                while (vueltas <= QuantumSO) {
                    printf("\r\t\t\t\t\t\t\t                     ");
                    fflush(stdout);
                    printf("\r\t\t\t\t\t\t\t\t\t\t\tChequeo número: %d", vueltas);
                    fflush(stdout);

                    if (_kbhit()) {
                        char tecla = _getch();
                        if (tecla == 'C' || tecla == 'c') {
                            printf("\nInterrupción aplicada, para el producto No. %d...", i + 1);
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
                    Sleep(500);
                }
            }
            if (terminarEjecucion) {
                printf("\033[E\033[K");
                printf("\nProceso completo interrumpido por el sistema cuando se procesaba el producto No. %d\n", i + 1);
                return;
            }
        }

        // **Verificar si aún hay valores en quantum mayores a 0**
        for (int i = 0; i < numeroP; i++) {
            if (quantum[i] > 0) {
                hayQuantumPendiente = true;
                break; // Si al menos uno es mayor a 0, no necesitamos seguir revisando
            }
        }

        // Si ya no hay valores mayores a 0, salir del while
        if (!hayQuantumPendiente) break;
    }
    if (!interrupcionPorUsuario && !terminarEjecucion) {
        printf("\033[E\033[K");
        printf("\n\t\t\tFin, proceso completado con éxito\n");
    }
    if (interrupcionPorUsuario) {
        printf("\033[E\033[K");
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
