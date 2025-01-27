#include <stdio.h>
#include <iostream>
#include <conio.h> // Para detectar la tecla presionada
#include <windows.h> // Para Sleep y detectar clics del mouse
#include <stdbool.h> // Para usar variables booleanas
using namespace std;

int main() {
    int contador = 0;
    int numeroP = 0;
    string productos[] = {"Zanahoria", "Brócoli", "Espinaca", "Coliflor", "Pepino", "Pimiento", "Calabacín", "Lechuga", "Apio", "Berenjena"};
    
    // Pide el número de productos
    cout << "Ingresa el numero de productos: ";
    cin >> numeroP;
    
    // Pide el valor de "Producto dañado".
    int productoDañado;
    cout << "Ingresa el producto dañado (número de producto entre 1 y " << numeroP << "): ";
    cin >> productoDañado;
    
    cout << "La cantidad de productos es: " << numeroP << endl;
    cout << "El producto dañado es el número: " << productoDañado << endl;
    
    bool terminarEjecucion = false;
    bool interrupcionPorUsuario = false;

    if (numeroP < 5 || numeroP > 10) {
        printf("No se puede ingresar menos de 5 o más de 10 productos\n");
    } else {
        while (contador < numeroP) {
            contador++;

            // Imprime el mensaje de procesamiento en la misma línea
            printf("\rProcesando el producto No. %d: %s", contador, productos[contador - 1].c_str());
            fflush(stdout);            

            char letra = _getch();
            if (contador == productoDañado || letra == 'p') { 
                printf("\nInterrupción aplicada, para el producto No. %d...\n", contador);
            } else if (letra == 'A' || letra == 'a') {
                int vueltas = 1;
                while (vueltas <= 10) {
                    // Limpia la línea anterior antes de imprimir
                    printf("\r\t\t\t\t\t\t\t                     "); // Sobrescribe con espacios
                    fflush(stdout);

                    // Actualiza el número del chequeo
                    printf("\r\t\t\t\t\t\t\tChequeo número: %d", vueltas);
                    fflush(stdout);

                    if (_kbhit()) {
                        char tecla = _getch();
                        if (tecla == 'C' || tecla == 'c') {
                            printf("\nInterrupción aplicada, para el producto No. %d\n...", contador);
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
                printf("\nProceso completo interrumpido por el sistema cuando se procesaba el producto No. %d\n", contador);
                break; 
            }
        }
        if (!interrupcionPorUsuario && !terminarEjecucion) {
            printf("\n\t\t\tFin, proceso completado con éxito\n");
        }
        if (interrupcionPorUsuario) {
            printf("\n\t\t\tFin, proceso completado con interrupciones\n");
        }
    }   
    return 0;
}
