/*
 * ============================================================
 *  Jogo de Batalha com Sistema de Missões
 *  Baseado no código original do aluno, com adição de:
 *    - Vetor de missões com sorteio aleatório
 *    - Atribuição dinâmica de missão por jogador
 *    - Verificação automática de missão a cada turno
 *    - Modularização completa em funções
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ── Constantes ─────────────────────────────────────────── */
#define TAM_NOME    30
#define TAM_COR     10
#define TAM_MISSAO 100
#define TOTAL_MISSOES 5

/* ── Estrutura de Território ────────────────────────────── */
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int  tropas;
} Territorio;

/* ── Protótipos ─────────────────────────────────────────── */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int  verificarMissao(char *missao, Territorio *mapa, int tamanho);
void exibirMissao(char *missao);          /* passagem por valor (ponteiro para char, mas sem modificar) */
void atacar(Territorio *atacante, Territorio *defensor);
void exibirMapa(Territorio *mapa, int tamanho);
void liberarMemoria(Territorio *mapa, char **missoes, int numJogadores);

/* ================================================================
   main
   ================================================================ */
int main(void) {

    srand(time(NULL));

    /* ── 1. Vetor de missões disponíveis ──────────────────── */
    char *missoes[TOTAL_MISSOES] = {
        "Conquistar 3 territorios seguidos no mesmo turno",
        "Eliminar todas as tropas da cor Vermelho",
        "Dominar ao menos metade dos territorios do mapa",
        "Atacar e vencer 2 batalhas consecutivas",
        "Ter mais de 20 tropas distribuidas no mapa"
    };

    /* ── 2. Cadastro de territórios ───────────────────────── */
    int num_territorios;
    printf("=== BEM-VINDO AO JOGO DE BATALHA ===\n\n");
    printf("Digite o numero de territorios: ");
    scanf("%d", &num_territorios);

    /* Alocação dinâmica do mapa com malloc */
    Territorio *mapa = malloc(num_territorios * sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1;
    }

    for (int i = 0; i < num_territorios; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);
        printf("Nome  : ");
        scanf("%s", mapa[i].nome);
        printf("Cor   : ");
        scanf("%s", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    /* ── 3. Cadastro de jogadores e sorteio de missões ────── */
    int num_jogadores;
    printf("\nDigite o numero de jogadores: ");
    scanf("%d", &num_jogadores);

    /* Vetor de nomes dos jogadores */
    char (*nomes_jogadores)[TAM_NOME] = malloc(num_jogadores * sizeof(*nomes_jogadores));

    /* Vetor de missões dos jogadores — cada missão alocada dinamicamente */
    char **missoes_jogadores = malloc(num_jogadores * sizeof(char *));
    if (!missoes_jogadores || !nomes_jogadores) {
        printf("Erro ao alocar memoria para jogadores!\n");
        free(mapa);
        return 1;
    }

    printf("\n=== CADASTRO DOS JOGADORES ===\n");
    for (int j = 0; j < num_jogadores; j++) {
        printf("Nome do jogador %d: ", j + 1);
        scanf("%s", nomes_jogadores[j]);

        /* Aloca espaço para a missão deste jogador */
        missoes_jogadores[j] = malloc(TAM_MISSAO * sizeof(char));
        if (!missoes_jogadores[j]) {
            printf("Erro ao alocar missao do jogador %d!\n", j + 1);
            return 1;
        }

        /* Sorteia e copia a missão para o jogador */
        atribuirMissao(missoes_jogadores[j], missoes, TOTAL_MISSOES);
    }

    /* ── 4. Exibe missões de cada jogador (apenas uma vez) ── */
    printf("\n=== MISSOES SORTEADAS ===\n");
    for (int j = 0; j < num_jogadores; j++) {
        printf("\nJogador: %s\n", nomes_jogadores[j]);
        exibirMissao(missoes_jogadores[j]); /* passagem por valor (leitura) */
    }

    /* ── 5. Loop de batalha ───────────────────────────────── */
    printf("\n=== INICIO DAS BATALHAS ===\n");
    exibirMapa(mapa, num_territorios);

    int continuar = 1;
    while (continuar) {

        /* Escolha do atacante e defensor */
        int a, d;
        printf("\nDigite o numero do territorio ATACANTE (0 para encerrar): ");
        scanf("%d", &a);
        if (a == 0) break;

        printf("Digite o numero do territorio DEFENSOR: ");
        scanf("%d", &d);

        a--; d--;

        /* Validação de índices */
        if (a < 0 || a >= num_territorios || d < 0 || d >= num_territorios) {
            printf("Indice invalido! Tente novamente.\n");
            continue;
        }

        /* Validação: não pode atacar território aliado */
        if (strcmp(mapa[a].cor, mapa[d].cor) == 0) {
            printf("Nao e possivel atacar territorios da mesma cor!\n");
            continue;
        }

        /* Realiza o ataque */
        atacar(&mapa[a], &mapa[d]);

        /* Exibe mapa atualizado */
        exibirMapa(mapa, num_territorios);

        /* ── 6. Verificação silenciosa de missões ─────────── */
        for (int j = 0; j < num_jogadores; j++) {
            /* passagem por referência para verificação */
            if (verificarMissao(missoes_jogadores[j], mapa, num_territorios)) {
                printf("\n*** PARABENS, %s! Voce cumpriu sua missao! ***\n", nomes_jogadores[j]);
                printf("Missao: %s\n", missoes_jogadores[j]);
                printf("\n=== %s VENCEU O JOGO! ===\n", nomes_jogadores[j]);
                continuar = 0;
                break;
            }
        }
    }

    /* ── 7. Liberação de memória ──────────────────────────── */
    liberarMemoria(mapa, missoes_jogadores, num_jogadores);
    free(nomes_jogadores);

    printf("\nObrigado por jogar!\n");
    return 0;
}

/* ================================================================
   atribuirMissao
   Sorteia aleatoriamente uma missão do vetor e copia para 'destino'
   usando strcpy. Parâmetro 'destino' é passado por referência
   (ponteiro) para que a string seja modificada.
   ================================================================ */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

/* ================================================================
   verificarMissao
   Avalia se a missão foi cumprida com base no estado atual do mapa.
   Retorna 1 se cumprida, 0 caso contrário.
   Missão passada por referência (ponteiro) para leitura eficiente.
   ================================================================ */
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {

    /* Missão 1: Dominar ao menos metade dos territórios */
    if (strstr(missao, "metade dos territorios")) {
        /* Conta territórios por cor e verifica se alguma cor domina >= 50% */
        /* Simplificação: verifica se alguma cor aparece em mais da metade */
        int dominados[50] = {0};  /* contagem por índice de cor encontrada */
        char cores[50][TAM_COR];
        int num_cores = 0;

        for (int i = 0; i < tamanho; i++) {
            int encontrou = 0;
            for (int c = 0; c < num_cores; c++) {
                if (strcmp(mapa[i].cor, cores[c]) == 0) {
                    dominados[c]++;
                    encontrou = 1;
                    break;
                }
            }
            if (!encontrou) {
                strcpy(cores[num_cores], mapa[i].cor);
                dominados[num_cores] = 1;
                num_cores++;
            }
        }

        for (int c = 0; c < num_cores; c++) {
            if (dominados[c] >= tamanho / 2) return 1;
        }
        return 0;
    }

    /* Missão 2: Eliminar todas as tropas da cor Vermelho */
    if (strstr(missao, "cor Vermelho")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    }

    /* Missão 3: Ter mais de 20 tropas distribuídas */
    if (strstr(missao, "20 tropas")) {
        int total = 0;
        for (int i = 0; i < tamanho; i++)
            total += mapa[i].tropas;
        return (total > 20) ? 1 : 0;
    }

    /* Missões 4 e 5: lógica base — verificação futura por turno consecutivo */
    /* Retorna 0 por padrão (verificação de turnos consecutivos requer estado extra) */
    return 0;
}

/* ================================================================
   exibirMissao
   Exibe a missão do jogador. A string é passada por valor
   (ponteiro para char constante — somente leitura).
   ================================================================ */
void exibirMissao(char *missao) {
    printf("  Missao: [ %s ]\n", missao);
}

/* ================================================================
   atacar
   Simula uma batalha com rolagem de dados (1–6).
   Se atacante vencer: conquista o território transferindo cor
   e metade das tropas. Caso contrário: perde 1 tropa.
   ================================================================ */
void atacar(Territorio *atacante, Territorio *defensor) {
    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("\n  [BATALHA] %s (dado: %d) vs %s (dado: %d)\n",
           atacante->nome, dado_atacante,
           defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {
        printf("  >> Atacante VENCEU!\n");

        /* Transfere a cor (conquista o território) */
        strcpy(defensor->cor, atacante->cor);

        /* Move metade das tropas do atacante para o território conquistado */
        int metade = atacante->tropas / 2;
        defensor->tropas = metade;
        atacante->tropas -= metade;

    } else {
        printf("  >> Defensor resistiu!\n");
        atacante->tropas--;  /* Atacante perde 1 tropa */
    }

    printf("  Tropas: %s=%d | %s=%d\n",
           atacante->nome, atacante->tropas,
           defensor->nome, defensor->tropas);
}

/* ================================================================
   exibirMapa
   Exibe o estado atual de todos os territórios.
   ================================================================ */
void exibirMapa(Territorio *mapa, int tamanho) {
    printf("\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("  [%d] %-20s | Cor: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

/* ================================================================
   liberarMemoria
   Libera toda a memória alocada dinamicamente:
     - O array de territórios
     - Cada string de missão individualmente
     - O array de ponteiros de missões
   ================================================================ */
void liberarMemoria(Territorio *mapa, char **missoes, int numJogadores) {
    /* Libera cada missão individualmente */
    for (int j = 0; j < numJogadores; j++) {
        free(missoes[j]);
    }
    /* Libera o vetor de ponteiros de missões */
    free(missoes);
    /* Libera o mapa de territórios */
    free(mapa);
}
