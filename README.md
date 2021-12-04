# ultrassom
Trabalho de CSM30 - Desenvolvimento Integrado de Sistemas

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
 - [x] Fazer o parser ler números no formato X,XXXE-XX
 - [x] Tentar paralelisar o parser
 - [x] Gerar um relatório com custo de memória e tempo
 - [x] Fazer servidor
 - [x] Testar com a nova matriz modelo H
 - [x] Trocar cout por logs do oatpp
 - [x] Otimizar algoritmos mantendo em cache a transposta
 - [x] Rotas do usuário
 - [x] Terminar campos de imagens
 - [x] Servir as imagens
 - [x] Enum qualidade de imagem
 - [ ] Sistema de notificações via ws
 - [x] Atualizar documentação

## API
A documentação para API REST pode ser encontrada em [docs/API.md](docs/API.md)