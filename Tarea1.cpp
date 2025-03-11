#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <termios.h>  // Para capturar entrada sin bloqueo
#include <unistd.h>   // Para usleep()
#include <fcntl.h>    // Para detectar teclas sin bloqueo
#include <cstdlib>    // Para la gestión de memoria real
#include <sys/mman.h> // Para bloquear memoria con mlock()

using namespace std;

size_t memoria_MB = 2000; // 1GB (1024 MB)
size_t memoria_bytes = memoria_MB * 1024 * 1024; // Convertimos a bytes
void* memoriaReservada = nullptr; // Puntero para la memoria solicitada

// Método para solicitar RAM real y bloquearla
void SolicitaRam() {
    while (memoriaReservada == nullptr) {
        memoriaReservada = malloc(memoria_bytes); // Solicita memoria
        if (memoriaReservada == nullptr) {
            cerr << "\rError al asignar memoria. Reintentando...\r";
            fflush(stdout);
            usleep(500000);
        } else {
            while (mlock(memoriaReservada, memoria_bytes) != 0) {
                cerr << "\rMemoria solicitada no tuvo éxito     \r";
                fflush(stdout);
                usleep(500000);
            }
            cout << "";
            cout << "\rSe reservaron " << memoria_MB << "MB de RAM en la dirección: " << memoriaReservada << " y se bloqueó en RAM.        \r";
            fflush(stdout);
        }
    }
}

// Método para liberar la RAM real y desbloquearla
void LiberaRam() {
    if (memoriaReservada != nullptr) {
        munlock(memoriaReservada, memoria_bytes);
        free(memoriaReservada);
        memoriaReservada = nullptr;
        cout << "\rSe liberaron " << memoria_MB << "MB de RAM y se desbloqueó.        \r";
        fflush(stdout);
        usleep(1000000); // Pausa para que se pueda leer el mensaje
        cout << "\r                                                           \r";
        fflush(stdout); // Borra la línea
    }
}
// Función para leer un carácter sin necesidad de presionar Enter
char getchLinux() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}//cambio sutil

// Función para verificar si hay una tecla presionada sin bloquear
int kbhitLinux() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

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
        {"Genérico", {
            {1, "Libre de impurezas"}, {2, "Orgánicos"}, {3, "Vencimiento"}, {4, "Desinfección"},
            {5, "Control Fitosanitarias"}, {6, "Control de Plagas"}, {7, "Certificación Ambiental"},
            {8, "Valor nutricional"}, {9, "Empaque"}, {10, "Certificación"}
        }}
    };

    int bucle=0;
    while (true) {
        bucle++;
        printf("\rNúmero de bucles pasados: %d                                                                                                                   ", bucle);
        fflush(stdout);

        bool hayQuantumPendiente = false;
        printf("\r\n");
        fflush(stdout);
        printf("\r\n");
        fflush(stdout);
        for (int i = 0; i < numeroP; i++) {
            if (quantum[i] <= 0) continue;            
            printf("\r\t\t\t\t\t\t\t                                                                                          ");
            fflush(stdout);
            printf("\rProcesando el producto No. %d: %s", i + 1, productos[i].c_str());
            fflush(stdout);
            printf("\r\t\t\t\t\t\t\tQuantum del producto %d: %d", i + 1, quantum[i]);
            fflush(stdout);

            usleep(1000000);

            

            std::string productoActual = productos[i];
            std::map<int, std::string> chequeoActual;

            // Buscar el conjunto de chequeos para el producto actual
            if (chequeos.find(productoActual) != chequeos.end()) {
                chequeoActual = chequeos[productoActual];
            } else {
                chequeoActual = chequeos["Genérico"]; // Usar los chequeos generales si no está en la lista
            }
            char letra;
            do {
                letra = getchLinux();
            } while (letra != 'A' && letra != 'a' && letra != 'F' && letra != 'f');

            if (letra == 'F' || letra == 'f') {
                printf("\nProceso interrumpido cuando se procesaba el producto No. %d\n", i + 1);
                return;
            }
            printf("\r\n");
            fflush(stdout);
            SolicitaRam();
            cout << "\033[A\r]";

            int vueltas = 0;
            while (vueltas <= QuantumSO) {
                if (i + 1 == productoDañado){
                    printf("\nInterrupción aplicada para el producto No. %d...", i + 1);
                    cout << "\n";
                    usleep(1000000);
                    break;
                }
                if (vueltas >= quantum[i] + 1) break;
                printf("\r\t\t\t\t\t\t\t\t\t\t\t                                                             ");
                fflush(stdout);
                printf("\r\t\t\t\t\t\t\t\t\t\t\tChequeo número %d: %s", vueltas, chequeoActual[vueltas].c_str());
                fflush(stdout);

                if (kbhitLinux()) {
                    char tecla = getchLinux();
                    if (tecla == 'C' || tecla == 'c') {
                        printf("\nInterrupción aplicada para el producto No. %d...", i + 1);
                        cout << "\n";
                        usleep(1000000);
                        interrupcionPorUsuario = true;
                        break;
                    } else if (tecla == 'F' || tecla == 'f') {
                        terminarEjecucion = true;
                        break;
                    }
                }
                vueltas++;
                if (vueltas == 11){
                    quantum[i] = 0;
                }
                usleep(500000);
            }
            printf("\r\n");
            fflush(stdout);
            LiberaRam();
            cout << "                                                                                              ";
            cout << "\033[A\r]";
            if (terminarEjecucion) {
                printf("\nProceso interrumpido cuando se procesaba el producto No. %d\n", i + 1);
                return;
            }
            quantum[i] = max(quantum[i] - QuantumSO, 0);
        }

        for (int i = 0; i < numeroP; i++) {
            if (quantum[i] > 0) {
                hayQuantumPendiente = true;
                break;
            }
        }

        if (!hayQuantumPendiente) break;
        
    }    
    printf("\nProceso completado con %s\n", interrupcionPorUsuario ? "interrupciones" : "éxito");
}

int main() {
    string productos[] = {"Atún", "Aceite de soya", "Arroz", "Manteca", "Manteca", "Tomate", "Papas", "Sandias", "Melones", "Papayas"};
    int quantum[] = {5, 12, 4, 8, 2, 6, 1, 7, 9, 10};

    int numeroP, QuantumSO, productoDañado;

    do {
        cout << "Ingresa el quantum del SO entre 5 y 10: ";
        cin >> QuantumSO;
    } while (QuantumSO < 5 || QuantumSO > 10);

    do {
        cout << "Ingresa un número de productos entre 5 y 10: ";
        cin >> numeroP;
    } while (numeroP < 5 || numeroP > 10);

    do{
        cout << "Ingresa el producto dañado (1-" << numeroP << "): ";
        cin >> productoDañado;
    } while (productoDañado > numeroP || productoDañado > 10);
    
    cout << "La cantidad de productos es: " << numeroP << "";
    cout << "\nEl producto dañado es el número: " << productoDañado << "";
    cout << "\n\t\tPresione la tecla A para continuar.  Presione F para terminar el programa.    Presione la tecla C para interrumpir un producto";
    cout << "\n";
    cout << "\n";
    procesarProductos(numeroP, QuantumSO, productoDañado, productos, quantum);
    return 0;
}
