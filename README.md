Sistema Inteligente de Monitoramento de Umidade e Evaporação do Solo
Integrantes
Gabriel Ambrosio Saraiva
(Adicionar demais integrantes)
Descrição do Projeto

Este projeto foi desenvolvido para a disciplina Disruptive Architectures: IoT, IOB & Generative AI, com o objetivo de demonstrar a aplicação de Internet das Coisas (IoT) no monitoramento agrícola.

A solução utiliza um ESP32 para coletar dados ambientais, calcular um índice de evaporação e auxiliar na tomada de decisão sobre a necessidade de irrigação do solo.

Os dados são transmitidos utilizando o protocolo MQTT, armazenados e processados através do Node-RED e visualizados em uma dashboard web.

Objetivo

Desenvolver um sistema capaz de:

Monitorar a umidade do solo;
Monitorar temperatura e umidade ambiente;
Calcular um índice de evaporação;
Informar a necessidade de irrigação;
Disponibilizar os dados localmente em um LCD;
Disponibilizar os dados remotamente através de dashboard.
Arquitetura da Solução
Potenciômetro (Solo)
          │
          │
DHT22 Base│
          │
          ▼
        ESP32
          ▲
          │
DHT22 Topo│
          │
          ▼
       Wi-Fi
          │
          ▼
    HiveMQ Cloud
          │
          ▼
      Node-RED
          │
          ▼
     Dashboard
Tecnologias Utilizadas
Hardware (Simulado no Wokwi)
ESP32 DevKit V4
2 Sensores DHT22
Potenciômetro (simulação da umidade do solo)
Display LCD 16x2 I2C
LED Verde
LED Vermelho
Botão de coleta
Software
Wokwi Simulator
Arduino IDE
MQTT
HiveMQ Cloud
Node-RED Dashboard
GitHub
Componentes do Sistema
ESP32

Responsável pela leitura dos sensores, processamento dos dados e envio das informações via MQTT.

Potenciômetro

Utilizado para simular o sensor de umidade do solo.

Faixa de operação:

0% = Solo extremamente seco
100% = Solo extremamente úmido
DHT22 Base

Responsável pela leitura da umidade próxima ao solo.

DHT22 Topo

Responsável pela leitura da temperatura ambiente e da umidade na região superior.

LCD 16x2

Exibe:

Estado do sistema;
Leitura dos sensores;
Nível de evaporação;
Recomendação de irrigação.
LED Verde

Indica coleta e envio de dados.

LED Vermelho

Indica condição crítica:

Solo seco;
Evaporação elevada.
Comunicação MQTT

O sistema utiliza três tópicos MQTT.

Tópico 1
agricola/solo

Exemplo:

{
  "solo": 65
}
Tópico 2
agricola/clima

Exemplo:

{
  "temp_topo": 31.6,
  "hum_base": 82.0,
  "hum_topo": 44.5
}
Tópico 3
agricola/analise

Exemplo:

{
  "evaporacao": 37.5,
  "nivel_evaporacao": "ALTA",
  "irrigacao": "SIM"
}
Lógica de Funcionamento
O usuário pressiona o botão de coleta.
O ESP32 realiza a leitura dos sensores.
O sistema calcula o índice de evaporação.
O LCD exibe os resultados.
Os LEDs indicam o estado do sistema.
Os dados são enviados para o HiveMQ Cloud.
O Node-RED recebe os dados.
A dashboard é atualizada automaticamente.
Critérios de Evaporação
Baixa
Evaporação < 15
Média
15 ≤ Evaporação < 30
Alta
Evaporação ≥ 30
Critérios de Irrigação

A irrigação é recomendada quando:

Solo < 40%

ou

Evaporação > 30%
Dashboard

A dashboard apresenta:

Leitura Atual
Umidade do Solo
Temperatura
Evaporação
Diagnóstico
Nível de Evaporação
Necessidade de Irrigação
Histórico
Histórico da Umidade do Solo
Histórico da Temperatura
Histórico da Evaporação
Demonstração
Cenário 1

Solo úmido:

Solo: 80%
Evaporação: Baixa
Irrigação: Não
Cenário 2

Solo seco:

Solo: 20%
Evaporação: Alta
Irrigação: Sim
Requisitos Atendidos
ESP32
Simulação no Wokwi
Duas entradas
Duas saídas
Interface LCD
Comunicação Wi-Fi
MQTT
Três tópicos MQTT documentados
Dashboard para monitoramento
Comunicação em tempo real
Repositório

Adicionar:

Código do ESP32
Arquivo diagram.json do Wokwi
Fluxo Node-RED
Capturas da Dashboard
Vídeo de demonstração
Disciplina

Disruptive Architectures: IoT, IOB & Generative AI

Global Solution
