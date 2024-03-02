#include <iostream>
#include <fstream>
#include "../B+Tree/IndicePrimario.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  // Validação da entrada
  if (argc < 2) {
    cout << "Erro: ID não especificado." << endl;
    cout << "Ex: seek1 <ID>" << endl;
    return 1;
  }

  // Obtendo o ID do registro a ser buscado
  int id = atoi(argv[1]);

  // Buscando o registro no índice primário
  seek1(HASH_FILE_NAME, PRIM_INDEX_FILE_NAME, id);

  return 0;
}
