#include <stdio.h>

#define TAM_TABULEIRO 10
#define TAM_HABILIDADE 5   // Tamanho das matrizes de habilidade (5x5)
#define TAM_NAVIO 3
#define AGUA 0
#define NAVIO 3
#define HABILIDADE 5

// ========================
// Função para imprimir o tabuleiro
// ========================
void imprimirTabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]) {
    printf("\n=== TABULEIRO BATALHA NAVAL ===\n\n");

    printf("   ");
    for (int c = 0; c < TAM_TABULEIRO; c++) {
        printf("%d ", c);
    }
    printf("\n");

    for (int i = 0; i < TAM_TABULEIRO; i++) {
        printf("%2d ", i);
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            int valor = tabuleiro[i][j];

            // Exibição diferenciada por tipo de célula
            if (valor == AGUA) printf(". ");
            else if (valor == NAVIO) printf("N ");
            else if (valor == HABILIDADE) printf("* ");
            else printf("? "); // fallback (caso apareça algo inesperado)
        }
        printf("\n");
    }

    printf("\nLegenda: . = Água | N = Navio | * = Área de habilidade\n");
}

// ========================
// Gera matriz em forma de CONE (5x5)
// ========================
void gerarCone(int cone[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            // Forma triangular (cone apontando para baixo)
            if (j >= (TAM_HABILIDADE / 2) - i && j <= (TAM_HABILIDADE / 2) + i)
                cone[i][j] = 1;
            else
                cone[i][j] = 0;
        }
    }
}

// ========================
// Gera matriz em forma de CRUZ (5x5)
// ========================
void gerarCruz(int cruz[TAM_HABILIDADE][TAM_HABILIDADE]) {
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            // Cruz com linha e coluna centrais ativas
            if (i == TAM_HABILIDADE / 2 || j == TAM_HABILIDADE / 2)
                cruz[i][j] = 1;
            else
                cruz[i][j] = 0;
        }
    }
}

// ========================
// Gera matriz em forma de OCTAEDRO (5x5)
// ========================
void gerarOctaedro(int octaedro[TAM_HABILIDADE][TAM_HABILIDADE]) {
    int centro = TAM_HABILIDADE / 2;
    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            // Distância de Manhattan (|dx| + |dy| <= raio)
            if (abs(i - centro) + abs(j - centro) <= centro)
                octaedro[i][j] = 1;
            else
                octaedro[i][j] = 0;
        }
    }
}

// ========================
// Aplica uma matriz de habilidade sobre o tabuleiro
// ========================
void aplicarHabilidade(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO],
                       int habilidade[TAM_HABILIDADE][TAM_HABILIDADE],
                       int origemLinha, int origemColuna) {
    int offset = TAM_HABILIDADE / 2; // deslocamento do centro

    for (int i = 0; i < TAM_HABILIDADE; i++) {
        for (int j = 0; j < TAM_HABILIDADE; j++) {
            if (habilidade[i][j] == 1) {
                int linha = origemLinha - offset + i;
                int coluna = origemColuna - offset + j;

                // Verifica se está dentro dos limites do tabuleiro
                if (linha >= 0 && linha < TAM_TABULEIRO &&
                    coluna >= 0 && coluna < TAM_TABULEIRO) {
                    // Marca o efeito apenas sobre a água (sem sobrescrever navios)
                    if (tabuleiro[linha][coluna] == AGUA)
                        tabuleiro[linha][coluna] = HABILIDADE;
                }
            }
        }
    }
}

// ========================
// Função principal
// ========================
int main() {
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    int cone[TAM_HABILIDADE][TAM_HABILIDADE];
    int cruz[TAM_HABILIDADE][TAM_HABILIDADE];
    int octaedro[TAM_HABILIDADE][TAM_HABILIDADE];

    // Inicializa o tabuleiro com água
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            tabuleiro[i][j] = AGUA;
        }
    }

    // === Posiciona alguns navios (como nos desafios anteriores) ===
    for (int i = 0; i < TAM_NAVIO; i++) tabuleiro[2][3 + i] = NAVIO; // horizontal
    for (int i = 0; i < TAM_NAVIO; i++) tabuleiro[5 + i][7] = NAVIO; // vertical
    for (int i = 0; i < TAM_NAVIO; i++) tabuleiro[i][i] = NAVIO;     // diagonal ↘
    for (int i = 0; i < TAM_NAVIO; i++) tabuleiro[3 + i][9 - i] = NAVIO; // diagonal ↙

    // === Gera as matrizes de habilidade ===
    gerarCone(cone);
    gerarCruz(cruz);
    gerarOctaedro(octaedro);

    // === Define pontos de origem para as habilidades ===
    int origemConeLinha = 1, origemConeColuna = 5;
    int origemCruzLinha = 6, origemCruzColuna = 2;
    int origemOctaLinha = 7, origemOctaColuna = 7;

    // === Aplica as habilidades ao tabuleiro ===
    aplicarHabilidade(tabuleiro, cone, origemConeLinha, origemConeColuna);
    aplicarHabilidade(tabuleiro, cruz, origemCruzLinha, origemCruzColuna);
    aplicarHabilidade(tabuleiro, octaedro, origemOctaLinha, origemOctaColuna);

    // === Exibe o resultado final ===
    imprimirTabuleiro(tabuleiro);

    return 0;
}
