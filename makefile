#Trabaja con el numero de datos en el archivo, la ultima vez que se probo se dejo en 250000 datos
scriptBuscar:
	gcc buscarViaje.c -o buscarViaje
	./buscarViaje
scriptMenu:
	gcc menu.c -o menu
	./menu
#Recordar que construir la tabla binaria completa con la complejidad actual del codigo, toma demasiado tiempo
#por eso se deja con un valor bajo de linea/estructuras a escribir en el archivo binario, 10000.
scriptCrearTabla:
	gcc tablaBinaria.c -o tablaBinaria
	./tablaBinaria
