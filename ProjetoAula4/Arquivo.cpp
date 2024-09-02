#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

int carregarInsercoes(Registro** vetorInsercoes) {

	FILE* arquivoInsercoes = fopen("insere.bin", "rb");

	if (arquivoInsercoes == NULL) {
		perror("Erro ao abrir insere.bin");
		return 0;
	}

	fseek(arquivoInsercoes, 0, SEEK_END);
	int tamanhoArquivo = ftell(arquivoInsercoes);
	fseek(arquivoInsercoes, 0, SEEK_SET);

	int numeroRegistros = tamanhoArquivo / sizeof(Registro);
	*vetorInsercoes = (Registro*)malloc(tamanhoArquivo);

	if (*vetorInsercoes == NULL) {
		perror("Erro ao alocar memória para inserções");
		fclose(arquivoInsercoes);
		return 0;
	}

	fread(*vetorInsercoes, sizeof(Registro), numeroRegistros, arquivoInsercoes);
	fclose(arquivoInsercoes);

	return numeroRegistros;
}

int carregarRemocoes(ChaveRemocao** vetorRemocoes) {

	FILE* arquivoRemocoes = fopen("remove.bin", "rb");

	if (arquivoRemocoes == NULL) {
		perror("Erro ao abrir remove.bin");
		return 0;
	}

	fseek(arquivoRemocoes, 0, SEEK_END);
	int tamanhoArquivo = ftell(arquivoRemocoes);
	fseek(arquivoRemocoes, 0, SEEK_SET);

	int numeroRegistros = tamanhoArquivo / sizeof(ChaveRemocao);
	*vetorRemocoes = (ChaveRemocao*)malloc(tamanhoArquivo);

	if (*vetorRemocoes == NULL) {
		perror("Erro ao alocar memória para remoções");
		fclose(arquivoRemocoes);
		return 0;
	}

	fread(*vetorRemocoes, sizeof(ChaveRemocao), numeroRegistros, arquivoRemocoes);
	fclose(arquivoRemocoes);

	return numeroRegistros;
}

void inicializarArquivo(const char* nomeArquivo) {

	FILE* arquivo = fopen(nomeArquivo, "rb+");

	if (arquivo == NULL) {
		// Se o arquivo não existir, crie-o e inicialize o cabeçalho
		arquivo = fopen(nomeArquivo, "wb");
		if (arquivo == NULL) {
			perror("Erro ao criar o arquivo");
			exit(EXIT_FAILURE);
		}

		CabecalhoArquivo cabecalho;
		cabecalho.inicioLista = -1;
		fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);
	}
	fclose(arquivo);
}

CabecalhoArquivo buscarCabecalho(FILE* arquivo) {
	CabecalhoArquivo cabecalho;
	fseek(arquivo, 0, SEEK_SET);
	fread(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);

	return cabecalho;
}

void inserirRegistro(FILE* arquivo, Registro* novoRegistro) {

	CabecalhoArquivo cabecalho = buscarCabecalho(arquivo);

	// colocar registro no buffer
	char buffer[256];
	sprintf(buffer, "%s#%s#%s#%s#%.2f#%.2f",
		novoRegistro->idAluno, novoRegistro->siglaDisciplina,
		novoRegistro->nomeAluno, novoRegistro->nomeDisciplina,
		novoRegistro->media, novoRegistro->frequencia);

	int tamanhoRegistro = strlen(buffer) + 1 + sizeof(int); //buffer + \0 + tamanho do registro

	// usar first-fit para procurar espaço livre
	NoEspacoLivre espacoLivre;
	int offsetAnterior = -1;
	int offsetAtual = cabecalho.inicioLista;
	while (offsetAtual != -1) {
		fseek(arquivo, offsetAtual, SEEK_SET);
		fread(&espacoLivre, sizeof(NoEspacoLivre), 1, arquivo);

		if (espacoLivre.tamanho >= tamanhoRegistro) {
			// Encontrou um espaço livre adequado
			if (offsetAnterior == -1) {
				cabecalho.inicioLista = espacoLivre.prox;
			}
			else {
				NoEspacoLivre noAnterior;
				fseek(arquivo, offsetAnterior, SEEK_SET);
				fread(&noAnterior, sizeof(NoEspacoLivre), 1, arquivo);
				noAnterior.prox = espacoLivre.prox;
				fseek(arquivo, offsetAnterior, SEEK_SET);
				fwrite(&noAnterior, sizeof(NoEspacoLivre), 1, arquivo);
			}

			fseek(arquivo, offsetAtual, SEEK_SET);
			fwrite(&tamanhoRegistro, sizeof(int), 1, arquivo);
			fwrite(buffer, tamanhoRegistro - sizeof(int), 1, arquivo);

			fseek(arquivo, 0, SEEK_SET);
			fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);
			return;
		}
		offsetAnterior = offsetAtual;
		offsetAtual = espacoLivre.prox;
	}

	// Se nenhum espaço livre foi encontrado, inserir no final do arquivo
	fseek(arquivo, 0, SEEK_END);
	fwrite(&tamanhoRegistro, sizeof(int), 1, arquivo);
	fwrite(buffer, tamanhoRegistro - sizeof(int), 1, arquivo);
}

void removerRegistro(FILE* arquivo, const char* idAluno, const char* siglaDisciplina) {

	CabecalhoArquivo cabecalho = buscarCabecalho(arquivo);

	char chave[8];
	sprintf(chave, "%s%s", idAluno, siglaDisciplina);

	int offsetAtual = sizeof(CabecalhoArquivo);

	int tamanhoRegistro;
	while (fread(&tamanhoRegistro, sizeof(int), 1, arquivo) == 1) {

		char buffer[256];
		fread(buffer, tamanhoRegistro - sizeof(int), 1, arquivo);

		ChaveRemocao chaveRemocao;
		strcpy(chaveRemocao.idAluno, strtok(buffer, "#"));
		strcpy(chaveRemocao.siglaDisciplina, strtok(NULL, "#"));

		if (strncmp(strcat(chaveRemocao.idAluno, chaveRemocao.siglaDisciplina), chave, 6) == 0) {
			
			// Encontrou o registro, marcar como espaço livre
			fseek(arquivo, offsetAtual, SEEK_SET);
			NoEspacoLivre novoEspaco;
			novoEspaco.offset = offsetAtual;
			novoEspaco.tamanho = tamanhoRegistro;
			novoEspaco.prox = cabecalho.inicioLista;

			//TODO: Arrumar para escrever o tamanho*proximo do jeito que a prof pediu
			fwrite(&novoEspaco, sizeof(NoEspacoLivre), 1, arquivo);
			cabecalho.inicioLista = offsetAtual;

			fseek(arquivo, 0, SEEK_SET);
			fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);

			printf("\nRegistro removido.\n");

			return;
		}

		offsetAtual += tamanhoRegistro;
		fseek(arquivo, offsetAtual, SEEK_SET);
	}

	printf("Registro não encontrado.\n");
}

void compactarArquivo(const char* nomeArquivo) {
	FILE* arquivo = fopen(nomeArquivo, "rb");
	if (arquivo == NULL) {
		perror("Erro ao abrir o arquivo");
		return;
	}

	FILE* arquivoTemp = fopen("temp.bin", "wb");
	if (arquivoTemp == NULL) {
		perror("Erro ao criar arquivo temporário");
		fclose(arquivo);
		return;
	}

	CabecalhoArquivo cabecalho;
	fread(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivo);
	fwrite(&cabecalho, sizeof(CabecalhoArquivo), 1, arquivoTemp);

	int tamanhoRegistro;
	while (fread(&tamanhoRegistro, sizeof(int), 1, arquivo) == 1) {
		char buffer[256];
		fread(buffer, tamanhoRegistro - sizeof(int), 1, arquivo);

		if (buffer[0] != '*') {
			fwrite(&tamanhoRegistro, sizeof(int), 1, arquivoTemp);
			fwrite(buffer, tamanhoRegistro - sizeof(int), 1, arquivoTemp);
		}
	}

	fclose(arquivo);
	fclose(arquivoTemp);
	remove(nomeArquivo);
	rename("temp.bin", nomeArquivo);

	printf("\nCompactação concluída.\n");
}