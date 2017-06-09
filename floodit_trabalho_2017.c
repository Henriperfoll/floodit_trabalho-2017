//Trabalho de 2017 da matéria de algoritmos mestrado PPGInf UFPR
//Aluno: Henrique Perfoll Neto
//E-mail: hperfoll@gmail.com

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
  int nlinhas;
  int ncolunas;
  int ncores;
  int **mapa;
} tmapa;

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

  printf("%d %d %d\n\n", m->nlinhas, m->ncolunas, m->ncores);
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

int verifica(tmapa *m, int l, int c, int fundo, int parede, int cor){
    int i,j;
    for(i=0;i<fundo;i++){
        for(j=0;j<parede;j++){
            if(m->mapa[i][j] != cor){
                return -1;
            }
        }
    }
    return 1;
}

int verifica_mapa(tmapa *m){
    int cor;
    cor = m->mapa[0][0];
    return verifica(m,0,0,m->nlinhas,m->ncolunas,cor);
}


int main(int argc, char **argv) {
    int cor,semente, verificou;
    tmapa m;

    printf("Bem vindo ao Floodit - Trabalho de 2017\n");
    printf("\nInsera os valores:<Numero de Linhas><Numero de colunas><Numero de Cores>\n");
    scanf("%d", &m.nlinhas);
    scanf("%d", &m.ncolunas);
    scanf("%d", &m.ncores);

    printf("\n %d - %d - %d \n",m.nlinhas,m.ncolunas,m.ncores);
    semente = -1;
    gera_mapa(&m, semente);
    //verificou = -1;
    do{
        mostra_mapa(&m);
        //verificou = verifica_mapa(&m);
        //printf("\n%d\n",verificou);
        cor = pergunta_cor();
        pinta_mapa(&m, cor);
    }while(cor > 0 && cor <= m.ncores && (verifica_mapa(&m) < 0));

    mostra_mapa(&m);

    return 0;
}
