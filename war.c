#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TAM_STRING 30

typedef struct {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int tropas;
} Territorio;

void atacar(Territorio *atacante, Territorio *defensor);

int main() {

    int num_territorios;

    srand(time(NULL));

    printf("Digite o número de territorios: ");
    scanf("%d", &num_territorios);

    Territorio *territorios = malloc(num_territorios * sizeof(Territorio));

    // Cadastro
    for (int i = 0; i < num_territorios; i++) {
        printf("\nDigite o nome do território %d: ", i + 1);
        scanf("%s", territorios[i].nome);

        printf("Digite a cor do território %d: ", i + 1);
        scanf("%s", territorios[i].cor);

        printf("Digite o número de tropas no território %d: ", i + 1);
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição inicial
    printf("\nTerritórios cadastrados:\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               i + 1,
               territorios[i].nome,
               territorios[i].cor,
               territorios[i].tropas);
    }

    int a, d;

    printf("\nDigite o número do território atacante: ");
    scanf("%d", &a);

    printf("Digite o número do território defensor: ");
    scanf("%d", &d);

    // Ajuste de índice (usuário começa em 1)
    a--;
    d--;

    // Validação
    if (a < 0 || a >= num_territorios || d < 0 || d >= num_territorios) {
        printf("Índice inválido!\n");
    } 
    else if (strcmp(territorios[a].cor, territorios[d].cor) == 0) {
        printf("Não é possível atacar territórios da mesma cor!\n");
    } 
    else {
        atacar(&territorios[a], &territorios[d]);
    }

    // Exibição após ataque
    printf("\n--- Estado após ataque ---\n");
    for (int i = 0; i < num_territorios; i++) {
        printf("%s | Cor: %s | Tropas: %d\n",
               territorios[i].nome,
               territorios[i].cor,
               territorios[i].tropas);
    }

    free(territorios);

    return 0;
}

// Função de ataque
void atacar(Territorio *atacante, Territorio *defensor) {

    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("\nAtacante (%s) rolou: %d\n", atacante->nome, dado_atacante);
    printf("Defensor (%s) rolou: %d\n", defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {

        printf("Atacante venceu!\n");

        // Troca de dono (cor)
        strcpy(defensor->cor, atacante->cor);

        // Transferência de tropas
        int metade = atacante->tropas / 2;

        defensor->tropas = metade;
        atacante->tropas -= metade;

    } else {

        printf("Defensor venceu!\n");

        atacante->tropas--;
    }

    printf("Tropas restantes - Atacante: %d | Defensor: %d\n",
           atacante->tropas,
           defensor->tropas);
}
