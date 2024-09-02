typedef struct {
    char idAluno[4];
    char siglaDisciplina[4];
    char nomeAluno[50];
    char nomeDisciplina[50];
    float media;
    float frequencia;
} Registro;

typedef struct NoEspacoLivre {
    int offset; // Posi��o do espa�o livre no arquivo
    int tamanho; // Tamanho do espa�o livre
    int prox; // Offset para o pr�ximo n� na lista encadeada (-1 se n�o houver pr�ximo)
} NoEspacoLivre;

typedef struct {
    int inicioLista; // Offset do primeiro n� da lista encadeada de espa�os livres
} CabecalhoArquivo;

typedef struct {
    char idAluno[4];
    char siglaDisciplina[4];
} ChaveRemocao;

//Assinar fun��es
int carregarInsercoes(Registro** vetorInsercoes);
int carregarRemocoes(ChaveRemocao** vetorRemocoes);
void inicializarArquivo(const char* nomeArquivo);
CabecalhoArquivo buscarCabecalho(FILE* arquivo);
void inserirRegistro(FILE* arquivo, Registro* novoRegistro);
void removerRegistro(FILE* arquivo, const char* idAluno, const char* siglaDisciplina);
void compactarArquivo(const char* nomeArquivo);