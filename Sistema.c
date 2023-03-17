#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Eventos.h"
#include "Boletos.h"
#include "Sistema.h"

int main(void)
{
    printf("Prueba");
    menuPrincipal();

    return 0;
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

void menuOpcionesGenerales() {
    int opcion;
    do {
    printf("\n--- MENU OPCIONES GENERALES ---\n");
    printf("1. Consulta por evento\n");
    printf("2. Compra de boleto\n");
    printf("3. Volver al menu principal\n");

    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            consultaEvento();
            break;
        case 2:
            compraBoleto();
            break;
        case 3:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion invalida. Por favor seleccione una opcion valida.\n");
            break;
    }
} while (opcion != 3);
}

void menuPrincipal() {
    int opcion;
    do {
        printf("\n--Menu General--\n");
        printf("1. Menu Operativo\n");
        printf("2. Menu Opciones Generales\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                // Llamar a la función que implementa el menú operativo
                menuOperativo();
                break;
            case 2:
                // Llamar a la función que implementa el menú de opciones generales
                menuOpcionesGenerales();
                break;
            case 3:
                printf("Hasta luego!\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 3);
}

