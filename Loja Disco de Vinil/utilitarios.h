
#include <stdio.h>
#include <stdlib.h>


void limparTela() {
    system("cls");
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void limparTexto(char texto[]) {
    int i;

    for (i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == '\n') {
            texto[i] = '\0';            
            break;
        }
        if (texto[i] == ';') {
            texto[i] = ',';
        }
    }
}
