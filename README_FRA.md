# Bureau d'Etudes en Langage C et Réseaux

**Auteurs:** Oskar, ORVIK & Brage, JOHNSEN

Ce projet ce fait partie du cours des résaux. Il met aussi en application le codage en langage C. Le projet est divisé en deux parties: lien source-puits et application d'un serveur Boite Aux Lettres couplés avec les clients emetteur/recepteur.
On est aussi demandé d'implementer l'option de faire un nombre de messages et une longueur de message, choisit par l'utilisateur.

## Fonctions créées

On a mis tous nos fonctions dans le fichier tsockSource.c comme notre Source File, dans le tsock.h on a mit les definitions des fonctions utilisées et enfin notre main est ecrit dans tsock.c. On a aussi fait un fichier Makefile qui compile notre programme.

>struct BoiteContenuP * CreeBoiteContenu(char * MsgEntry);
>>Met un message dans une bo^te aux lettres, et renvoie la liste chainée avec le noeud ajouté.

>struct Env_Info * TrouverMessages(struct BoitePointer *ListeDesBoites, int NumeroDest);
>> Trouve tous les messages et retourne un struct avec le nombre des messages et leur tailles pour la boîte aux lettres du destinataire.

>void EnvoyerMessages(int client_socket, struct BoitePointer *ListeDesBoites, int NumeroDest);
>>Envoye les messages de la boîte aux lettres d'une destinataire, utilise la fonction send().

>void construire_message (char * message, char motif, int lg, int numero);
>>Crée un message avec un motif, longueur et le numero de destinataire.

**Pour compiler le programme :**
>make clean

>make

## Les options :

### Communication bout-à-bout : 
* -s : Met la machine en mode source qui envoie les messages.
* -p : Met la machine en mode puits prêt pour réçevoir les messages.
* -u : Protocolose choisit est UDP, le protole TCP est choisit par defauts (sans arguments)

### Communication Boîtes Aux Lettres-aux-Emetteur/Récepteur
* -b : On se met la machine en mode serveur pour ressembler une boîte aux lettres.
* -e : On met la machine en mode emetteur, cette option necessite un numéro destinataire.
* -r : On met la machine en mode récepteur, cette option necessite le numéro de récepteur qu'on répresente.

#### Options commune
* -n: On choisit le numéro des messages à envoyer.
* -l: On choisit le longueur de chaque message à envoyer.

## Comment executer :

>**Bout-à-bout :**

>>**Source :** 
>>./tsock.exe -s [-n ##] [-l ##] \<port-number>

>>**Puits :** 
>>./tsock.exe -p [-n ##] [-l ##] \<IP-adress> \<port-number>

>**Serveur-Emétteur/Recepteur :**

>>**Boîte Aux Lettres :**
>>./tsock.exe -b [-n ##] [-l ##] \<port-number>

>>**Recepteur :** 
>>./tsock.exe -r [-n ##] [-l ##] \<IP-adress-BAL> \<port-number>

>**Emétteur :**
>>./tsock.exe -e [-n ##] [-l ##] \<IP-adress-BAL> \<port-number>

