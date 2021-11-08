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
 - Fazer o parser ler números no formato X,XXXE-XX
 - Tentar paralelisar o parser
 - Gerar um relatório com custo de memória e tempo
 - Gerar uma saída mais profissional

