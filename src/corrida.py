import threading
import queue
import time
import random

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
        print(f"\n[Carro] Iniciando volta {volta}")
        
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
        status_queue.put(status)

        # Aguarda comando da equipe
        comando = command_queue.get()
        if comando == 'box':
            print("[Carro] Entrando nos boxes para reabastecer e trocar pneus.")
            time.sleep(2)
            combustivel = 100
            pneus = 100
        elif comando == 'reduzir':
            print("[Carro] Reduzindo velocidade.")
            velocidade -= 20
        elif comando == 'acelerar':
            print("[Carro] Acelerando ao máximo!")
            velocidade += 20
        else:
            print("[Carro] Continuando normalmente.")
        
        # Simula tempo da volta
        time.sleep(1)

    print("\n[Carro] Corrida finalizada!")

# Função da thread da equipe
def equipe_tecnica():
    while True:
        status = status_queue.get()
        print(f"[Equipe] Status recebido: Volta {status['volta']} | Combustível: {status['combustivel']}% | Pneus: {status['pneus']}%")

        # Decide o comando
        if status['combustivel'] < 30 or status['pneus'] < 25:
            comando = 'box'
        elif status['velocidade'] > 220:
            comando = 'reduzir'
        elif status['velocidade'] < 180:
            comando = 'acelerar'
        else:
            comando = 'manter'
        
        # Envia comando
        print(f"[Equipe] Enviando comando: {comando.upper()}")
        command_queue.put(comando)

        if status['volta'] == 10:
            break

# Criando as threads
thread_carro = threading.Thread(target=carro_corrida)
thread_equipe = threading.Thread(target=equipe_tecnica)

# Iniciando as threads
thread_carro.start()
thread_equipe.start()

# Aguardando finalização
thread_carro.join()
thread_equipe.join()
