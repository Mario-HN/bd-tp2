#ifndef SEC_INDEX_H
#define SEC_INDEX_H

using namespace std;
#include <fstream>
#include "../Implementacao/hash.hpp"

//Ordem da árvore
#define M 50

//Quantidade de apontadores para cada nó
#define N_POINTERS 2 * M + 1

//Quantidade de chaves para cada nó
#define N_KEYS 2 * M

#define SEC_INDEX_FILE_NAME "secondaryIndexFile"

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
	int qtdNoh;            
};

void InsereArqIndiceSec(fstream *hashFile, fstream *secIdxFile);
int pesquisaChavArqIndSec(fstream *caminhoArquivoDados, fstream *caminhoArquivoIndice, char chave[300]);

#endif