#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libpq-fe.h>
#include "Eventos.h"
#include "Boletos.h"
#include "Sistema.h"

extern struct sitio_evento sitiosEventos[MAX_SITIOS_EVENTOS];
extern int numSitiosEventos;
extern struct evento eventos[MAX_EVENTOS];
extern int numEventos;
extern struct factura facturas[MAX_FACTURAS];
extern int numFacturas;
extern struct cliente clientes[MAX_CLIENTES];
extern int numClientes;

int main(void)
{
    PGconn *conn = PQconnectdb("host=localhost port=5432 dbname=P1Lenguajes user=postgres password=admin");

    if (PQstatus(conn) == CONNECTION_OK) {
        printf("Coneccion exitosa a la base");
    }else{
    printf("Connection to database failed: %s", PQerrorMessage(conn));
    PQfinish(conn);
    exit(1);
    }
    menuPrincipal();
    subirDatos();
    PQfinish(conn);
    return 0;
}

void subirDatos(){
    PGconn *conn = PQconnectdb("host=localhost port=5432 dbname=P1Lenguajes user=postgres password=admin");
    PGresult *res;
    if (PQstatus(conn) == CONNECTION_OK) {
        printf("Coneccion exitosa a la base");
    }else{
        printf("Connection to database failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    for (int i = 0; i < numSitiosEventos; i++) {
        int id = i+1;
        char nombre[50];
        strcpy(nombre,sitiosEventos[i].nombre);
        char ubicacion[150];
        strcpy(ubicacion,sitiosEventos[i].ubicacion);
        char sitioweb[150];
        strcpy(sitioweb,sitiosEventos[i].sitioWeb);
        int cantidad_sectores = sitiosEventos[i].cantidadSectores;
        char insert_query[400];
        snprintf(insert_query, 400, "INSERT INTO SITIOS_EVENTO (id, nombre, ubicacion, sitioweb, cantidad_sectores) VALUES (%d, '%s', '%s', '%s', %d)", id, nombre, ubicacion, sitioweb, cantidad_sectores);

        res = PQexec(conn, insert_query);

    }

    for (int i = 0; i < numSitiosEventos; i++){
        for (int j = 0; j < sitiosEventos[i].cantidadSectores; j++){
            int id = i+1;
            char nombre[50];
            strcpy(nombre,sitiosEventos[i].sectores[j].nombre);
            int cantidad_asientos = sitiosEventos[i].sectores[j].cantidadAsientos;
            char inicial = sitiosEventos[i].sectores[j].inicial;
            char insert_query[400];
            snprintf(insert_query, 400, "INSERT INTO SECTORES (sitio_id, nombre, cantidad_asientos, inicial) VALUES (%d, '%s', %d, '%c')", id, nombre, cantidad_asientos, inicial);
            res = PQexec(conn, insert_query);
        }
    }

    for (int i = 0; i < numSitiosEventos; i++){
        for (int j = 0; j < sitiosEventos[i].cantidadSectores; j++){
            for(int k = 0; k < sitiosEventos[i].sectores[j].cantidadAsientos; k++){
                char nombreSector[50];
                strcpy(nombreSector,sitiosEventos[i].sectores[j].nombre);
                char nombre[5];
                strcpy(nombre,sitiosEventos[i].sectores[j].asientos[k].nombre);
                int estado = sitiosEventos[i].sectores[j].asientos[k].estado;
                float precio = sitiosEventos[i].sectores[j].asientos[k].precio;

                char insert_query[400];
                snprintf(insert_query, 400, "INSERT INTO ASIENTOS (nombreSector, nombre, estado, precio) VALUES ('%s', '%s', %d, %.2f)", nombreSector, nombre, estado, precio);
                res = PQexec(conn, insert_query);
            }
        }
    }

    for (int i = 0; i < numEventos; i++){

        char nombre[50];
        strcpy(nombre, eventos[i].nombre);
        char productora[50];
        strcpy(productora, eventos[i].productora);
        char fecha[50];
        int id;
        strcpy(fecha, eventos[i].fecha);
        for (int j = 0; j < numSitiosEventos; j++){
            if (strcmp(eventos[i].sitioEvento->nombre, sitiosEventos[j].nombre)==0){
                id = j+1;
                break;
            }
        }
        char insert_query[400];
        snprintf(insert_query, 400, "INSERT INTO EVENTOS (nombre, productora, fecha, sitio_evento_id) VALUES ('%s', '%s','%s', %d)", nombre, productora, fecha, id);

        res = PQexec(conn, insert_query);
    }
    for (int i = 0; i < numClientes; i++) {
        char nombre[50];
        strcpy(nombre,clientes[i].nombre);
        int idCliente = clientes[i].idCliente;
        char insert_query[400];
        snprintf(insert_query, 400, "INSERT INTO CLIENTES (idCliente, nombre) VALUES (%d, '%s')", idCliente, nombre);

        res = PQexec(conn, insert_query);

    }

    for(int i = 0; i < numFacturas; i++){
        int idFactura = facturas[i].idFactura;
        char nombreEvento[50];
        strcpy(nombreEvento,facturas[i].evento.nombre);
        char nombreSector[50];
        strcpy(nombreSector, facturas[i].sector);
        char fechaCompra[20];
        strcpy(fechaCompra, facturas[i].fechaCompra);
        int idCliente = facturas[i].idCliente;
        float subtotal = facturas[i].subtotal;
        float total = facturas[i].total;

        char insert_query[400];
        snprintf(insert_query, 400, "INSERT INTO FACTURAS (idFactura, nombreEvento, sector, fechaCompra, idCliente, subtotal, total) VALUES (%d, '%s', '%s', '%s', %d, %.2f, %.2f)", idCliente, nombreEvento, nombreSector, fechaCompra, idCliente, subtotal, total);

        res = PQexec(conn, insert_query);
    }
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

