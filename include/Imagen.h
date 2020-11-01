/**
 * @file Imagen.h
 * @brief Contiene las cabeceras del TDA de imágenes
 */

 #ifndef _IMAGEN_
 #define _IMAGEN_

 #define byte unsigned char   //Definimos un tipo de dato llamado byte [0, 255]

 class Imagen {
     private:

       byte **img;                          // La imagen en sí: una matriz dinamica 2D de bytes
       int cols, filas;                     // Número de columnas y filas de la imagen, respectivamente

       /**
       * @brief Reserva memoria para la matriz 2D
       * @param f Número de filas de la matriz
       * @param c Número de columnas de la matriz
       */
       void allocate(int f, int c);
       /**
       * @brief Libera la memoria reservada
       */
       void deallocate();

       /*
       * @brief Método privado para copiar imágenes
       */
       void copy(const Imagen& i);

     public:


       /**
       * @brief Constructor por defecto
       */
       Imagen ();

       /**
       * @brief Constructor de copia
       * @param J La imagen que se quiere copiar
       */
       Imagen (const Imagen & J);

       /**
       * @brief Constructor con parámetros
       * @param filas ancho de la Imagen
       * @param cols alto de la Imagen
       */
       Imagen (int filas, int cols);

       /**
       * @brief Destructor
       */
       ~Imagen();

       /**
       * @brief Consultor del atributo filas
       */
       inline int num_filas() const{
         return  filas;
       }

       /**
       * @brief Consultor del atributo columnas
       */
       inline int num_cols () const{
         return cols;
       }

       /**
       * @brief Sobrecarga del operador =
       * @param i La imagen a copiar
       */
       Imagen& operator=(const Imagen& i);

       /**
       * @brief Setter
       * @param fila indice de fila
       * @param col indice de columna
       */
       void asigna_pixel (int fila, int col, byte valor);

       /**
       * @brief el byte correspondiente a esa zona
       * @param fila indice de fila
       * @param col indice de columna
       */
       byte valor_pixel (int fila, int col) const;
 };

 #endif
