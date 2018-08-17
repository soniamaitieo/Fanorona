#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define HAUTEUR 5
#define LARGEUR 9
#define VIDE 0
#define NOIR 1
#define BLANC 2

/* Definition des structures */


typedef struct {
  int lig, col;
}position;

typedef struct {
  position pos_i, pos_f; // pos_i : position initiale
}deplacement;          // pos_f : position finale
 
typedef struct {
  int plateau[HAUTEUR][LARGEUR];//Etat du plateau
} plateau_jeu;
    
    
enum direction {N, S, E, W, NE, NW, SE, SW};

/* Initialisation  et remplissage du plateau */

void initialiser_plateau_vide(int plateau[HAUTEUR][LARGEUR]){
  int i, j;
  for (i=0; i<HAUTEUR; i++){
    for (j=0; j<LARGEUR; j++){
      plateau[i][j]=VIDE;
    }
  }
}



void placer_pions(int plateau[HAUTEUR][LARGEUR]){
  int i, j;
  for (i=0; i<HAUTEUR/2; i++){
    for (j=0; j<LARGEUR; j++){
      plateau[i][j]=NOIR;
      plateau[i+HAUTEUR/2+1][j]=BLANC;
    }}
  int milieu = HAUTEUR/2;
  for (j=0; j<LARGEUR/2; j++){
    if(j%2==0){
      plateau[milieu][j]=NOIR;}
    else{
      plateau[milieu][j]=BLANC;}
    plateau[milieu][j+LARGEUR/2+1]=plateau[milieu][j];
  }
}
      
/* Affichage du plateau */         

void afficher_plateau(int plateau[HAUTEUR][LARGEUR]){
  int i, j,t;
  printf("\n");
  for (i=0; i<HAUTEUR; i++){
    if (i!= HAUTEUR -1){
      for (j=0; j<LARGEUR; j++)
        { if (j!=LARGEUR-1){
            
            printf(" %d ", plateau[i][j]);
            printf("-");
	  }else if(j==LARGEUR-1){
            printf(" %d ", plateau[i][j]);
	  }}
      printf("\n");
      for (t=0; t<LARGEUR-1; t++)
	{
	  if ((i+t)%2==0)
	    {printf(" | \\");}      
	  if ((i+t)%2!=0)
	    {printf(" | /");}
	}
      printf(" | ");
      printf("\n");   
    }else if(i==HAUTEUR -1){
      for (j=0; j<LARGEUR; j++)
        { if (j!=LARGEUR-1){
            
            printf(" %d ", plateau[i][j]);
            printf("-");
	  }else if(j==LARGEUR-1){
            printf(" %d ", plateau[i][j]);
	  }
	}
    }
  } 
  printf("\n");
  printf("\n");
}




/* fonction qui pointe vers le déplacement : donne la position après déplacement du pion */

deplacement* position_suivante(deplacement* pos, enum direction dir){
  switch (dir) {
  case N : pos->pos_f.lig = pos->pos_i.lig - 1; 
    pos->pos_f.col = pos->pos_i.col; break;
  case S : pos->pos_f.lig = pos->pos_i.lig + 1; 
    pos->pos_f.col = pos->pos_i.col; break;
  case E : pos->pos_f.col = pos->pos_i.col + 1; 
    pos->pos_f.lig = pos->pos_i.lig; break;
  case W : pos->pos_f.col = pos->pos_i.col - 1; 
    pos->pos_f.lig = pos->pos_i.lig; break;
  case NE : pos->pos_f.lig = pos->pos_i.lig - 1;
    pos->pos_f.col = pos->pos_i.col + 1; break;
  case NW : pos->pos_f.lig = pos->pos_i.lig - 1;
    pos->pos_f.col = pos->pos_i.col - 1; break;
  case SE : pos->pos_f.lig = pos->pos_i.lig + 1;
    pos->pos_f.col = pos->pos_i.col + 1; break;
  case SW : pos->pos_f.lig = pos->pos_i.lig + 1;
    pos->pos_f.col = pos->pos_i.col - 1; break;
  }
  return pos;
}



//Deplacement du pion dans le plateau de jeu

void deplace_pion(deplacement* d, plateau_jeu* jeu, int joueur_courant){  
  jeu->plateau[d->pos_i.lig][d->pos_i.col] = VIDE;
  jeu->plateau[d->pos_f.lig][d->pos_f.col] = joueur_courant;
}


//Conditions pour ne pas faire des déplacements illégaux 

int test_deplacement(deplacement* d, int plateau_jeu[HAUTEUR][LARGEUR], enum direction dir ){
  if ((((d->pos_i.col)+ (d->pos_i.lig))%2!=0) & ((dir == SE) || ( dir == NE ) || (dir==SW) || (dir==NW))) { 
    return 0 ;}
  else { return 1 ; }}

//Conditions pour ne pas se déplacer quand la case n'est pas vide

int est_occupe(deplacement* d, int plateau_jeu[HAUTEUR][LARGEUR]){
  if ((d->pos_f.lig > HAUTEUR - 1 || d->pos_f.lig < 0) || (d->pos_f.col > LARGEUR - 1 || d->pos_f.col < 0)){
    return 0;
  }
  if (plateau_jeu[d->pos_f.lig][d->pos_f.col] != VIDE ){ 
    return 0; 
  }else if(plateau_jeu[d->pos_f.lig][d->pos_f.col] == VIDE){
    return 1;
  }
  return 0;
}

// Detection Capture approche

int capture_approche(deplacement*d, enum direction dir, int plateau[HAUTEUR][LARGEUR], int joueur_courant){
  deplacement *dsuiv=malloc(sizeof(deplacement)); 
  dsuiv ->  pos_i.lig= d -> pos_f.lig ;
  dsuiv ->  pos_i.col= d -> pos_f.col; 
  dsuiv=position_suivante(dsuiv,dir);                      //simule déplacement dans la même direction  
  if (dsuiv->pos_f.lig >= 0 && dsuiv->pos_f.lig < HAUTEUR && dsuiv->pos_f.col >= 0 && dsuiv->pos_f.col < LARGEUR){  //pour ne pas sortir du plateau
    if(plateau[dsuiv->pos_f.lig][dsuiv->pos_f.col]!=VIDE && plateau[dsuiv->pos_f.lig][dsuiv->pos_f.col]!=joueur_courant){  // il faut que la position finale de la simution soit égale à un pion adverse
      return 1;
    }
    return 0;
           
  }   
  return 0;
}





int direction_opposee ( enum direction dir) {
  switch (dir) {
  case N : return ( S ); break;
  case S : return ( N ); break;
  case E : return ( W ); break;
  case W : return ( E ); break;
  case NE : return ( SW ); break;
  case NW : return ( SE ); break;
  case SE : return ( NW ); break;
  case SW : return ( NE ); break;
  }
  return -1;
}



/* Detection Capture approche*/

int capture_retrait(deplacement*d, enum direction dir, int plateau[HAUTEUR][LARGEUR], int joueur_courant)
{
  deplacement *dopp=malloc(sizeof(deplacement));
  dopp -> pos_i.lig = d -> pos_i.lig ;
  dopp -> pos_i.col = d -> pos_i.col ;
  int diropp=direction_opposee(dir);
  dopp=position_suivante(dopp,diropp); //simule déplacement dans la direction  opposée
  if (dopp->pos_f.lig >= 0 && dopp->pos_f.lig < HAUTEUR && dopp->pos_f.col >= 0 && dopp->pos_f.col < LARGEUR){
    if(plateau[dopp->pos_f.lig][dopp->pos_f.col]!=VIDE && plateau[dopp->pos_f.lig][dopp->pos_f.col]!=joueur_courant){ // il faut que la position finale de la simution soit égale à un pion adverse
      return 1;
    }
    return 0;
  }
  return 0;
}



/* Detection Capture dans l'ensemble du plateau*/

int capture_possible_plateau(int joueur_courant, int plateau[HAUTEUR][LARGEUR]){
  deplacement *d= malloc(sizeof(deplacement));
  int approche_possible;
  int occupe, testdep;
  int retrait_possible;
  int i, j;
  for (i=0; i<HAUTEUR; i++){
    for(j=0; j<LARGEUR; j++){
      if (plateau[i][j]==joueur_courant){
	d->pos_i.lig=i;
	d->pos_i.col=j;
	d=position_suivante(d, 0);    //simule position suivante nord
	occupe=est_occupe(d, plateau);             //case occupée ?
	testdep=test_deplacement(d, plateau, 0);	//deplacement valide ?
	if(occupe==1 && testdep==1){				//case vide + deplacement valide
	  approche_possible=capture_approche(d, 0, plateau, joueur_courant);  // detection approche 
	  retrait_possible= capture_retrait(d, 0, plateau, joueur_courant);  // detection retrait
	  if(approche_possible==1 || retrait_possible==1){			//  une des captures possibles détectée
	    return 1;}}

	d=position_suivante(d, 1);    // simule position suivante sud
	occupe=est_occupe(d, plateau);
	testdep=test_deplacement(d, plateau, 1);
	if(occupe==1 && testdep==1){
	  d=position_suivante(d, 1);
	  approche_possible=capture_approche(d, 1, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 1, plateau, joueur_courant);
	  if(approche_possible==1 || retrait_possible==1){
	    return 1;}}

	d=position_suivante(d, 2);    // simule position suivante est
	occupe=est_occupe(d, plateau);
	testdep=test_deplacement(d, plateau, 2);                
	if(occupe==1 && testdep==1){
	  approche_possible=capture_approche(d, 2, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 2, plateau, joueur_courant);
	  if(approche_possible==1 || retrait_possible==1){
	    return 1;}}

	d=position_suivante(d, 3);    //simule position suivante ouest
	occupe=est_occupe(d, plateau);
	if(occupe==1 && testdep==1){  
	  testdep=test_deplacement(d, plateau, 3);                    
	  approche_possible=capture_approche(d, 3, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 3, plateau, joueur_courant);
	  if(approche_possible==1 || retrait_possible==1){
	    return 1;}}

	d=position_suivante(d, 4);   //simule position suivante Nord-est
	occupe=est_occupe(d, plateau);
	testdep=test_deplacement(d, plateau, 4);                
	if(occupe==1 && testdep==1){  
	  approche_possible=capture_approche(d, 4, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 4, plateau, joueur_courant);
	  if(approche_possible==1 || retrait_possible==1){
	    return 1;}}

	d=position_suivante(d, 5);   //simule position suivante Nord-ouest
	occupe=est_occupe(d, plateau);
	testdep=test_deplacement(d, plateau, 5);                
	if(occupe==1 && testdep==1){
	  approche_possible=capture_approche(d, 5, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 5, plateau, joueur_courant);
	  if(approche_possible==1 || retrait_possible==1){
	    return 1;}}

	d=position_suivante(d, 6);  //simule position suivante Sud-est
	occupe=est_occupe(d, plateau);
	testdep=test_deplacement(d, plateau, 6);                
	if(occupe==1 && testdep==1){
	  approche_possible=capture_approche(d, 6, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 6, plateau, joueur_courant);
	  if(approche_possible==1 || retrait_possible==1){
	    return 1;}}

	d=position_suivante(d, 7);  //simule position suivante Sud-ouest
	occupe=est_occupe(d, plateau);
	testdep=test_deplacement(d, plateau, 7);                
	if(occupe==1 && testdep==1){
	  approche_possible=capture_approche(d, 7, plateau, joueur_courant);
	  retrait_possible= capture_retrait(d, 7, plateau, joueur_courant);
	  if(approche_possible==1|| retrait_possible==1){
	    return 1;}}}}}
  return 0;
}




void pions_manges_app(plateau_jeu* jeu, deplacement*d, enum direction dir, int joueur_courant){   // Capture des pions par approche
  deplacement *dsuiv=malloc(sizeof(deplacement));
  dsuiv -> pos_i.lig = d -> pos_f.lig ;
  dsuiv -> pos_i.col = d -> pos_f.col ; 
  dsuiv=position_suivante(dsuiv,dir);
  while (jeu-> plateau[dsuiv->pos_f.lig ][dsuiv->pos_f.col]!=VIDE && jeu-> plateau[dsuiv->pos_f.lig ][dsuiv->pos_f.col]!=joueur_courant && dsuiv->pos_f.lig >= 0 && dsuiv->pos_f.lig < HAUTEUR && dsuiv->pos_f.col >= 0 && dsuiv->pos_f.col < LARGEUR){
    jeu->plateau[dsuiv->pos_f.lig][dsuiv->pos_f.col]=VIDE ;
    dsuiv -> pos_i.lig = dsuiv -> pos_f.lig ;
    dsuiv -> pos_i.col = dsuiv -> pos_f.col ; 
    dsuiv=position_suivante(dsuiv,dir);
    if ((dsuiv -> pos_f.lig <0 )|| (dsuiv -> pos_f.lig >= HAUTEUR )||  (dsuiv -> pos_f.col<0 )|| (dsuiv -> pos_f.col >= LARGEUR) )
      {
	break;
      }
        
  }
}
  



void pions_manges_ret(plateau_jeu* jeu, deplacement*d, enum direction dir, int joueur_courant){    //Capture des pions par retrait
  int diropp;
  deplacement*dopp=malloc(sizeof(deplacement));
  dopp -> pos_i.lig = d -> pos_i.lig ;
  dopp -> pos_i.col = d -> pos_i.col ; 
  diropp=direction_opposee(dir);
  dopp=position_suivante(dopp,diropp);
  while (jeu-> plateau[dopp->pos_f.lig ][dopp->pos_f.col]!=VIDE && jeu-> plateau[dopp->pos_f.lig ][dopp->pos_f.col]!=joueur_courant && dopp->pos_f.lig >= 0  && dopp->pos_f.lig < HAUTEUR && dopp->pos_f.col >= 0 && dopp->pos_f.col < LARGEUR)  {
    jeu->plateau[dopp->pos_f.lig][dopp->pos_f.col]=VIDE ;
    dopp -> pos_i.lig = dopp -> pos_f.lig ;
    dopp -> pos_i.col = dopp -> pos_f.col ; 
    dopp=position_suivante(dopp,diropp);
    if ((dopp -> pos_f.lig <0 )|| (dopp -> pos_f.lig >= HAUTEUR )||  (dopp -> pos_f.col<0 )|| (dopp -> pos_f.col >= LARGEUR) )
      {
	break;
      }
  }

}


int nb_pions_blancs(int plateau_jeu[HAUTEUR][LARGEUR]){		//Calcul du nombre de pions blancs dans le plateau
  int i, j, nb_blanc;
  nb_blanc = 0 ;
  for (i=0; i<HAUTEUR; i++) {
    for (j=0; j<LARGEUR; j++) { 
      if (plateau_jeu[i][j] == BLANC) {nb_blanc++ ;}
    }
  }
     
  return( nb_blanc);
}

int nb_pions_noirs(int plateau_jeu[HAUTEUR][LARGEUR]){		//Calcul du nombre de pions noirs dans le plateau
  int i, j, nb_noir ;
  nb_noir = 0 ;
  for (i=0; i<HAUTEUR; i++) {
    for (j=0; j<LARGEUR; j++) { 
      if (plateau_jeu[i][j] == NOIR) {nb_noir++;}
    }
  }
     
  return( nb_noir);
}


int mon_pion(deplacement*d, int joueur_courant, int plateau[HAUTEUR][LARGEUR]){  //Le pion sélectionné est-il celui du joueur courant ?
  printf("%d\n", plateau[d->pos_i.lig][d->pos_i.col]);
  if (plateau[d->pos_i.lig][d->pos_i.col] == joueur_courant) {
    return 1;}
  else {
    return 0; 
  }
}


deplacement* choix_pion( int joueur_courant){
  deplacement* d=malloc(sizeof(deplacement));			 //Sélection d'un pion dans le plateau de jeu
  int i,j; 
  printf("\nEntrez les numeros de ligne et de colonne du pion à déplacer : \n");
  scanf("%d", &i);
  scanf("%d", &j);
  d->pos_i.lig=i;
  d->pos_i.col=j;
  return(d);
}


int choix_direction(int joueur_courant){				//Sélection d'une direction par le joueur
  int dir;
  enum direction {N, S, E, W, NE, NW, SE, SW};
  printf("Dans quelle direction voulez-vous déplacer votre pion ? : \n\n 0-N \n 1-S \n 2-E \n 3-W \n 4-NE \n 5-NW \n 6-SE \n 7-SW \n");
  printf("\n");
  scanf("%d", &dir);
       
  return(dir);

}


void message_erreurs(int testdep, int occupe, int bon_pion){            //Affichage des messages d'erreurs en fonction des différents tests de deplacement
  if (testdep==0){
    printf("\n___________________________________________________________________________________________________________ \n"); 		
    printf("\n DEPLACEMENT IMPOSSIBLE : Vous ne pouvez pas effectuer de mouvement en diagonale à partir de cette case \n"); 
    printf("\n___________________________________________________________________________________________________________ \n"); 		
  }else if (occupe==0){
    printf("\n___________________________________________________________________________________________________________ \n"); 				
    printf("\n DEPLACEMENT IMPOSSIBLE : La case d'arrivée de votre pion n'est pas vide ou le coup effectuee sort du plateau \n");
    printf("\n___________________________________________________________________________________________________________ \n"); 				
  }else if (bon_pion==0){
    printf("\n___________________________________________________________________________________________________________ \n"); 		
    printf("\n ATTENTION : Vous ne pouvez pas jouer avec un pion adverse \n");
    printf("\n___________________________________________________________________________________________________________ \n"); 		

  }
}


int main(){
  int joueur_1;
  int joueur_2;
  int joueur_courant;
  joueur_1=NOIR;
  joueur_2=BLANC;
  plateau_jeu* plat=malloc(sizeof(plateau_jeu));
  int dir;
  initialiser_plateau_vide(plat->plateau);
  placer_pions(plat->plateau);
  joueur_courant=joueur_1;
  printf("\n------------------------------------------------------------------\n");
  printf("\n=*=*=*=*=*=*=*=*=*=*=*=*=*==FANORONA==*=*=*=*=*=*=*=*=*=*=*=*=*=*=\n\n");
  printf("Pour effectuer un mouvement, veuillez  : \n--Entrer la ligne du pion à déplacer ( de 0 à 8 ) puis -Entrée-.\n");
  printf("--Entrer la colonne du pion à déplacer ( de 0 à 4 ) puis -Entrée-\n");
  printf("--Puis, la direction qui vous sera proposée, puis -Entrée- \n");
  printf("\n------------------------------------------------------------------\n\n");
    
  afficher_plateau(plat->plateau);

  printf("\n\nJoueur %d, à vous de jouer :\n", joueur_courant);
  int nb_blancs,nb_noirs;
  nb_blancs=nb_pions_blancs(plat->plateau);
  nb_noirs=nb_pions_noirs(plat->plateau);
  printf("___________________________________________________________________\n \n");
  printf("JOUEUR 2 : %d pions \nJOUEUR 1 : %d pions \n", nb_noirs , nb_blancs);
  printf("___________________________________________________________________\n");
    
  do{  
    int testdep, occupe,bon_pion;
    int approche, retrait;
    deplacement* d=malloc(sizeof(deplacement));
    d=choix_pion(joueur_courant );
    dir=choix_direction(joueur_courant);
    d=position_suivante(d, dir);
    testdep=test_deplacement(d, plat->plateau, dir);
    occupe=est_occupe(d, plat->plateau);
    bon_pion=mon_pion(d, joueur_courant, plat->plateau);
    message_erreurs(testdep, occupe, bon_pion);

    while (testdep==0 ||occupe==0 ||bon_pion==0) {   //Tant que le joueur n'entre pas un coup valide
      d=choix_pion(joueur_courant );
      dir=choix_direction(joueur_courant);
      d=position_suivante(d, dir);
      testdep=test_deplacement(d, plat->plateau, dir);
      occupe=est_occupe(d, plat->plateau);
      bon_pion=mon_pion(d, joueur_courant, plat->plateau);
      message_erreurs(testdep, occupe, bon_pion);
    }

    approche=capture_approche(d, dir, plat->plateau , joueur_courant);
    retrait=capture_retrait(d , dir, plat->plateau, joueur_courant);


    if(approche==0 && retrait==0){			//Le coup n'implique aucune capture
      int capture_poss;
      capture_poss=capture_possible_plateau(joueur_courant,  plat->plateau );
      if (capture_poss==0 ) {			//Aucune capture possible pour les pions du joueur courant
	deplace_pion(d, plat,joueur_courant);
	afficher_plateau(plat->plateau);
      }else{					//Une capture est possible pour un des pions du joueur courant
        while ((approche!=1  && retrait!=1 )){		//Tant que le coup joue par le joueur courant n'implique pas de capture
	  printf("\n___________________________________________________________________________________________________________ \n"); 		
	  printf("\n ATTENTION : Une capture est possible dans le jeu. Veuillez en effectuer une. \n");
	  printf("\n___________________________________________________________________________________________________________ \n");
	  d=choix_pion(joueur_courant );
	  dir=choix_direction(joueur_courant);
	  d=position_suivante(d, dir);
	  testdep=test_deplacement(d, plat->plateau, dir);
	  occupe=est_occupe(d, plat->plateau);
	  bon_pion=mon_pion(d, joueur_courant, plat->plateau);
	  approche=capture_approche(d, dir, plat->plateau , joueur_courant); 
	  retrait=capture_retrait(d , dir, plat->plateau, joueur_courant);
	  message_erreurs(testdep, occupe, bon_pion);

	  while (testdep==0 ||occupe==0 ||bon_pion==0) {	//Le coup joue n'est pas valide
	    d=choix_pion(joueur_courant );
	    dir=choix_direction(joueur_courant);
	    d=position_suivante(d, dir);
	    testdep=test_deplacement(d, plat->plateau, dir);
	    occupe=est_occupe(d, plat->plateau);
	    bon_pion=mon_pion(d, joueur_courant, plat->plateau);
	    message_erreurs(testdep, occupe, bon_pion);
	  }
        }

      }
    }
    if(approche==1 && retrait==0){				//Le coup joue implique une capture par approche
      deplace_pion(d, plat,joueur_courant);
      pions_manges_app (  plat, d, dir, joueur_courant);
      printf("\n");
      afficher_plateau(plat->plateau);
    }
    else if(approche==0 && retrait==1){			//Le coup joue implique une capture par retrait
      deplace_pion(d, plat,joueur_courant);
      pions_manges_ret (  plat, d, dir, joueur_courant);
      printf("\n");
      afficher_plateau(plat->plateau);
    }
    else if (approche==1 && retrait==1){			//Le coup joue implique une capture par retrait et une capture par approche
      int i;
      printf("Vous pouvez effectuer une capture par approche ou par retrait. Que choisissez-vous ?\n1-approche \n2-retrait\n");
      scanf("%d", &i);
      if(i==1){
	deplace_pion(d, plat,joueur_courant);
	pions_manges_app (plat, d, dir, joueur_courant);
	printf("\n");
	afficher_plateau(plat->plateau);
      }else if(i==2){ 
	deplace_pion(d, plat,joueur_courant);
	pions_manges_ret (plat, d, dir, joueur_courant);
	printf("\n");
	afficher_plateau(plat->plateau);}
                
    }
    
    if (joueur_courant==joueur_1){
      joueur_courant=joueur_2;
    }else{
      joueur_courant=joueur_1;
    }

    nb_blancs=nb_pions_blancs(plat->plateau);
    nb_noirs=nb_pions_noirs(plat->plateau);  
    printf("_____________________________SCORE_________________________________\n \n");
    printf("JOUEUR 2 : %d pions \nJOUEUR 1 : %d pions \n", nb_noirs , nb_blancs);
    printf("___________________________________________________________________\n");
    printf("\n\nJOUEUR %d, à vous de jouer :\n", joueur_courant);
  }while(nb_blancs!=0 && nb_noirs!=0);
  if(nb_blancs==0){
    printf("Le gagnant est le joueur %d \n", joueur_1);
  }else if(nb_noirs==0){
    printf("Le gagnant est le joueur %d \n", joueur_2);
  }
  return 0;
}
