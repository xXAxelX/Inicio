#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <conio.h> // Para detectar la tecla presionada
#include <windows.h> // Para Sleep y detectar clics del mouse
#include <stdbool.h> // Para usar variables booleanas
using namespace std;

void procesarProductos(int numeroP, int QuantumSO, int productoDañado, string productos[], int quantum[]) {
    int contador = 0;
    int QuantumA = 0;
    bool terminarEjecucion = false;
    bool interrupcionPorUsuario = false;

    while (contador < numeroP) {
        contador++;
        // Limpia la línea anterior antes de imprimir
        printf("\r\t\t\t\t\t\t\t                                                                   ");
        fflush(stdout);
        // Imprime el mensaje de procesamiento en la misma línea
        printf("\rProcesando el producto No. %d: %s", contador, productos[contador - 1].c_str());
        fflush(stdout);

        printf("\r\t\t\t\t\t\t\tQuantum del producto %d: %d", contador, quantum[contador - 1]);
        fflush(stdout);
        QuantumA = quantum[contador - 1];
        

        char letra = _getch();
        if (contador == productoDañado || letra == 'p') { 
            // Solo se imprime la interrupción, no el mensaje de "Procesando"
            printf("\r\nInterrupción aplicada, para el producto No. %d...", contador);
            fflush(stdout);
            Sleep(1000);  // Espera 1 segundo para que el usuario lo vea
            printf("\033[F\033[K");  
        } 
        else if (letra == 'A' || letra == 'a') {
            int vueltas = 1;
            while (vueltas <= QuantumSO) {
                // Limpia la línea anterior antes de imprimir
                printf("\r\t\t\t\t\t\t\t                     ");
                fflush(stdout);
                // Actualiza el número del chequeo
                printf("\r\t\t\t\t\t\t\t\t\t\t\tChequeo número: %d", vueltas);
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
}

int main() {
    std::map<int, std::string> atun = {
        {1, "Peso"},
        {2, "Etiqueta"},
        {3, "Vencimiento"},
        {4, "Proteínas"},
        {5, "Sodio"},
        {6, "Grasa"},
        {7, "Grasa saturada"},
        {8, "Valor nutricional"},
        {9, "Evaluación física del producto"},
        {10, "Certificación vigente"}
    };

    std::map<int, std::string> manteca = {
        {1, "Peso"},
        {2, "Alto contenido de grasa láctea"},
        {3, "Vencimiento"},
        {4, "Contenidos máximos de agua del 16%"},
        {5, "Textura suave"},
        {6, "Determinación del % de humedad: Máximo 16%"},
        {7, "Grasa 2%"},
        {8, "Valor nutricional"},
        {9, "Consistencia"},
        {10, "Control de ausencia de antisépticos"}
    };

    std::map<int, std::string> manteca2 = {
        {1, "Libre de impurezas"},
        {2, "Verificación el contenido de contaminantes"},
        {3, "Vencimiento"},
        {4, "Tipo"},
        {5, "Grano entero"},
        {6, "Grano suelto"},
        {7, "Sin preservativos"},
        {8, "Valor nutricional"},
        {9, "Empaque"},
        {10, "Certificación vigente"}
    };
    int contador = 0;
    int numeroP = 0;
    int QuantumSO = 0;
    bool CantidadC = false;
    bool QSO = false;
    bool Malo = false;
    int productoDañado;
    bool terminarEjecucion = false;
    bool interrupcionPorUsuario = false;
    bool mensajeInterrupcion = false; 
    int QuantumA = 0;
    string productos[] = {"Atún", "Aceite de soya", "Arroz", "Manteca", "Manteca", "Tomate", "Papas", "Sandias", "Melones ", "Papayas"};
    int quantum [] = {5, 12, 4, 8, 2, 6, 1, 7, 9, 10};

    // Pide el número de productos
    while (QSO == false){
        cout << "Ingresa el quantum del SO entre 5 y 10: ";
        cin >> QuantumSO;
        // Validar si la entrada es un número (evitar letras)
        if (cin.fail()) {
            cin.clear();  // Limpia el estado de error de cin
            cin.ignore(10000, '\n');  // Descarta la entrada incorrecta
            cout << "Error: No puedes ingresar letras, solo numeros.\n";
            continue;
        }
        if (QuantumSO >= 5 && QuantumSO <= 10){
            QSO = true;
        }
        else{
            QSO = false;
        }
    }    
    while (CantidadC == false){
        cout << "Ingresa un numero de productos entre 5 y 10: ";
        cin >> numeroP;
        // Validar si la entrada es un número (evitar letras)
        if (cin.fail()) {
            cin.clear();  // Limpia el estado de error de cin
            cin.ignore(10000, '\n');  // Descarta la entrada incorrecta
            cout << "Error: No puedes ingresar letras, solo numeros.\n";
            continue;
        }
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
    cout << "\tPresione la tecla A para continuar" << "\t\tPrecione F para terminar programa." << "\t\tPrecione la tecla C para interrumpir un producto" << endl;
    
    procesarProductos(numeroP, QuantumSO, productoDañado, productos, quantum);
    return 0;
}
