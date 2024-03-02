#include "hash.hpp"
#include "../B+Tree/IndicePrimario.hpp"
#include "../B+Tree/IndiceSecundario.hpp"
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

// Remove arquivos anteriormente gerados no diretório
void removeArquivos() {
    remove(HASH_FILE_NAME);
    remove(PRIM_INDEX_FILE_NAME);
    remove(SEC_INDEX_FILE_NAME);
}

// Copia uma string a para b
void copiaString(char* a, string b, int tamaho) {
    strncpy(a, b.c_str(), tamaho);
    a[tamaho-1] = 0;
}

// Parser de arquivo e armazena na estrutura Article
Article parserLinhaArquivoDeEntrada(std::string line) {
    Article article;
    std::string delimiter = "\";"; //Delimitador utilizado   ";
    size_t pos = 0;
    std::string token;
    int column = 0;

    while ((pos = line.find(delimiter)) != std::string::npos) {
       
       token = line.substr(1, pos-1);
       //Extracao dos campos
        if (column == 0){
            article.id= atoi(token.c_str());
        }
        else if(column == 1){
            copiaString(article.title, token.c_str(), T_TITLE);
        }
        else if(column == 2){
            article.year = atoi(token.c_str());
        }
        else if(column == 3){

            copiaString(article.author, token.c_str(), T_AUTHOR);
        }
        else if(column == 4){
            article.citations = atoi(token.c_str());
        }
        else if(column == 5){
            copiaString(article.update, token.c_str(), T_UPDATE);
        }
        line.erase(0, pos + delimiter.length());   
        column ++;
    }

    if(line.length() > 0){
        token = line.substr(0, line.length()-1);

        if(strcmp(token.c_str(), "NULL") != 0){
            token = line.substr(1, line.length()-3); //Se Snippet é NULL
        }

        copiaString(article.snippet, token.c_str(), T_SNIPPET);
    }
    return article;
}


int main(int argc, char *argv[]) {
    if(argc<2) {
        cout << "Qual o nome do arquivo de entrada?\nEx: upload <file>\n";
        return 1;
    }

    ifstream file(argv[1]);
    string line;

    if (!file){
        printf("Não foi possível encontrar o arquivo.\n");
        exit(1);
    }
    removeArquivos();                       // Remove arquivos antigos que foram gerados

    fstream *hashFile = new fstream(HASH_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary); //Cria arquivo de dados
    inicializaArquivoDeSaida(hashFile);                       // Aloca o arquivo de dados

    fstream *secondaryIndexFile = new fstream(SEC_INDEX_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary); //Cria arquivo de indice secundário
    fstream *primaryIndexFile = new fstream(PRIM_INDEX_FILE_NAME,fstream::in|fstream::out|fstream::trunc|ios::binary); //Cria arquivo de indice primário

    cout << "Carregando os registros no arquivo de dados..."<<endl;

    while (getline(file, line)){
        Article article = parserLinhaArquivoDeEntrada(line); //Faz o parserLinhaArquivoDeEntrada de cada linha do arquivo de entrada
        insereArquivoHash(hashFile, article);   //Insere artigo no arquivo de dados
    }

    cout << "Carregando os dados no arquivo índice primário..." << endl;
    InsereArqIndicePrim(hashFile, primaryIndexFile);

    cout << "Carregando os dados no arquivo índice secundário..." << endl;
    InsereArqIndiceSec(hashFile, secondaryIndexFile); //Carrega os dados no arquivo de índice secundário

    cout << "Concluído." << endl;
    hashFile->close();
    primaryIndexFile->close();
    secondaryIndexFile->close();
    
}
