#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

typedef short unsigned Shu;

struct graphe {
  int nbs;
  Shu * tab;
} ;
typedef struct graphe graphe;

struct composantConnex {
  //int nbs;
  int poids;
  int noeud;
  //Shu * tab;
} ;
typedef struct composantConnex composantConnex_t;

struct mco {
  int nbs;
  int poids; // to cahnge later
  int noeud;
 // int noeud;
  composantConnex_t * tab;
} ;
typedef struct mco MCO_t;


graphe creegraphe (int nbs) {
  Shu i, j, max, num;
  float v, taux;
  graphe g;
  g.nbs = nbs;
  max = nbs * nbs;
  taux = 25.0;
  num = nbs / 10;
  while (num > 1) {
  num /= 5;
  taux /= 3.0;
  }
  taux /= 100.0;
  printf("taux %g\n", taux);
  g.tab = (Shu *) malloc (max * sizeof(Shu));
  memset(g.tab, 0, max);
  srand(time(NULL));
  for (num = 0, i = 0; i < nbs; i++)
  for (j = 0; j < nbs; j++) {
    v = (float) rand () / RAND_MAX;
    g.tab[num++] = v < taux ? (Shu) (v * 1000.) : 0;
  }
  return g;
}


void voirgraf (graphe g) {
  Shu i, j, nb, num;
  nb = g.nbs;
  printf("Graphe\n");
  for (i = 0, num = 0; i < nb; i++) {
	for (j = 0; j < nb; j++)
	 // printf("%c ", g.tab[num++]? '1' : ' ');
     printf("%d ", g.tab[num++]);
	printf("\n");
  }
}

MCO_t * trouverMatriceCompacte(graphe g, int nb){

  MCO_t  *MCO = (MCO_t *) malloc (nb * sizeof(MCO_t));
  int i,j,cpt;
  for(i=0;i<nb;i++){
      cpt=0;
      // calculer le nombre de conenxion par ligne
      for(j=0;j<nb;j++){
        if(g.tab[i*nb+j] != 0)
          cpt++;
        }

    // ccréer le tableau de chaque case de la matrice compacte
    MCO[i].nbs = cpt;
    MCO[i].noeud = i;

    MCO[i].tab = (composantConnex_t *) malloc (cpt * sizeof(composantConnex_t));

    // remplir chaque case de la matrice compacte
    int k;
    k=0;
    printf("\n");
    for(j=0;j<nb;j++){
      if(g.tab[i*nb+j] != 0){
        MCO[i].tab[k].poids = g.tab[i*nb+j];
        MCO[i].tab[k].noeud = j;
        k++;
      }
    }

    MCO[i].nbs = k;
    printf("\n");
    }

  return  MCO;

}

MCO_t * trouverComposantConnexe(MCO_t * MCO, int nb){
  MCO_t * composant_connexe = (MCO_t *) malloc (nb * sizeof(MCO_t));
  int nonVisite[nb], aVisiter[nb], count, totalCount,cptsuccesseurs,indvisiter,nComposant,i,j;
  int new[nb];

  totalCount = nb;
  // initialiser tout les noeud non visité au début
  for(i=0;i<nb;i++){
    nonVisite[i] = i;
  }

  int nComposantTab;
  nComposant =-1;
  int idc;
  int k=-1;

  while(totalCount > 0){ // tant que la viste des noeud non visité est pleine
    nComposant++;
    k++;
    for(i=0;i<nb;i++){
    nonVisite[i] = i;
  }
    aVisiter[0] = k;
    nonVisite[k]=-1;

        totalCount--;
    
    nComposantTab = 0;
    // je crée le tableau ou je met chaque composant connexe
    composant_connexe[nComposant].nbs = nb;
    composant_connexe[nComposant].tab = (composantConnex_t *) malloc (nb * sizeof(composantConnex_t));

    // juste une initialisation pour savoir ce qui se passe
    for(i=0;i<nb;i++)
      composant_connexe[nComposant].tab[i].noeud = -1;

    idc = 0;
    count = 1;
    cptsuccesseurs=1;
    int c=0;
    int p;

    while(cptsuccesseurs > 0){ // tant qu'il a des successeurs à visiter, liste est pleine mais successeur est 0
      cptsuccesseurs = 0;
      c = 0;
          indvisiter=0;
      for(i=0;i<count;i++){

            int t ;
        for(j=0;j< MCO[aVisiter[i]].nbs;j++){ 

          idc = MCO[aVisiter[i]].tab[j].noeud;
          t = aVisiter[i];

          // si le noeud est dejà visité 
          if(nonVisite[idc] != -1){
            c++;
            new[indvisiter] = idc;
            composant_connexe[nComposant].tab[nComposantTab].noeud = MCO[t].tab[j].noeud;
            composant_connexe[nComposant].tab[nComposantTab].poids = MCO[t].tab[j].poids;
            nComposantTab++;
            nonVisite[idc] = -1; // met -1 pour éviter de le revisiter
            cptsuccesseurs++; // inscrementer pour rester dans la boucle tant qu'il y a des successeurs
            indvisiter++;
          }
        }
      }

        for(p=0;p<nb;p++)
          aVisiter[p] = new[p];
      
        count=cptsuccesseurs;
    }
  }
  return composant_connexe; 

}


int * dijkstra(MCO_t * MCO,int nb,  int start , int end) 
{
  int i;

  // pour stouqer les distances et le chemin finale
  int dijkstraMatrix[nb][2]; // une colonne pour stoquer les distances et l'autre pour le chemin

  for(i=0;i<nb;i++){ // juste pour teter
    dijkstraMatrix[i][0] = -1;
    dijkstraMatrix[i][1] = -1;
  }
  
  int currentNoeud;
  int min_index, min_value;
  int nonVisite[nb]; // pour mettre les noeuds non visité, au moment que je visite un noeud , je met -1 pour éviter la recherche en boucle que fois

  for ( i = 0; i < nb; i++)
    nonVisite[i] = i;


    // initialiser tout les distances
  for ( i = 0; i < nb; i++) 
    dijkstraMatrix[i][0]= INT_MAX;

    // Distance of source vertex from itself is always 0 
  dijkstraMatrix[start][0] = 0;

    // ajouter le premier noeud à vsiter 
  currentNoeud = start;

  int countVisited;
  countVisited = 1;


    // generer les successeur et calculer les distances

  int next;

  while(countVisited < nb){

    for(i=0;i<MCO[currentNoeud].nbs;i++){ // je commence par zero pour verifier la première valeur d'initialisation avec la matrice disktra

      next = MCO[currentNoeud].tab[i].noeud; // garder le successeur gagnant
      min_value = dijkstraMatrix[currentNoeud][0]+MCO[currentNoeud].tab[i].poids;

      if(nonVisite[next] != -1){ // verifier si c'est déjà visiter ou non

        // verifier si l'ancien valeur du noeud courant à verifier est supérieur ou non

        if(dijkstraMatrix[next][0] > min_value){

          dijkstraMatrix[next][0] = min_value; // modifier le poids du début au noeud courant et successeur précedant
          dijkstraMatrix[next][1] = currentNoeud;
          min_index = MCO[currentNoeud].tab[i].noeud;
        }
      } 
    }
    countVisited++;
    nonVisite[currentNoeud]  = -1; // marque le noeud comme noeud visité
    currentNoeud = min_index;
  }
    
  int * chemin = (int*) malloc(nb + 1 * sizeof(chemin)); // le +1 c'est pour stoquer le nombre de noeud dans le chemin

  

  int aux;
  aux = dijkstraMatrix[end][1];
  int cheminCount;
  cheminCount = 0;
  chemin[cheminCount] = end;
    
  while(aux != -1){
    cheminCount++;
    chemin[cheminCount] = aux;
    aux = dijkstraMatrix[aux][1];
  }
    // ça me permeterai de calculer le nombre de noeud dans le chemin pour afficher
  chemin[nb]= cheminCount;

  return chemin;
} 

void printPath(int * chemin, int nb){
  int i;
  for(i=chemin[nb];i>=0;i--){
    printf("%d -> ", chemin[i]);
  }
  printf("\n");
}

int verifierChemin(int start, int end, MCO_t*  composant_connexe, int nb){
  int i;
    // verifier s'il y a un chemin ou non
  for(i=0;i<nb;i++)
    if(composant_connexe[start].tab[i].noeud == end) // si la ligne de start contient la fin du chemin ça veut dire il y a un chemin
      return 1;
  return 0;
}


int main () {
  graphe g;
  int nb;
  nb = 10;
  g = creegraphe (nb);
  voirgraf(g);


  MCO_t * MCO;
  int i,j;
  MCO_t * composant_connexe = (MCO_t *) malloc (nb * sizeof(MCO_t)); // je met chaque composant connexe ensemble
  MCO = trouverMatriceCompacte(g,nb); 
  composant_connexe = trouverComposantConnexe(MCO,nb);

  printf("\n\n");
  printf("Matrice Compacte : \n");

  for(i=0;i<nb;i++){
    for(j=0;j<  MCO[i].nbs;j++){
      printf("%d ",MCO[i].tab[j].noeud );
    }
    printf("\n");
  }


  // affichage composants connexes
  for(i=0;i<nb;i++){
    for(j=0;j< nb;j++){
      printf("%d ",composant_connexe[i].tab[j].noeud);
    }
    printf("\n\n\n\n\n");
  }
  printf("\n\n");
  int start,end;

  srand(time(NULL));
  start = (int) rand () % nb;
  end = (int) rand () % nb;
  printf("Chemin  Aléatoire Entre  %d  Et %d \n\n", start,end);
  int * chemin;
  //if(verifierChemin(start,end,composant_connexe,nb) == 1)
  if(verifierChemin(start,end,composant_connexe, nb)){
    chemin = dijkstra(MCO,nb,start , end);
    printf("chemin : \n");
    printPath(chemin,nb);
  }
  else
    printf("pas de chemin\n");

}