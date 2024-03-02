#include "IndicePrimario.hpp"

Header *header = NULL;
fstream *indexFile, *dataFile;

void atualizaCabecalhoIndPrimario(){
	indexFile->seekp(0,indexFile->beg);
	indexFile->write((char*)header, sizeof(Header));
}

void inicializaCabecalhoPrimario(){
	header = (Header *)malloc(sizeof(Header));
	header->posicaoRaiz = -1;
	header->qtdNo = 0;
	atualizaCabecalhoIndPrimario();
}

NoPrimario* criarNovoNoPrimario(){
	NoPrimario *node = (NoPrimario *) malloc(sizeof(NoPrimario));
	node->tamanho = 0;
	node->posicao = 0;

	//Inicializa as chaves e apontadores
	for(int i = 0; i < N_CHAV_PRIM; i++){
		node->chave[i] = 0;
		node->apontador[i] = 0;
	}
	node->apontador[N_POINTERS_PRIM] = 0;
	return node;
}

int insereNoArqIndice(NoPrimario *block){
	//qtd nós adicionados
	header->qtdNo++;
	//Posiciona cursor
	indexFile->seekp(header->qtdNo*sizeof(NoPrimario), indexFile->beg);
	//Escreve o nó no arquivo de indice
	indexFile->write((char*)block,sizeof(NoPrimario));
	//qtd nós
	atualizaCabecalhoIndPrimario();
	return header->qtdNo;
}

void atualizaNoArquivo(NoPrimario *block){
	indexFile->seekp(block->posicao*sizeof(NoPrimario), indexFile->beg);
	indexFile->write((char*)block,sizeof(NoPrimario));
}

void InsereChavPagDisponivel(NoPrimario *node, int chave, int apontador){
	int i,j;
	for(i=0; i<node->tamanho; i++){
		if(node->chave[i] > chave){
			for(j=node->tamanho; j>i; j--){
				node->chave[j] = node->chave[j-1];
				node->apontador[j] = node->apontador[j-1];
			}
			break;
		}
	}
	//Insere chave no nó
	node->chave[i] = chave;
	node->apontador[i] = apontador;
	node->tamanho++;
	atualizaNoArquivo(node);
}

void InsereChavNoDisponivel(NoPrimario *node, int chave, int apontador){
	int i,j;
	for(i=0; i<node->tamanho; i++){
		if(node->chave[i] > chave){
			for(j=node->tamanho; j>i; j--){
				node->chave[j] = node->chave[j-1];
				node->apontador[j+1] = node->apontador[j];
			}
			break;
		}
	}
	node->chave[i] = chave;
	node->apontador[i+1] = -1 * apontador;
	node->tamanho++;
	atualizaNoArquivo(node);
}

AuxNode* InsereChavPagCheia(NoPrimario *node, int chave, int apontador){
	// Trata a inserção da chave para o caso em que a página de dados já está cheia
	int i,j, pivot, inserted=0;
	NoPrimario *newNodeReturn = NULL;
	AuxNode *fatherNode = NULL;
	newNodeReturn = criarNovoNoPrimario();
	fatherNode = (AuxNode *) malloc(sizeof(AuxNode));
	pivot = node->chave[ORDER_M];
	node->chave[ORDER_M] = 0;
	node->tamanho--;
	if(chave < pivot){
		j=0;
		newNodeReturn->apontador[0] = node->apontador[ORDER_M+1];
		for(i=ORDER_M+1; i<=N_CHAV_PRIM; i++){
			newNodeReturn->chave[j] = node->chave[i];
			newNodeReturn->apontador[j+1] = node->apontador[i+1];
			newNodeReturn->tamanho++;
			node->chave[i] = 0;
			node->apontador[i+1] = 0;
			node->tamanho--;
			j++;
		}
		for(i=ORDER_M; i>0; i--){
			if(chave > node->chave[i]){
				break;
			}else{
				node->chave[i] = node->chave[i-1];
				node->apontador[i+1] = node->apontador[i];
			}
		}
		node->chave[i] = chave;
		node->apontador[i+1] = -1 * apontador;
		node->tamanho++;
	}else{
		j=0;
		newNodeReturn->apontador[0] = node->apontador[ORDER_M+1];
		for(i=ORDER_M+1; i<=N_CHAV_PRIM; i++){
			if(inserted == 0 && chave < node->chave[i]){
				newNodeReturn->chave[j] = chave;
				newNodeReturn->apontador[j+1] = -1 * apontador;
				newNodeReturn->tamanho++;
				j++;
				inserted = 1;
			}
			newNodeReturn->chave[j] = node->chave[i];
			newNodeReturn->apontador[j+1] = node->apontador[i+1];
			newNodeReturn->tamanho++;
			node->chave[i] = 0;
			node->apontador[i+1] = 0;
			node->tamanho--;
			j++;
		}
		if(inserted == 0){
			newNodeReturn->chave[j] = chave;
			newNodeReturn->apontador[j+1] = -1 * apontador;
			newNodeReturn->tamanho++;
		}
	}
	newNodeReturn->posicao = insereNoArqIndice(newNodeReturn);
	atualizaNoArquivo(newNodeReturn);
	atualizaNoArquivo(node);
	fatherNode->chave = pivot;
	fatherNode->ponteiroEsquerda = -1 * node->posicao;
	fatherNode->ponteiroDireita = -1 * newNodeReturn->posicao;
	free(newNodeReturn);
	return fatherNode;
}

AuxNode* InsereChavNoCheio(NoPrimario *node, int chave, int apontador){
	int i,j, pivot, inserted=0;
	NoPrimario *newNodeReturn = NULL;
	AuxNode *fatherNode = NULL;
	newNodeReturn = criarNovoNoPrimario();
	fatherNode = (AuxNode *) malloc(sizeof(AuxNode));
	pivot = node->chave[ORDER_M];
	if(chave < pivot){
		j=0;
		for(i=ORDER_M; i<= N_CHAV_PRIM; i++){
			newNodeReturn->chave[j] = node->chave[i];
			newNodeReturn->apontador[j] = node->apontador[i];
			newNodeReturn->tamanho++;
			node->chave[i] = 0;
			node->apontador[i] = 0;
			node->tamanho--;
			j++;
		}
		for(i=ORDER_M-1; i>=0; i--){
			if(chave > node->chave[i]){
				break;
			}else{
				node->chave[i+1] = node->chave[i];
				node->apontador[i+1] = node->apontador[i];
			}
		}
		node->chave[i+1] = chave;
		node->apontador[i+1] = apontador;
		node->tamanho++;
	}else{
		j=0;
		for(i=ORDER_M; i<=N_CHAV_PRIM; i++){
			if(inserted == 0 && chave < node->chave[i]){
				newNodeReturn->chave[j] = chave;
				newNodeReturn->apontador[j] = apontador;
				newNodeReturn->tamanho++;
				j++;
				inserted = 1;
			}
			newNodeReturn->chave[j] = node->chave[i];
			newNodeReturn->apontador[j] = node->apontador[i];
			newNodeReturn->tamanho++;
			node->chave[i] = 0;
			node->apontador[i] = 0;
			node->tamanho--;
			j++;
		}
		if(inserted == 0){
			newNodeReturn->chave[j] = chave;
			newNodeReturn->apontador[j] = apontador;
			newNodeReturn->tamanho++;
		}
	}
	newNodeReturn->apontador[N_POINTERS_PRIM] = node->apontador[N_POINTERS_PRIM];
	newNodeReturn->posicao = insereNoArqIndice(newNodeReturn);
	node->apontador[N_POINTERS_PRIM] = -1 * newNodeReturn->posicao;
	atualizaNoArquivo(node);
	atualizaNoArquivo(newNodeReturn);
	fatherNode->chave = pivot;
	fatherNode->ponteiroEsquerda = -1 * node->posicao;
	fatherNode->ponteiroDireita = -1 * newNodeReturn->posicao;
	free(newNodeReturn);
	return fatherNode;
}

NoPrimario* consultaNoPrimArquivo(int posicao){
	//Retorna o nó do arquivo de indice a partir da posição informada
	if(posicao > 0){
		cout << "Endereço errado. Insira novamente!" << endl;
		return NULL;
	}
	NoPrimario *node = criarNovoNoPrimario();
	//Posiciona o cursor na posição
	indexFile->seekg(-1 * posicao * sizeof(NoPrimario),indexFile->beg);
	//Copia o nó 
	indexFile->read((char *)node, sizeof(NoPrimario));
	return node;
}

AuxNode* insereChaveArvore(NoPrimario *node, int chave, int apontador){
	int i,j;
	AuxNode *nodeReturn = NULL;
	//Página é índice
	if(node->apontador[0] < 0){
		int smaller=0,larger=node->tamanho-1,posicao;
		//Encontra a posição do apontador
		while(smaller <= larger){
			posicao = (smaller + larger) / 2;
			if(chave < node->chave[posicao]){
				larger = posicao - 1;
			}
			//chave atual é maior ou igual a uma chave já existente na posição
			else{
				smaller = posicao + 1;
			}
		}
		//O apontador na primera posição
		if(larger < 0){
			larger = 0;
		}
		//O apontador está na posição posterior à chave
		else if(chave >= node->chave[larger]){
			larger++;
		}
		posicao = larger;
		nodeReturn = insereChaveArvore(consultaNoPrimArquivo(node->apontador[posicao]),chave,apontador);
		if(nodeReturn != NULL){
			//Caso a página tenha espaço
			if(node->chave[N_CHAV_PRIM] == 0){
				InsereChavNoDisponivel(node, nodeReturn->chave, -1 * nodeReturn->ponteiroDireita);
				free(nodeReturn);
				nodeReturn = NULL;
			}
			//Caso a página não tenha espaço
			else{
				AuxNode *newNodeReturn = NULL;
				newNodeReturn = InsereChavNoCheio(node, nodeReturn->chave, -1 * nodeReturn->ponteiroDireita);
				free(nodeReturn);
				nodeReturn = newNodeReturn;
			}
		}
	}
	//É página de dados
	else{
		//Caso a página tenha espaço
		if(node->chave[N_CHAV_PRIM] == 0){
			InsereChavPagDisponivel(node,chave,apontador);
		}
		//Caso a página não tenha espaço
		else{
			nodeReturn = InsereChavPagCheia(node,chave,apontador);
		}
	}
	free(node);
	return nodeReturn;
}

void insereNaArvore(int chave, int apontador){
	AuxNode *nodeReturn;
	nodeReturn = insereChaveArvore(consultaNoPrimArquivo(header->posicaoRaiz),chave,apontador);
	if(nodeReturn != NULL){
		NoPrimario *newNodeReturn;
		int pos;
		newNodeReturn = criarNovoNoPrimario();
		newNodeReturn->chave[0] = nodeReturn->chave;
		newNodeReturn->apontador[0] = nodeReturn->ponteiroEsquerda;
		newNodeReturn->apontador[1] = nodeReturn->ponteiroDireita;
		newNodeReturn->tamanho++;
		pos = insereNoArqIndice(newNodeReturn);
		newNodeReturn->posicao = pos;
		atualizaNoArquivo(newNodeReturn);
		header->posicaoRaiz = -1 * pos;
		atualizaCabecalhoIndPrimario();
		free(newNodeReturn);
		free(nodeReturn);
	}
}

void populaArqIndicePrim(){
	Block buffer = {0};
	//Posiciona cursor no início do arquivo de dados
	dataFile->clear();
	dataFile->seekg(0,dataFile->beg);

	//insere o id inicial de cada bloco na arvore
	for (int i = 0; i < N_BUCKETS; i++) {
		//faz a leitura de um bloco
        dataFile->read((char*)&buffer,sizeof(Block));
		//verifica se o bloco nao esta vazio
        if(buffer.nRegisters > 0) {
            Article *vet;
            vet=(Article*)&buffer.body;
			//insere o id do primeiro registro do bloco na arvore
            insereNaArvore(vet[0].id, i);
        }
    }
}

void InsereArqIndicePrim(fstream *hashFile, fstream *primIdxFile){
	indexFile = primIdxFile;
	dataFile = hashFile;

	inicializaCabecalhoPrimario();
	NoPrimario *root = criarNovoNoPrimario();
	root->posicao = insereNoArqIndice(root); //Escreve nó raiz no arquivo
	atualizaNoArquivo(root);
	free(root);
	populaArqIndicePrim();
}


void consultaCabecalhoArqIndicePrim(){
	if(header == NULL){
		header = (Header *)malloc(sizeof(Header));
	}
	//cursor início do arquivo
	indexFile->seekg(0,indexFile->beg);
	//Copia o cabeçalho do arquivo para header
	indexFile->read((char *)header, sizeof(Header));
}

void abreArqIndice(const char *pathIndexFile){
	indexFile = new fstream(pathIndexFile, fstream::in | fstream::out | ios::binary);
	if(!indexFile){
		cout << "Não foi possível abrir o arquivi de índice." << endl;
		exit(EXIT_FAILURE);
	}
}

void abreArqDados(const char *pathDataFile){
	dataFile = new fstream(pathDataFile, fstream::in | ios::binary);
	if(!indexFile){
		cout << "Não foi possível abrir o arquivo de dados." << endl;
		exit(EXIT_FAILURE);
	}
}

void closeFiles(){
	indexFile->close();
	dataFile->close();
	free(indexFile);
	free(dataFile);
	free(header);
}

int pesquisaChavArqIndPrim(const char *pathDataFile,const char *pathIndexFile, int chave){
	int apontador, pos, smaller, larger, posPointer, i, numReadBlocks=0;
	NoPrimario *node,*aux;
	abreArqIndice(pathIndexFile);
	abreArqDados(pathDataFile);
	consultaCabecalhoArqIndicePrim();
	numReadBlocks++;
	pos = header->posicaoRaiz;

	while(1){
		node = consultaNoPrimArquivo(pos);
		numReadBlocks++;
		if(node != NULL){
			if(node->apontador[0] < 0){
				smaller=0;
				larger=node->tamanho-1;
				while(smaller <= larger){
					posPointer = (smaller + larger) / 2;
					if(chave < node->chave[posPointer]){
						larger = posPointer - 1;
					}else{
						smaller = posPointer + 1;
					}
				}
				if(larger < 0){
					larger = 0;
				}else if(chave >= node->chave[larger]){
					larger++;
				}
				posPointer = larger;
				pos = node->apontador[posPointer];
			}else{
				break;
			}
		}else{
			cout << "Erro: não foi possível carregar o nó na função pesquisaChavArqIndPrim" << endl;
			return -1;
		}
	}
	cout << "Quantidade de Blocos lidos: " << numReadBlocks << endl;
	smaller=0;
	larger=node->tamanho-1;
	while(smaller <= larger){
		posPointer = (smaller + larger) / 2;
		if(chave == node->chave[posPointer]){
			return node->apontador[posPointer];
		}else if(chave < node->chave[posPointer]){
			larger = posPointer - 1;
		}else{
			smaller = posPointer + 1;
		}
	}
	closeFiles();
	return -1;
}

Block * lerBlocoNoArqDados(fstream *arq, int posicao) {
    Block *buffer= NULL;
    int i,j;

    buffer = (Block*) malloc(sizeof(Block));

    //Encontrando a posição do bucket no hashfile
    arq->seekg(posicao*sizeof(Block),ios::beg);

    //Leitura
    arq->read((char*)buffer,sizeof(Block));
    return buffer;
}

void copDadosSourceParaTarget(Article *target, Article *source){
    target->id = source->id;
    strcpy(target->title,source->title);
    target->year = source->year;
    strcpy(target->author,source->author);
    target->citations = source->citations;
    strcpy(target->update,source->update);
    strcpy(target->snippet,source->snippet);
}

Article* consultDadosRegPorID(fstream *arq, int posicao, int id){
    Block *bucket;
    Article *article, *result=NULL;
    int larger, smaller, posPointer;

    result = (Article *) malloc(sizeof(Article));
    bucket = lerBlocoNoArqDados(arq, posicao);
    article = (Article*)&bucket->body;
    smaller=0;
    larger=bucket->nRegisters-1;

    while(smaller <= larger){
        posPointer = (smaller + larger) / 2;

        if(id == article[posPointer].id){
            copDadosSourceParaTarget(result, &article[posPointer]);
            break;
        }else if(id < article[posPointer].id){
            larger = posPointer - 1;
        }else{
            smaller = posPointer + 1;
        }
    }

    free(bucket);
    return result;
}

void seek1(const char *pathDataFile,const  char *pathIndexFile, int chave){
	int posicao = 0;
	Article *article;
	posicao = pesquisaChavArqIndPrim(pathDataFile,pathIndexFile,chave);

    //verificação
	if(posicao < 0){
		cout << "Registro com o id " << chave << " não encontrado" << endl;
	}
	//se encontra, busca chave no arquivo de dados e imprime
	else{
		abreArqDados(pathDataFile);
		article = consultDadosRegPorID(dataFile,posicao, chave);
		imprimirRegistroArt(*article);
		free(article);
		dataFile->close();
	}
}