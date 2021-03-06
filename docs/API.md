# Documentação da API

## Criar usuário
### [POST] /users
- Request:
  - Body:
    - `name`: String. Nome do usuário.
- Response:
  - Status: 201.
  - Body:
    - `id`: Int. ID do usuário.
    - `name`: String. Nome do usuário.

## Requisitar usuário
### [GET] /users/{id}
- Request:
  - URL:
    - `id`: Int. ID da imagem retornada quando criada.
- Response:
  - Status: 200.
  - Body:
    - `id`: Int. ID do usuário.
    - `name`: String. Nome do usuário.
### [GET] /users
- Request:
  - QUERY:
    - `name`: String. Nome do usuário.
- Response:
  - Status: 200.
  - Body:
    - `id`: Int. ID do usuário.
    - `name`: String. Nome do usuário.

## Criar imagem no servidor
### [POST] /images
- Request:
  - Body:
    - `user`: Int. ID do usuário.
    - `algo`: ["CGNE", "CGNR"]. Algoritmo escolhido.
    - `quality`: [0, 1, 2]. Qualidade escolhida (0 = baixo, 1 = médio, 2 = alto).
- Response:
  - Status: 201.
  - Body:
    - `id`: Int. ID da imagem.
    - `algo`: String. Algoritmo escolhido ["CGNE", "CGNR"].
    - `quality`: Int. Qualidade escolhida [0, 1, 2].
    - `status`: Int. Status do processamento [0 = "ENQUEUED", 1 = "START_PROCESSING", 2 = "FINISH_PROCESSING", 3 = "FAILED_PROCESSING"].
    - `start_date`: String. Data de início de processamento (formato ISO). *(Nulo se a imagem ainda não foi processada)*.
    - `end_date`: String. Data de fim da imagem (formato ISO). *(Nulo se a imagem ainda não foi processada)*.
    - `data`: String. UID da imagem para ser requisitada. *(Nulo se a imagem ainda não foi processada)*.
    - `height`: Int. Altura da imagem. (60 por padrão).
    - `width`: Int. Largura da imagem. (60 por padrão).
    - `iterations`: Int. Número de iterações. *(Nulo se a imagem ainda não foi processada)*.
    - `time`: Float. Tempo de processamento em segundos. *(Nulo se a imagem ainda não foi processada)*.
    - `user`: Int. ID do usuário.

## Enviar imagem para processamento
### [POST] /images/{id}/signal
- Request:
  - URL:
    - `id`: Int. ID da imagem retornada quando criada.
  - Payload:
    - *Arquivo CSV em text/csv*.

## Requisitar imagem em formato PNG
### [GET] /images/{filename}
- Request:
  - URL:
    - `filename`: String. Nome da imagem retornada quando termina de processar.
  - QUERY:
    - `download`: Bool. Opcional. Se "true", adiciona cabeçalho Content-Disposition: attachment na resposta.
- Response:
  - Status: 200.
  - *Imagem em formato PNG*.

## Requisitar imagens de um usuário
### [GET] /users/{id}/images
- Request:
  - URL:
    - `id`: String. ID do usuário.
- Response:
  - Body:
    - Lista:
      - `id`: Int. ID da imagem.
      - `algo`: String. Algoritmo escolhido ["CGNE", "CGNR"].
      - `quality`: Int. Qualidade escolhida [0, 1, 2].
      - `start_date`: String. Data de início de processamento (formato ISO). *(Nulo se a imagem ainda não foi processada)*.
      - `status`: Int. Status do processamento [0 = "ENQUEUED", 1 = "START_PROCESSING", 2 = "FINISH_PROCESSING", 3 = "FAILED_PROCESSING"].
      - `end_date`: String. Data de fim da imagem (formato ISO). *(Nulo se a imagem ainda não foi processada)*.
      - `data`: String. UID da imagem para ser requisitada. *(Nulo se a imagem ainda não foi processada)*.
      - `height`: Int. Altura da imagem. (60 por padrão).
      - `width`: Int. Largura da imagem. (60 por padrão).
      - `iterations`: Int. Número de iterações. *(Nulo se a imagem ainda não foi processada)*.
      - `time`: Float. Tempo de processamento em segundos. *(Nulo se a imagem ainda não foi processada)*.
      - `user`: Int. ID do usuário.

## Receber notificações por WS
### [WS] /users/{id}/ws
- Request:
  - URL:
    - `id`: String. ID do usuário.
- Events:
  - `ENQUEUED`: JSON. Notifica que o processamento da imagem foi adicionado na fila de processamento.
    - payload:
      - `type`: String. Nome do evento
      - `image`: Image. Igual ao Body das outras rotas de imagem.
  - `START_PROCESSING`: JSON. Notifica que o processamento da imagem foi iniciado.
    - payload:
      - `type`: String. Nome do evento
      - `image`: Image. Igual ao Body das outras rotas de imagem.
  - `FINISH_PROCESSING`: JSON. Notifica que o processamento da imagem foi finalizado.
    - payload:
      - `type`: String. Nome do evento
      - `image`: Image. Igual ao Body das outras rotas de imagem.
  - `FAILED_PROCESSING`: JSON. Notifica que o processamento da imagem falhou.
    - payload:
      - `type`: String. Nome do evento
      - `image`: Image. Igual ao Body das outras rotas de imagem.



