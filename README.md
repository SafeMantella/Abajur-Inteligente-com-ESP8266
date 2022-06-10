# Resumo
Projeto para controlar um abajur com ESP8266 remotamente.
O abajur pode ser controlado via assitente pessoal da Amazon, a Alexa.

# Como foi feito?
A principal bilbioteca utilizada foi a FauxmoESP(https://github.com/vintlabs/fauxmoESP), que permite que seu ESP8266 ou ESP32 seja reconhecido pela alexa como uma lâmpada Phillips Hue. A partir daí é possível monitorar os valores que a Alexa envia para o ESP e processá-los para regular estado(Ligado ou Desligado), Brilho(de 0 a 100%), ou cor(Não Implementado pois a lâmpada usada é de LED Branco).

# Como faço para fazer o meu abajur inteligente?
Isso é simples! Basta conectar o pino positivo do seu LED no pino 2 do ESP32/8266 e adicionar as bibliotecas para programar o ESP32/8266 pela IDE do Arduino. São elas:
  - O suporte para placas da Espressif(ESP32, ESP8266 e +): https://arduino-esp8266.readthedocs.io/en/latest/installing.html
  - A biblioteca FauxmoESP: https://github.com/vintlabs/fauxmoESP
  - Baixe o código deste repositório e adicione o nome da sua rede Wi-Fi e sua senha nas linhas 5 e 6.
  - Se necessário, mude o pino do seu LED no código na linha 11.
  - Aproveite!
