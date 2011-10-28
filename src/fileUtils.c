#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
//#include <sys/stat.h>//Habilitar caso não compile sem

//Lê uma linha(Unix style) de um arquivo e copia para a string informada
bool readFileLine(char *text, char *filename) {
	FILE *pFile;
	int ch, count = 0;
	bool result, valid;
	
	pFile = fopen(filename, "r");
	if(pFile == NULL) {
		result = false;
	} else {
		do {
			ch = fgetc(pFile);
			valid = (ch != EOF) && (ch != '\n');
			if(valid == true) {
				text[count] = ch;
				count++;
			}
		} while(valid == true);
		text[count] = '\0';
		fclose(pFile);
		result = true;
	}
	return result;
}

//Escreve uma string, seguida de um pulo de linha(Unix style), em um arquivo
bool writeFileLine(char *text, char *filename) {
	FILE *pFile;
	bool result;
	
	pFile = fopen(filename, "a");
	if(pFile == NULL) {
		result = false;
	} else {
		result = (fprintf(pFile, "%s\n", text) < 0) ? false : true;
		fclose(pFile);
	}
	return result;
}

//Escreve uma sequências de bytes no arquivo especificado
bool writeBinFile(const void *datastruct, size_t typesize, size_t count, char *filename) {
	FILE *pFile;
	bool result;
	
	pFile = fopen(filename, "wb");
	if(pFile == NULL) {
		result = false;
	} else {
		result = (fwrite(datastruct, typesize, count, pFile) != count) ? false : true;
		fclose(pFile);
	}
	return result;
}

//Lê uma sequências de bytes do arquivo especificado
//Cuidados: Deve ser usado em conjunto com 'sizeOfBinFile'
bool readBinFile(void *datastruct, size_t typesize, size_t filesize, char *filename) {
	FILE *pFile;
	bool result;
	
	pFile = fopen(filename, "rb");
	if(pFile == NULL) {
		result = false;
	} else {
		result = (fread(datastruct, typesize, filesize, pFile) != filesize) ? false : true;
		fclose(pFile);
	}
	return result;
}

//Calcula o tamanho de um arquivo texto ou binário
long sizeOfFile(char *filename, bool isBinary) {
	FILE *pFile;
	long file_size = 0;
	struct stat stbuf;
	int fd;

	fd = open(filename, O_RDONLY);
	if(fd != -1) {
		pFile = fdopen(fd, (isBinary == true) ? "rb" : "r");
		if(pFile != NULL) {
			if (fstat(fd, &stbuf) != -1) {
				file_size = stbuf.st_size;
			}
			fclose(pFile);
		}
	}
	return file_size;
}

//Verifica se um arquivo existe
bool fileExists(const char * filename) {
	bool result = false;
	FILE * pFile = fopen(filename, "r");
	if(pFile != NULL) {
		fclose(pFile);
		result = true;
	}
	return result;
}

//////// TESTES ////////
/**
int main(int argc, char *argv[]) {
	char str[1000];
	long size = sizeOfFile(argv[1], false);
	if(argc > 1) {
		if(readFileLine(str, argv[1]) == true) {
			printf("%s - fstat: %ld - strlen: %d\n", str, size, strlen(str)+1);
		} else {
			printf("Erro ao ler arquivo\n");
		}
	} else {
		printf("Parâmetros?\n");
	}
	return 0;
}
//*/
