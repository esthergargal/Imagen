/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Esther García Gallego
 *
 * Created on 1 de noviembre de 2020, 20:00
 */

#include <cassert>
#include <cstring>
#include <string>
#include <iostream>
#include "imagenES.h"
#include "Imagen.h"
#include <math.h>            // Para el ceil

#define byte unsigned char   //Definimos un tipo de dato llamado byte [0, 255]
#define ERROR_UNKNOWN 1
#define ERROR_OPEN 2
#define ERROR_ARGS 3
#define ERROR_FUNC 4

using namespace std;


//----------------------------------AUXILIARES---------------------------------//

/**
 * @brief Función auxiliar para convertir un vector en una Imagen
 * @param v el vector a convertir
 * @param imagen La imagen resultante
 * @return imagen La imagen resultante
 */
Imagen conversorVectorImagen(byte *v, Imagen &imagen) {
    for (int f = 0; f < imagen.num_cols(); f++)
        for (int c = 0; c < imagen.num_filas(); c++)
            imagen.asigna_pixel(f, c, v[f + c]);

    return imagen;
}

/**
 * @brief Función auxiliar para convertir una Imagen en un vector
 * @param imagen La imagen a convertir
 * @param v El vector resultante
 * @return v El vector resultante
 */
byte conversorImagenVector(const Imagen &imagen, byte *v) {
    for (int f = 0; f < imagen.num_filas(); f++)
        for (int c = 0; c < imagen.num_cols(); c++) {
            v[f + c] = imagen.valor_pixel(f, c);
        }

    return *v;
}

/**
 * @brief Reporta un error importante y termina el programa
 * @param errorcode Código del error detectado correspondiente a ERROR_UNKNOWN y ERROR_OPEN
 * @param errordata Información adicional sobre el error (si no está relacionado con un fichero, se deja un string vacío)
 */
void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch (errorcode) {
        case ERROR_UNKNOWN:
            cerr << "ERROR: Unknown file format." << endl;
            break;
        case ERROR_OPEN:
            cerr << "Error opening file " << errordata << endl;
            break;
        case ERROR_ARGS:
            cerr << "Please supply 2 arguments " << endl;
            break;
        case ERROR_FUNC:
            cerr << "Error in function " << errordata << endl;
            break;
    }
    exit(1);
}

/**
 * @brief Función auxiliar que permite leer un vector según su tipo
 * @param tipo Tipo de imagen a tratar
 * @param fin Nombre del fichero de entrada
 * @param f Filas de la imagen
 * @param c Columnas de la imagen
 * @return img Vector que contiene la imagen
 */
byte *leerVector(TipoImagen tipo, const char *fin, int &f, int &c) {
    byte *img;

    if (tipo == TipoImagen::IMG_PGM) {
        img = LeerImagenPGM(fin, f, c); // Devuelve un vector
    } else if (tipo == TipoImagen::IMG_PPM) {
        img = LeerImagenPPM(fin, f, c); // Devuelve un vector
    } else
        errorBreak(1, "");

    return img;
}

/**
 * @brief Función auxiliar que permite escribir un vector según su tipo
 * @param tipo Tipo de imagen a tratar
 * @param fout Nombre del fichero de salida
 * @param f Filas de la imagen
 * @param c Columnas de la imagen
 * @param img vector que contiene la imagen
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */
bool escribirVector(TipoImagen tipo, const char *fout, const int f, const int c, byte *img) {
    bool okay;

    if (tipo == TipoImagen::IMG_PGM)
        okay = EscribirImagenPGM(fout, img, f, c);
    else
        okay = EscribirImagenPPM(fout, img, f, c);

    return okay;
}

/**
 * @brief Función auxiliar que permite cambiar el contraste
 * @param min Umbral mínimo
 * @param constante Constante de la transformación
 * @param rmin Valor mínimo de los píxeles de la imagen
 * @param nivelOriginal Valor original del pixel
 * @return byteF Valor tras la transformación
 */
byte transformacion(double min, double constante, double rmin, double nivelOriginal) {
    double nivelFinal;
    nivelFinal = min + (constante * (nivelOriginal - rmin));

    byte byteF = round(nivelFinal);

    return byteF;
}


//--------------------------------EJERCICIO 1---------------------------------//

/**
 * @brief Función que permite umbralizar una Imagen
 * @param fin Nombre del fichero de entrada
 * @param fout Nombre del fichero de salida
 * @param T_1 Umbral mínimo
 * @param T_2 Umbral máximo
 * @precond T_1 < T_2
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */

bool umbralizar(const char *fin, const char *fout, int T_1, int T_2) {
    bool okay;
    byte *img;
    int f = 0, c = 0, pixel = 0;

    TipoImagen tipo = LeerTipoImagen(fin);
    img = leerVector(tipo, fin, f, c); // Si el formato es desconocido, se termina la ejecución

    for (int i = 0; i < f * c; i++) { // Umbralizamos según T_1 y T_2
        pixel = img[i];
        if (pixel <= T_1 || pixel >= T_2)
            img[i] = 255; // Si está dentro del umbral no se hace nada
    }

    okay = escribirVector(tipo, fout, f, c, img);

    return okay;
}

//--------------------------------EJERCICIO 3---------------------------------//

/**
 * @brief Función que permite aumentar el tamaño de una porción de la Imagen
 * @param fin Nombre del fichero de entrada
 * @param fout Nombre del fichero de salida
 * @param x1 Coordenada x de la esquina superior izquierda
 * @param y1 Coordenada y de la esquina superior izquierda
 * @param x2 Coordenada x de la esquina inferior derecha
 * @param y2 Coordenada y de la esquina inferior derecha
 * @precond Debe ser un trozo cuadrado
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */
bool zoom(const char *fin, const char *fout, int x1, int y1, int x2, int y2) {

    bool okay;
    byte *vOriginal;
    int f = 0, c = 0, newF = 0, newC = 0;
    double media = 0;

    TipoImagen tipo = LeerTipoImagen(fin);
    vOriginal = leerVector(tipo, fin, f, c);

    Imagen imagenOriginal(f, c);
    conversorVectorImagen(vOriginal, imagenOriginal);

    newF = x2 - x1;
    newC = y2 - y1;
    Imagen imagen(2 * newF - 1, 2 * newC - 1); // Creamos y reservamos memoria para la imagen aumentada

    for (int i = 0; i < newC; i++)
        for (int j = 0; j < newF; j++)
            imagen.asigna_pixel(2 * i, 2 * j, imagenOriginal.valor_pixel(y1 + i, x1 + j));

    for (int i = 0; i < imagen.num_filas(); i += 2)
        for (int j = 1; j < imagen.num_cols(); j += 2) {
            media = (imagen.valor_pixel(i, j - 1) + imagen.valor_pixel(i, j + 1)) / 2;
            imagen.asigna_pixel(i, j, media);
        }

    for (int i = 1; i < imagen.num_filas(); i += 2)
        for (int j = 0; j < imagen.num_cols(); j++) {
            media = (imagen.valor_pixel(i - 1, j) + imagen.valor_pixel(i + 1, j)) / 2;
            imagen.asigna_pixel(i, j, media);
        }

    byte *vFinal = new unsigned char [imagen.num_filas() * imagen.num_cols()];
    conversorImagenVector(imagen, vFinal);
    okay = escribirVector(tipo, fout, newF, newC, vFinal);

    delete [] vFinal;
    return okay;
}

//--------------------------------EJERCICIO 4---------------------------------//

/**
 * @brief Función que permite disminuir el tamaño de una porción de la Imagen
 * @param forig Nombre del fichero de entrada
 * @param frdo Nombre del fichero de salida
 * @param nf Número de filas del icono
 * @param nc Número de columnas del icono
 * @precond Debe ser un trozo cuadrado
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */

bool icono(const char *forig, const char *frdo, int nf, int nc) {
    bool okay;
    byte *vOriginal;
    int f, c, rel;
    double media = 0;

    f = c = rel = 0;

    TipoImagen tipo = LeerTipoImagen(forig);
    vOriginal = leerVector(tipo, forig, f, c); // Guardamos la imagen en un vector

    Imagen imagenOriginal(f, c), icono(nf, nc); // Creamos la imagen original y el icono
    conversorVectorImagen(vOriginal, imagenOriginal); // Convertimos el vector a imagen para trabajar con ella

    if (f / nf < c / nc)
        rel = ceil(f / nf); // Si es par, va a dar exacto
    else
        rel = ceil(c / nc);

    for (int k = 0; k < nf; k++) { // Recorrer el icono
        for (int z = 0; z < nc; z++) {
            for (int i = 0; i < rel; i++) {
                for (int j = 0; j < rel; j++) {
                    if (j + z * rel > imagenOriginal.num_cols() || i + k * rel > imagenOriginal.num_filas())
                        if (j + z * rel > imagenOriginal.num_cols())
                            media += imagenOriginal.valor_pixel(i + k * rel, j + z * rel - 1);
                        else if (i + k * rel > imagenOriginal.num_filas())
                            media += imagenOriginal.valor_pixel(i + k * rel - 1, j + z * rel);
                        else
                            media += imagenOriginal.valor_pixel(i + k * rel - 1, j + z * rel - 1);
                    else
                        media += imagenOriginal.valor_pixel(i + k*rel, j + z * rel);
                }
            }
            icono.asigna_pixel(k, z, ceil(media / (rel * rel)));
            media = 0;
        }
    }

    byte *vFinal = new unsigned char [icono.num_filas() * icono.num_cols()];
    conversorImagenVector(icono, vFinal);
    okay = escribirVector(tipo, frdo, nf, nc, vFinal);

    delete [] vFinal;
    return okay;
}

//--------------------------------EJERCICIO 5---------------------------------//

/**
 * @brief Función que permite aumentar el contraste de la Imagen
 * @param fichE Nombre del fichero de entrada
 * @param fichS Nombre del fichero de salida
 * @param min 
 * @param max 
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */

bool aumentarContraste(const char *fichE, const char *fichS, const int min, const int max) {

    bool okay;
    byte *vOriginal;
    int f, c, pixel;
    double rmax, rmin;

    f = c = 0;

    TipoImagen tipo = LeerTipoImagen(fichE);
    vOriginal = leerVector(tipo, fichE, f, c); // Guardamos la imagen en un vector

    Imagen imagen(f, c);
    conversorVectorImagen(vOriginal, imagen);
    rmax = rmin = imagen.valor_pixel(0, 0); // Inicializamos el valor de los niveles de grises

    for (int i = 0; i < f; i++) // Calcular niveles min y max de gris
        for (int j = 1; j < c; j++) {
            pixel = imagen.valor_pixel(i, j);
            if (pixel > rmax)
                rmax = pixel;
            if (pixel < rmin)
                rmin = pixel;
        }

    const double cte = (max - min) / (rmax - rmin);

    for (int i = 0; i < f; i++)
        for (int j = 0; j < c; j++)
            imagen.asigna_pixel(i, j, transformacion(min, cte, rmin, imagen.valor_pixel(i, j)));

    byte *vFinal = new unsigned char [imagen.num_filas() * imagen.num_cols()];
    conversorImagenVector(imagen, vFinal);
    okay = escribirVector(tipo, fichS, f, c, vFinal);

    delete [] vFinal;
    return okay;
}

/*
 * @brief Main program
 */
int main(int argc, char** argv) {

    string salida, nsalida, nentrada;
    bool correcto;
    int op = 0, f = 0, c = 0, f2 = 0, c2 = 0, i = 0;

    if (argc != 3)
        errorBreak(3, "");
    else {
        nentrada = argv[1];
        salida = argv[2];
    }

    salida.erase(salida.size() - 4, 4); // Le quitamos la extensión

    do {
        nsalida = salida;
        cout << "Elija una opción (-1 para terminar): " << endl;
        cout << "[1] Umbralizar" << endl;
        cout << "[2] Zoom" << endl;
        cout << "[3] Icono" << endl;
        cout << "[4] Aumentar contraste" << endl;
        cin >> op;

        nsalida += to_string(i) + ".pgm";

        switch (op) {

            case 1:
                cout << endl << "Ejercicio 1: UMBRALIZAR" << "\nInserte un umbral mínimo y máximo:" << endl;
                cin >> f >> c;
                correcto = umbralizar(nentrada.c_str(), nsalida.c_str(), f, c);

                if (!correcto)
                    errorBreak(4, "umbralizar");
                break;
            case 2:
                cout << endl << "Ejercicio 2: ZOOM" << "\nInserte las coordenadas iniciales:" << endl;
                cin >> f >> c;
                cout << "\nInserte las coordenadas finales:" << endl;
                cin >> f2 >> c2;
                correcto = zoom(nentrada.c_str(), nsalida.c_str(), f, c, f2, c2);

                if (!correcto)
                    errorBreak(4, "zoom");
                break;
            case 3:
                cout << "\nEjercicio 3: ICONO" << "\nInserte un número de filas y columnas para el icono (debe ser cuadrado):" << endl;
                cin >> f >> c;
                correcto = icono(nentrada.c_str(), nsalida.c_str(), f, c);

                if (!correcto)
                    errorBreak(4, "icono");
                break;
            case 4:
                cout << endl << "Ejercicio 4: AUMENTAR CONTRASTE" << "\nInserte un mínimo y un máximo:" << endl;
                cin >> f >> c;
                correcto = aumentarContraste(nentrada.c_str(), nsalida.c_str(), f, c);

                if (!correcto)
                    errorBreak(4, "aumentar contraste");
                break;
        }

        i++;
    } while (op > 0);

    return 0;
}