#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <termios.h>    
#include <unistd.h>     
#include <fcntl.h>      
#include <cstdlib>      
#include <sys/mman.h>  

using namespace std;

static std::mutex console_mutex;

// Parámetros de memoria a reservar
size_t memoria_MB = 2000; 
size_t memoria_bytes = memoria_MB * 1024 * 1024;

// Estructura para representar un producto
struct ProductProcess {
    string nombre;
    int quantum;      
    int index;         
    int checkNumber;   
    void* memoriaReservada = nullptr; 
};

// Función para capturar un carácter sin bloqueo
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
}

// Función para verificar si hay tecla presionada
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
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


// ------------------- Funciones de hilo --------------------

void Reserva_H(ProductProcess &prod) {
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        printf("\r\33[2K\r");
        printf("\rIniciando hilo: Reserva_H");
        fflush(stdout);
        usleep(1000000);
    }
    cout << "\n\n\n";
    while(prod.memoriaReservada == nullptr) {
        prod.memoriaReservada = malloc(memoria_bytes); 
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            if(prod.memoriaReservada == nullptr) {
                printf("\rError al asignar memoria. Reintentando...\r");
                fflush(stdout);
            }
        }
        if(prod.memoriaReservada == nullptr) {
            usleep(500000);
        } else {
            while(mlock(prod.memoriaReservada, memoria_bytes) != 0) {
                {
                    std::lock_guard<std::mutex> lock(console_mutex);
                    printf("\rMemoria solicitada no tuvo éxito     \r");
                    fflush(stdout);
                }
                usleep(500000);
            }
            {
                std::lock_guard<std::mutex> lock(console_mutex);
                printf("\rSe reservaron %zuMB de RAM en la dirección: %p y se bloqueó en RAM.", memoria_MB, prod.memoriaReservada);
                fflush(stdout);
                usleep(1500000);
            }
        }
    }
    std::cout << "\033[A";
    std::cout << "\033[A";
}

void Productos_H(ProductProcess &prod, int QuantumSO, int productoDaniado) {
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "\033[A";
        printf("\r\33[2K\r");
        printf("\rIniciando hilo: Productos_H");
        fflush(stdout);
        usleep(1000000);
        std::cout << "\033[B";
    }
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        printf("\r\33[2K\r");
        printf("Procesando el producto No. %d: %s                 Quantum: %d", 
               prod.index, prod.nombre.c_str(), prod.quantum);
        fflush(stdout);
    }
    usleep(1000000);
    
    if(prod.index == productoDaniado) {
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            printf("\rProducto dañado detectado en Productos_H. Quantum se pone en 0.\n");
            fflush(stdout);
        }
        prod.quantum = 0;
    }
    
    
    if (kbhitLinux()) {
        char tecla = getchLinux();
        if (tecla == 'F' || tecla == 'f') {
            {
                std::lock_guard<std::mutex> lock(console_mutex);
                printf("\r\33[2K\r");
                printf("Proceso interrumpido en Productos_H para el producto No. %d \n", prod.index);
                fflush(stdout);
            }
            exit(0);
        }
    }
}

void Unidad_H(ProductProcess &prod, int QuantumSO, int productoDaniado,
              const map<string, map<int, string>> &chequeos) {
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "\033[A";
        printf("\r\33[2K\r");
        printf("\rIniciando hilo: Unidad_H");
        fflush(stdout);
        usleep(1000000);
        std::cout << "\033[2B";
    }
    
    int chequeosRound = (QuantumSO < prod.quantum) ? QuantumSO : prod.quantum;
    int processed = 0;
    
    for (int i = 0; i < chequeosRound; i++) {
        if (kbhitLinux()) {
            char tecla = getchLinux();
            cout << "\n";
            if (tecla == 'C' || tecla == 'c') {
                int opcion;
                {
                    std::lock_guard<std::mutex> lock(console_mutex);
                    cout << "\nSeleccione una opción de interrupción:\n";
                    cout << "1. Desperfecto de hardware en la máquina de chequeo\n";
                    cout << "2. Por tiempo de procesamiento\n";
                    cout << "3. Por fin de jornada\n";
                    cout << "4. Por cambio de jornada\n";
                    cout << "5. Mantenimiento de la máquina de chequeo\n";
                    cout << "Seleccione: ";
                    fflush(stdout);
                }
                do {
                    cin >> opcion;
                } while (opcion < 1 || opcion > 5);
        
                {
                    std::lock_guard<std::mutex> lock(console_mutex);
                    for (int i = 0; i < 7; i++){
                        printf("\033[1A\033[2K");
                    }
                    cout << "\nInterrupción por usuario en Unidad_H.\n";
                    fflush(stdout);
                    std::cout << "\033[4A";
                }
                usleep(1000000);
                break;  
            } else if (tecla == 'F' || tecla == 'f') {
                {
                    std::lock_guard<std::mutex> lock(console_mutex);
                    printf("\r\33[2K\r");
                    printf("Proceso interrumpido en Unidad_H para el producto No. %d \n", prod.index);
                    fflush(stdout);
                }
                exit(0);
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            printf("\r                                                                       \r");
            const map<int, string>* chequeoProducto;
            auto it = chequeos.find(prod.nombre);
            if(it != chequeos.end()) {
                chequeoProducto = &it->second;
            } else {
                chequeoProducto = &chequeos.at("Genérico");
            }
            printf("\rChequeo número %d: %s", prod.checkNumber, chequeoProducto->at(prod.checkNumber).c_str());
            fflush(stdout);
        }
        usleep(500000);
        prod.checkNumber++;
        processed++;
    }
    prod.quantum -= processed;
    printf("\r                                                                       \r");
    std::cout << "\033[A";
}

void Libera_H(ProductProcess &prod) {
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "\033[A";
        printf("\r\33[2K\r");
        printf("\rIniciando hilo: Libera_H");
        fflush(stdout);
        usleep(1000000);
    }
    if(prod.memoriaReservada != nullptr) {
        munlock(prod.memoriaReservada, memoria_bytes);
        free(prod.memoriaReservada);
        cout << "\n\n\n";
        prod.memoriaReservada = nullptr;
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            printf("\r\33[2K\r");
            printf("\rSe liberaron %zuMB de RAM y se desbloqueó.", memoria_MB);
            fflush(stdout);
            usleep(1500000);
            printf("\r\33[2K\r");
            fflush(stdout);
        }
        std::cout << "\033[A";
        std::cout << "\033[A";
        std::cout << "\033[A";
        usleep(1000000);
    }
}

void procesarProductoRound(ProductProcess &prod, int QuantumSO, int productoDaniado,
                           const map<string, map<int, string>> &chequeos) {
    std::thread tReserva(Reserva_H, std::ref(prod));
    tReserva.join();
    
    std::thread tProductos(Productos_H, std::ref(prod), QuantumSO, productoDaniado);
    tProductos.join();
    
    std::thread tUnidad(Unidad_H, std::ref(prod), QuantumSO, productoDaniado, std::cref(chequeos));
    tUnidad.join();
    
    std::thread tLibera(Libera_H, std::ref(prod));
    tLibera.join();
}

int main() {
    string productos[] = {"Atún", "Aceite de soya", "Arroz", "Manteca", "Tomate"};
    int quantumArr[] = {5, 12, 4, 8, 6};

    int QuantumSO, numeroP, productoDaniado;
    
    while (true) {
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            cout << "Ingresa el quantum del SO entre 5 y 10: ";
            cout.flush();
        }
        cin >> QuantumSO;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            continue; 
        }
        if (QuantumSO >= 5 && QuantumSO <= 10) {
            break;
        }
    }
    
    while (true) {
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            cout << "Ingresa un número de productos entre 5 y 10: ";
            cout.flush();
        }
        cin >> numeroP;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (numeroP >= 5 && numeroP <= 10) {
            break;
        }
    }
    
    while (true) {
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            cout << "Ingresa el producto dañado (1-" << numeroP << "): ";
            cout.flush();
        }
        cin >> productoDaniado;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (productoDaniado < numeroP || productoDaniado < 10) {
            break;
        }
    }
    
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        cout << "\nLa cantidad de productos es: " << numeroP << "\n";
        cout << "El producto dañado es el número: " << productoDaniado << "\n";
        cout << "\tPresione la tecla A para continuar.  Presione F para terminar el programa.  Presione C para interrumpir un producto.\n\n";
        fflush(stdout);
    }
    
    vector<ProductProcess> lista;
    for (int i = 0; i < numeroP; i++) {
        ProductProcess p;
        p.nombre = productos[i];
        p.quantum = quantumArr[i];
        p.index = i + 1;
        p.checkNumber = 1;
        lista.push_back(p);
    }
    
    map<string, map<int, string>> chequeos = {
        {"Atún", {
            {1, "Peso"}, {2, "Etiqueta"}, {3, "Vencimiento"}, {4, "Proteínas"}, {5, "Sodio"},
            {6, "Grasa"}, {7, "Grasa saturada"}, {8, "Valor nutricional"}, {9, "Evaluación física"}, {10, "Certificación"}
        }},
        {"Aceite de soya", {
            {1, "Calidad"}, {2, "Ácido Graso"}, {3, "Vencimiento"}, {4, "Proteínas"}, {5, "Sodio"},
            {6, "Grasa"}, {7, "Grasa saturada"}, {8, "Tipo de semilla"}, {9, "Envase"}, {10, "Certificación"}
        }},
        {"Arroz", {
            {1, "Peso"}, {2, "Etiqueta"}, {3, "Vencimiento"}, {4, "Inocuidad"}, {5, "Limpieza"},
            {6, "Grano 99%"}, {7, "Dietético 95%"}, {8, "Secado"}, {9, "Evaluación física"}, {10, "Certificación"}
        }},
        {"Manteca", {
            {1, "Peso"}, {2, "Alto contenido de grasa"}, {3, "Vencimiento"}, {4, "Máximo 16% agua"},
            {5, "Textura suave"}, {6, "Humedad max 16%"}, {7, "Grasa 2%"}, {8, "Valor nutricional"},
            {9, "Consistencia"}, {10, "Control de antisépticos"}
        }},
        {"Genérico", {
            {1, "Libre de impurezas"}, {2, "Orgánicos"}, {3, "Vencimiento"}, {4, "Desinfección"},
            {5, "Control fitosanitario"}, {6, "Control de plagas"}, {7, "Certificación ambiental"},
            {8, "Valor nutricional"}, {9, "Empaque"}, {10, "Certificación"}
        }}
    };

    int round = 0;
    while (true) {
        bool anyLeft = false;
        for (auto &p : lista) {
            if (p.quantum > 0) { 
                anyLeft = true; 
                break; 
            }
        }
        if (!anyLeft) break;
        
        round++;
        {
            std::lock_guard<std::mutex> lock(console_mutex);
            printf("\r\33[2K\r");
            printf("Secuencia N.%d\n", round);
            fflush(stdout);
        }
        
        for (auto &p : lista) {
            if (p.quantum > 0) {
                while (true) {
                    char c = getchLinux();
                    if (c == 'A' || c == 'a') break;
                    if (c == 'F' || c == 'f') {
                        {
                            std::lock_guard<std::mutex> lock(console_mutex);
                            printf("\r\33[2K\r");
                            printf("Proceso interrumpido antes de iniciar cualquier hilo\n");
                            fflush(stdout);
                        }
                        exit(0);
                    }
                }
                procesarProductoRound(p, QuantumSO, productoDaniado, chequeos);
            }
        }
    }
    
    {
        std::lock_guard<std::mutex> lock(console_mutex);
        cout << "\nProceso completado.\n";
        fflush(stdout);
    }
    return 0;
}