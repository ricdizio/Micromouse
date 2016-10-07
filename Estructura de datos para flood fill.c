#include <stdio.h>

//Declaracion de la Estructura tipo nodo de una pila
typedef struct nodo{
    struct nodo *siguiente;
    int* dato;
    }tipoNodo;   
tipoNodo *ultimo = NULL;
//Funciones para la pila
//Verificar si la pila esta o no vacia

int IsEmpty(){
    if(ultimo == NULL){
        return 1;}
    else{
        return 0;}
}
//Agregar elemento a la pila
void push(tipoNodo *_elemento){
     _elemento->siguiente = NULL;
     //Si  la pila esta vacia 
     if(IsEmpty() == 1){
         ultimo = _elemento;}
     else{
         _elemento->siguiente = ultimo;
         ultimo = _elemento;
         }
     }
// Remover y retornar el ultimo elemento de la pila
tipoNodo* pop(){
    if(IsEmpty() == 1){
        return NULL;}
    else{
        tipoNodo *_elemento_retorno = ultimo;
        ultimo = _elemento_retorno->siguiente;
        return _elemento_retorno;
        }
}
