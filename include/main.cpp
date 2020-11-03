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

 /**
 * @brief Reporta un error importante y termina el programa
 * @param errorcode Código del error detectado correspondiente a ERROR_UNKNOWN y ERROR_OPEN
 * @param errordata Información adicional sobre el error (si no está relacionado con un fichero, se deja un string vacío)
 */
void errorBreak(int errorcode, const std::string &errordata);

/**
 * @brief Función auxiliar para convertir un vector en una Imagen
 * @param v el vector a convertir
 * @param imagen La imagen resultante
 * @return imagen La imagen resultante
 */
Imagen conversorVectorImagen(byte *v, Imagen &imagen);

/**
 * @brief Función auxiliar para convertir una Imagen en un vector
 * @param imagen La imagen a convertir
 * @param v El vector resultante
 * @return v El vector resultante
 */
byte conversorImagenVector(const Imagen &imagen, byte *v);

byte *leerVector(TipoImagen tipo, const std::string fin, int &f, int &c);

bool escribirVector(TipoImagen tipo, std::string& fout, const int f, const int c, byte *img);

byte transformacion(double min, double constante, double rmin, double nivelOriginal);

/**
 * @brief Función que permite umbralizar una Imagen
 * @param fin Nombre del fichero de entrada
 * @param fout Nombre del fichero de salida
 * @param T_1 Umbral mínimo
 * @param T_2 Umbral máximo
 * @precond T_1 < T_2
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */
bool umbralizar(std::string fin, std::string& fout, int T_1, int T_2);

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
bool zoom(std::string fin, std::string fout, int x1, int y1, int x2, int y2);

/**
 * @brief Función que permite disminuir el tamaño de una porción de la Imagen
 * @param forig Nombre del fichero de entrada
 * @param frdo Nombre del fichero de salida
 * @param nf Número de filas del icono
 * @param nc Número de columnas del icono
 * @precond Debe ser un trozo cuadrado
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */
bool icono(std::string forig, std::string& frdo, int nf, int nc);

/**
 * @brief Función que permite aumentar el contraste de la Imagen
 * @param fichE Nombre del fichero de entrada
 * @param fichS Nombre del fichero de salida
 * @param min 
 * @param max 
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */
bool aumentarContraste(string fichE, string& fichS, const int min, const int max);

/*
 * 
 */
int main(int argc, char** argv) {
    
    string nentrada, nsalida;
    bool correcto;
    int op = 0, f = 0, c = 0, f2 = 0, c2 = 0, i = 1;
    
    if(argc != 2)
        errorBreak(3,"");
    else{
        nentrada = argv[1];
    }
    
    do{
    cout << "Elija una opción (-1 para terminar): " << endl;
    cout << "[1] Umbralizar" << endl;
    cout << "[2] Zoom" << endl;
    cout << "[3] Icono" << endl;
    cout << "[4] Aumentar contraste" << endl;
    cin >> op;
    
    nsalida = nentrada + to_string(i);
    switch (op){
        
        case 1:
            cout << endl << "Ejercicio 1: UMBRALIZAR" << "\nInserte un umbral mínimo y máximo:"<< endl;
            cin >> f >> c;
            correcto = umbralizar(nentrada, nsalida, f, c);
    
            if(!correcto)
                errorBreak(4,"umbralizar");
            break;
        case 2: 
            cout << endl << "Ejercicio 2: ZOOM" << "\nInserte las coordenadas iniciales:" << endl;
            cin >> f >> c;
            cout << "\nInserte las coordenadas finales:" << endl;
            cin >> f2 >> c2;
            correcto = zoom(nentrada, nsalida, f, c, f2, c2);

            if (!correcto)
                errorBreak(4, "zoom");
            break;
        case 3: 
            cout << "\nEjercicio 3: ICONO" << "\nInserte un número de filas y columnas para el icono (debe ser cuadrado):" << endl;
            cin >> f >> c;
            correcto = icono(nentrada, nsalida, f, c);

            if (!correcto)
                errorBreak(4, "icono");
            break;
        case 4: 
            cout << endl << "Ejercicio 4: AUMENTAR CONTRASTE" << "\nInserte un mínimo y un máximo:" << endl;
            cin >> f >> c;
            correcto = aumentarContraste(nentrada, nsalida, f, c);

            if (!correcto)
                errorBreak(4, "aumentar contraste");
            break;
    }
    
    i++;
    } while (op != -1);
       
    return 0;
}


//----------------------------------AUXILIARES---------------------------------//
Imagen conversorVectorImagen(byte *v, Imagen &imagen) {
    for (int f = 0; f < imagen.num_cols(); f++)
        for (int c = 0; c < imagen.num_filas(); c++)
            imagen.asigna_pixel(f, c, v[f+c]);

    return imagen;
}

byte conversorImagenVector(const Imagen &imagen, byte *v){
    for (int f = 0; f < imagen.num_cols(); f++)
        for (int c = 0; c < imagen.num_filas(); c++)
            v[f+c] = imagen.valor_pixel(f,c);

    return *v;
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_UNKNOWN:
            cerr<<"ERROR: Unknown file format."<<endl;
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<< errordata << endl;
            break;
        case ERROR_ARGS:
            cerr<<"Please supply 2 arguments "<< endl;
            break;
        case ERROR_FUNC:
            cerr<<"Error in function " << errordata << endl;
            break;
    }
    exit(1);
}

byte *leerVector(TipoImagen tipo, const string fin, int &f, int &c){
    byte *img;

    if (tipo == TipoImagen::IMG_PGM) {
        img = LeerImagenPGM(fin.c_str(), f, c);                                // Devuelve un vector
    } else if (tipo == TipoImagen::IMG_PPM) {
        img = LeerImagenPPM(fin.c_str(), f, c);                                // Devuelve un vector
    } else
        errorBreak(1,"");

    return img;
}

bool escribirVector(TipoImagen tipo, string& fout, const int f, const int c, byte *img){
    bool okay;

    if (tipo == TipoImagen::IMG_PGM)
        okay = EscribirImagenPPM(fout.c_str(), img, f, c);
    else
        okay = EscribirImagenPGM(fout.c_str(), img, f, c);

    return okay;
}

byte transformacion(double min, double constante, double rmin, double nivelOriginal){
    double nivelFinal;
    nivelFinal = min + (constante * (nivelOriginal - rmin));
    
    byte byteF = round(nivelFinal);
    
    return byteF;
}
//--------------------------------EJERCICIO 1---------------------------------//

bool umbralizar(string fin, string fout, int T_1, int T_2) {
    // hay que abrir ficheros me cago en satanas
    bool okay;
    byte *img;
    Imagen imagen;
    int f = 0, c = 0;

    if (T_1 < 0)
        T_1 = 0;
    if (T_2 > 255)
        T_2 = 255;
    TipoImagen tipo = LeerTipoImagen(fin.c_str());

    img = leerVector(tipo, fin, f, c);                                          // Si el formato es desconocido, se termina la ejecución
    conversorVectorImagen(img, imagen);                                         // Como nos devuelven un vector, necesitamos tenerlo en formato matriz

    for (int i = 0; i < f; i++)                                                 // Umbralizamos según T_1 y T_2
        for (int j = 0; j < c; j++) {
            if (imagen.valor_pixel(i,j) <= T_1 || imagen.valor_pixel(i,j) >= T_2)
                imagen.asigna_pixel(i, j, 255);                                 // Si está dentro del umbral no se hace nada
        }

    conversorImagenVector(imagen, img);                                         // Volvemos a copiar la imagen ya umbralizada
    okay = escribirVector(tipo, fout, f, c, img);

    return okay;
}

//--------------------------------EJERCICIO 3---------------------------------//

bool zoom(string fin, string& fout, int x1, int y1, int x2, int y2){

    bool okay;
    byte *vOriginal, *vFinal;
    int f = 0, c = 0, newF = 0, newC = 0;
    double media = 0;

    TipoImagen tipo = LeerTipoImagen(fin.c_str());
    vOriginal = leerVector(tipo, fin, f, c);

    Imagen imagenOriginal;
    conversorVectorImagen(vOriginal, imagenOriginal);

    newF = x2 - x1;
    newC = y2 - y1;
    Imagen imagen(2*newF-1, 2*newC-1);                                          // Creamos y reservamos memoria para la imagen aumentada

    for(int i = 0; i < newC; i++)
      for(int j = 0; j < newF; j++)
        imagen.asigna_pixel(2*i, 2*j, imagenOriginal.valor_pixel(y1 + i, x1 + j));

    for(int i = 0; imagen.num_filas(); i+2)
      for(int j = 1; j < imagen.num_cols(); j+2){
        media = (imagen.valor_pixel(i,j-1) + imagen.valor_pixel(i, j+1))/ 2;
        imagen.asigna_pixel(i,j, media);
      }

      for(int i = 1; imagen.num_filas(); i+2)
        for(int j = 0; j < imagen.num_filas(); j++){
          media = (imagen.valor_pixel(i-1,j) + imagen.valor_pixel(i+1,j))/ 2;
          imagen.asigna_pixel(i,j, media);
        }

      conversorImagenVector(imagen, vFinal);
      okay = escribirVector(tipo, fout, newF, newC, vFinal);

    return okay;
}

//--------------------------------EJERCICIO 4---------------------------------//

bool icono(string forig, string& frdo, int nf, int nc){
    bool okay;
    byte *vOriginal, *vFinal;
    int f, c, rel;
    double media = 0;

    f = c = rel = 0;
    
    TipoImagen tipo = LeerTipoImagen(forig.c_str());
    vOriginal = leerVector(tipo, forig, f, c);                                  // Guardamos la imagen en un vector

    Imagen imagenOriginal(f,c), icono(nf,nc);                                   // Creamos la imagen original y el icono
    conversorVectorImagen(vOriginal, imagenOriginal);                           // Convertimos el vector a imagen para trabajar con ella
    
    if(f/nf < c/nc)
        rel = ceil(f/nf);                                                       // Si es par, va a dar exacto
    else
        rel = ceil(c/nc);
    
    for(int k = 0; k < nf; k++){                                                // Recorrer el icono
        for (int z = 0; z < nc; z++){
            for(int i = 0 ; i < rel; i++){
                for (int j = 0; j < rel; j++){
                    if(j + z*rel > imagenOriginal.num_cols() || i+ k*rel > imagenOriginal.num_filas())
                        if(j + z*rel > imagenOriginal.num_cols())
                            media += imagenOriginal.valor_pixel(i+ k*rel, j+ z*rel-1);
                        else if (i+ k*rel > imagenOriginal.num_filas())
                            media += imagenOriginal.valor_pixel(i+ k*rel -1, j+ z*rel);
                        else
                            media += imagenOriginal.valor_pixel(i+ k*rel -1, j+ z*rel -1);
                    else
                        media += imagenOriginal.valor_pixel(i+ k*rel, j+ z*rel);
                }
            }
            icono.asigna_pixel(k, z, ceil(media/(rel*rel)));
            media = 0;
        }        
    }
        

    conversorImagenVector(icono, vFinal);
    okay = escribirVector(tipo, frdo, nf, nc, vFinal);
    
    return okay;
}

//--------------------------------EJERCICIO 5---------------------------------//

bool aumentarContraste(string fichE, string& fichS, const int min, const int max){
    
    bool okay;
    byte *vOriginal, *vFinal;
    int f, c;
    double rmax, rmin;
    
    f = c = 0;
    
    TipoImagen tipo = LeerTipoImagen(fichE.c_str());
    vOriginal = leerVector(tipo, fichE, f, c);                                  // Guardamos la imagen en un vector

    Imagen imagen(f,c);
    conversorVectorImagen(vOriginal, imagen); 
    rmax = rmin = imagen.valor_pixel(0,0);                                      // Inicializamos el valor de los niveles de grises
    
    for (int i = 0; i < f; i++)                                                 // Calcular niveles min y max de gris
        for (int j=1; j<c; j++){
            if(imagen.valor_pixel(f,c) > rmax)
                rmax = imagen.valor_pixel(f,c);
            if(imagen.valor_pixel(f,c) < rmin)
                rmin = imagen.valor_pixel(f,c);
        } 
            
    const double cte = (max - min)/(rmax - rmin);
    
    for (int i = 0; i < f; i++)
        for (int j=0; j<c; j++)
            imagen.asigna_pixel(f,c, transformacion(min, cte, rmin, imagen.valor_pixel(f,c)));
    
    conversorImagenVector(imagen, vFinal);
    okay = escribirVector(tipo, fichS, f, c, vFinal);
    
    return okay;
}