#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Boletos.h"
#include "Eventos.h"

struct cliente clientes[100];
int numClientes = 0;
struct factura facturas[MAX_FACTURAS];
int numFacturas = 0;
struct cliente clientes[MAX_CLIENTES];

extern struct sitio_evento sitiosEventos[MAX_SITIOS_EVENTOS];
extern int numSitiosEventos;
extern struct evento eventos[MAX_EVENTOS];
extern int numEventos;

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
        printf("Cliente: %s\n", facturas[i].cliente->nombre);
        printf("Cedula del cliente: %d\n", facturas[i].cliente->idCliente);
        printf("Subtotal: %.2f\n", facturas[i].subtotal);
        printf("Total: %.2f\n", facturas[i].total);
        printf("=======================================\n");
    }
}

void estadisticas() {
    printf("Estadisticas\n");
    // Aquí va el código para las estadísticas
}

void compraBoleto() {
    char nombreEvento[50];
    struct evento* evento = NULL;
    int sectorIndex, asientoIndex, i;
    //struct factura factura;
    float subtotal, costoServicio, total;
    
    printf("Ingrese el nombre del evento: ");
    scanf("%s", nombreEvento);
    
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
    
    printf("Ingrese el número de sector: ");
    scanf("%d", &sectorIndex);
    sectorIndex--; // Ajustar índice a 0-based
    
    if (sectorIndex < 0 || sectorIndex >= evento->sitioEvento->cantidadSectores) {
        printf("Sector inválido.\n");
        return;
    }
    
    printf("Asientos disponibles para el sector '%s':\n", evento->sitioEvento->sectores[sectorIndex].nombre);
    for (i = 0; i < evento->sitioEvento->sectores[sectorIndex].cantidadAsientos; i++) {
        if (evento->sitioEvento->sectores[sectorIndex].asientos[i].estado == 0) {
            printf("%d ", i + 1);
        }
    }
    
    printf("\nIngrese el número de asiento: ");
    scanf("%d", &asientoIndex);
    asientoIndex--; // Ajustar índice a 0-based
    
    if (asientoIndex < 0 || asientoIndex >= evento->sitioEvento->sectores[sectorIndex].cantidadAsientos || evento->sitioEvento->sectores[sectorIndex].asientos[asientoIndex].estado == 1) {
        printf("Asiento inválido.\n");
        return;
    }
    
    // Registrar venta de boleto
    evento->sitioEvento->sectores[sectorIndex].asientos[asientoIndex].estado = 1;
    subtotal = evento->sitioEvento->sectores[sectorIndex].asientos[0].precio;
    
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char fechaCompra[128];
    strftime(fechaCompra, 128, "%d/%m/%Y %H:%M:%S", tlocal);
    int idCliente = 0;
    char nombreCliente[50];
    char TempNombreCliente[50];
    printf("Ingrese la cédula del comprador: ");
    scanf("%d", &idCliente);
    printf("Ingrese el nombre del comprador: ");
    scanf("%s", TempNombreCliente);
    strcpy(nombreCliente, TempNombreCliente);
    total = subtotal * 1.05;
    
    struct cliente cliente = {
        .idCliente = idCliente,
        .nombre = strdup(nombreCliente)
    };
    
    struct factura factura = {
        .idFactura = numFacturas + 1,
        .evento = *evento,
        .sector = evento->sitioEvento->sectores[sectorIndex].nombre,
        .fechaCompra = strdup(fechaCompra),
        .cliente = &cliente,
        .subtotal = subtotal,
        .total = subtotal * 1.05
    };

    printf("\nFactura:\n");
    printf("ID: %d\n", factura.idFactura);
    printf("Evento: %s\n", factura.evento.nombre);
    printf("Sector: %s\n", factura.sector);
    printf("Fecha de compra: %s\n", factura.fechaCompra);
    printf("Cliente:\n");
    printf("- Cédula: %d\n", factura.cliente->idCliente);
    printf("- Nombre: %s\n", factura.cliente->nombre);
    printf("Subtotal: %.2f\n", factura.subtotal);
    printf("Total: %.2f\n", total);
    facturas[numFacturas] = factura;
    numFacturas++;
}

