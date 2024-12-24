![](https://repobeats.axiom.co/api/embed/c652bc89f0bf0ea8aee6e41326f7d0a0c8b78017.svg)

# Bash 0.5
Esse projeto é um projeto de estudos do funcionamento da bash do linux e do seu sistema de arquivos (EXT4).
O projeto será todo desenvolvido na linguagem C visando a sua usabilidade em ambiente windows. 
Algumas funcionalidades podem não ser possíveis em ambiente linux.

## Estrutura
Atualmente o projeto está estruturado em duas pastas principais:
- **lib**: Onde ficarão armazenadas as estruturas, divididas por arquivo.
- **functions**: Onde ficarão as funções de manipulação das estruturas, divididas por arquivo. 
<br>
Além disso conta também com um arquivo main.c separado das pastas lib e functions. Este então será o local de execução padrão do projeto.

## Bibliotecas
Neste projeto será explorado o uso das seguintes bibliotecas:
- **stdio.h**: Biblioteca padrão do C.
- **stdlib.h**: Biblioteca padrão do C.
- **stdint.h**: Para a utilização das variáveis uint_t.
- **string.h**: Para a manipulação de strings.
- **sys/stat.h**: Para a conversação com o sistem.
- **conio.h**: Para melhor leitura de entradas.
- **ctype.h**: Para formatação das entradas.

## Funcionalidades
- `mkdir nome_diretorio`: Cria uma pasta com o nome especificado.
- `cd /`: Retorna para o diretório raíz.
- `cd ..`: Retorna para o diretório anterior.
- `cd nome_diretorio`: Entra no diretorio especificado.
- `ls`: Lista os componentes da pasta atual.
- `cat nome_arquivo`: Mostra as informações gravadas no arquivo especificado.
- `cat > nome_arquivo`: Altera o arquivo especificado. Caso não exista, cria um arquivo com o nome informado. (Apertar `CTRL+D` para salvar).
- `rm nome_arquivo`: Remove o arquivo especificado.
- `rmdir nome_diretorio`: Remove o diretório especificado.
- `mv nome_arquivo`: Renomeia o arquivo ou pasta especificado.
- `run`: Roda um script padrão realizando algumas funções.
- `run script.sh`: Roda um arquivo .sh que vai realizar as funções presentes no arquivo. (Apenas arquivos .sh são válidos).
- `clear` ou `cls`: Limpa a tela do terminal.
- `exit`: Finaliza o programa.

## Como executar
Por conta do projeto ter sido desenvolvido primáriamente no `Visual Studio Code`, existem algumas configurações a serem realizadas antes de sua execução. 
1) No VSCode baixe a extensão `Compile Run`.
2) Vá até as configurações da extensão e pesquise pela seguinte propriedade:
  ```bash
    @ext:danielpinto8zz6.c-cpp-compile-run C-cpp-compile-run: Output-location 
  ```
3) Apague o campo caso haja algo (por padrão é comum vir como output). Na propriedade, coloque `..`
4) (Opcional) Para melhor exepriência é recomendável que deixe marcada a opção: 
  ```bash
    @ext:danielpinto8zz6.c-cpp-compile-run C-cpp-compile-run: Run-in-external-terminal
  ```
