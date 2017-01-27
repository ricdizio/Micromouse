
/*
Name:    Micromouse_maze.ino
Created:    10/25/2016 8:56:42 PM
Author:     Ricardo Di Zio
*/

#include "coord.h"
#include "entry.h"
#include "StackList.h"
#include "QueueList.h"
#include "instruction.h"

//Coordenadas Ejes Globales

#define X 6
#define Y 6

coord inicio = {0,0};

//Cardinales
byte cardinalGlobal = 4;

//Cardinales N,S,E,W
byte cardinales[] = { 1,2,4,8 };


//Instrucciones de navegacion
QueueList <instruction> navegacion;


// Constantes de LEDS
int Media_L = 1, DiagonalD = 1, DiagonalI = 1, DelanteroD = 1, DelanteroI = 1, ParedD = 1, ParedI = 1;

// Test de generacion del maze (distancia y walls)

// Descripcion walls

/*
NSEW = 0
-SEW = 1
N-EW = 2
NS-W = 4
NSE- = 8
NS-- = 12
--EW = 3
N--W = 6
N-E- = 10
-S-W = 5
-SE- = 9
N--- = 14
-S-- = 13
--E- = 11
---W = 7
---- = 15
*/

//mapa
entry maze[X][Y];

//Coordenadas GLobales
coord globalCoord = { 0,0 };
coord globalEnd = { 0,0 };

// the setup function runs once when you press reset or power the board

void setup() {
  Serial.begin(9600);
  iniciar_maze();
  printMazewalls();
}


//###### CODIGO FLOOD FILL

void printMazewalls(){
  for(int j=0; j<Y; j++){
    for(int i=0; i<X; i++){
      Serial.print(maze[j][i].walls);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void printMazedistancia(){
  for(int j=0; j<Y; j++){
    for(int i=0; i<X; i++){
      Serial.print(maze[j][i].distancia);
      Serial.print(" ");
    }
    Serial.println();
  }
}

//Calcula la distancia mas optima entre dos puntos en el arreglo sin paredes
int calcDist(int posx, int posy, int destinoX, int destinoY) {
  int dist = (int)(abs(destinoY - posy) + abs(destinoX - posx));
  return dist;
}


//Chequear con las esquinas si la coordenada esta dentro o fuera del maze
//Si la coordena esta fuera del maze chequear_esquina retorna false

boolean chequear_esquina(coord actual) {
  if ((actual.x >= X) || (actual.y >= Y) || (actual.x < 0) || (actual.y < 0)) {
    return false;
  }
  else {
    return true;
  }
}

// Calcular distacia al centro 
int calcCentro(int posx, int posy, int dim) {
  int centro = dim / 2;
  int dist = 0;

  if (posy<centro) {
    if (posx<centro) {
      //Si estas en la esquina superior izquierda del maze
      dist = calcDist(posx, posy, (centro - 1), (centro - 1));
    }
    else {
      //Si estas en la esquina superior derecha del maze
      dist = calcDist(posx, posy, centro, (centro - 1));
    }
  }
  else {
    if (posx >= centro) {
      //Si estás en la parte inferior derecha del maze
      dist = calcDist(posx, posy, centro, centro);
    }
    else {
      //Si estás en la parte inferior izquierda del maze
      dist = calcDist(posx, posy, (centro - 1), centro);
    }
  }
  return dist;
}

void iniciar_maze() {
  int j;
  int i;
  maze[0][0].walls = 6;
  maze[Y - 1][0].walls = 5;
  maze[0][X - 1].walls = 10;
  maze[X - 1][Y - 1].walls = 9;
  for (j = 0; j<Y; j++) {
    for (i = 0; i<X; i++) {
      maze[j][i].check = 0;
      maze[j][i].distancia = calcCentro(i, j, X);
      if (j != 0 && j != Y - 1 && i != 0 && i != X - 1) {
        maze[j][i].walls = 15;
      }
      //Si es la columna izquierda (0,x)
      if (i == 0 && j != 0 && j != Y - 1) {
        maze[j][i].walls = 7;
      }
      //Si es la fila superior
      if (j == 0 && i != 0 && i != X - 1) {
        maze[j][i].walls = 14;
      }
      //si es la fila inferior
      if (j == (Y - 1) && i != X - 1 && i != 0) {
        maze[j][i].walls = 13;
      }
      //Si es la columna de la derecha
      if (i == (X - 1) && j != Y - 1 && j != 0) {
        maze[j][i].walls = 11;
      }
    }
  }
}

//INPUT: Coordenada actual ,  cardinal
//OUTPUT: Coordenada 
coord coordVecina(coord actual, byte cardinal) {
  coord sig_coord = { 0,0 };
  switch (cardinal) {
  case 1:
    //Me dirijo hacia el Norte
    sig_coord.x = actual.x;
    sig_coord.y = actual.y - 1;
    break;
  case 2:
    //Me dirijo hacia el Sur
    sig_coord.x = actual.x;
    sig_coord.y = actual.y + 1;
    break;
  case 4:
    //Me dirijo hacia el Este
    sig_coord.x = actual.x + 1;
    sig_coord.y = actual.y;
    break;
  case 8:
    //Me dirijo hacia el Oeste
    sig_coord.x = actual.x - 1;
    sig_coord.y = actual.y;
    break;
  }
  return sig_coord;
}

/*
INPUT: void
OUTOUT: Retorna un int que indica las paredes de la casilla actual
*/
byte Haypared() {
  byte casilla = 15;
  byte norte = 0;
  byte sur = 0;
  byte este = 0;
  byte oeste = 0;
  switch (cardinalGlobal) {
  case 1:
    Serial.println("Cardinal Global hacia el norte");
    //Si hay pared al frente
    //Senseores frente
    Serial.println("Sensor frente");
    if (Medir(1)>DelanteroD) {
      //Norte
      norte = 1;
    }
    //Si hay pared a la derecha del carrito
    //Sensor derecho
    Serial.println("Sensor derecho");
    if (Medir(3)>ParedD) {
      //Este
      este = 4;
    }
    //Si hay Pared a la izquierda del carrito
    //Sensor izquierdo
    Serial.println("Sensor izquierdo");
    if (Medir(4)>ParedI) {
      //Oeste
      oeste = 8;
    }
    //15 - Valor obtenido
    casilla -= (norte + este + oeste);
    break;
  case 2:
    Serial.println("Cardinal Global hacia el sur");
    //Si hay pared al frente
    //Senseores frente
    Serial.println("Sensor frente");
    if (Medir(1)>DelanteroD) {
      //Sur
      sur = 2;
    }
    //Si hay pared a la derecha del carrito
    //Sensor derecho
    Serial.println("Sensor derecho");
    if (Medir(3)>ParedD) {
      //Oeste
      oeste = 8;
    }
    //Si hay pared a la izquierda del carrito
    //Sensor izquierdo
    Serial.println("Sensor izquierdo");
    if (Medir(4)>ParedI) {
      //Este     
      este = 4;
    }
    //15 - valor obtenido
    casilla -= (sur + este + oeste);
    break;
  case 4:
    Serial.println("Cardinal Global hacia el este");
    //Si hay pared al frente
    //Senseores frente
    Serial.println("Sensor frente");
    if (Medir(1)>DelanteroD) {
      //Este
      este = 4;
    }
    //Si hay pared a la derecha del carrito
    //Sensor derecho
    Serial.println("Sensor derecho");
    if (Medir(3)>ParedD) {
      //Sur
      sur = 2;
    }
    //Si Hay pared a la izquierda del carrito
    //Sensor izquierdo
    Serial.println("Sensor izquierdo");
    if (Medir(4)>ParedI) {
      //Norte
      norte = 1;
    }
    //15 - valor obtenido
    casilla -= (norte + sur + este);
    break;
  case 8:
    Serial.println("Cardinal Global hacia el oeste");
    //Si hay pared al frente
    //Senseores frente
    Serial.println("Sensor frente");
    if (Medir(1)>DelanteroD) {
      //Oeste
      oeste = 8;
    }
    //Si Hay pared a la derecha del carrito
    //Sensor derecho
    Serial.println("Sensor derecho");
    if (Medir(3)>ParedI) {
      //Norte
      norte = 1;
    }
    //Si hay pared a la izquierda1 del carrito
    //Sensor izquierdo
    Serial.println("Sensor izquierdo");
    if (Medir(4)>ParedD) {
      //Sur
      sur = 2;
    }
    //15 - Valor obtenido
    casilla -= (oeste + norte + sur);
    break;
  }
  return casilla;
}

/*
INPUT: Coordenada actual , cardinal
OUTPUT: Optima direccion en funcion de la coordenada.
*/

byte orientar(coord actual, byte cardinal) {
  coord sig_menor = { 0,0 };
  //El valor mas largo posible (dimension de la matriz)
  int sig_menor_val = X*Y;
  byte sig_dir = cardinal;
  //Si al frente esta disponible seguir
  if ((maze[actual.y][actual.x].walls & cardinal) != 0){
     coord sig_temp = coordVecina(actual, cardinal);    
     if(chequear_esquina(sig_temp)){
       sig_menor = sig_temp;
       sig_menor_val = maze[sig_menor.y][sig_menor.x].distancia;
     }
  }
  
  for (int i = 0; i<sizeof(cardinales); i++) {
    byte dir = cardinales[i];
    //Si la direccion es accesible
    if ((maze[actual.y][actual.x].walls & dir) != 0) {    
      //definimos la coord para la direccion
      coord dirCoord = coordVecina(actual, dir);
      //Chequeo Direcciones disponibles
      if (chequear_esquina(dirCoord)) {
        //Si este direccion es más óptima que continuar recto
        if (maze[dirCoord.y][dirCoord.x].distancia < sig_menor_val) {
          //actualiza el sig menor valor disponible
          sig_menor_val = maze[dirCoord.y][dirCoord.x].distancia;
          //actualiza el valor de sig menor a la direccion
          sig_menor = dirCoord;
          sig_dir = dir;
          Serial.print("Siguente direccion posible del for: ");
          Serial.println(sig_dir);
        }
      }
    }
  }
  Serial.print("Cardinal actual: ");
  Serial.print(cardinal);
  Serial.print("\n");
  return sig_dir;
}



/*
INPUT: coordenada y byte con la informacion de las paredes actuales
OUTPUT: Actualiza el maze con el valor correspondiente segun si hay pared
*/

void actualizar_coord(coord coordenada, byte pared) {
  if (chequear_esquina(coordenada)) {
    if ((maze[coordenada.y][coordenada.x].walls & pared) != 0) {
      maze[coordenada.y][coordenada.x].walls = maze[coordenada.y][coordenada.x].walls - pared;
    }
  }
}


// ##### Flood Fill ####

void flood_fill(coord destino[], coord actual, bool movimiento) {
  coord coord_actual = actual;
  byte cardinal = cardinalGlobal;
  byte sig_cardinal;
  /* Norte / Sur / Este / Oeste
  * 1 = N
  * 4 = E
  * 2 = S
  * 8 = W
  */
  //Mientras no este en la meta !(0) actualizar el maze y moverse a las casillas sig
  while (maze[coord_actual.y][coord_actual.x].distancia != 0) {
    floodFillUpdate(coord_actual, destino, movimiento);
    sig_cardinal = orientar(coord_actual, cardinal);
    Serial.print("decision posible: ");
    Serial.println(sig_cardinal);
    coord sig_coordenada = coordVecina(coord_actual, sig_cardinal);
    Serial.print("pos x: ");
    Serial.print(sig_coordenada.x);
    Serial.println();
    Serial.print("pos y: ");
    Serial.print(sig_coordenada.y);
    Serial.println();

    if (movimiento) {
      /*
      crear_instrucciones(actual, sig_cardinal);
      ejercutar_instrucciones(cola);
      */
      int i = 1;
    }
    //Actualizar el valor para las siguientes entradas del loop
    //coord_actual = sig_coordenada;
    cardinal = sig_cardinal;
    coord_actual = sig_coordenada;
    if (movimiento) {
      cardinalGlobal = cardinal;
      globalCoord = coord_actual;
    }
    printMazewalls();
    printMazedistancia();
    Serial.print("Valor cardinal: ");
    Serial.println(cardinalGlobal);
    Serial.print("Valor coord actual x: ");
    Serial.println(coord_actual.x);
    Serial.print("Valor coord actual y: ");
    Serial.println(coord_actual.y);
  }
  //colocar el globalend como la posicion actual.
  globalEnd = coord_actual;
}

boolean terminado(coord coordenada){
  coord meta[] = { { 2,2},{ 2,3 },{ 3,2 },{ 3,3 } };
  boolean fin = false;
  for (int i = 0; i<4; i++) {
    coord sig = meta[i];
    if (chequear_esquina(coordenada)) {
      if ((coordenada.x == sig.x) && (coordenada.y == sig.y)) {
        fin = true;
      }
    }
  }
  return fin;
}


/*
INPUT: Coord
OUTPUT: un entero dice la distancia menor de todos los vecinos disponibles
*/
int chequear_vecinos(coord coordenada) {
  int minVal = X*Y;
  for (int i = 0; i<sizeof(cardinales); i++) {
    byte dir = cardinales[i];
    //si la direccion es accesible 
    if ((maze[coordenada.y][coordenada.x].walls & dir) != 0) {
      //Obetener la coordenada de la direccion
      coord coordenada_vecina = coordVecina(coordenada, dir);
      //Chequeamos que la coordenada esta dentro del tamano del maze
      if (chequear_esquina(coordenada_vecina)) {
        //si el vecino es uno menos que el actual, almacenar el min
        //Si el minimo es muy grande, probar otro camino
        if (maze[coordenada_vecina.y][coordenada_vecina.x].distancia < minVal) { 
          minVal = maze[coordenada_vecina.y][coordenada_vecina.x].distancia; 
        }
      }
    }
  }
  return minVal;
}


/*
INPUT: Coordenada actual del mouse
OUTPUT: Maze actualizado con las paredes
*/

void floodFillUpdate(coord actual, coord destino[], bool movimiento) {
  StackList<coord> coordenadas;
  if(movimiento){
    maze[actual.y][actual.x].walls = Haypared();
  }
  //maze[actual.y][actual.x].visitado = 1;
  coordenadas.push(actual); 
  for (int i = 0; i<sizeof(cardinales); i++) {
    byte dir = cardinales[i];
    //Si no hay pared en esta coordenada
    if ((maze[actual.y][actual.x].walls & dir) == 0) {
      //Coordenada temporal
      coord workingCoord = { actual.x,actual.y };
      switch (dir) {
      case 1:
        workingCoord.y = workingCoord.y - 1;
        actualizar_coord(workingCoord, 2);
        break;
      case 2:
        workingCoord.y = workingCoord.y + 1;
        actualizar_coord(workingCoord, 1);
        break;
      case 4:
        workingCoord.x = workingCoord.x + 1;
        actualizar_coord(workingCoord, 8);
        break;
      case 8:
        workingCoord.x = workingCoord.x - 1;
        actualizar_coord(workingCoord, 4);
        break;
      }
      //Si la celda tiene una coordenada valida y la coordenada no es la meta push en la pila
      if (chequear_esquina(workingCoord) && (!terminado(workingCoord))) {
        coordenadas.push(workingCoord);
        Serial.println("Coordenada del workingCoord");
        Serial.print(workingCoord.x);
        Serial.print(" ");
        Serial.print(workingCoord.y);
        Serial.println("\n");
      }
    }
  }
  //Mientras la pila no este vacia (tenga coordenadas por actualizar)
  while (!coordenadas.isEmpty()) {
    //Remuevo el ultimo elemento de la pila
    coord celda = coordenadas.pop();
    int vecino_chequear = chequear_vecinos(celda);
    //Regla del flood fill
    if (vecino_chequear + 1 != maze[celda.y][celda.x].distancia) {
      maze[celda.y][celda.x].distancia = vecino_chequear + 1;
      for (int i = 0; i<sizeof(cardinales); i++) {
        byte dir = cardinales[i];
        if ((maze[celda.y][celda.x].walls & dir) != 0) {
          coord sig_coord = coordVecina(celda, dir);
          if (chequear_esquina(sig_coord)) {
            if (!terminado(sig_coord)) {
              coordenadas.push(sig_coord);
            }
          }
        }
      }
    }
  }
}


void retorno(coord coordenada, coord global){
  for(int j = 0; j<Y; j++){
    for(int i = 0; i<X; i++){
      maze[j][i].distancia = calcDist(i, j, coordenada.x, coordenada.y);
    }
  }
  //Entrada por Sur/Este
  if ( (global.x==(X/2))&& (global.y==(Y/2)) ){
    maze[global.x][global.y-1].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x-1][global.y].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x-1][global.y-1].distancia = maze[global.x][global.y].distancia +2;
  }

  //Entrada por Oeste/Este
  if ( (global.x==(X/2)-1)&& (global.y==(Y/2)) ){
    maze[global.x][global.y-1].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x+1][global.y].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x+1][global.y-1].distancia = maze[global.x][global.y].distancia +2;
  }
  //entrada por Sur/norte
  if ( (global.x==(X/2))&& (global.y==(Y/2)-1) ){
    maze[global.x][global.y+1].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x-1][global.y].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x-1][global.y+1].distancia = maze[global.x][global.y].distancia +2;
  }

  //entrada por Oeste / Norte
  if ( (global.x==(X/2)-1)&& (global.y==(Y/2)-1) ){
    maze[global.x][global.y+1].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x+1][global.y].distancia = maze[global.x][global.y].distancia +1;
    maze[global.x+1][global.y+1].distancia = maze[global.x][global.y].distancia +2;
  }
  
        
}


//####### FIN FLOOD FILL ##########


int Medir(int x) { 
  while (Serial.available() == 0){;}
  int valor = Serial.parseInt(); //read int or parseFloat for ..float...
  Serial.println("Valor introducido: ");
  Serial.println(valor);
  return valor;
}



///// ##### REFLOOD



//######## Procedimiento Reflood ########
void reflood(coord meta[]){
  //Colocar distancia en inicial pero ahora el maze tiene las paredes almacenadas
  reiniciar_maze();
  
  //Run flood fill but without actual motion
  coord actual = {0,0};
  flood_fill(meta, actual, false);
  
  //El mouse permanece en el inicio del maze pero con la distancia mas optima
  
  //creamos cola de velocidad
  //crear_cola_velocidad();
  
  //Ejecutar instrucciones
  //ejecutar_instrcciones();
  
  
}

//Reiniciar distancias
void reiniciar_maze(){
  for(int j = 0; j<Y; j++){
    for(int i = 0; i<X; i++){
      maze[j][i].distancia = calcCentro(i, j, X);
    }
  }
}


void loop() {
  printMazedistancia();
  coord meta[] = { { 2,2},{ 2,3 },{ 3,2 },{ 3,3 } };
  //Flood fill
  flood_fill(meta, globalCoord, true);
  Serial.println("Llegue al centro, Inicio Retorno");
 //Nuevo punto a retorno ( Inicio)
  coord inicio[] = {{0,0}};
  retorno(inicio[0], globalCoord);
  
  printMazedistancia();
  flood_fill(inicio, globalCoord, true);
  Serial.println("Termino fase de exploracion");
  reflood(meta);
  Serial.println("Comienza reflood");
  Serial.println("Imprimir Matrices resultantes");
  Serial.println("MATRIZ PAREDES");
  printMazewalls();
  Serial.print("\n");
  Serial.println("MATRIZ DISTANCIA");
  printMazedistancia();
  
  while(1);
}

