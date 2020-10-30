/**
 * @file Imagen.cpp
 * @brief TDA de imágenes
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

using namespace std;

//--------------------------------MEMORIA--------------------------------------//

void Imagen::allocate(int f, int c){
  if (f > 0)
    filas = f;
  else
    filas = 0;
  if (c > 0)
    cols = c;
  else
    cols = 0;

    if (filas > 0) {
        img = new byte*[f];
        if (cols > 0) {
            img[0] = new byte[filas * cols];
            for (int i = 1; i < filas; i++)
                img[i] = img[i - 1] + cols;
        } else
            for (int i = 0; i < filas; i++)
                img[i] = nullptr;
    } else
        img = nullptr;
}

void Imagen::deallocate(){
  if(img){ // img != nullptr
    delete[] img[0];
    delete[] img;
    img = nullptr; // se puede quitar
    filas = cols = 0;
  }
}

//----------------------------------AUXILIARES---------------------------------//

void Imagen::copy(const Imagen& i){
  for(int f = 0; f < i.num_filas(); f++)
    for(int c = 0; c < i.num_cols(); c++)
      img[f][c] = i.img[f][c];
}

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

//-----------------------CONSTRUCTORES Y DESTRUCTORES-------------------------//

Imagen::Imagen (){                                  // Constructor por defecto
  filas = cols = 0;
  img = nullptr;
}

Imagen::Imagen (const Imagen & J){                  // Constructor de copia
  img = nullptr;
  allocate(J.filas, J.cols);
  copy(J);
}

Imagen::Imagen (int f, int c){                     // Constructor con parámetros
  allocate(f, c);
}

Imagen::~Imagen(){                                 // Destructor
  deallocate();                                    // El propio deallocate modifica el número de filas y columnas de la matriz
}

//----------------------------------OPERADORES---------------------------------//

Imagen& Imagen::operator=(const Imagen& i){  // Operador de asignacion
  if(this != &i){
    deallocate();
    allocate(i.filas, i.cols);
    copy(i);
  }
  return *this;
}

//----------------------------------MÉTODOS-----------------------------------//

void Imagen::asigna_pixel (int fila, int col, byte valor){
    assert( (fila >= 0 && fila < filas) && (col < cols && col >= 0) );
    img[fila][col] = valor;
}

byte Imagen::valor_pixel (int fila, int col) const{
    assert( (fila >= 0 && fila < filas) && (col < cols && col >= 0) );  //Si ambas están dentro de rango, no pasa nada
    return img[fila][col];
}

//--------------------------------EJERCICIO 1---------------------------------//

bool umbralizar(string fin, string fout, byte T_1, byte T_2) {
    // hay que abrir ficheros me cago en satanas
    bool okay;
    byte *img;
    Imagen imagen;
    int f = 0, c = 0;

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
    
    if(f/nf > c/nc)
        rel = ceil(f/nf);                                                           // Si es par, va a dar exacto
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

bool aumentarConstraste(string fichE, string& fichS, const int min, const int max){
    
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