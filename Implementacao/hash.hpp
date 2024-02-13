#ifndef HASH_H
#define HASH_H

using namespace std;
#include <fstream>

//Define o tamanho dos campos de tipo alfa
#define T_TITLE 300
#define T_AUTHOR 150
#define T_UPDATE 20
#define T_SNIPPET 1024


#define N_BUCKETS 270973
#define N_REGISTERS 7

//Tamanho do corpo do bloco
#define T_BODY N_REGISTERS * (12 + T_TITLE +  T_AUTHOR +  T_UPDATE + T_SNIPPET)

//Nome do arquivo hashing
#define HASH_FILE_NAME "artigo"

struct Article{
    unsigned int id;
    char title[T_TITLE];
    unsigned int year;
    char author[T_AUTHOR];
    unsigned int citations;
    char update[T_UPDATE];
    char snippet[T_SNIPPET];
}; 

struct Block{
    unsigned int nRegisters; //Guarda informação sobre a quantidade de registros armazenados no bloco
    char body[T_BODY];       //Vetor de artigos
};

void inicializaArquivoDeSaida(fstream *f);
bool insereArquivoHash(fstream *f, Article article);
Article buscaRegistroPorId(fstream *f,int id);
Article buscaBucketPorTitulo(fstream *f, int posicao, char title[T_TITLE]);
void imprimirRegistroArt(Article article);

#endif