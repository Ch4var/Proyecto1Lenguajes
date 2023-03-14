CREATE TABLE SITIOS_EVENTO (
    id INTEGER PRIMARY KEY,
    nombre VARCHAR(50),
    ubicacion VARCHAR(150),
    sitioweb VARCHAR(150),
    cantidad_sectores INTEGER
);

CREATE TABLE SECTORES (
    sitio_id INTEGER,
    nombre VARCHAR(50) PRIMARY KEY,
    cantidad_asientos INTEGER,
    inicial VARCHAR(1),
    FOREIGN KEY (sitio_id) REFERENCES SITIOS_EVENTO(id)
);

CREATE TABLE ASIENTOS (
    nombreSector VARCHAR(50),
    nombre VARCHAR(4),
    estado INTEGER,
    precio NUMERIC(10, 2),
    FOREIGN KEY (nombreSector) REFERENCES SECTORES(nombre)
);

CREATE TABLE EVENTOS (
    nombre VARCHAR(50) PRIMARY KEY,
    productora VARCHAR(50),
    fecha DATE,
    sitio_evento_id INTEGER,
    FOREIGN KEY (sitio_evento_id) REFERENCES SITIOS_EVENTO(id)
);

CREATE TABLE CLIENTES (
    idCliente INTEGER PRIMARY KEY,
    nombre VARCHAR(60)
);

CREATE TABLE FACTURAS (
    idFactura INTEGER PRIMARY KEY,
    nombreEvento VARCHAR(50),
    sector VARCHAR(50),
    fechaCompra DATE,
    idCliente INTEGER,
    subtotal NUMERIC(10, 2),
    total NUMERIC(10, 2),
    FOREIGN KEY (sector) REFERENCES SECTORES(nombre),
    FOREIGN KEY (idCliente) REFERENCES CLIENTES(idCliente)
);
