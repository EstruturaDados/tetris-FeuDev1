#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ---------------------------
// Estrutura de uma peça
// ---------------------------
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

// ---------------------------
// Estrutura da fila circular
// ---------------------------
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// ---------------------------
// Estrutura da pilha
// ---------------------------
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------------------------
// Funções auxiliares
// ---------------------------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica estados
int filaCheia(Fila *f) { return f->tamanho == MAX_FILA; }
int filaVazia(Fila *f) { return f->tamanho == 0; }
int pilhaCheia(Pilha *p) { return p->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }

// ---------------------------
// Operações da fila
// ---------------------------
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = nova;
    f->tamanho++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->tamanho--;
    return p;
}

// ---------------------------
// Operações da pilha
// ---------------------------
void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->itens[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca pecaRemovida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("⚠️  Pilha vazia! Nenhuma peça para usar.\n");
        return pecaRemovida;
    }
    return p->itens[p->topo--];
}

// ---------------------------
// Geração automática de peça
// ---------------------------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------------------------
// Exibição do estado atual
// ---------------------------
void exibirFila(Fila *f) {
    printf("Fila de peças:\t");
    if (filaVazia(f)) {
        printf("[vazia]");
    } else {
        for (int i = 0; i < f->tamanho; i++) {
            int idx = (f->frente + i) % MAX_FILA;
            printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
        }
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
    }
    printf("\n");
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n------------------------------\n");
    exibirFila(f);
    exibirPilha(p);
    printf("------------------------------\n");
}

// ---------------------------
// Funções de troca
// ---------------------------
void trocarTopoComFrente(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("⚠️  Não há peças suficientes para trocar!\n");
        return;
    }

    int idxFrente = f->frente;
    Peca temp = f->itens[idxFrente];
    f->itens[idxFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("🔄 Troca realizada entre a frente da fila e o topo da pilha.\n");
}

void trocaMultiplas(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("⚠️  É necessário pelo menos 3 peças em cada estrutura para a troca múltipla!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % MAX_FILA;
        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("🔁 Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha!\n");
}

// ---------------------------
// Função principal
// ---------------------------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idGlobal = 0;
    int opcao;

    // Preenche fila inicial
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(idGlobal++));
    }

    printf("\n🧩 Bem-vindo ao Tetris Stack \n");

    do {
        exibirEstado(&fila, &pilha);

        printf("Opções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Reservar peça (fila -> pilha)\n");
        printf("3 - Usar peça reservada (desempilhar)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Troca múltipla (3 primeiras da fila com 3 da pilha)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1)
                    printf("🕹️  Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                enfileirar(&fila, gerarPeca(idGlobal++));
                break;
            }
            case 2: {
                if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("🎒 Peça [%c %d] reservada!\n", reservada.tipo, reservada.id);
                    enfileirar(&fila, gerarPeca(idGlobal++));
                }
                break;
            }
            case 3: {
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("🔧 Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
                break;
            }
            case 4:
                trocarTopoComFrente(&fila, &pilha);
                break;
            case 5:
                trocaMultiplas(&fila, &pilha);
                break;
            case 0:
                printf("\n🏁 Encerrando o jogo... Até a próxima!\n");
                break;
            default:
                printf("❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}