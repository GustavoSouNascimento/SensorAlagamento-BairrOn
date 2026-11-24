# BairrOn IoT - Sensor de nível de alagamento

Neste README será documentado tudo sobre o protótipo de IoT do **BairrOn**, o mapa colaborativo em que moradores registram problemas do bairro. Um sensor ultrassônico ligado a um **Arduino Uno** mede o nível da água em um ponto de alagamento e classifica a situação em quatro níveis (**Normal, Atenção, Alerta e Crítico**), sinalizados por **quatro LEDs** em cores de semáforo e por um **buzzer** com um padrão de som para cada nível. Como **proposta conceitual**, o Uno seria substituído por um **ESP32**, que enviaria a leitura à API do BairrOn para registrar o alagamento no mapa sem depender de um morador.

## 1. Integrantes

| RM | Nome | Turma |
|---|---|---|
| RM566948 | Murillo Kenzo | 2TDSPA |
| RM566990 | Kauan Prado | 2TDSPA |
| RM567593 | Cauã Rocha | 2TDSPR |
| RM567134 | Gustavo Souza | 2TDSPR |

## 2. Problema e proposta CitizenTech

**Cidadão beneficiado:** Moradores de áreas sujeitas a alagamento, em geral bairros de baixada e periféricos, que hoje só descobrem o problema quando a água já invadiu a rua ou a casa.

**Problema:** O alagamento é percebido tarde e só entra em um registro se alguém parar para avisar. Não há monitoramento contínuo nos pontos críticos, e a informação não chega a quem precisa decidir (moradores, comerciantes, Defesa Civil).

**Solução:** Um nó sensor fixo em um ponto crítico mede continuamente a distância até a água e a traduz em quatro níveis. O aviso é imediato e local (luz e som), sem depender de internet. Na evolução proposta (conceitual), a mesma leitura viraria, pela API, uma ocorrência de *Alagamento* com prioridade calculada, que os moradores podem confirmar.

**ODS:** 11 (cidades e comunidades sustentáveis), 13 (ação contra a mudança global do clima) e 9 (indústria, inovação e infraestrutura).

## 3. O protótipo

Montado e simulado no **Tinkercad**, utilizando **Arduino Uno R3**.

**Projeto no Tinkercad:** https://www.tinkercad.com/things/fG4EetYZejR-fabulous-snaget-trug/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=EyVnKYekcX121giSY8GRWuxpjv_VPIO0CPsgwOduRq4

### Como funciona

1. O Arduino envia um pulso no **TRIG** do HC-SR04 (pino D8) e mede, com `pulseIn`, o tempo do eco no pino **ECHO** (D7).
2. Converte o tempo em distância: `cm = 0,01723 × tempo do eco (µs)`. O valor é guardado em `int`, então é truncado em centímetros.
3. Compara a distância com três limites e define **um** nível: apenas o LED daquele nível fica aceso.
4. Aciona o buzzer conforme o nível e imprime no Monitor Serial (9600 baud) a distância e o nível.

O sensor fica apontado para baixo, em um ponto alto sobre o local de alagamento (por exemplo, um viaduto). **Quanto mais a água sobe, menor a distância medida e mais grave o nível.**

### Níveis, LEDs e som

| Distância medida | Nível | LED (pino) | Som no buzzer (D6) |
|---|---|---|---|
| 336 cm ou mais | Normal | verde (D2) | silêncio |
| 224 a 335 cm | Atenção | amarelo (D3) | **um** bipe de 440 Hz por 500 ms ao entrar na faixa, não repete enquanto a distância continuar nela |
| 112 a 223 cm | Alerta | laranja (D4) | bipes de 440 Hz e 150 ms, um a cada 1 s |
| menos de 112 cm | Crítico | vermelho (D5) | dois tons (550 Hz e 650 Hz, 150 ms cada), repetidos a cada cerca de 1,15 s |

As faixas têm 112 cm de largura cada. Os limites são uma escala do protótipo e **devem ser calibrados** para a altura real de instalação.

### Ligações

| Componente | Pino do Arduino Uno |
|---|---|
| LED verde, nível Normal | D2 (com resistor em série) |
| LED amarelo, nível Atenção | D3 (com resistor em série) |
| LED laranja, nível Alerta | D4 (com resistor em série) |
| LED vermelho, nível Crítico | D5 (com resistor em série) |
| Buzzer piezo (+) | D6 |
| HC-SR04 ECHO | D7 |
| HC-SR04 TRIG | D8 |
| HC-SR04 VCC | 5V |
| HC-SR04 GND, buzzer (−), catodos dos LEDs | GND |

### Materiais

| Qtde | Componente |
|---|---|
| 1 | Arduino Uno R3 e cabo USB |
| 1 | Sensor ultrassônico HC-SR04 |
| 4 | LEDs: verde, amarelo, laranja e vermelho |
| 4 | Resistores em série com os LEDs (~220 Ω) |
| 1 | Buzzer piezo |
| 1 | Protoboard e fios de ligação |

## 4. Componentes e fluxo de comunicação em rede

<img width="1158" height="381" alt="Arquitetura" src="https://github.com/user-attachments/assets/bb15022c-0f0b-4e9d-8f5d-ec7208d1898d" />

**O que está implementado e o que é proposta.** O protótipo (sensor, Arduino Uno, LEDs, buzzer e Monitor Serial) é a parte funcional. Toda a parte de rede, ESP32 e API descrita nesta seção é uma **proposta conceitual**. O código, os testes, as evidências e a demonstração do repositório são do protótipo atual, que não usa rede.

### Substituição do Arduino Uno por um ESP32

O Uno **não tem rede**. Para falar com a API, a placa é substituída por um **ESP32**, que tem **Wi-Fi 802.11 b/g/n (2,4 GHz) integrado**, mais memória e processamento, e pode ser programado na IDE do Arduino. A lógica do protótipo (ler, classificar, acionar LEDs e buzzer) é aproveitada, e acrescenta-se o envio pela rede.

| Item | Arduino Uno (protótipo) | ESP32 (proposta) |
|---|---|---|
| Conectividade | nenhuma | Wi-Fi integrado |
| Tensão dos pinos | 5 V | **3,3 V** |
| Saída ECHO do HC-SR04 | ligada direto ao pino | **divisor de tensão** (por exemplo, 1 kΩ e 2 kΩ), pois o ECHO entrega 5 V |
| Pinos | D2 a D8 | GPIOs equivalentes, a definir na montagem (por exemplo, LEDs em 25, 26, 27 e 33, TRIG em 5, ECHO em 18 e buzzer em 32) |
| Som | `tone()` | `tone()` |

### Dispositivos e papéis

| Dispositivo | Função |
|---|---|
| HC-SR04 | Mede a distância até a água |
| ESP32 | Lê o sensor, define o nível, aciona o alerta local e envia os dados |
| LEDs e buzzer | Alerta local imediato, sem rede |
| Roteador / ponto de acesso | Conecta o ESP32 à rede local e à internet |
| Servidor | Executa a API do BairrOn (Spring Boot) e o banco H2 |
| Celular com o app BairrOn | Mostra o mapa e as prioridades |

### Meios de comunicação

- **Sensor → placa:** sinais digitais TRIG e ECHO, por fio.
- **ESP32 → rede:** Wi-Fi 2,4 GHz (IEEE 802.11 b/g/n).
- **Rede local → servidor:** internet pelo provedor.
- **Servidor → app:** HTTPS.

### Componentes de rede necessários

- **Roteador / ponto de acesso Wi-Fi**, com **DHCP** (IP do ESP32) e **NAT** (saída à internet).
- **DNS** para resolver o endereço da API.
- Onde não houver Wi-Fi, um **roteador 4G/LTE** como alternativa.

### Caminho dos dados, da água ao usuário

1. A água sobe e a distância medida pelo HC-SR04 diminui.
2. O microcontrolador converte o eco em centímetros e define o nível.
3. **Local e imediato:** LEDs e buzzer alertam quem está perto.
4. **Rede (proposta):** quando o nível muda, o ESP32 monta uma mensagem **JSON** e a envia por **HTTPS** ao roteador, que a encaminha pela internet até a API.
5. **API do BairrOn:** o ESP32 faz login como usuário de serviço (`POST /auth/login`, que devolve um token JWT de 60 minutos) e usa endpoints que a API já tem.:
   - nível Alerta ou Crítico: `POST /ocorrencias` cria uma ocorrência de *Alagamento* na posição fixa do sensor (ou, se já houver uma ativa, `POST /ocorrencias/{id}/confirmacoes` com `AINDA_EXISTE`)
   - volta ao Normal: `POST /ocorrencias/{id}/confirmacoes` com `RESOLVIDO`.
6. A API calcula a **prioridade** (gravidade da categoria vezes 10, mais 5 por confirmação de "ainda existe" e menos 5 por "resolvido") e o app mostra o ponto no mapa e na lista de prioridades do bairro.
7. Moradores e Defesa Civil veem o alerta e podem confirmar se a situação continua.

Mensagem de exemplo, no formato que o `POST /ocorrencias` aceita:

```json
{
  "titulo": "Alagamento detectado pelo sensor BAIRRON-01",
  "descricao": "Nível crítico: água a 53 cm do sensor.",
  "latitude": -23.5589,
  "longitude": -46.5976,
  "categoriaId": 1,
  "bairroId": 1
}
```

### Segurança e confiabilidade

- **HTTPS** entre ESP32 e API, e **credencial própria do dispositivo** (usuário de serviço). A senha ficaria em um arquivo de configuração fora do repositório.
- **Envio só quando o nível muda**, mais uma mensagem periódica de "estou vivo" (intervalo configurável), para reduzir tráfego e consumo.
- **Fila local** para reenviar mudanças se a rede cair, o alerta local não depende dela.
- **Renovação do token:** em caso de 401, novo login.
- **Escala:** em áreas sem Wi-Fi, o mesmo desenho usaria um enlace de longo alcance e baixo consumo (por exemplo, LoRaWAN) com um *gateway*. A API e o app não mudam.

## 5. Justificativa das escolhas técnicas

| Escolha | Por que |
|---|---|
| **HC-SR04 (ultrassônico)** | Mede o nível **sem contato** com a água, então não corrói nem acumula sujeira como sensores de eletrodo, e é barato e fácil de ligar. Alcance de 2 a 400 cm segundo o fabricante. |
| **Sensor apontado para baixo, em ponto alto** | Fica fora d'água e protegido. A gravidade cresce quando a distância diminui. |
| **4 níveis, 4 LEDs em cores de semáforo** | Linguagem conhecida por qualquer pessoa. Cada nível tem um LED próprio, então a posição do LED também informa o nível, o que ajuda quem distingue mal as cores. |
| **Buzzer com um padrão por nível** | O alerta chega também a quem não está olhando para o equipamento. Atenção toca uma vez, Alerta repete a cada segundo e Crítico usa dois tons mais agudos, o que distingue a urgência pelo som. |
| **Alerta local** | A informação chega a quem passa pelo local, mesmo sem celular ou internet. |
| **Monitor Serial** | Permite acompanhar e registrar as leituras para os testes. |
| **ESP32 na evolução** | Tem Wi-Fi integrado, necessário para falar com a API, e mantém a programação no ambiente do Arduino. |
| **Medida pelo tempo de eco** | A conversão `0,01723 cm/µs` equivale a uma velocidade do som de cerca de 344,6 m/s (aproximadamente 21 °C), dividida por dois por causa da ida e volta. |

## 6. Como executar

### No Tinkercad 

1. Abra o projeto pelo link no início deste README, ou crie um circuito novo com os componentes da seção 2 e as ligações da tabela.
2. Em **Código**, escolha **Texto** e cole o conteúdo de `SensorAlagamentoBairrOn.ino`.
3. Abra o **Monitor Serial** e clique em **Iniciar simulação**.
4. Clique no sensor e arraste o obstáculo (a água) para perto e para longe. Acompanhe a distância, o LED aceso e o som.

### Em hardware real (Arduino Uno)

1. Monte o circuito conforme a tabela de ligações.
2. Na Arduino IDE, abra `SensorAlagamentoBairrOn.ino`, selecione **Arduino Uno** e a porta, e faça o upload.
3. Abra o Monitor Serial.

### Teste de Logica

No Tinkercad, cole `TesteLogica.ino`, mude o número da linha `cm = 300` para cada caso e observe o Monitor Serial.

| Distância (cm) | Valor no Serial |
|---|---|
| maior possível (400 ou +) | [LED Verde] Normal |
| 336 (limite) | [LED Verde] Normal |
| 300 | [LED Amarelo] Atenção |
| 224 (limite) | [LED Amarelo] Atenção |
| 150 | [LED Laranja] Alerta |
| 112 (limite) | [LED Laranja] Alerta |
| 100 | [LED Vermelho] Critico |
| 53 | [LED Vermelho] Critico |

Não há credenciais ou chaves neste repositório, porque o protótipo não usa nenhuma.
