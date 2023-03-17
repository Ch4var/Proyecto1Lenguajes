#ifndef BOLETOS
#define BOLETOS
#include "Eventos.h"

#define MAX_FACTURAS 100
#define MAX_CLIENTES 100


struct cliente {
    int idCliente;
    char* nombre;
};

struct factura {
    int idFactura;
    struct evento evento;
    char* sector;
    char* fechaCompra;
    struct cliente* cliente;
    float costoServicio;
    float subtotal;
    float total;
};

void listaFacturas();
void estadisticas();
void compraBoleto();

#endif// BOLETOS