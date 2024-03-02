#include <iostream>
#include <fstream>
#include "hash.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  // Validação da entrada
  if (argc < 2) {
    cout << "Erro: ID não especificado." << endl;
    cout << "Ex: findrec <ID>" << endl;
    return 1;
  }

  // Abrindo o arquivo de hash
  fstream hashFile(HASH_FILE_NAME, ios::in | ios::binary);

  // Verificando se o arquivo foi aberto com sucesso
  if (!hashFile.is_open()) {
    cout << "Erro ao abrir o arquivo de hash." << endl;
    return 1;
  }

  // Obtendo o ID do registro a ser buscado
  int id = atoi(argv[1]);

  // Buscando o registro no arquivo de hash
  buscaRegistroPorId(&hashFile, id);

  // Fechando o arquivo de hash
  hashFile.close();

  return 0;
}
