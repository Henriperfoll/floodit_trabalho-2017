//Trabalho de 2017 da matéria de algoritmos mestrado PPGInf UFPR
//Aluno: Henrique Perfoll Neto
//E-mail: hperfoll@gmail.com

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct {
  int nlinhas;
  int ncolunas;
  int ncores;
  int **mapa;
} tmapa;

typedef struct celula{
    int linha;
    int coluna;
    int cor;
    struct celula *prox;
};

typedef struct play{
    int cor;
    struct play *prox;
};

typedef struct celula borda;
typedef struct play jogada;

void gera_mapa(tmapa *m, int semente) {
  int i, j;

  if(semente < 0)
    srand(time(NULL));
  else
    srand(semente);
  m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
  for(i = 0; i < m->nlinhas; i++) {
    m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
    for(j = 0; j < m->ncolunas; j++)
      m->mapa[i][j] = 1 + rand() % m->ncores;
  }
}

void mostra_mapa(tmapa *m) {
  int i, j;

  printf("\n%d %d %d\n\n", m->nlinhas, m->ncolunas, m->ncores);
  for(i = 0; i < m->nlinhas; i++) {
    for(j = 0; j < m->ncolunas; j++)
      if(m->ncores > 10)
	printf("%02d ", m->mapa[i][j]);
      else
	printf("%d ", m->mapa[i][j]);
    printf("\n");
  }
}

void pinta(tmapa *m, int l, int c, int fundo, int cor) {
  m->mapa[l][c] = cor;
  if(l < m->nlinhas - 1 && m->mapa[l+1][c] == fundo)
    pinta(m, l+1, c, fundo, cor);
  if(c < m->ncolunas - 1 && m->mapa[l][c+1] == fundo)
    pinta(m, l, c+1, fundo, cor);
  if(l > 0 && m->mapa[l-1][c] == fundo)
    pinta(m, l-1, c, fundo, cor);
  if(c > 0 && m->mapa[l][c-1] == fundo)
    pinta(m, l, c-1, fundo, cor);
}

void pinta_mapa(tmapa *m, int cor) {
  if(cor == m->mapa[0][0])
    return;
  pinta(m, 0, 0, m->mapa[0][0], cor);
}

int pergunta_cor(){
    int x;
    printf("Digite uma cor:");
    scanf("%d",&x);
    return x;
}

void insere_borda(int x,int l,int c, borda *p){
   borda *nova;
   nova = malloc(sizeof (borda));
   nova->cor = x;
   nova->coluna = c;
   nova->linha = l;
   nova->prox = p->prox;
   p->prox = nova;
}

void insere_jogada(int x, jogada *p){
   jogada *nova;
   nova = malloc(sizeof (jogada));
   nova->cor = x;
   nova->prox = p->prox;
   p->prox = nova;
}

void imprime_borda (borda *le) {
   borda *p;
   int x;
   for (p = le->prox; p != NULL; p = p->prox){
      printf ("->%d ->%d, ->%d\n", p->cor, p->linha, p->coluna);
   }
}

void imprime_jogadas (jogada *le) {
   jogada *p;
   for (p = le->prox; p != NULL; p = p->prox){
      printf ("%d ", p->cor);
   }
}

void remove_borda(borda *p)
{
   borda *lixo;
   lixo = p->prox;
   p->prox = lixo->prox;
   free (lixo);
}

void remove_tudo_borda(borda *p){
   while (p->prox != NULL){
        remove_borda(p);
   }
}

void remove_jogada(jogada *p)
{
   jogada *lixo;
   lixo = p->prox;
   p->prox = lixo->prox;
   free (lixo);
}

void remove_todas_jogadas(jogada *p){
   while (p->prox != NULL){
        remove_borda(p);
   }
}

int verifica_R(tmapa *m, int l, int c, int cor, borda *b){
    int x,z;
    if(m->mapa[l][c] != cor){
        insere_borda(m->mapa[l][c],l,c,b);
        m->mapa[l][c] = -1;
        return -1;
    }
    if((l+1) < m->nlinhas && m->mapa[l+1][c] != -1)
        x = verifica_R(m,l+1,c,cor,b);
    if((c+1) < m->ncolunas && m->mapa[l][c+1] != -1)
        z = verifica_R(m,l,c+1,cor,b);
    if(x+z < 0)
        return -1;
    return 0;
}

int verifica_borda(tmapa *m, borda *b){
    tmapa *x = NULL;
    remove_tudo_borda(b);
    verifica_R(m,0,0,m->mapa[0][0],b);
    return 0;
}

void quantidade_borda(tmapa *m, borda *b, int *qtd){
    int quantidade[m->ncores],i,maior;
    maior = 0;
    for (i = 0; i < m->ncores; i++){
        qtd[i] = 0;
    }
    borda *aux;
    for (aux = b->prox; aux != NULL; aux = aux->prox){
      qtd[aux->cor-1]++;
      m->mapa[aux->linha][aux->coluna] = aux->cor;
    }
}

void ajusta_prioridades(int *prior, int jogado, int tam){
    int i;
    for (i=0;i<tam;i++){
        if(i == jogado-1){
            prior[i] = 1;
        }else{
            prior[i]++;
        }
    }
}

int proxima_jogada(int *prior,int *qtd,int tam){
    int jogada,i;
    jogada = 0;
    for(i=1;i<tam;i++){
        if((qtd[i]*prior[i]) > (qtd[i-1]*prior[i-1])){
            jogada = i;
        }
    }
    return jogada+1;
}

int grava_jogadas(int qtd_jogadas,jogada *jogadas){
    FILE *ptr_file;
    int x;
    char buffer[128];
    ptr_file =fopen("floodit_jogadas.txt", "w");
    if (!ptr_file)
        return 1;
    fprintf(ptr_file,"%d\n", qtd_jogadas);
    jogada *p;
    for (p = jogadas->prox; p != NULL; p = p->prox){
        fprintf(ptr_file,"%d ", p->cor);
    }
    fprintf(ptr_file,"\n");
    fclose(ptr_file);

    return  0;
}

int carrega_mapa_arquivo(tmapa *m){
    FILE* f = fopen("mapa_floodit.txt", "r");
    int j,i;
    fscanf(f, "%d,", &m->nlinhas);
    fscanf(f, "%d,", &m->ncolunas);
    fscanf(f, "%d,", &m->ncores);
    m->mapa = (int**) malloc(m->nlinhas * sizeof(int*));
    for(i=0;i<m->nlinhas;i++){
        m->mapa[i] = (int*) malloc(m->ncolunas * sizeof(int));
        for(j=0;j<m->ncolunas;j++){
            fscanf(f, "%d,", &m->mapa[i][j]);
        }
    }
    fclose(f);
}

void jogar_manualmente(){
    int cor,semente, i, jogada;
    tmapa m;
    borda *b;

    printf("Bem vindo ao Floodit - Trabalho de 2017\n");
    printf("\nInsera os valores:<Numero de Linhas><Numero de colunas><Numero de Cores>\n");
    b = malloc(sizeof (borda));
    b->prox = NULL;
    scanf("%d", &m.nlinhas);
    scanf("%d", &m.ncolunas);
    scanf("%d", &m.ncores);
    int prioridades[m.ncores], quantidade[m.ncores];
    for (i = 0; i < m.ncores; i++){
        prioridades[i] = 1;
    }
    semente = -1;
    gera_mapa(&m, semente);

    printf("\n %d - %d - %d \n",m.nlinhas,m.ncolunas,m.ncores);
    do{
        mostra_mapa(&m);
        cor = pergunta_cor();
        ajusta_prioridades(prioridades,cor-1,m.ncores);
        pinta_mapa(&m, cor);
        verifica_borda(&m, b);
        if(b->prox!=NULL){
            quantidade_borda(&m,b,quantidade);
            jogada = proxima_jogada(prioridades,quantidade,m.ncores);
            printf("\n\nSugestão de proxima jogada: %d \n",jogada);
        }
    }while(cor > 0 && cor <= m.ncores && (b->prox!=NULL));

    remove_tudo_borda(b);
    mostra_mapa(&m);
}

int jogar_pelo_arquivo(){
    tmapa m;
    carrega_mapa_arquivo(&m);
    int cor,i,nJogadas,prioridades[m.ncores],quantidade[m.ncores];
    borda *b;
    jogada *game;
    b = malloc(sizeof (borda));
    b->prox = NULL;
    game = malloc(sizeof(jogada));
    game->prox = NULL;
    cor = 1 + rand() % m.ncores;
    for (i = 0; i < m.ncores; i++){
        prioridades[i] = 1;
    }
    printf("-Seu mapa-");
    mostra_mapa(&m);
    printf("\n\nJogadas: ");
    nJogadas = 0;
    do{
        //printf("%d ",cor);
        insere_jogada(cor,game);
        nJogadas++;
        pinta_mapa(&m, cor);
        ajusta_prioridades(prioridades,cor-1,m.ncores);
        verifica_borda(&m, b);
        if(b->prox!=NULL){

            quantidade_borda(&m,b,quantidade);
            cor = proxima_jogada(prioridades,quantidade,m.ncores);

        }
    }while(cor > 0 && cor <= m.ncores && (b->prox!=NULL));
    imprime_jogadas(game);
    remove_tudo_borda(b);
    mostra_mapa(&m);
    printf("\n--%d",nJogadas);

    grava_jogadas(nJogadas,game);

    return  0;
}

int main(int argc, char **argv) {
    int opcao;
    printf("Maneira de jogar: 1 - Gerar mapa; 2 - Ler arquivo mapa_floodit.txt\n");
    scanf("%d", &opcao);
    switch(opcao){
        case 1:
            jogar_manualmente();
            break;
        case 2:
            printf("-%d\n",jogar_pelo_arquivo());
            break;
        default:
            printf("Finalizado!");
    }
    return 0;
}
