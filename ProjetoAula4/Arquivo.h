typedef struct {
    char idAluno[4];
    char siglaDisciplina[4];
    char nomeAluno[50];
    char nomeDisciplina[50];
    float media;
    float frequencia;
} Registro;

typedef struct NoEspacoLivre {
    int tamanho; // Tamanho do espaço livre
    const char marcador = '*'; // Marcador para indicar que o espaço está livre
    int prox; // Offset para o próximo nó na lista encadeada (-1 se não houver próximo)
} NoEspacoLivre;

typedef struct {
    int inicioLista; // Offset do primeiro nó da lista encadeada de espaços livres
    int numeroInsercoes;
    int numeroRemocoes;
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
void inserirRegistro(const char* nomeArquivo, Registro* novoRegistro);
void EscreverCabecalho(FILE* arquivo, CabecalhoArquivo& cabecalho);
void EscreverRegistro(int& tamanhoRegistro, FILE* arquivo, char  buffer[256]);
void removerRegistro(const char* nomeArquivo, ChaveRemocao chaveRemocao);
void compactarArquivo(const char* nomeArquivo);