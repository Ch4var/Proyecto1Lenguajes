#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SITIOS_EVENTOS 100
#define MAX_EVENTOS 100
#define MAX_FACTURAS 100

struct asiento {
    char* nombre;
    int estado;
    float precio;
};

struct sector {
    char* nombre;
    int cantidadAsientos;
    char inicial;
    struct asiento* asientos;
};

struct sitio_evento {
    char* nombre;
    char* ubicacion;
    char* sitioWeb;
    int cantidadSectores;
    struct sector* sectores;
};

struct evento {
    char* nombre;
    char* productora;
    char* fecha;
    struct sitio_evento* sitioEvento;
};

struct cliente {
    int idCliente;
    char* nombre;
    char* apellido;
    char* telefono;
    char* email;
};

struct factura {
    int idFactura;
    struct evento evento;
    char* sector;
    char* fechaCompra;
    struct cliente* cliente;
    float subtotal;
};

struct sitio_evento sitiosEventos[MAX_SITIOS_EVENTOS]; // Arreglo global de sitios de eventos
int numSitiosEventos = 0; // Contador de sitios de eventos
struct evento eventos[MAX_EVENTOS];
int numEventos = 0;
struct cliente clientes[100];
int numClientes = 0;
struct factura facturas[MAX_FACTURAS];
int numFacturas = 0;

void leerSitiosEventos() {
    FILE* archivo = fopen("sitios.txt", "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }

    char linea[100];
    while (fgets(linea, 100, archivo) != NULL) {
        // Eliminar el salto de línea al final de la línea
        linea[strcspn(linea, "\r\n")] = 0;

        char* nombre = strtok(linea, ",");
        char* ubicacion = strtok(NULL, ",");
        char* sitioWeb = strtok(NULL, ",");

        // Verificar si el sitio ya está en el arreglo
        int encontrado = 0;
        for (int i = 0; i < numSitiosEventos; i++) {
            if (strcmp(sitiosEventos[i].nombre, nombre) == 0) {
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            // Agregar el sitio al arreglo
            struct sitio_evento sitio = {
                .nombre = strdup(nombre),
                .ubicacion = strdup(ubicacion),
                .sitioWeb = strdup(sitioWeb),
                .cantidadSectores = 0,
                .sectores = NULL
            };

            sitiosEventos[numSitiosEventos++] = sitio;
        }
    }

    fclose(archivo);
}

void mostrarSitiosEventos() {
    printf("Lista de sitios de eventos:\n");
    for (int i = 0; i < numSitiosEventos; i++) {
        printf("Sitio de evento %d:\nNombre: %s\nUbicacion: %s\nSitio Web: %s\n\n", i+1, sitiosEventos[i].nombre, sitiosEventos[i].ubicacion, sitiosEventos[i].sitioWeb);
    }
}

void gestionSitiosEventos() {
    printf("Gestion de sitios de eventos\n\n");
    leerSitiosEventos();
    mostrarSitiosEventos();
}


void mostrarEspaciosSitiosEventos() {
    printf("Espacios en sitios de eventos:\n");
    for (int i = 0; i < numSitiosEventos; i++) {
        printf("%d. %s - %s\n", i+1, sitiosEventos[i].nombre, sitiosEventos[i].ubicacion);

        if (sitiosEventos[i].cantidadSectores == 0) {
            printf("   Aun no se han definido sectores.\n");
        } else {
            for (int j = 0; j < sitiosEventos[i].cantidadSectores; j++) {
                printf("   %s: %d espacios, precio: %.2f\n", sitiosEventos[i].sectores[j].nombre, sitiosEventos[i].sectores[j].cantidadAsientos, sitiosEventos[i].sectores[j].asientos[0].precio);

                if (sitiosEventos[i].sectores[j].cantidadAsientos > 0) {
                    printf("      Asientos: ");
                    for (int k = 0; k < sitiosEventos[i].sectores[j].cantidadAsientos; k++) {
                        printf("%c%d ", sitiosEventos[i].sectores[j].inicial, k+1);
                    }
                    printf("\n");
                } else {
                    printf("      No hay asientos definidos.\n");
                }
            }
        }
    }
}

void gestionEspaciosSitiosEventos(){
    printf("Gestion Espacios Sitios de Eventos");
}

void gestionEventos(){
    printf("Gestion Eventos");
}

void estadoEventos(){
    printf("Estado Evento\n")
}


void listaFacturas() {
    printf("Lista de facturas\n");
    // Aquí va el código para la lista de facturas
}

void estadisticas() {
    printf("Estadisticas\n");
    // Aquí va el código para las estadísticas
}

void menuOperativo() {
    int opcion;

    do {
        printf("\n--MENU OPERATIVO--\n");
        printf("1. Gestion de sitios de eventos\n");
        printf("2. Gestion de espacios de sitios de eventos\n");
        printf("3. Gestion de eventos\n");
        printf("4. Estado de eventos\n");
        printf("5. Lista de facturas\n");
        printf("6. Estadisticas\n");
        printf("7. Volver\n");
        printf("Elija una opcion: ");

        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                gestionSitiosEventos();
                break;
            case 2:
                gestionEspaciosSitiosEventos();
                break;
            case 3:
                gestionEventos();
                break;
            case 4:
                estadoEventos();
                break;
            case 5:
                listaFacturas();
                break;
            case 6:
                estadisticas();
                break;
            case 7:
                printf("Volviendo al menu principal...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, elija una opcion valida.\n");
                break;
        }
    } while (opcion != 7);
}

int main() {
    menuOperativo();
    return 0;
}