#include <iostream>
#include <stdlib.h>
#include <string>
#include "../B+Tree/IndiceSecundario.hpp"
#include "hash.hpp"

int main(int argc, char *argv[]){
	if(argc != 2){
		cout << "Qual o título do registro que deseja procurar?.\nEx: seek2 <Titulo>\n" <<endl;
		return 1;
	}
    fstream *hashFile = new fstream(HASH_FILE_NAME,fstream::in|ios::binary);
    fstream *secIdxFile = new fstream(SEC_INDEX_FILE_NAME,fstream::in|ios::binary);

    if (hashFile->is_open() && secIdxFile->is_open()) {
        pesquisaChavArqIndSec(hashFile,secIdxFile, argv[1]);   
    }
    else {
        cout << "Não foi possível abrir o arquivo.\n";
    }
    return 0;
	
}