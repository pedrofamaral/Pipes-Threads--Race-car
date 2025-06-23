#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>     // sleep
#include <time.h>       // time, srand, rand

// Cores ANSI (funciona em terminais compatíveis)
#define CYAN    "\x1b[36m"
#define BLUE    "\x1b[34m"
#define YELLOW  "\x1b[33m"
#define GREEN   "\x1b[32m"
#define RESET   "\x1b[0m"

// Estrutura de status enviada pelo carro
typedef struct {
    int volta;
    int combustivel;
    int pneus;
    int velocidade;
} status_t;

// Variáveis globais para comunicação
status_t status_global;
char comando_global[16];
int status_pronto = 0;
int comando_pronto = 0;

// Mutex e variáveis de condição
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_status = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_comando = PTHREAD_COND_INITIALIZER;

// Thread do carro
void* carro_corrida(void* arg) {
    const int voltas = 10;
    int combustivel = 100;
    int pneus = 100;
    int velocidade = 200;  // km/h média

    for (int volta = 1; volta <= voltas; volta++) {
        printf(CYAN "\n==================== VOLTA %d ====================\n" RESET, volta);

        // Simula desgaste
        combustivel -= (rand() % 6 + 5);   // 5 a 10%
        pneus      -= (rand() % 6 + 3);     // 3 a 8%

        // Prepara status
        status_t s = { volta, combustivel, pneus, velocidade };

        printf(BLUE "[Carro] 🚗 Enviando status: Combustível = %d%%, Pneus = %d%%, Velocidade = %d km/h\n" RESET,
               s.combustivel, s.pneus, s.velocidade);

        // Envia status para a equipe
        pthread_mutex_lock(&mtx);
        // Espera o slot ficar livre (status_pronto == 0)
        while (status_pronto)
            pthread_cond_wait(&cond_status, &mtx);
        status_global = s;
        status_pronto = 1;
        pthread_cond_signal(&cond_status);
        // Aguarda comando
        while (!comando_pronto)
            pthread_cond_wait(&cond_comando, &mtx);
        // Lê comando
        char cmd[16];
        strcpy(cmd, comando_global);
        comando_pronto = 0;
        pthread_cond_signal(&cond_comando);
        pthread_mutex_unlock(&mtx);

        // Executa ação conforme comando
        if (strcmp(cmd, "box") == 0) {
            printf(YELLOW "[Carro] 🏁 Entrando nos BOXES para reabastecimento e troca de pneus...\n" RESET);
            sleep(2);
            combustivel = 100;
            pneus = 100;
            printf(YELLOW "[Carro] ✅ Pronto para voltar à pista!\n" RESET);
        }
        else if (strcmp(cmd, "reduzir") == 0) {
            printf(YELLOW "[Carro] ⚠️ Reduzindo velocidade.\n" RESET);
            velocidade -= 20;
        }
        else if (strcmp(cmd, "acelerar") == 0) {
            printf(YELLOW "[Carro] 🟢 Acelerando ao máximo!\n" RESET);
            velocidade += 20;
        }
        else {
            printf(YELLOW "[Carro] ➡️ Mantendo ritmo de corrida.\n" RESET);
        }

        // Simula tempo da volta
        sleep(1);
    }

    printf(CYAN "\n🏁 [Carro] Corrida finalizada! 🏆\n" RESET);
    return NULL;
}

// Thread da equipe técnica
void* equipe_tecnica(void* arg) {
    while (1) {
        // Aguarda status do carro
        pthread_mutex_lock(&mtx);
        while (!status_pronto)
            pthread_cond_wait(&cond_status, &mtx);
        // Copia status
        status_t s = status_global;
        status_pronto = 0;
        pthread_cond_signal(&cond_status);

        // Decide comando
        char cmd[16];
        if (s.combustivel < 50 || s.pneus < 45) {
            strcpy(cmd, "box");
        }
        else if (s.velocidade > 220) {
            strcpy(cmd, "reduzir");
        }
        else if (s.velocidade < 180) {
            strcpy(cmd, "acelerar");
        }
        else {
            strcpy(cmd, "manter");
        }

        printf(GREEN "[Equipe] 📡 Recebido: Volta %d | Combustível = %d%% | Pneus = %d%% | Velocidade = %d km/h\n" RESET,
               s.volta, s.combustivel, s.pneus, s.velocidade);
        printf(GREEN "[Equipe] 🗣️ Comando: %s\n" RESET, cmd);

        // Envia comando de volta
        strcpy(comando_global, cmd);
        comando_pronto = 1;
        pthread_cond_signal(&cond_comando);
        pthread_mutex_unlock(&mtx);

        if (s.volta == 10)
            break;
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t thread_carro, thread_equipe;

    // Cria threads
    pthread_create(&thread_carro, NULL, carro_corrida, NULL);
    pthread_create(&thread_equipe, NULL, equipe_tecnica, NULL);

    // Aguarda fim
    pthread_join(thread_carro, NULL);
    pthread_join(thread_equipe, NULL);

    // Limpeza
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond_status);
    pthread_cond_destroy(&cond_comando);

    return 0;
}
