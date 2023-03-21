#ifndef EVENTOS
#define EVENTOS
#define MAX_SITIOS_EVENTOS 100
#define MAX_EVENTOS 100

/*
Modelo para guardar la informacion de un asiento en el sistema
*/
struct asiento {
    char* nombre;
    int estado;
    float precio;
};

/*
Modelo para guardar la informacion de un sector en el sistema
*/
struct sector {
    char* nombre;
    int cantidadAsientos;
    char inicial;
    struct asiento* asientos;
};

/*
Modelo para guardar la informacion de un sitio donde se realizara un evento en el sistema
*/
struct sitio_evento {
    char* nombre;
    char* ubicacion;
    char* sitioWeb;
    int cantidadSectores;
    struct sector* sectores;
};

/*
Modelo para guardar la informacion de un evento en el sistema
*/
struct evento {
    char* nombre;
    char* productora;
    char* fecha;
    struct sitio_evento* sitioEvento;
};

/*
Prototipos de las funciones de Eventos.h
*/
void leerSitiosEventos();
void mostrarSitiosEventos();
void gestionSitiosEventos();
void mostrarEspaciosSitiosEventos();
void agregarSectorSitioEvento();
void resetEspaciosSitioEvento();
void gestionEspaciosSitiosEventos();
void gestionEventos();
void estadoEventos();
void consultaEvento();

#endif// EVENTOS
