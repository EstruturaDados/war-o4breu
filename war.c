#include <stdio.h>
#include <string.h>

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {

    struct Territorio territorios[5];
    int i;

    
    for (i = 0; i < 5; i++) {

        printf("\nCadastro do Território %d\n", i + 1);

        printf("Nome do território: ");
        scanf(" %[^\n]", territorios[i].nome);

        printf("Cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    
    printf("\n=== Territórios Cadastrados ===\n");

    for (i = 0; i < 5; i++) {

        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}