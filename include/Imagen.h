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

byte leerVector(TipoImagen tipo, std::string fin, int &f, int &c);

bool escribirVector(TipoImagen tipo, std::string fout, const int f, const int c, byte *img);

/**
 * @brief Función que permite umbralizar una Imagen
 * @param fin Nombre del fichero de entrada
 * @param fout Nombre del fichero de salida
 * @param T_1 Umbral mínimo
 * @param T_2 Umbral máximo
 * @precond T_1 < T_2
 * @return @retval true si la operación se ha realizado con éxito @retval false en caso contrario
 */
bool umbralizar(std::string fin, std::string& fout, byte T_1, byte T_2);

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

 #endif
