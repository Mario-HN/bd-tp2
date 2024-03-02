# Alunos
Beatriz Emilly Silva Aguiar, 
Matheus Souza de Oliveira e 
Mario Haddad Neto


# Pré-requisitos

- É necessário descompactar o arquivo "artigo.csv" na pasta root.
- Tente executar os arquivos nas pastas que estão, para não precisar alterar paths. 


# Como Executar

Os comandos estão organizados para serem executados dentro do diretório root:

- Para rodar upload.cpp

  ```
  g++ B+Tree/IndicePrimario.cpp B+Tree/IndiceSecundario.cpp Implementacao/hash.cpp Implementacao/upload.cpp -o upload
  ```
  ```
  ./upload artigo.csv
  ```

- Para rodar findrec.cpp

  ```
  g++ Implementacao/findrec.cpp Implementacao/hash.cpp -o findrec
  ```
  ```
  ./findrec <ID do Registro>
  ```

- Para o seek1

  ```
  g++ B+Tree/IndicePrimario.cpp B+Tree/IndiceSecundario.cpp Implementacao/hash.cpp Implementacao/seek1.cpp -o seek1
  ```
  ```
  ./seek1 <ID ou Título do Registro>
  ```

- Para o seek2

  ```
  g++ B+Tree/IndicePrimario.cpp B+Tree/IndiceSecundario.cpp Implementacao/hash.cpp Implementacao/seek2.cpp -o seek2
  ```
  ```
  ./seek2 <ID ou Título do Registro>
  ```
