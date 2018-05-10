#include "headers/ejercicio1.h"

typedef struct {
    char *entrada;
    Dictionary* funciones;
} argumentos;

void calcular(void* entrada){
	argumentos* input = (argumentos*) entrada;
	char funcion[100], x[100], valor[100];

	// Primero debemos buscar la función en el diccionario

	char *token, *string, *tofree;
	tofree = string = strdup(input->entrada);
	strcpy(funcion, strsep(&string, "("));
	strcat(funcion, "(x)");
	strcpy(x, strsep(&string, "("));
	free(tofree);
	strcpy(valor, dict_get(input->funciones, funcion));
	
	tofree = string = strdup(valor);
	assert(string != NULL);
	while ((token = strsep(&string, "-")) != NULL)
           printf("%s\n", token);
       
    while ((token = strsep(&string, "+")) != NULL)
           printf("%s\n", token);
   free(tofree);




	sthread_exit(1);
	//char * regexString = "[a-z]*([0-9]+)([a-z]*)";
}

int main(){
	char cantidad_funciones[3];

	FILE *fptr;
	// Abrir archivo
	if ((fptr = fopen("funciones.txt","r")) == NULL){
		printf("Error! opening file");
		exit(1);
	}
	char linea[101];
	fgets(cantidad_funciones,sizeof(cantidad_funciones), fptr);
	//printf("Cantidad de lineas %s\n", cantidad_funciones);
	char funcion[5];
	char valor[96];

	Dictionary *funciones = dict_new();

	// Cargar Funciones
	while (fgets(linea, sizeof(linea), fptr) != 0){
		char *string, *tofree, *pos;
	    tofree = string = strdup(linea);
		strcpy(funcion, strsep(&string, "="));
		strcpy(valor, strsep(&string, "="));
		if ((pos=strchr(valor, '\n')) != NULL)
    		*pos = '\0';
//		printf("S: |%s| \n", valor);
		dict_add(funciones, funcion, valor);
		free(tofree);
	}
	fclose(fptr);

	printf("Funciones Ingresadas!\n");
	//while(strcmp(linea, "-1") != 0){
	printf("Ingrese Operacion:\n>");
	scanf("%[^\n]", linea);
	argumentos arg;
	arg.entrada = linea;
	arg.funciones = funciones;
	sthread_t hilo;
	sthread_create(&hilo, &calcular, &arg);
	int salida;

	salida = sthread_join(hilo);

	printf("La salida es %d" , salida);
//	}
	
	return 0;
}

