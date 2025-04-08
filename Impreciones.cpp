#include <iostream>   // Para cin, cout
#include <cstdlib>    // Para system()
#include <limits>     // Para numeric_limits

using namespace std;

void mostrarMenu() {
    cout << "\n--- MENU DE OPCIONES ---\n";
    cout << "1. Marcas de tiempo\n";
    cout << "2. Duraciones de tiempo\n";
    cout << "3. Hora con microsegundos\n";
    cout << "4. ID de procesos\n";
    cout << "5. Informe de llamadas\n";
    cout << "6. Puntero de instrucción\n";
    cout << "7. Hora por línea\n";
    cout << "8. Llamadas de memoria\n";
    cout << "9. Información de depuración\n";
    cout << "10. Ayuda de strace\n";
    cout << "0. Salir\n";
}

int main() {
    int opcion;

    do {
        mostrarMenu();
        cout << "Seleccione una opción: ";
        cin >> opcion;

        // Limpiar posibles datos restantes en el buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(opcion) {
            case 1:
                system("strace -tt -o tiempo.txt ./Tarea1");
                system("cat tiempo.txt");
                break;
            case 2:
                system("strace -T -o duratiempo.txt ./Tarea1");
                system("cat duratiempo.txt");
                break;
            case 3:
                system("strace -t -o hora.txt ./tarea1");
                system("cat hora.txt");
                break;
            case 4:
                system("strace -f -o Identificador.txt ./tarea1");
                system("cat Identificador.txt");
                break;
            case 5:
                system("strace -c -o Informe.txt ./tarea1");
                system("cat Informe.txt");
                break;
            case 6:
                system("strace -i -o Puntero.txt ./tarea1");
                system("cat Puntero.txt");
                break;
            case 7:
                system("strace -ttt -o Horalinea.txt ./tarea1");
                system("cat Horalinea.txt");
                break;
            case 8:
                system("strace -e trace=memory -o memoria.txt ./tarea1");
                system("cat memoria.txt");
                break;
            case 9:
                system("strace -D -o depura.txt ./tarea1");
                system("cat depura.txt");
                break;
            case 10:
                system("strace --help > ayuda.txt");
                system("cat ayuda.txt");
                break;
            case 0:
                cout << "Finalizando..." << endl;
                break;
            default:
                cout << "Opción inválida\n";
        }

    } while(opcion != 0);

    return 0;
}
