# Micromouse


Código realizado en conjunto con la agrupación de CITE de la Universidad Simón Bolívar para la competencia de robótica en la categoría de laberinto, realizada el 7,8,9 de noviembre de 2016.

El algoritmo flood fill para un mouse o robot inicia en la esquina del laberinto, y el objetivo (la celda objetivo) es llegar al centro del mismo. Al inicio del código se asume que el laberinto no posee paredes y se le asigna a cada celda un tamaño predefinido. Al tener todo el laberinto inicializado el carrito puede comenzar su trayectoria atreves del laberinto. Al comenzar el movimiento el robot avanza una celda y actualiza su laberinto en memoria con las nuevas paredes encontradas. A continuación, el robot comprueba si el valor de la a una nueva celda y chequee si es la meta o no, luego de esto si no se encuentra en la celda destino, se procede a actualizar su celda actual y todas las anteriores incluyendo algunas sin visitar que se hayan visto afectadas por el descubrimiento de nuevas paredes dentro del laberinto. Al terminar el proceso el robot posee en su memoria la resolución del laberinto y es capaz de llegar al centro por el camino más optimo

