#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include "IndiceSecundario.hpp"
#include "../Implementacao/hash.hpp"

Head *head = NULL;
fstream *hashF, *secF;


void atualizaCabecalhoIndSecundario(){
	//Posiciona cursor no início do arquivo
	secF->seekp(0,secF->beg);
	//Escreve o cabeçalho no arquivo
	secF->write((char*)head, sizeof(Head));
}

void inicializaCabecalhoSecundario(){
	head = (Head *)malloc(sizeof(Head));
	head->posicaoRaiz = -1;
	head->qtdNo = 0;
	//Escreve cabeçalho no arquivo
	atualizaCabecalhoIndSecundario();
}

Node* criarNovoNoSecundario(){
	Node *node = (Node *) malloc(sizeof(Node));
	node->tamaho = 0;
	node->posicao = 0;

	//Inicializa chaves e apontadores
	for(int i = 0; i < N_KEYS - 1; i++){
		strcpy(node->chave[i],"\0");
		node->apontador[i] = 0;
	}
	node->apontador[N_POINTERS - 1] = 0;

	return node;
}

int insereNoArqIndice(Node *node){
	//qtd nós adicionados
	head->qtdNo++;
	//Posiciona cursor
	secF->seekp(head->qtdNo*sizeof(Node), secF->beg);
	//Escreve o nó no arquivo de indice
	secF->write((char*)node,sizeof(Node));
	//qtd nós
	atualizaCabecalhoIndSecundario();

	return head->qtdNo;
}

void atualizaNoArquivo(Node *node){
	//Posiciona o cursor
	secF->seekp(node->posicao*sizeof(Node), secF->beg);
	//Escreve o nó no arq
	secF->write((char*)node,sizeof(Node));
}

void InsereChavPagDisponivel(Node *node, char chave[T_TITLE], int apontador){
	int i;
	//Percorre as chaves
	for(i=0; i< node->tamaho; i++){
		//Se a chave existente na posição i for maior que a chave atual
		if(strcmp(node->chave[i], chave) > 0){
			for(int j=node->tamaho; j>i; j--){
				//Desloca chave existente
				strcpy(node->chave[j],node->chave[j-1]);
				node->apontador[j] = node->apontador[j-1];
			}
			break;
		}
	}
	//Insere chave no nó
	strcpy(node->chave[i],chave);
	node->apontador[i] = apontador;
	node->tamaho++;
	atualizaNoArquivo(node);
}

void InsereChavNoDisponivel(Node *node, char chave[T_TITLE], int apontador){
	int i;
	for(i=0; i < node->tamaho; i++){
		if(strcmp(node->chave[i],chave) > 0){
			for(int j=node->tamaho; j>i; j--){
                strcpy(node->chave[j],node->chave[j-1]);
				node->apontador[j+1] = node->apontador[j];
			}
			break;
		}
	}
	strcpy(node->chave[i],chave);
	node->apontador[i+1] = -1 * apontador;
	node->tamaho++;
	atualizaNoArquivo(node);
}

NodeAux* InsereChavPagCheia(Node *node, char chave[T_TITLE], int apontador){
	// Trata a inserção da chave para o caso em que a página de dados já está cheia
	int added=0;
    char pivot[T_TITLE];
	Node *newNode = criarNovoNoSecundario();
	NodeAux *newParent = (NodeAux *) malloc(sizeof(NodeAux));
	strcpy(pivot,node->chave[M]);

	if(strcmp(chave,pivot) < 0){
		int i,j=0;
		for(i=M; i<=N_KEYS - 1; i++){
			strcpy(newNode->chave[j],node->chave[i]);
			newNode->apontador[j] = node->apontador[i];
			newNode->tamaho++;
			node->apontador[i] = 0;
			strcpy(node->chave[i],"\0");
			node->tamaho--;
			j++;
		}
		for(i=M-1; i>=0; i--){
			if(strcmp(chave,node->chave[i]) > 0){
				break;
			}else{
				strcpy(node->chave[i+1], node->chave[i]);
				node->apontador[i+1] = node->apontador[i];
			}
		}
		strcpy(node->chave[i+1],chave);
		node->apontador[i+1] = apontador;
		node->tamaho++;
	}else{
		int i,j=0;
		for(i=M; i<=N_KEYS - 1; i++){
			if(added == 0 && strcmp(chave,node->chave[i]) < 0){
				strcpy(newNode->chave[j],chave);
				newNode->apontador[j] = apontador;
				newNode->tamaho++;
				j++;
				added = 1;
			}
			strcpy(newNode->chave[j],node->chave[i]);
			newNode->apontador[j] = node->apontador[i];
			newNode->tamaho++;
			strcpy(node->chave[i],"\0");
			node->apontador[i] = 0;
			node->tamaho--;
			j++;
		}
		if(added == 0){
			strcpy(newNode->chave[j],chave);
			newNode->apontador[j] = apontador;
			newNode->tamaho++;
		}
	}
	newNode->apontador[N_POINTERS - 1] = node->apontador[N_POINTERS - 1];
	newNode->posicao = insereNoArqIndice(newNode);
	node->apontador[N_POINTERS - 1] = -1 * newNode->posicao;
	atualizaNoArquivo(node);
	atualizaNoArquivo(newNode);
	strcpy(newParent->chave, pivot);
	newParent->ponteiroEsquerda = -1 * node->posicao;
	newParent->ponteiroDireita = -1 * newNode->posicao;
	free(newNode);
	return newParent;
}

NodeAux* InsereChavNoCheio(Node *node, char chave[T_TITLE], int apontador){
	int added=0;
    char pivot[T_TITLE];
	Node *newNode = criarNovoNoSecundario();
	NodeAux *newParent = (NodeAux *) malloc(sizeof(NodeAux));
	strcpy(pivot,node->chave[M]);
	strcpy(node->chave[M],"\0");

	node->tamaho--;

	if(strcmp(chave,pivot) < 0){
		int i,j=0;
		newNode->apontador[0] = node->apontador[M+1];
		for(i=M+1; i<=N_KEYS - 1; i++){
			strcpy(newNode->chave[j],node->chave[i]);
			newNode->apontador[j+1] = node->apontador[i+1];
			newNode->tamaho++;
			strcpy(node->chave[i],"\0");
			node->apontador[i+1] = 0;
			node->tamaho--;
			j++;
		}
		for(i=M; i>0; i--){
			if(strcmp(chave,node->chave[i]) > 0){
				break;
			}else{
				strcpy(node->chave[i],node->chave[i-1]);
				node->apontador[i+1] = node->apontador[i];
			}
		}
		strcpy(node->chave[i],chave);
		node->apontador[i+1] = -1 * apontador;
		node->tamaho++;
	}

	else{
		int i,j=0;
		newNode->apontador[0] = node->apontador[M+1];
		for(i=M+1; i<=N_KEYS - 1; i++){
			if(added == 0 && strcmp(chave,node->chave[i]) < 0){
				strcpy(newNode->chave[j],chave);
				newNode->apontador[j+1] = -1 * apontador;
				newNode->tamaho++;
				j++;
				added = 1;
			}
			strcpy(newNode->chave[j],node->chave[i]);
			newNode->apontador[j+1] = node->apontador[i+1];
			newNode->tamaho++;
			node->apontador[i+1] = 0;
			strcpy(node->chave[i],"\0");
			node->tamaho--;
			j++;
		}
		if(added == 0){
			strcpy(newNode->chave[j], chave);
			newNode->apontador[j+1] = -1 * apontador;
			newNode->tamaho++;
		}
	}

	newNode->posicao = insereNoArqIndice(newNode);
	atualizaNoArquivo(newNode);
	atualizaNoArquivo(node);

	strcpy(newParent->chave, pivot);
	newParent->ponteiroEsquerda = -1 * node->posicao;
	newParent->ponteiroDireita = -1 * newNode->posicao;
	free(newNode);
	return newParent;
}

Node* consultaNoSecArquivo(int posicao){
	//Retorna o nó do arquivo de indice a partir da posição informada
	if(posicao > 0){
		cout << "Endereço errado. Insira novamente!" << endl;
		return NULL;
	}

	Node *node = criarNovoNoSecundario();
	//Posiciona o cursor na posição
	secF->seekg(-1 * posicao * sizeof(Node),secF->beg);
	//Copia o nó
	secF->read((char *)node, sizeof(Node));
	return node;
}

NodeAux* insereChaveArvore(Node *node, char chave[T_TITLE], int apontador){

	NodeAux *index = NULL;
	//Página é índice
	if(node->apontador[0] < 0){
		int posicao;
		int minPos = 0;
		int maxPos=node->tamaho-1;

		//Encontra a posição do apontador
		while(minPos <= maxPos){
			posicao = (minPos + maxPos) / 2;
			if(strcmp(chave,node->chave[posicao]) < 0){
				maxPos = posicao - 1;
			}
			//chave atual é maior ou igual a uma chave já existente na posição
			else{
				minPos = posicao + 1;
			}
		}
		//O apontador na primera posição
		if(maxPos < 0){
			maxPos = 0;
		}
		//O apontador está na posição posterior à chave
		else if(strcmp(chave,node->chave[maxPos]) >= 0){
			maxPos++;
		}
		posicao = maxPos;

		index = insereChaveArvore(consultaNoSecArquivo(node->apontador[posicao]),chave,apontador);

		if(index!= NULL){
			//Caso a página tenha espaço
			if(strcmp(node->chave[N_KEYS - 1], "\0") == 0){
				InsereChavNoDisponivel(node, index->chave, -1 * index->ponteiroDireita);
				free(index);
				index = NULL;
			}
			//Caso a página não tenha espaço
			else{
				NodeAux *novoRetorno = InsereChavNoCheio(node, index->chave, -1 * index->ponteiroDireita);
				free(index);
				index = novoRetorno;
			}
		}
	}
	//É página de dados
	else{
		//Caso a página tenha espaço
		if(strcmp(node->chave[N_KEYS - 1],"\0") == 0){
			InsereChavPagDisponivel(node,chave,apontador);
		}
		//Caso a página não tenha espaço
		else{
			index = InsereChavPagCheia(node,chave,apontador);
		}
	}
	free(node);
	return index;
}

void populaArqIndiceSec(){
	Block buffer = {0};
	int pos;

	//Posiciona cursor no início
	hashF->clear();
	hashF->seekg(0,hashF->beg);

	//Insere o título dos registros do arquivo de dados na árvore
	for (int i = 0; i <N_BUCKETS; i++) {
		//Copia o bucket para o buffer
        hashF->read((char*)&buffer,sizeof(Block));
		//Se não for vazio
        if(buffer.nRegisters > 0) {
            Article *v_article = (Article*)&buffer.body;
			//Percorre os registros do bloco
            for(int j=0; j<buffer.nRegisters; j++){
				//Insere o título (chave) do registro na árvore
				NodeAux *page = insereChaveArvore(consultaNoSecArquivo(head->posicaoRaiz),v_article[j].title,i);

				if(page != NULL){
					Node *node = criarNovoNoSecundario();
					//O novo nó recebe as informações do nó criado na árvore
					strcpy(node->chave[0],page->chave);
					node->apontador[0] = page->ponteiroEsquerda;
					node->apontador[1] = page->ponteiroDireita;
					node->tamaho++;
					//Insere o nó no arquivo
					int pos = insereNoArqIndice(node);
					node->posicao = pos;
					atualizaNoArquivo(node);
					//Atualiza posição da raiz
					head->posicaoRaiz = -1 * pos;
					atualizaCabecalhoIndSecundario();

					free(node);
					free(page);
				}
			}
        }
    }
}

void InsereArqIndiceSec(fstream *hashFile, fstream *secIdxFile){
	hashF =  hashFile;
	secF = secIdxFile;
	inicializaCabecalhoSecundario();
	Node *root = criarNovoNoSecundario();
	root->posicao = insereNoArqIndice(root); 	//Escreve nó raiz no arquivo						
	atualizaNoArquivo(root);
	free(root);
	populaArqIndiceSec();
}


void consultaCabecalhoArqIndiceSec(){
	if(head == NULL){
		head = (Head *)malloc(sizeof(Head));
	}
	//cursor início do arquivo
	secF->seekg(0,secF->beg);
	//Copia o cabeçalho do arquivo para head
	secF->read((char *)head, sizeof(Head));
}

int pesquisaChavArqIndSec(fstream *caminhoArquivoDados,fstream *caminhoArquivoIndice, char chave[T_TITLE]){
	hashF = caminhoArquivoDados;
	secF = caminhoArquivoIndice;

	int ponteiro, pos, minPos, maxPos, pPosition, i, blocks =0;
	Node *node,*aux;
	consultaCabecalhoArqIndiceSec();
	
	pos = head->posicaoRaiz;

	while(1){
		node = consultaNoSecArquivo(pos);
		blocks++;
		if(node != NULL){
			if(node->apontador[0] < 0){
				minPos = 0;
				maxPos=node->tamaho-1;
				while(minPos <= maxPos){
					pPosition = (minPos + maxPos) / 2;
					if(strcmp(chave,node->chave[pPosition]) < 0){
						maxPos = pPosition - 1;
					}
					else{
						minPos = pPosition + 1;
					}
				}
				if(maxPos < 0){
					maxPos = 0;
				}
				else if(strcmp(chave, node->chave[maxPos]) >= 0){
					maxPos++;
				}
				pPosition = maxPos;
				pos = node->apontador[pPosition];
			}else{
				break;
			}
		}
		else{
			cout << "Erro: não foi possível carregar o nó na função pesquisaChavArqIndSec" << endl;
			cout << "Registro " << chave << " nao encontrado" << endl;
			return -1;
		}
	}

	minPos=0;
	maxPos=node->tamaho-1;

	while(minPos <= maxPos){
		pPosition = (minPos + maxPos) / 2;
		if(strcmp(chave,node->chave[pPosition]) == 0){
			break;
		}else if(strcmp(chave,node->chave[pPosition]) < 0){
			maxPos = pPosition - 1;
		}else{
			minPos = pPosition + 1;
		}
	}

	//Consulta o arquivo de dados de acordo com a posição do apontador para o registro
	buscaBucketPorTitulo(hashF,node->apontador[pPosition], chave);
	cout<< "------------------------------------------------" << endl;
	cout << endl << "Total de blocos de índice armazenados: " << head->qtdNo << endl;
	cout << "Quantidade total de blocos lidos: " << blocks << endl;
	secF->close();
	hashF->close();
	free(secF);
	free(hashF);
	free(head);
	return -1;
}




















