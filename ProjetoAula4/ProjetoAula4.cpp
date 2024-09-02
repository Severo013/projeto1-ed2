#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arquivo.h"

int main()
{
	Registro* vetorInsercoes;
	ChaveRemocao* vetorRemocoes;

	int numeroRegistroInsere = carregarInsercoes(&vetorInsercoes);
	int numeroRegistroRemove = carregarRemocoes(&vetorRemocoes);

	const char* nomeArquivo = "dados.bin";
	inicializarArquivo(nomeArquivo);

	FILE* arquivo = fopen(nomeArquivo, "rb+");
	if (arquivo == NULL) {
		perror("Erro ao abrir arquivo principal");
		return EXIT_FAILURE;
	}

	int opcao;
	do {
		printf("Menu:\n");
		printf("1. Inserir Registros\n");
		printf("2. Remover Registros\n");
		printf("3. Compactar Arquivo\n");
		printf("4. Sair\n");
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);
		printf("\n");

		int indice;
		switch (opcao) {
		case 1:
			printf("Registros disponíveis: \n");
			for (int i = 0; i < numeroRegistroInsere; i++) {
				printf("%d. %s - %s\n", i, vetorInsercoes[i].idAluno, vetorInsercoes[i].siglaDisciplina);
			}
			printf("\nEscolha um registro para inserir: ");
			scanf("%d", &indice);
			inserirRegistro(arquivo, &vetorInsercoes[indice]);
			break;
		case 2:
			printf("Registros disponíveis: \n");
			for (int i = 0; i < numeroRegistroRemove; i++) {
				printf("%d. %s - %s\n", i, vetorRemocoes[i].idAluno, vetorRemocoes[i].siglaDisciplina);
			}
			printf("Escolha um registro para remover: ");
			scanf("%d", &indice);
			removerRegistro(arquivo, vetorRemocoes[indice].idAluno, vetorRemocoes[indice].siglaDisciplina);
			break;
			case 3:
			    compactarArquivo(nomeArquivo);
			    break;
		case 4:
			printf("Saindo...\n");
			break;
		default:
			printf("Opção inválida!\n");
			break;
		}
	} while (opcao != 4);

	free(vetorInsercoes);
	free(vetorRemocoes);
	fclose(arquivo);
	return 0;
}
