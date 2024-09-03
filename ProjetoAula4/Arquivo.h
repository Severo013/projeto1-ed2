typedef struct {
    char idAluno[4];
    char siglaDisciplina[4];
    char nomeAluno[50];
    char nomeDisciplina[50];
    float media;
    float frequencia;
} Registro;

typedef struct NoEspacoLivre {
    int tamanho; // Tamanho do espa�o livre
    const char marcador = '*'; // Marcador para indicar que o espa�o est� livre
    int prox; // Offset para o pr�ximo n� na lista encadeada (-1 se n�o houver pr�ximo)
} NoEspacoLivre;

typedef struct {
    int inicioLista; // Offset do primeiro n� da lista encadeada de espa�os livres
    int numeroInsercoes;
    int numeroRemocoes;
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
void inserirRegistro(const char* nomeArquivo, Registro* novoRegistro);
void EscreverCabecalho(FILE* arquivo, CabecalhoArquivo& cabecalho);
void EscreverRegistro(int& tamanhoRegistro, FILE* arquivo, char  buffer[256]);
void removerRegistro(const char* nomeArquivo, ChaveRemocao chaveRemocao);
void compactarArquivo(const char* nomeArquivo);