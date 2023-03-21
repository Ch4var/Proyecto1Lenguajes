#ifndef BOLETOS
#define BOLETOS
#include "Eventos.h"

#define MAX_FACTURAS 100
#define MAX_CLIENTES 100

/*
Modelo para guardar la informacion de un cliente en el sistema
*/
struct cliente {
    int idCliente;
    char* nombre;
};

/*
Modelo para guardar la informacion de una factura en el sistema
*/
struct factura {
    int idFactura;
    struct evento evento;
    char* sector;
    char* fechaCompra;
    int idCliente;
    float subtotal;
    float total;
};

/*
Prototipos de las funciones de Boletos.c
*/
void listaFacturas();
void compraBoleto();

#endif// BOLETOS
