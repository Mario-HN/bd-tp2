#ifndef PRIMARY_INDEX_HPP
#define PRIMARY_INDEX_HPP

#define PRIM_INDEX_FILE_NAME "primaryIndexFile"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include "../Implementacao/hash.hpp"
using namespace std;

//Ordem da árvore
#define ORDER_M 1000

//Quantidade de apontadores para cada nó
#define N_POINTERS_PRIM 2 * ORDER_M

//Quantidade de chaves para cada nó
#define N_CHAV_PRIM N_POINTERS_PRIM - 1

typedef struct NohPrimario{
	int tamanho;
	int posicao;
	int chave[2*ORDER_M];
	int apontador[2*ORDER_M+1];
} NohPrimario;

typedef struct AuxNode{
	int chave;
	int ponteiroEsquerda;
	int ponteiroDireita;
} AuxNode;

typedef struct Header{
	int posicaoRaiz;
	int qtdNoh;
} Header;

void InsereArqIndicePrim(fstream *hashFile, fstream *primIdxFile);
void seek1(const char *caminhoArquivoDados, const char *caminhoArquivoIndice, int chave);

#endif