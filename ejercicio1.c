#include "headers/ejercicio1.h"

typedef struct {
	char *entrada;
	char signo;
	Dictionary* funciones;
} argumentos;

void removeChar(char *str, char garbage) {
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void calcular(void* entrada){
	argumentos* input = (argumentos*) entrada;
	int suma = 0;
	int cantidadThreads = 0;
	char funcion[100], x[100], valor[100];

	// Primero debemos buscar la función en el diccionario

	char *string, *tofree;
	tofree = string = strdup(input->entrada);
	strcpy(funcion, strsep(&string, "("));
	strcat(funcion, "(x)");
	strcpy(x, strsep(&string, ")"));
	free(tofree);
	strcpy(valor, dict_get(input->funciones, funcion));
	
	//printf("%s", valor);
	
	// Ahora regex para encontrar funciones dentro
	// https://gist.github.com/ianmackinnon/3294587
	char * regexString = "[-+]?(\\w\\(x\\)|\\w+)";
	sthread_t* threads = (sthread_t*)malloc(sizeof(sthread_t)*100);
	size_t maxMatches = 100;
	size_t maxGroups = 1;

	regex_t regexCompiled;
	regmatch_t groupArray[maxGroups];
	unsigned int m;
	char * cursor;
	if (regcomp(&regexCompiled, regexString, REG_EXTENDED))
	{
	  printf("Could not compile regular expression.\n");
	  sthread_exit(-1);
	};

	m = 0;
	cursor = valor;

	for (m = 0; m < maxMatches; m ++) {
	  	if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
			break;  // No more matches

		unsigned int g = 0;
		unsigned int offset = 0;
		for (g = 0; g < maxGroups; g++){
			if (groupArray[g].rm_so == (size_t)-1)
				break;  // No more groups

			if (g == 0)
				offset = groupArray[g].rm_eo;

			char cursorCopy[strlen(cursor) + 1];
			strcpy(cursorCopy, cursor);
			cursorCopy[groupArray[g].rm_eo] = 0;


			// Crear Threads que resuelvan los demas hijos
			//printf("Match %u, Group %u: [%2u-%2u]: %s\n",
			//		 m, g, groupArray[g].rm_so, groupArray[g].rm_eo,
			//		 cursorCopy + groupArray[g].rm_so);
			
			// Verificar Si es Funcion o entero
			char texto[100];
			texto[0] = '\0';
			strcpy(texto, cursorCopy + groupArray[g].rm_so);

			char signo;

			// Detectar si debemos sumar o restar
			if(texto[0] == '-'){
				signo = '-';
			}
			else{
				signo = '+';
			}

			removeChar(texto, '-');
			removeChar(texto, '+');
			if(dict_has(input->funciones, texto) == 1){
				// Es función, crear thread y esperar por el

				// Reconstruir String
				char *string, *tofree, toCall[100];
				toCall[0] = '\0';
				tofree = string = strdup(texto);


				strcat(toCall, strsep(&string, "("));
				strcat(toCall, "(");
				strcat(toCall, x);
				strcat(toCall, ")");


				free(tofree);
				

				argumentos arg;
				arg.entrada = toCall;
				arg.funciones = input->funciones;
				arg.signo = signo;
				//printf("Creando thread con signo %c\n", signo);
				sthread_create(&threads[cantidadThreads], &calcular, &arg);
				cantidadThreads+=1;
			}
			else{
				// Verificar si es x o numero
				if(texto[0] == 'x'){
					strcpy(texto, x);
				}

				if(signo == '-'){
					//printf("Restando %d\n", atoi(texto));
					suma-= atoi(texto);
				}
				else if(signo == '+'){
					//printf("Sumando %d\n", atoi(texto));
					suma+= atoi(texto);
				}
				
			}

		  cursor += offset;
		}
	}
	int i;
	for(i=0; i < cantidadThreads; i++){
        int respuesta = sthread_join(threads[i]);
		suma+=respuesta;
    }


	regfree(&regexCompiled);

	free(threads);

	if(input->signo == '-'){
		//printf("Retornando suma: %d\n", -suma);
		sthread_exit(-suma);
	}
    else{
    	//printf("Retornando suma: %d\n", suma);
		sthread_exit(suma);
	}

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
	while(strcmp(linea, "-1") != 0){
		printf("Ingrese Operación:\n>");
		scanf("%s", linea);
		argumentos arg;
		arg.entrada = linea;
		arg.funciones = funciones;
		arg.signo = '=';
		sthread_t hilo;
		sthread_create(&hilo, &calcular, &arg);
		int salida;

		salida = sthread_join(hilo);

		printf("La salida es %d\n\n" , salida);
	}
	
	return 0;
}

