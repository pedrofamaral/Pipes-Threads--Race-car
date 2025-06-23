import threading
import queue
import time
import random
from colorama import init, Fore, Style

# Inicializa a colorama
init(autoreset=True)

# Queues de comunicação
status_queue = queue.Queue()
command_queue = queue.Queue()

# Função da thread do carro
def carro_corrida():
    voltas = 10
    combustivel = 100
    pneus = 100
    velocidade = 200  # km/h média

    for volta in range(1, voltas + 1):
        print(Fore.CYAN + f"\n==================== VOLTA {volta} ====================")

        # Simula desgaste
        combustivel -= random.randint(5, 10)
        pneus -= random.randint(3, 8)

        # Envia status para equipe
        status = {
            'volta': volta,
            'combustivel': combustivel,
            'pneus': pneus,
            'velocidade': velocidade
        }

        print(Fore.BLUE + f"[Carro] 🚗 Enviando status: Combustível = {combustivel}%, Pneus = {pneus}%, Velocidade = {velocidade} km/h")
        status_queue.put(status)

        # Aguarda comando da equipe
        comando = command_queue.get()

        if comando == 'box':
            print(Fore.YELLOW + "[Carro] 🏁 Entrando nos BOXES para reabastecimento e troca de pneus...")
            time.sleep(2)
            combustivel = 100
            pneus = 100
            print(Fore.YELLOW + "[Carro] ✅ Pronto para voltar à pista!")
        elif comando == 'reduzir':
            print(Fore.YELLOW + "[Carro] ⚠️ Reduzindo velocidade.")
            velocidade -= 20
        elif comando == 'acelerar':
            print(Fore.YELLOW + "[Carro] 🟢 Acelerando ao máximo!")
            velocidade += 20
        else:
            print(Fore.YELLOW + "[Carro] ➡️ Mantendo ritmo de corrida.")

        # Simula tempo da volta
        time.sleep(1)

    print(Fore.CYAN + "\n🏁 [Carro] Corrida finalizada! 🏆")

# Função da thread da equipe
def equipe_tecnica():
    while True:
        status = status_queue.get()

        print(Fore.GREEN + f"[Equipe] 📡 Recebido: Volta {status['volta']} | Combustível = {status['combustivel']}% | Pneus = {status['pneus']}% | Velocidade = {status['velocidade']} km/h")

        # Lógica de decisão
        if status['combustivel'] < 50 or status['pneus'] < 45:
            comando = 'box'
        elif status['velocidade'] > 220:
            comando = 'reduzir'
        elif status['velocidade'] < 180:
            comando = 'acelerar'
        else:
            comando = 'manter'

        print(Fore.GREEN + f"[Equipe] 🗣️ Comando: {comando.upper()}")
        command_queue.put(comando)

        if status['volta'] == 10:
            break

# Criando e iniciando as threads
thread_carro = threading.Thread(target=carro_corrida)
thread_equipe = threading.Thread(target=equipe_tecnica)

thread_carro.start()
thread_equipe.start()

thread_carro.join()
thread_equipe.join()
