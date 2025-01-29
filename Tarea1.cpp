#include <stdio.h>
#include <iostream>
#include <conio.h> // Para detectar la tecla presionada
#include <windows.h> // Para Sleep y detectar clics del mouse
#include <stdbool.h> // Para usar variables booleanas
using namespace std;

int main() {
    int contador = 0;
    int numeroP = 0;
    bool CantidadC = false; 
    bool Malo = false;
    int productoDañado;
    bool terminarEjecucion = false;
    bool interrupcionPorUsuario = false;
    bool mensajeInterrupcion = false; // Variable para controlar la interrupción
    string productos[] = {"Zanahoria", "Brócoli", "Espinaca", "Coliflor", "Pepino", "Pimiento", "Calabacín", "Lechuga", "Apio", "Berenjena"};
    
    // Pide el número de productos
    while (CantidadC == false){
        cout << "Ingresa un numero de productos entre 5 y 10: ";
        cin >> numeroP;
        if (numeroP >= 5 && numeroP <= 10){
            CantidadC = true;
        }
        else{
            CantidadC = false;
        }
    }    
    // Pide el valor de "Producto dañado".      
    while (Malo == false){
        cout << "Ingresa el producto dañado (número de producto entre 1 y " << numeroP << "): ";
        cin >> productoDañado;
        if (productoDañado <= numeroP || productoDañado == 0){
            Malo = true;
        }
        else {
            Malo = false;
        }
    }    
    cout << "\t\t\tLa cantidad de productos es: " << numeroP << endl;
    cout << "\t\t\tEl producto dañado es el número: " << productoDañado << endl;
    cout << "\tPresione la tecla A para continuar" << "\t\tPrecione F para terminar programa." << "\t\tPrecione la tecla C para interrumpir un producto" << endl;
    
    while (contador < numeroP) {
        contador++;
        // Limpia la línea anterior antes de imprimir
        printf("\r\t\t\t\t\t\t\t                     ");
        fflush(stdout);
        // Imprime el mensaje de procesamiento en la misma línea
        printf("\rProcesando el producto No. %d: %s", contador, productos[contador - 1].c_str());
        fflush(stdout);            

        char letra = _getch();
        if (contador == productoDañado || letra == 'p') { 
            // Solo se imprime la interrupción, no el mensaje de "Procesando"
            printf("\r\nInterrupción aplicada, para el producto No. %d...", contador);
            fflush(stdout);
            Sleep(1000);  // Espera 1 segundo para que el usuario lo vea
            printf("\033[F\033[K");  // Borra la línea de interrupción (opcional)
        } 
        else if (letra == 'A' || letra == 'a') {
            int vueltas = 1;
            while (vueltas <= 10) {
                // Limpia la línea anterior antes de imprimir
                printf("\r\t\t\t\t\t\t\t                     ");
                fflush(stdout);

                // Actualiza el número del chequeo
                printf("\r\t\t\t\t\t\t\tChequeo número: %d", vueltas);
                fflush(stdout);
                if (_kbhit()) {
                    char tecla = _getch();
                    if (tecla == 'C' || tecla == 'c') {
                        printf("\nInterrupción aplicada, para el producto No. %d...", contador);
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
            printf("\nProceso completo interrumpido por el sistema cuando se procesaba el producto No. %d\n", contador);
            break; 
        }
        }
        if (!interrupcionPorUsuario && !terminarEjecucion) {
            printf("\033[E\033[K");
            printf("\n\t\t\tFin, proceso completado con éxito\n");
        }
        if (interrupcionPorUsuario) {
            printf("\033[E\033[K");
            printf("\n\t\t\tFin, proceso completado con interrupciones\n");
        }
    return 0;
}
