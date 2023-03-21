#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libpq-fe.h>
#include "Boletos.h"
#include "Eventos.h"

int numClientes = 0;
struct factura facturas[MAX_FACTURAS];
int numFacturas = 0;
struct cliente clientes[MAX_CLIENTES];

extern struct sitio_evento sitiosEventos[MAX_SITIOS_EVENTOS];
extern int numSitiosEventos;
extern struct evento eventos[MAX_EVENTOS];
extern int numEventos;

/*
Funcion cuyo objetivo es mostrar el top 3 mes-anio con mas eventos
Esto lo hace contando los eventos de cada mes-anio y ordenandolos y mostrando los primeros 3
*/
void estadisticasA() {
    int i, j, k;
    int numEventosMesAnio[MAX_EVENTOS] = {0}; // Array para almacenar el número de eventos por mes-año
    char* mesAnio[MAX_EVENTOS]; // Array para almacenar los mes-anio de los eventos
    int numMesAnio = 0; // Contador para el número de mes-año distintos

    // Recorrido por los eventos
    for (i = 0; i < numEventos; i++) {
        // Obtener el mes-anio del evento
        char* fecha = eventos[i].fecha;
        char* mes = strtok(fecha, "-");
        char* anio = strtok(NULL, "-");
        char* mesAnioStr = (char*) malloc(sizeof(char) * 8);
        sprintf(mesAnioStr, "%s-%s", anio, mes);

        // Buscar si ya se ha contabilizado un evento en ese mes-año
        int encontrado = 0;
        for (j = 0; j < numMesAnio; j++) {
            if (strcmp(mesAnioStr, mesAnio[j]) == 0) {
                numEventosMesAnio[j]++;
                encontrado = 1;
                break;
            }
        }

        // Si no se ha contabilizado ningún evento en ese mes-año, se añade al array
        if (!encontrado) {
            mesAnio[numMesAnio] = mesAnioStr;
            numEventosMesAnio[numMesAnio]++;
            numMesAnio++;
        }
    }

    // Ordenar los mes-anio por número de eventos (orden descendente)
    for (i = 0; i < numMesAnio - 1; i++) {
        for (j = i + 1; j < numMesAnio; j++) {
            if (numEventosMesAnio[i] < numEventosMesAnio[j]) {
                // Intercambiar los valores de numEventosMesAnio
                int temp = numEventosMesAnio[i];
                numEventosMesAnio[i] = numEventosMesAnio[j];
                numEventosMesAnio[j] = temp;

                // Intercambiar los valores de mesAnio
                char* tempStr = mesAnio[i];
                mesAnio[i] = mesAnio[j];
                mesAnio[j] = tempStr;
            }
        }
    }

    // Mostrar el top 3 de mes-anio con mas eventos
    printf("Top 3 de mes-anio con mas eventos:\n");
    for (k = 0; k < 3 && k < numMesAnio; k++) {
        printf("%d eventos en %s\n", numEventosMesAnio[k], mesAnio[k]);
    }
}

/*
Funcion cuyo objetivo es mostrar los top 3 eventos con mas recaudacion
Esto lo logra almacenando los ingresos de cada evento y luego ordenandonos y listandolos
*/
void estadisticasB() {
    int i, j, k;
    struct eventoRecaudacion {
        char* nombre;
        float recaudacion;
    };
    struct eventoRecaudacion eventosRecaudacion[MAX_EVENTOS] = {0}; // Array para almacenar el nombre de los eventos y su recaudación
    int numEventosRecaudacion = 0; // Contador para el número de eventos distintos

    // Recorrido por las facturas
    for (i = 0; i < numFacturas; i++) {
        // Obtener el nombre del evento y la recaudación
        char* nombreEvento = facturas[i].evento.nombre;
        float recaudacion = facturas[i].subtotal;

        // Buscar si ya se ha contabilizado una factura para ese evento
        int encontrado = 0;
        for (j = 0; j < numEventosRecaudacion; j++) {
            if (strcmp(nombreEvento, eventosRecaudacion[j].nombre) == 0) {
                eventosRecaudacion[j].recaudacion += recaudacion;
                encontrado = 1;
                break;
            }
        }

        // Si no se ha contabilizado ninguna factura para ese evento, se añade al array
        if (!encontrado) {
            struct eventoRecaudacion eventoRecaudacion = {nombreEvento, recaudacion};
            eventosRecaudacion[numEventosRecaudacion] = eventoRecaudacion;
            numEventosRecaudacion++;
        }
    }

    // Ordenar los eventos por recaudación (orden descendente)
    for (i = 0; i < numEventosRecaudacion - 1; i++) {
        for (j = i + 1; j < numEventosRecaudacion; j++) {
            if (eventosRecaudacion[i].recaudacion < eventosRecaudacion[j].recaudacion) {
                // Intercambiar los valores de eventosRecaudacion
                struct eventoRecaudacion temp = eventosRecaudacion[i];
                eventosRecaudacion[i] = eventosRecaudacion[j];
                eventosRecaudacion[j] = temp;
            }
        }
    }

    // Mostrar el top 3 de eventos con más recaudación por venta de boletos
    printf("Top 3 de eventos con mas recaudacion por venta de boletos:\n");
    for (k = 0; k < 3 && k < numEventosRecaudacion; k++) {
        printf("%.2f de recaudación para %s\n", eventosRecaudacion[k].recaudacion, eventosRecaudacion[k].nombre);
    }

    // Liberar la memoria utilizada por los strings de nombre de evento
    for (i = 0; i < numEventosRecaudacion; i++) {
        free(eventosRecaudacion[i].nombre);
    }
}

/*
Funcion cuyo objetivo es mostrar todas las facturas almacenadas.
Esto lo logra recorriendo el arreglo global de facturas y mostrando cada uno de sus atributos
*/
void listaFacturas() {
    int i, j;

    if (numFacturas == 0) {
        printf("No hay facturas registradas.\n");
        return;
    }

    printf("Facturas:\n");

    for (i = 0; i < numFacturas; i++) {
        printf("ID: %d\n", facturas[i].idFactura);
        printf("Evento: %s\n", facturas[i].evento.nombre);
        printf("Sector: %s\n", facturas[i].sector);
        printf("Fecha de compra: %s\n", facturas[i].fechaCompra);
        printf("Cedula del cliente: %d\n", facturas[i].idCliente);
        printf("Subtotal: %.2f\n", facturas[i].subtotal);
        printf("Total: %.2f\n", facturas[i].total);
        printf("=======================================\n");
    }
}

/*
Funcion cuyo objetivo es lograr que el usuario pueda comprar un boleto.
Esto lo logra pidiendo al usuario un nombre de evento, revisa si existe y luego pide el sector del asiento que quiere comprar, luego el numero del asiento
Luego pide la cédula y el nombre del cliente.
Almacena todos estos datos en los arreglos correspondientes.
Luego despliega en pantalla la informacion de la factura
*/
void compraBoleto() {
    char nombreEvento[50];
    struct evento* evento = NULL;
    int sectorIndex, asientoIndex, i;
    //struct factura factura;
    float subtotal, costoServicio, total;

    getchar();
    printf("Ingrese el nombre del evento: ");
    fgets(nombreEvento, sizeof(nombreEvento), stdin);
    nombreEvento[strcspn(nombreEvento, "\n")] = '\0';

    // Buscar evento por nombre
    for (i = 0; i < numEventos; i++) {
        if (strcmp(nombreEvento, eventos[i].nombre) == 0) {
            evento = &eventos[i];
            break;
        }
    }

    if (evento == NULL) {
        printf("El evento no existe.\n");
        return;
    }

    // Mostrar sectores y asientos disponibles
    printf("Sectores disponibles:\n");
    for (i = 0; i < evento->sitioEvento->cantidadSectores; i++) {
        printf("%d. %s - Precio: %.2f\n", i + 1, evento->sitioEvento->sectores[i].nombre, evento->sitioEvento->sectores[i].asientos[0].precio);
    }

    printf("Ingrese el numero de sector: ");
    scanf("%d", &sectorIndex);
    sectorIndex--; // Ajustar índice a 0-based

    if (sectorIndex < 0 || sectorIndex >= evento->sitioEvento->cantidadSectores) {
        printf("Sector invalido.\n");
        return;
    }

    printf("Asientos disponibles para el sector '%s':\n", evento->sitioEvento->sectores[sectorIndex].nombre);
    for (i = 0; i < evento->sitioEvento->sectores[sectorIndex].cantidadAsientos; i++) {
        if (evento->sitioEvento->sectores[sectorIndex].asientos[i].estado == 0) {
            printf("%d ", i + 1);
        }
    }

    printf("\nIngrese el numero de asiento: ");
    scanf("%d", &asientoIndex);
    asientoIndex--; // Ajustar índice a 0-based

    if (asientoIndex < 0 || asientoIndex >= evento->sitioEvento->sectores[sectorIndex].cantidadAsientos || evento->sitioEvento->sectores[sectorIndex].asientos[asientoIndex].estado == 1) {
        printf("Asiento invalido.\n");
        return;
    }

    // Registrar venta de boleto
    evento->sitioEvento->sectores[sectorIndex].asientos[asientoIndex].estado = 1;
    subtotal = evento->sitioEvento->sectores[sectorIndex].asientos[0].precio;

    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char fechaCompra[128];
    strftime(fechaCompra, 128, "%Y-%m-%d", tlocal);
    int idCliente = 0;
    char nombreCliente[50];
    char TempNombreCliente[50];
    printf("Ingrese la cedula del comprador: ");
    scanf("%d", &idCliente);
    getchar();
    printf("Ingrese el nombre del comprador: ");
    fgets(TempNombreCliente, sizeof(TempNombreCliente), stdin);
    TempNombreCliente[strcspn(TempNombreCliente, "\n")] = '\0';
    strcpy(nombreCliente, TempNombreCliente);
    total = subtotal * 1.05;

    struct cliente cliente = {
        .idCliente = idCliente,
        .nombre = strdup(nombreCliente)
    };
    clientes[numClientes] = cliente;
    numClientes++;
    struct factura factura = {
        .idFactura = numFacturas + 1,
        .evento = *evento,
        .sector = evento->sitioEvento->sectores[sectorIndex].nombre,
        .fechaCompra = strdup(fechaCompra),
        .idCliente = idCliente,
        .subtotal = subtotal,
        .total = subtotal * 1.05
    };

    printf("\nFactura:\n");
    printf("ID: %d\n", factura.idFactura);
    printf("Evento: %s\n", factura.evento.nombre);
    printf("Sector: %s\n", factura.sector);
    printf("Fecha de compra: %s\n", factura.fechaCompra);
    printf("Cliente:\n");
    printf("- Cedula: %d\n", factura.idCliente);
    printf("Subtotal: %.2f\n", factura.subtotal);
    printf("Total: %.2f\n", total);
    facturas[numFacturas] = factura;
    numFacturas++;
}

