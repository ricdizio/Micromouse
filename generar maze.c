#include <stdio.h>
#include <math.h>
//Coordenadas Ejes Globales
#define X 10
#define Y 10

// Test de generacion del maze (distancia y walls)
// Descripcion walls
//
// 15 -> casilla sin paredes en los 4 lados
// 13 -> casilla con pared inferior. 3 lados libres
// 10 -> casilla con pared derecha y superior, 2 lados libres
// 9  -> casilla con pared lado inferior y derecho, 2 lados libres
// 7  -> casilla con pared izquierda, 3 lados libres
// 6  -> casilla con pared lado izquierdo y superior, 2 lados libres
// 5  -> casilla con pared lado izquiero e inferior, 2 lados libres
//
// en la estructura coord x y y en entry walls pueden ser definidas como bytes para ahorrar
// memoria

// Estructura coordenada
typedef struct coord
{
  int x;
  int y;
}coord;

//Estructutura Entrada
typedef struct entry
{
  int distancia;
  int walls;
}entry;

//mapa
entry maze[Y][X];

//Coordenadas GLobales
coord globalCoord = {0,0};
coord globalEnd = {0,0};

//Calcula la distancia mas optima entre dos puntos en el arreglo sin paredes
int calcDist(int posx, int posy, int destinoX, int destinoY){
  int dist = (int) (abs(destinoY-posy)+abs(destinoX-posx));
  return dist;
}

int calcCentro(int posx, int posy, int dim){
  int centro = dim/2;
  int dist = 0;
  
  if(posy<centro){
    if(posx<centro){
      //Si estas en la esquina superior izquierda del maze
      dist=calcDist(posx, posy, (centro-1), (centro-1));
    }else{
      //Si estas en la esquina superior derecha del maze
      dist=calcDist(posx,posy,centro,(centro-1));
    }
  }else{
    if(posx>=centro){
      //Si estás en la parte inferior derecha del maze
      dist=calcDist(posx,posy,centro,centro);
    }else{
      //Si estás en la parte inferior izquierda del maze
      dist=calcDist(posx,posy, (centro-1),centro);
    }
  }
return dist;
}


void iniciar_maze(){
  int j;
  int i;
  maze[0][0].walls = 6;
  maze[Y-1][0].walls = 5;
  maze[0][X-1].walls = 10;
  maze[X-1][Y-1].walls = 9;
  for(j = 0; j<Y; j++){
    for(i = 0; i<X; i++){
      maze[j][i].distancia = calcCentro(i, j, X);
      if(j!=0 && j!=Y-1 && i!=0 && i!=X-1){
        maze[j][i].walls = 15;}
      //Si es la columna izquierda (0,x)
      if(i==0 && j!=0 && j!=Y-1 ){
        maze[j][i].walls = 7;
      }
      //Si es la fila superior
      if(j==0 && i!=0 && i!=X-1){
        maze[j][i].walls = 14;
      }
      //si es la fila inferior
      if(j==(Y-1) && i!=X-1 && i!=0){
        maze[j][i].walls = 13;
      }
      //Si es la columna de la derecha
      if(i==(X-1) && j!=Y-1 && j!=0){
        maze[j][i].walls = 11;
      }
    }
  }
}
// ################################# MAIN #################################
// Resultado de la generacion del maze

int main (){
  iniciar_maze();
  int j;
  int i;
  //print de las distancias del maze
  for(j=0; j<Y; j++){
    for(i=0; i<X; i++){
      printf("%d",maze[j][i].distancia);
      printf(" ");
    }
    printf("\n");
  }
  printf("\n");
  //print de las paredes del maze
  for(j=0; j<Y; j++){
    for(i=0; i<X; i++){
      printf("%d",maze[j][i].walls);
      printf(" ");
    }
    printf("\n");
  }
  system("PAUSE");
}




