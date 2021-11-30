# ultrassom
Trabalho de CSM30 - Desenvolvimento Integrado de Sistemas

A pasta "data" vocês têm que baixar no PC de vocês lá do moodle, muito pesado...

# Instalação

### Dependências:
  - CMake
  - Conan

```sh
# Para fazer funcionar o Magick++, ele precisa usar a libstdc++11 ao invés de libstdc++
conan profile update settings.compiler.libcxx=libstdc++11 default
mkdir build && cd build
conan install ..
# Especifica o gerador (Unix Makefiles ou Visual Studio 16)
cmake .. -G "Unix Makefiles"
# Ou utilizar scripts/build caso deseje
cmake --build .
cp bin/ultrassom ..
```

Adicionar no includePath do c_cpp_properties.json caso usar VSCode

```json
{
...
  "includePath": [
      ...
      "~/.conan/data/**"
  ],
...
}
```

# TODO
 - [ ] Fazer o parser ler números no formato X,XXXE-XX
 - [ ] Fazer o parser ler de blob ou base64 ou o que a gente usar para fazer o upload da imagem
 - [x] Tentar paralelisar o parser
 - [x] Gerar um relatório com custo de memória e tempo
 - [ ] Fazer servidor
 - [ ] Testar com a nova matriz modelo H

# API
## Cadastrar imagem POST /images
### Request:
  - `name`: String, Identificação do usuário;
  - `algo`: String, ["CGNE", "CGNR"], Identificação do algoritmo utilizado;
  - `data`: Vetor, vetor de dados de ultrassom (originalmente .csv);
  - `size`:
    - `height`: Inteiro, altura da imagem;
    - `width`: Inteiro, largura da imagem;

### Response:
  - `image_id`: Inteiro

## Retornar imagens de um usuário GET /users/:name/images

### Response:
  - `images`: Lista:
    - `name`: String, Identificação do usuário;
    - `algo`: String, ["CGNE", "CGNR"], Identificação do algoritmo utilizado;
    - `start_date`: String, Data e hora do início da reconstrução
    - `end_date`: Vetor, Data e hora do término da reconstrução;
    - `img`: Blob, dados de ultrassom processado;
    - `size`:
      - `height`: Inteiro, altura da imagem;
      - `width`: Inteiro, largura da imagem;
    - `iterations`: Inteiro, numero de iterações executadas;
    - `reconstruction_time`: Timestamp, tempo de reconstrução.
