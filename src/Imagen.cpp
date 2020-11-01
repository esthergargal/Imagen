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
