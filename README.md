# Pipes-Threads--Race-car

# Simulação de Corrida com Comunicação Carro-Equipe via Threads

Este projeto é uma simulação de corrida de carros com foco em conceitos de **Sistemas Operacionais**, utilizando **threads** e **queues** para demonstrar **comunicação concorrente entre entidades independentes**: o carro e a equipe técnica.

## 🚀 Objetivo

Simular o comportamento de um carro de corrida durante 10 voltas, com troca contínua de informações entre o carro e sua equipe técnica. A simulação enfatiza:

- Comunicação entre threads usando `queue.Queue`
- Compartilhamento e processamento de dados em tempo real
- Lógica de decisão da equipe com base no status do carro
- Conceitos de concorrência e controle de fluxo

## 🧠 Como funciona

- O **carro** é representado por uma thread que executa 10 voltas.
- A cada volta, ele envia seu status (combustível, pneus, velocidade) para a equipe técnica.
- A **equipe técnica**, em outra thread, analisa o status e decide:
  - Manter
  - Reduzir velocidade
  - Acelerar
  - Chamar o carro para os boxes
- Toda a comunicação é feita por duas filas (queues):
  - `status_queue`: Carro → Equipe
  - `command_queue`: Equipe → Carro

## 🛠️ Execução

1. Abra a pasta `src` no terminal.
2. Execute o código em alguma IDE compativel com a linguagem C.
