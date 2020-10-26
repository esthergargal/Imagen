/**
 * @file Imagen.cpp
 * @brief TDA de imágenes
 */


#include <cassert>
#include <cstring>
#include <iostream>
#include "imagenES.h"
#include "Imagen.h"

#define byte unsigned char   //Definimos un tipo de dato llamado byte [0, 255]
#define ERROR_UNKNOWN 1
#define ERROR_OPEN 2
#define ERROR_DATA 3

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

byte leerVector(TipoImagen tipo, string fin, int &f, int &c){
    byte *img;

    if (tipo == TipoImagen::IMG_PGM) {
        img = LeerImagenPGM(fin.c_str(), f, c);                                // Devuelve un vector
    } else if (tipo == TipoImagen::IMG_PPM) {
        img = LeerImagenPPM(fin.c_str(), f, c);                                // Devuelve un vector
    } else
        errorBreak(1,"");

    return img;
}

bool escribirVector(TipoImagen tipo, string fout, const int f, const int c, byte *img){
    bool okay;

    if (tipo == TipoImagen::IMG_PGM)
        okay = EscribirImagenPPM(fout.c_str(), img, f, c);
    else
        okay = EscribirImagenPGM(fout.c_str(), img, f, c);

    return okay;
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

    /*for(int i = x1; i < x2; i++){
      for (int j = y1 ; j < y2; j++){
        if(i%2 == 0)
          imagen.asigna_pixel(i, j, imagenOriginal.valor_pixel(i,j));                             // Asignamos la casilla de la izquierda
        else
          imagen.asigna_pixel(i+2, j, imagenOriginal.valor_pixel(i,j));                             // Asignamos la casilla de la izquierda
        if(j+1 < y2){
          media = (imagenOriginal.valor_pixel(i,j) + imagenOriginal.valor_pixel(i,j+1)) / 2;      // Calculamos la media de una casilla y la siguiente
          imagen.asigna_pixel(i, j+1, media);                                                   // Asignamos la casilla de la media
        }

      }

      //COMPROBAR LA INVENTADA DEL SIGLO OTRO DÍA
      for(int j = y1; j < y2; j++){
        for (int i = x1 ; i < x2; i++){
          media = (imagenOriginal.valor_pixel(i,j) + imagenOriginal.valor_pixel(i+1,j)) / 2;    // Calculamos la media de una casilla y la siguiente
          if(i%2 == 0)
            imagen.asigna_pixel(i, j, imagenOriginal.valor_pixel(i,j));                           // Asignamos la casilla de la izquierda
          else
            imagen.asigna_pixel(i, j+2, imagenOriginal.valor_pixel(i,j));
          if(i+1 < x2)
            imagen.asigna_pixel(i+1, j, media);                                                 // Asignamos la casilla de la media
        }*/

      conversorImagenVector(imagen, vFinal);
      okay = escribirVector(tipo, fout, newF, newC);

    return okay;
}

//--------------------------------EJERCICIO 4---------------------------------//
