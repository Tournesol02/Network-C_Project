#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "./tsock.h"

#define BUFFER_SIZE 1024

//Ceci est un fichier qui contient les fonctions que nous avons utilisé dans le program principal

void construction_message ( char * message, char motif, int lg,int recepteur) {
  int i;
  //Les espaces du debut
  int end = 5;
  sprintf(message,"%*d",end,recepteur);
  
  //Succession des lettres qui bouclent après Z
  for(i=end; i<lg; i++){
    message[i] = motif;
    end = i;
  }
  if (i<lg){
    //Fin du message
    message[i]='\0';
  }
  else {
    message[lg-1] = '\0';
  }
}

void construire_message (char * message, char motif, int lg, int numero) {
  //Les espaces du debut
  int end = 5;
  sprintf(message, "%*d",end,numero);

  //Succession des lettres qui bouclent après Z
  for(int i=end; i<lg; i++){
    message[i] = motif;
    end = i;
  }
  //Fin du message
  message[end]='\0';
  }

void afficher_message (char *message, int lg) {
  int i;
  // printf("message construit : [");
  printf(" [");
  for (i=0 ; i<lg ; i++){
    printf("%c", message[i]);
  }
  printf("]\n"); 
}

struct BoiteContenuP * CreeBoiteContenu(char * MsgEntry){ 
  struct BoiteContenuP * NouvelTablette = (struct BoiteContenuP*)malloc(sizeof(struct BoiteContenuP));
  NouvelTablette -> Msg =  strdup(MsgEntry);
  NouvelTablette -> ProchaingTablette = NULL;
  return NouvelTablette; 
}

struct BoitePointer * CreeBoitePointer(int NumDestEntry){ 
  struct BoitePointer * NouvelBoite = (struct BoitePointer*)malloc(sizeof(struct BoitePointer));
  struct BoiteContenuP * NouvelleTab = (struct BoiteContenuP*)malloc(sizeof(struct BoiteContenuP));
  NouvelBoite -> NumDest = NumDestEntry;
  NouvelBoite->ProchaingMsg = NouvelleTab; //Une tablette est initialisée
  NouvelleTab->ProchaingTablette = NULL;
  NouvelBoite -> prochaingBoite = NULL;
  return NouvelBoite; 
}

void PlaceDansUneBoite(struct BoitePointer * ListeDesBoites, int NumeroDest, char * Message, int *info){
  struct BoitePointer * tmp = ListeDesBoites; //La premiere boite dans la liste des boites
  while (tmp -> NumDest != NumeroDest && tmp->prochaingBoite != NULL){
    tmp = tmp->prochaingBoite;
  }
  if (tmp->NumDest != NumeroDest){//Cas où la boite aux lettres n'existe pas
    struct BoitePointer * NouvelleBoite = CreeBoitePointer(NumeroDest);
    NouvelleBoite->ProchaingMsg->Msg = strdup(Message);
    tmp->prochaingBoite = NouvelleBoite;
    *info = 0;
    
  }
  //Quand on a trouvé la correcte boite, on cherche le fin des tablettes
  struct BoiteContenuP * NouvelleTablette = CreeBoiteContenu(Message);
  if (tmp->ProchaingMsg == NULL){ //Si la  boite existe mais qu'il y a pas des tablettes 
    tmp->ProchaingMsg=NouvelleTablette;
    *info = 1;
  }
  else{
    //On cherche le long de la tabletterie de la boite aux lettres
    struct BoiteContenuP * CurrentTablette = tmp->ProchaingMsg;
    while (CurrentTablette->ProchaingTablette != NULL){
      CurrentTablette = CurrentTablette->ProchaingTablette;
     *info = 2;
    }
    CurrentTablette->ProchaingTablette = NouvelleTablette;
  }
}


// Fonction pour trouver et afficher les messages d'une boîte spécifique
struct Env_Info * TrouverMessages(struct BoitePointer *ListeDesBoites, int NumeroDest) {
    if (ListeDesBoites == NULL) {
        fprintf(stderr, "Liste des boîtes est vide.\n");
        return NULL;
    }

    struct BoitePointer *tmp = ListeDesBoites;
    while (tmp != NULL && tmp->NumDest != NumeroDest) {
        tmp = tmp->prochaingBoite;
    }

    if (tmp == NULL) {
        printf("La boîte aux lettres avec le numéro %d n'existe pas.\n", NumeroDest);
        return NULL;
    }
    struct Env_Info * InfoMessages = malloc(sizeof(struct Env_Info));
    printf("Messages dans la boîte aux lettres %d:\n", NumeroDest);
    struct BoiteContenuP *CurrentTablette = tmp->ProchaingMsg;
    int i = 0;
    int lg;
    while (CurrentTablette != NULL) {
        printf("Message %d trouvée : ", i+1);
        lg = strlen(CurrentTablette -> Msg) + 1;
        afficher_message(CurrentTablette->Msg, lg);
        CurrentTablette = CurrentTablette->ProchaingTablette;
        i++;
    }
    InfoMessages -> lngMsg = lg;
    InfoMessages -> nbMsg = i;
    return InfoMessages;
}


// Fonction pour trouver et envoyer les messages d'une boîte spécifique
void EnvoyerMessages(int client_socket, struct BoitePointer *ListeDesBoites, int NumeroDest) {
    if (ListeDesBoites == NULL) {
        const char *message = "Liste des boîtes est vide.\n";
        send(client_socket, message, strlen(message), 0);
        return;
    }

    struct BoitePointer *tmp = ListeDesBoites;
    while (tmp != NULL && tmp->NumDest != NumeroDest) {
        tmp = tmp->prochaingBoite;
    }

    if (tmp == NULL) {
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "La boîte aux lettres avec le numéro %d n'existe pas.\n", NumeroDest);
        send(client_socket, message, sizeof(message), 0);
        return;
    }
    struct BoiteContenuP *CurrentTablette = tmp->ProchaingMsg;
    tmp -> ProchaingMsg = NULL;
    
    while (CurrentTablette != NULL) {
      struct BoiteContenuP * liberateur = CurrentTablette;
      send(client_socket, CurrentTablette->Msg, strlen(CurrentTablette->Msg) + 1, 0);

      CurrentTablette = CurrentTablette->ProchaingTablette;
      free(liberateur -> Msg);
      free(liberateur);
    }

}
