# Alunos
Gabriel Pacheco, Max Souza, Noah Diunkz

------------------------------------------------------------------

# Pré-requisitos

- artigo.csv precisa estar descompactado e na pasta raiz.
- os arquivos estão organizados por diretórios, então ao executar fora das pastas é importante passar o path. 

------------------------------------------------------------------

# Como Executar

Os comandos abaixo já estão organizados para serem executados dentro do diretório raiz

- Para rodar upload.cpp

  ```
  g++ B+Tree/IndicePrimario.cpp B+Tree/IndiceSecundario.cpp Implementacao/hash.cpp Implementacao/upload.cpp -o upload
  ./upload artigo.csv
  ```

- Para rodar findrec.cpp

  ```
  g++ Implementacao/findrec.cpp Implementacao/hash.cpp -o findrec
  ./findrec <idDoRegistro>
  ```

- Para o seek1

  ```
  g++ B+Tree/IndicePrimario.cpp B+Tree/IndiceSecundario.cpp Implementacao/hash.cpp Implementacao/seek1.cpp -o seek1
  ./seek1 <tituloDoRegistro>
  ```

- Para o seek2

  ```
  g++ B+Tree/IndicePrimario.cpp B+Tree/IndiceSecundario.cpp Implementacao/hash.cpp Implementacao/seek2.cpp -o seek2
  ./seek2 <tituloDoRegistro>
  ```
------------------------------------------------------------------
