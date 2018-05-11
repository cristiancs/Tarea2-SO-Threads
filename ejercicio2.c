#include "headers/ejercicio2.h"

typedef struct {
	int *ptr;
	int menor;
	int mayor;
} args;


void swap(int *p, int i, int j) {
	int temp = *(p+i);
	*(p+i) = *(p+j);
	*(p+j) = temp;
}

int particionar(int *q, int l, int r) {
	int i = l+1, j;
	int p = l;
    for (j = l+1; j <= r; j++) {
        if ( *(q+j) <= *(q+p) ) {
			swap(q, i, j);
			i += 1;
		}
	}
	swap(q, l, i-1);
	return (i-1);
}

void quicksort(void* entrada) {
	args* input = (args*) entrada;
	if (input->menor < input->mayor) {
		int pivote = particionar(input->ptr, input->menor, input->mayor);
		sthread_t hilos[2];

		args arg;
		arg.ptr = input->ptr;
		arg.menor = input->menor;
		arg.mayor = pivote-1;

		args arg2;
		arg2.ptr = input->ptr;
		arg2.menor = pivote+1;
		arg2.mayor = input->mayor;


		sthread_create(&hilos[0], &quicksort, &arg);
		sthread_create(&hilos[1], &quicksort, &arg2);

        sthread_join(hilos[0]);
        sthread_join(hilos[1]);
        sthread_exit(0);
	}
}


int main(){
	int elementos,i;
	while(1){
		printf("Ingrese largo del arreglo: \n> ");
	    scanf("%d",&elementos);

	    int a[elementos];

	    printf("Ingrese arreglo (Ej: 2,3,4):\n > ");
	    for(i=0;i<elementos;i++){
	        scanf("%d,",&a[i]);
	    }

		int *ptr = a;

		// Ordenamos

		args arg;
		arg.ptr = ptr;
		arg.menor = 0;
		arg.mayor = elementos-1;

		sthread_t hilo;

		sthread_create(&hilo, &quicksort, &arg);

		sthread_join(hilo);
		// Mostrar salida
		printf("El arreglo ordenado es: ");
		for (i = 0; i < elementos; ++i) {
			printf("%d", a[i]);
			if(i != elementos-1){
				printf(",");
			}

		}
		printf("\n");
	}
    
	return 0;
}
