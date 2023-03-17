#ifndef EVENTOS
#define EVENTOS
#define MAX_SITIOS_EVENTOS 100
#define MAX_EVENTOS 100

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