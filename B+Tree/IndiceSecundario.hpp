#ifndef SEC_INDEX_H

#define SEC_INDEX_H
#define M 50                   //Ordem da árvore
#define N_POINTERS 2 * M + 1   //Quantidade de apontadores para cada nó
#define N_KEYS 2 * M           //Quantidade de chaves para cada nó
#define SEC_INDEX_FILE_NAME "secondaryIndexFile"

#include <fstream>
#include "../Implementacao/hash.hpp"

using namespace std;

struct Node{
	int tamaho;               //Qtd de chaves ocupadas no nó
	int posicao;
	char chave[N_KEYS][T_TITLE]; //Valores de busca (chaves) do nó. A chave é o títul
	int apontador[N_POINTERS];    // Apontadores do nó
};

struct NodeAux{
	char chave[T_TITLE];      //Chave do nó. A chave é o título.
	int ponteiroEsquerda;       
	int ponteiroDireita;
};

struct Head{
	int posicaoRaiz;          
	int qtdNo;            
};

void InsereArqIndiceSec(fstream *hashFile, fstream *secIdxFile);
int pesquisaChavArqIndSec(fstream *caminhoArquivoDados, fstream *caminhoArquivoIndice, char chave[300]);

#endif