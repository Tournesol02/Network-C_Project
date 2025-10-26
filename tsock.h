#include <stdio.h>
#include <stdlib.h>

//Ceci est un Header File qui contient les fonctions: construction_message, afficher_message, CreeBoiteContenu, PlaceDansUneBoite. Ainsi que les structure: Msg_Info, BoitePointer, BoiteContenuP

//########################################################################
// Structs et constants:
//########################################################################
#define NUM_DEF  10
#define LNG_DEF  30



struct Msg_Info { //Toute l'information à l'interieur d'une message
  char type; //le type, soit emetteur ou recepteur
  int dest; //Le numero de destinataire, qui facilite la vie quand on va designer une BAL au destinataire
  int lng_msg; //Longeur d'une ligne 
  int nb_msg; //Combien de lignes
  int port;
  char Adresse_IP[64];
};

struct Env_Info {
  int lngMsg;
  int nbMsg;
  struct env_info * ProchainGroup;
};

struct BoiteContenuP{ //Contenu d'une BAL
  char * Msg; //Toute le message dans une char pointer
  struct BoiteContenuP * ProchaingTablette; //Pointe vers la prochaine tablette dans la mëme BAL
};

struct BoitePointer{ //La BAL
  int NumDest ; //Le numero de destinataire
  struct BoitePointer * prochaingBoite; //Pointeur sur la prochaine BAL
  struct BoiteContenuP * ProchaingMsg; //Pointeur sur la prochaine tablette dans la même BAL
};

extern struct BoitePointer * AdresseBAL; //Pour commencer une liste chainée des BALs
//###########################################################################
//Fonctions:
//###########################################################################
void construction_message ( char * message, char motif, int lg,int recepteur); //Crée un message

void afficher_message (char *message, int lg); //Affiche le message

//Fonction qui créée un noeud et renvoie la nouvelle liste chainée
struct BoiteContenuP * CreeBoiteContenu(char * MsgEntry);

//Fonction qui crée une boite et une tablette
struct BoitePointer * CreeBoitePointer(int NumDestEntry);

//Créer une fonction qui prend comme arguments le numero de boite et le message
void PlaceDansUneBoite(struct BoitePointer * ListeDesBoites, int NumeroDest, char * Message, int *info);

struct Msg_Info RetirerInformation(char *Msg_Avec_Info);

struct Env_Info * TrouverMessages(struct BoitePointer *ListeDesBoites, int NumeroDest);

void EnvoyerMessages(int client_socket, struct BoitePointer *ListeDesBoites, int NumeroDest);

void construire_message (char * message, char motif, int lg, int numero);

void communication (int argc, char **argv);
