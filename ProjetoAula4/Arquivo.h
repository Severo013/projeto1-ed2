typedef struct {
    char idAluno[4];
    char siglaDisciplina[4];
    char nomeAluno[50];
    char nomeDisciplina[50];
    float media;
    float frequencia;
} Registro;

typedef struct NoEspacoLivre {
    int offset; // Posição do espaço livre no arquivo
    int tamanho; // Tamanho do espaço livre
    int prox; // Offset para o próximo nó na lista encadeada (-1 se não houver próximo)
} NoEspacoLivre;

typedef struct {
    int inicioLista; // Offset do primeiro nó da lista encadeada de espaços livres
} CabecalhoArquivo;

typedef struct {
    char idAluno[4];
    char siglaDisciplina[4];
} ChaveRemocao;

//Assinar funções
int carregarInsercoes(Registro** vetorInsercoes);
int carregarRemocoes(ChaveRemocao** vetorRemocoes);
void inicializarArquivo(const char* nomeArquivo);
CabecalhoArquivo buscarCabecalho(FILE* arquivo);
void inserirRegistro(FILE* arquivo, Registro* novoRegistro);
void removerRegistro(FILE* arquivo, const char* idAluno, const char* siglaDisciplina);
void compactarArquivo(const char* nomeArquivo);