#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libpq-fe.h>
#include "Boletos.h"
#include "Eventos.h"

struct sitio_evento sitiosEventos[MAX_SITIOS_EVENTOS]; // Arreglo global de sitios de eventos
int numSitiosEventos = 0; // Contador de sitios de eventos
struct evento eventos[MAX_EVENTOS];
int numEventos = 0;

extern struct factura facturas[MAX_FACTURAS];
extern int numFacturas;

/*
Funcion cuyo objetivo es extraer los datos de los sitios de eventos en el archivo sitios.txt
Para esto, usa la funcion strtok para separar la informacion al encontrar una ","
Luego, agrega los datos del sitio al arreglo de eventos
*/
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

/*
Funcion cuyo objetivo es mostrar la lista de sitios de eventos al usuario.
Para esto, lo que hace es recorrer el arreglo global de sitios de eventos y desplegar en pantalla cada uno de sus atributos
*/
void mostrarSitiosEventos() {
    printf("Lista de sitios de eventos:\n");
    for (int i = 0; i < numSitiosEventos; i++) {
        printf("Sitio de evento %d:\nNombre: %s\nUbicacion: %s\nSitio Web: %s\n\n", i+1, sitiosEventos[i].nombre, sitiosEventos[i].ubicacion, sitiosEventos[i].sitioWeb);
    }
}

/*
Funcion cuyo objetivo es mostrar la lista de sitios de eventos al usuario.
Para esto, primero llama la funcion para cargar los datos desde el archivo y luego
lo que hace es llamar a la función que recorre el arreglo global de sitios de eventos y despliega en pantalla cada uno de sus atributos
*/
void gestionSitiosEventos() {
    printf("Gestion de sitios de eventos\n\n");
    leerSitiosEventos();
    mostrarSitiosEventos();
}

/*
Funcion cuyo objetivo es mostrar la de sectores por cada sitio de evento
Para esto, se fija en el arreglo de sectores de cada sitio de evento e imprime sus atributos
*/
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

/*
Funcion cuyo objetivo es agregar un sectore a un sitio de evento.
Para esto, le pide al usuario el sitio de evento en el que quiere agregar un sector,
el nombre del sector, la cantidad de espacios del sector, el precio de los asientos por sector y
la inicial de los asientos del sector
*/
void agregarSectorSitioEvento() {
    mostrarEspaciosSitiosEventos();

    int opcion;
    printf("\nSeleccione el sitio de eventos al que desea agregar un sector: ");
    scanf("%d", &opcion);
    opcion--;

    if (opcion < 0 || opcion >= numSitiosEventos) {
        printf("Opcion invalida.\n");
        return;
    }

    char nombre[100];
    int cantidadEspacios;
    float precio;
    char inicial;

    printf("\nIngrese el nombre del sector: ");
    getchar();
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';
    printf("Ingrese la cantidad de espacios: ");
    scanf("%d", &cantidadEspacios);

    printf("Ingrese el precio de los asientos: ");
    scanf("%f", &precio);
    getchar();
    printf("Ingrese la inicial (caracter alfabetico) de los asientos: ");
    scanf("%c", &inicial);

    struct sector sector = {
        .nombre = strdup(nombre),
        .cantidadAsientos = cantidadEspacios,
        .inicial = inicial,
        .asientos = malloc(cantidadEspacios * sizeof(struct asiento))
    };

    for (int i = 0; i < cantidadEspacios; i++) {
        char nombreAsiento[5];
        sprintf(nombreAsiento, "%c%d", inicial, i+1);

        sector.asientos[i] = (struct asiento) {
            .nombre = strdup(nombreAsiento),
            .estado = 0,
            .precio = precio
        };
    }

    struct sitio_evento* sitio = &sitiosEventos[opcion];

    sitio->sectores = realloc(sitio->sectores, (sitio->cantidadSectores + 1) * sizeof(struct sector));
    sitio->sectores[sitio->cantidadSectores++] = sector;

    printf("Sector %s agregado correctamente.\n", nombre);
}

/*
Funcion cuyo objetivo es resetear los espacios de un sitio de evento.
Para esto, le pide al usuario el sector que quiere resetear y cambia el estado de cada asiento a 0 (desocupado)
*/
void resetEspaciosSitioEvento() {
    mostrarEspaciosSitiosEventos();

    int opcion;
    printf("\nSeleccione el sitio de eventos para el cual desea hacer reset de los espacios: ");
    scanf("%d", &opcion);
    opcion--;

    if (opcion < 0 || opcion >= numSitiosEventos) {
        printf("Opcion invalida.\n");
        return;
    }

    struct sitio_evento* sitio = &sitiosEventos[opcion];

    if (sitio->cantidadSectores == 0) {
        printf("No hay sectores definidos para este sitio de eventos.\n");
        return;
    }

    for (int i = 0; i < sitio->cantidadSectores; i++) {
        struct sector* sector = &sitio->sectores[i];
        for (int j = 0; j < sector->cantidadAsientos; j++) {
            sector->asientos[j].estado = 0;
        }
    }

    printf("Espacios reseteados para el sitio de eventos %s.\n", sitio->nombre);
}

/*
Funcion cuyo objetivo es mostrar las opciones de gestion de los sectores de un sitio de eventos.
Para esto, muestra en pantalla las opciones y segun la entrada del usuario, llama a la funcion correspondiente
*/
void gestionEspaciosSitiosEventos() {
    int opcion;
    do {
    printf("\n\n--- Gestion de espacios en sitios de eventos ---\n");
    printf("1. Mostrar espacios en sitios de eventos.\n");
    printf("2. Agregar sector a sitio de eventos.\n");
    printf("3. Resetear espacios para un sitio de eventos.\n");
    printf("4. Volver al menu principal.\n");

    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            mostrarEspaciosSitiosEventos();
            break;
        case 2:
            agregarSectorSitioEvento();
            break;
        case 3:
            resetEspaciosSitioEvento();
            break;
        case 4:
            break;
        default:
            printf("Opcion invalida.\n");
    }
} while (opcion != 4);
}

/*
Funcion cuyo objetivo es crear un evento en un sitio de evento.
Para esto, le pide al usuario el sitio de evento, el nombre del evento, la productora y la fecha del evento.
Luego agrega estos datos al arreglo de eventos
*/
void gestionEventos() {
    // Verificar si hay sitios de eventos definidos
    if (numSitiosEventos == 0) {
        printf("No hay sitios de eventos definidos en el sistema.\n");
        return;
    }

    // Mostrar lista de sitios de eventos
    printf("Sitios de eventos disponibles:\n");
    for (int i = 0; i < numSitiosEventos; i++) {
        printf("%d. %s - %s\n", i+1, sitiosEventos[i].nombre, sitiosEventos[i].ubicacion);
    }

    // Solicitar seleccion de sitio de eventos
    int opcion;
    printf("\nSeleccione el sitio de eventos para el evento: ");
    scanf("%d", &opcion);
    opcion--;

    if (opcion < 0 || opcion >= numSitiosEventos) {
        printf("Opcion invalida.\n");
        return;
    }

    // Mostrar lista de sectores del sitio de eventos seleccionado
    printf("\nSectores disponibles en el sitio %s:\n", sitiosEventos[opcion].nombre);
    for (int i = 0; i < sitiosEventos[opcion].cantidadSectores; i++) {
        printf("%d. %s - Monto: %.2f\n", i+1, sitiosEventos[opcion].sectores[i].nombre, sitiosEventos[opcion].sectores[i].asientos[0].precio);
    }

    // Crear evento
    char nombre[50], productora[50], fecha[11];
    getchar();
    printf("\nIngrese el nombre del evento: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';
    printf("Ingrese el nombre de la productora: ");
    fgets(productora, sizeof(productora), stdin);
    productora[strcspn(productora, "\n")] = '\0';
    printf("Ingrese la fecha del evento (yyyy-mm-dd): ");
    scanf("%s", fecha);

    struct evento evento = {
        .nombre = strdup(nombre),
        .productora = strdup(productora),
        .fecha = strdup(fecha),
        .sitioEvento = &sitiosEventos[opcion]
    };

    eventos[numEventos++] = evento;

    printf("\nEvento %s creado correctamente.\n", nombre);
}

/*
Funcion cuyo objetivo es mostrar el estado de un evento en particular
Para esto, le pide al usuario el evento y muestra los ingresos y asientos disponibles por sector
*/
void estadoEventos() {
    printf("Eventos disponibles:\n");
    for (int i = 0; i < numEventos; i++) {
        printf("%d. %s\n", i+1, eventos[i].nombre);
    }

    int opcion;
    printf("\nSeleccione el evento para el cual desea ver el estado: ");
    scanf("%d", &opcion);
    opcion--;

    if (opcion < 0 || opcion >= numEventos) {
        printf("Opcion invalida.\n");
        return;
    }

    struct evento evento = eventos[opcion];
    printf("\nNombre: %s\n", evento.nombre);
    printf("Productora: %s\n", evento.productora);
    printf("Sitio de evento: %s\n", evento.sitioEvento->nombre);
    printf("Fecha: %s\n", evento.fecha);

    for (int i = 0; i < evento.sitioEvento->cantidadSectores; i++) {
        struct sector sector = evento.sitioEvento->sectores[i];
        float montoSector = sector.asientos[0].precio * sector.cantidadAsientos;
        float montoRecaudadoSector = 0;
        printf("\nSector: %s\n", sector.nombre);
        printf("Monto por asiento: %.2f\n", sector.asientos[0].precio);
        for (int j = 0; j < numFacturas; j++) {
            if (strcmp(facturas[j].evento.nombre, evento.nombre) == 0 && strcmp(facturas[j].sector, sector.nombre) == 0) {
                montoRecaudadoSector+=facturas[j].total;
            }
        }
        printf("Monto recaudado en este sector: %.2f\n", montoRecaudadoSector);

        for (int j = 0; j < sector.cantidadAsientos; j++) {
            printf("Asiento %s - Estado: %s\n", sector.asientos[j].nombre, sector.asientos[j].estado == 0 ? "disponible" : "vendido");
        }

        for (int j = 0; j < numFacturas; j++) {
            if (strcmp(facturas[j].evento.nombre, evento.nombre) == 0 && strcmp(facturas[j].sector, sector.nombre) == 0) {
                printf("Monto recaudado en factura: %.2f\n", facturas[j].subtotal);
            }
        }
    }
}

/*
Funcion cuyo objetivo es mostrar la informacion de los eventos que tienen una fecha posterior a
la ingresada. Para esto, pide al usuario una fecha y muestra todos los eventos posteriores a esa fecha
*/
void consultaEvento() {
    char fecha[11];
    printf("Ingrese una fecha en formato yyyy-mm-dd: ");
    scanf("%s", fecha);
    printf("\nEventos posteriores a la fecha %s:\n", fecha);
    for (int i = 0; i < numEventos; i++) {
        if (strcmp(eventos[i].fecha, fecha) > 0) {
            printf("%d - %s (%s)\n", i+1, eventos[i].nombre, eventos[i].fecha);
        }
    }
    printf("\n");
    int indexEvento;
    do {
        printf("Seleccione el indice del evento: ");
        scanf("%d", &indexEvento);
        if (indexEvento < 1 || indexEvento > numEventos) {
            printf("Indice de evento no valido, ingrese un valor entre 1 y %d\n", numEventos);
        }
    } while (indexEvento < 1 || indexEvento > numEventos);

    indexEvento--;

    printf("\nNombre del evento: %s\n", eventos[indexEvento].nombre);
    printf("Productora: %s\n", eventos[indexEvento].productora);
    printf("Fecha: %s\n", eventos[indexEvento].fecha);
    printf("Sitio: %s\n", eventos[indexEvento].sitioEvento->nombre);
    printf("\nDetalle de sectores:\n");
    for (int i = 0; i < eventos[indexEvento].sitioEvento->cantidadSectores; i++) {
        printf("- %s: $%.2f\n", eventos[indexEvento].sitioEvento->sectores[i].nombre, eventos[indexEvento].sitioEvento->sectores[i].asientos[0].precio);
        printf("Asientos disponibles: ");
        for (int j = 0; j < eventos[indexEvento].sitioEvento->sectores[i].cantidadAsientos; j++) {
            if (eventos[indexEvento].sitioEvento->sectores[i].asientos[j].estado == 0) {
                printf("%s ", eventos[indexEvento].sitioEvento->sectores[i].asientos[j].nombre);
            }
        }
        printf("\n");
    }
}

