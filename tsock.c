// Inclusion des libraies nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

//Inclusion de librarie créée par nous
#include "./tsock.h"




int main(int argc, char *argv[]) {

  // Déclaration des variables globales
  int c;
  char Type = 'A';
  int NumRecepteur = -1;
  int NumDest = -1;
  int lgMsg= -1;
  int nbMsg = -1;
  char protocol = 'T'; // Valeur par défaut
  const char *usage_msg = "usage: cmd [-s-|-p][-u][&&][-e|-r|-b][&&][-n ##][-l ##]\n";
    
  //*****************************************************************
  //Recuperation des options et démarrage
  //*****************************************************************

  while ((c = getopt(argc, argv, "spl:n:r:e:b:u")) != -1) {
    switch (c) {
    // Pour mettre la machine en mode puits
    case 'p':
      if (Type == 'S') {
        printf("usage: cmd [-p|-s][-n ##]\n");
        exit(1);
      }
      Type = 'P';
      break;

    // Pour mettre la machine en mode source
    case 's':
      if (Type == 'P') {
        printf("usage: cmd [-p|-s][-n ##]\n");
        exit(1) ;
      }
      Type = 'S';
      break;

    // Pour mettre la machine en mode recepteur. Cette option prends un arguement pour designer quelle destinataire on veut cibler.
    case 'r':
      if (Type != 'A' && Type == 'E') {
        fprintf(stderr, "%s", usage_msg);
        exit(1);
      }
      Type = 'R';
      NumRecepteur = atoi(optarg);
      break;

    // Pour mettre la machine en mode emetteur. Cette option prends un argument pour designer quelle recepteur on est.
    case 'e':

      if (Type != 'A' && Type == 'R') {
        fprintf(stderr, "%s", usage_msg);
        exit(1);
      }
      Type = 'E';
      NumDest = atoi(optarg);
      break;

    // Pour mettre la machine en mode serveur boîte aux lettres.
    case 'b':
      printf("On est dans la Boite aux lettres.\n");
      Type = 'B';
      break;

    // Pour choisir combien des messages on veut envoyer
    case 'n':
      nbMsg = atoi(optarg);
      break;

    // Pour choisir le longeur de tels messages à envoyer ou reçevoir
    case 'l':
      lgMsg = atoi(optarg);
      break;

    // Pour utiliser le protocol UDP à la place de TCP. Cette option n'est uniquement accessible pour le source ou le puits.
    case 'u':
      if ((Type == 'B') | (Type == 'E' ) | (Type == 'R')){
        printf("Cette option n'est pas disponible pour la partie BAL. Le fonctionnement des boîtes aux lettres est sur TCP uniquement.\n");
        exit(1);
      }
      protocol = 'U';
      break;

    // Pour informer du fonctionnnement des arguments.
    default:
      fprintf(stderr, "%s", usage_msg);
      exit(1);
    }
  }
  if ((Type == 'E') | (Type == 'R') |(Type == 'S')){ 
      if (argc - optind != 2){
	fprintf(stderr, "Il manque un argument imperatif.\n%s", usage_msg);
	exit(1); 
      }
    }

    if (Type == 'P'){
      printf("%d %d\n", argc,optind);
	if (argc - optind != 1){
	  fprintf(stderr, "Il manque un argument imperatif.\n%s", usage_msg);
	  exit(1); 
	}
    }
    if (Type == 'B'){
      if (argc - optind != 0){
	fprintf(stderr, "Il manque un argument imperatif.\n%s", usage_msg);
	exit(1); 
      }
    }
    
    
  // Si on n'a pas choisi ni source, ni puits, ni BAL ni emetteur ni recepteur
  if (Type == 'A') {
    fprintf(stderr, "%s", usage_msg);
    exit(1);
  }

  //Si on a change le nombre des messages avec l'option -n
  if (nbMsg != -1) {
    if (Type == 'E') {
      printf("nb de messages à envoyer : %d\n", nbMsg);
    }
    if (Type == 'S'){
      printf("nb de messages à envoyer : %d\n", nbMsg);
    }
    if(Type == 'P'){
      printf("nb de messages à reçevoir : %d\n", nbMsg);
    }
  }
  else { // Si on n'a pas utilitsé l'option -n et on utilise donc les valeurs de défaut
    if ((Type == 'E') |( Type == 'S')) {
      nbMsg = NUM_DEF;
      printf("nb de messages à envoyer = 10 par défaut\n");
    }
    if ((Type == 'R') | (Type == 'P')){
      //nbMsg = NUM_DEF;
      printf("nb de messages à reçevoir = infini  par défaut\n");
    }
  }
  //Si on a change le nombre des messages avec l'option -n
  if (lgMsg != -1) {
    if (Type == 'E') {
      printf("Le longeur des messages à envoyer : %d\n", lgMsg);
    }
    if (Type == 'S'){
      printf("Le longeur des messages à envoyer : %d\n", lgMsg);
    }
    if(Type == 'P'){
      printf("Longeur des messages à reçevoir : %d\n", lgMsg);
    }
  }
  else { // Si on n'a pas utilitsé l'option -n et on utilise donc les valeurs de défaut
    if ((Type == 'E') |( Type == 'S')) {
      lgMsg = LNG_DEF;
      printf("Longeur des messages à envoyer = 10 par défaut\n");
    }
    if ((Type == 'R') | (Type == 'P')){
      lgMsg = LNG_DEF;
      printf("Longeur des messages à reçevoir = 30 par défaut\n");
    }
  }




    // *************************************************************************************************
    // Source et puits
    // *************************************************************************************************

  if (protocol == 'U'){
    printf("Protocole choisi est UDP.\n");
    //cas où on est au côté source

    if (Type == 'S'){


      //Creation du socket en mode datagramme avec
      int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      struct sockaddr_in adr_locale;
      struct sockaddr_in puits_addr;
      struct hostent * puits;

      // Affectation de memoire pour l'@
      memset((char*)& adr_locale,0,sizeof(adr_locale));

      // Affectation du domaine
      adr_locale.sin_family = AF_INET;
      adr_locale.sin_addr.s_addr = INADDR_ANY;


      // Bind de l'adresse au socker
      if(bind(sockfd, (struct sockaddr*)&adr_locale, sizeof(adr_locale))==-1){
        printf("Binding malfunctioned\n");
        close(sockfd);
      }

      // Configurer l'adresse du puits
      puits = gethostbyname(argv[argc-2]);
      if (puits == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
      }
      bzero((char *) &puits_addr, sizeof(puits_addr));
      puits_addr.sin_family = AF_INET;
      bcopy((char *)puits->h_addr, (char *)&puits_addr.sin_addr.s_addr, puits->h_length);
      puits_addr.sin_port = htons(atoi(argv[argc-1]));

      for (int i = 0; i<nbMsg; i++){
        char * message = malloc(lgMsg);
        construire_message(message, 65 + i%26,lgMsg, i+1);
        printf("Message %d envoyé : %s\n", i + 1, message);
        // Si la fonction sendto() renvoie -1, ça veut dire qu'elle a echouée
        // Ainsi on voit quand un erreur d'envoi se passe
        if (sendto(sockfd, message, lgMsg, 0, (struct sockaddr*)&puits_addr, sizeof(puits_addr))==-1){
          printf("error\n");
        }
        free(message);
      }

      //Fermeture de socket
      close(sockfd);
      exit(1); // Pour sortir du script

    }


      ////////////////////////////////
    if( Type == 'P'){
      struct sockaddr_in adr_locale;
      printf("Cette machine fonctionne en mode puits\n");

      // Création du socket + gestion d'erreur
      int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
      }

      // Affectation de memoire pour l'@ du sockeet
      memset((char*)& adr_locale,0,sizeof(adr_locale));

      // Affectation du domaine pour le socket
      adr_locale.sin_family = AF_INET;
      adr_locale.sin_addr.s_addr = INADDR_ANY;
      adr_locale.sin_port = htons(atoi(argv[argc-1]));

      // Affecter le socket avec le domaine choisi au-dessus + gestion d'erreur
      if(bind(sockfd, (struct sockaddr*)&adr_locale, sizeof(adr_locale))==-1){
        printf("Binding malfunctioned\n");
        close(sockfd);
        exit(1);
      }

      struct sockaddr * ip_adresse_source = malloc(sizeof(struct sockaddr));
      char * adr_msg_recu = malloc(lgMsg*sizeof(char));
      socklen_t lg_struct = sizeof(ip_adresse_source); // On utilise socklen_t pour la taille de la structure

      ssize_t NbOctetsLu;
      int nb;
      // Selon les options, la machine attends un certain nombre des messages avec un certain longeur
      if ( nbMsg == -1 ){
        int i;
        while(1){
        // La fonction recvfrom retourne le nobmre des octets recu, et -1 si une erreur est arrivée
        NbOctetsLu = recvfrom(sockfd, adr_msg_recu, lgMsg,0, ip_adresse_source, &lg_struct);
        if(NbOctetsLu==-1){
          exit(1);
        }
        if(NbOctetsLu==0){
	  continue;
        }
        // Affichage des messages reçu
        printf("PUITS: Réception no. %d ( %d)", i + 1,(int)NbOctetsLu);
        afficher_message(adr_msg_recu,lgMsg);
        i++;
        }
        nb = i;



      }

      else{
        for (int i=1; i<nbMsg + 1;i++){
          // La fonction recvfrom retourne le nobmre des octets recu, et -1 si une erreur est arrivée
          NbOctetsLu = recvfrom(sockfd, adr_msg_recu, lgMsg,0, ip_adresse_source, &lg_struct);
          if(NbOctetsLu==-1){
            exit(1);
          }
          nb = i;

          // Affichage des messages reçu
          printf("PUITS: Réception no. %d ( %d)", i,(int)NbOctetsLu);
          afficher_message(adr_msg_recu,lgMsg);
        }
      }
      printf("Nombre de messages reçu : %d.\n",nb);
      printf("Nombre d'octets dans un message lu sont %d.\n",(int) NbOctetsLu);


    close(sockfd);
    exit(1);
    }
 }

    //////////////////////////////////////////////////////////////////
  if (protocol == 'T'){
    printf("Protocole TCP pris par default.\n");

    if (Type == 'S'){

      int sockfd;
      struct sockaddr_in serv_addr;
      struct hostent *server;

      // Créer une socket + gestion d'erreur
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(0);
      }

      // Configurer l'adresse du puits + gestion d'erreur
      server = gethostbyname(argv[argc-2]);
      if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
      serv_addr.sin_port = htons(atoi(argv[argc-1]));

      // Se connecter au serveur + gestion d'erreur
      //si la fonction sendto() renvoie -1, ça veut dire aue le dest n'existe pas
      if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
      }
      else{
        printf("Connection avec la machine à l'addresse IP: %s et numero de port: %s bien etabli!\n", argv[argc-2], argv[argc-1]);
      }

      int i;
      ssize_t bytesEnv;
      for (i = 0; i<nbMsg; i++){
        char * message = malloc(lgMsg*sizeof(char));
        construire_message(message,65 + i%26, lgMsg, i+1);
        printf("Message envoyé : ");
        afficher_message(message, lgMsg);
        // Envoi + fonctionnalité qui fait qu'on découvre l'erreur d'envoi
        bytesEnv = send(sockfd, message, strlen(message)+1, 0);
        if (bytesEnv == -1){
          printf("error");
        }
      }
      char * messageFin = "FIN";
      printf("Message envoyé : ");
      afficher_message(messageFin, strlen(messageFin) + 1);
      bytesEnv = send(sockfd, messageFin, strlen(messageFin)+1, 0);
      if (bytesEnv == -1){
        printf("error");
      }


      // Fin du programme qui détaille les messages envoyés
      printf("Nombre des messages envoyés : %d\n", i);
      printf("Bytes envoyés : %d\n", (int)bytesEnv);

      //Fermeture de socket
      close(sockfd);
      exit(1);
    }

    if (Type == 'P'){ //Cas où on est au coté puits
      printf("Dans un puits\n");
      int sockfd = socket(AF_INET,SOCK_STREAM,0);
      struct sockaddr_in adr_locale_TCP;

      // Affectation de memoire pour l'@ locale
      memset((char*)& adr_locale_TCP,0,sizeof(adr_locale_TCP));

      // Affectation du domaine
      adr_locale_TCP.sin_family = AF_INET;
      adr_locale_TCP.sin_addr.s_addr = INADDR_ANY;
      adr_locale_TCP.sin_port = htons(atoi(argv[argc-1]));

      // Affectation du socket avec l'adresse définie au-dessus
      if(bind(sockfd, (struct sockaddr*)&adr_locale_TCP, sizeof(adr_locale_TCP))==-1){
        printf("Binding malfunctioned\n");
        close(sockfd);
      }

      
      int second_socket_locale_TCP;

      // Message informatif qui apparaisse pour chaque fois que la machine attend une nouvelle utilisateur
      printf("\n#####################################\n");
      printf("Dans un puits en attente de connexion...\n\n\n");

      // Attend une requête de connexion + gestion d'erreur
      if(listen(sockfd, 1)==-1){
        printf("Le numéro du port est occupé\n");
        close(sockfd);
        exit(1);
      }
      else{
        socklen_t addr_len = sizeof(adr_locale_TCP);
        // Ouverture d'un nouvau socket + gestion d'erreur
        second_socket_locale_TCP = accept(sockfd,(struct sockaddr*)&adr_locale_TCP,&addr_len );
        if( second_socket_locale_TCP == -1){
          printf("Malfunction during accptance of message!\n");
          close(sockfd);
          exit(1);
        }
        else {
          char source_adresse_IP[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &(adr_locale_TCP.sin_addr),source_adresse_IP,ntohs(adr_locale_TCP.sin_port));
          printf("Connection avec la machine avec l'adresse IP: %s, Port: %d\n", source_adresse_IP, ntohs(adr_locale_TCP.sin_port));
        }
      }

      int msg;
      int i = 1;
      int nb_mem;
      int nb;
      ssize_t NbOctetsLu;
      char * messageFin = "FIN";
      char * message = malloc(lgMsg*sizeof(char));

      if (nbMsg == -1){

      while(1){
        NbOctetsLu = recv(second_socket_locale_TCP, message, lgMsg,0);
        if(NbOctetsLu==-1){
          exit(1);
        }
	 if(NbOctetsLu==0){
	   free(message);
	   close(second_socket_locale_TCP);
	   close(sockfd);
	   exit(1);
	 }
        if (strcmp(message, messageFin) == 0){
          nb = nb_mem;
          break;
        }
        msg =i;
        printf("PUITS: Réception no. %d ( %d)", i,(int)NbOctetsLu);
        afficher_message(message, lgMsg);
        i++;
        nb_mem = NbOctetsLu;

      }
      
      printf("PUITS: Réception no. %d ( %d)", i,(int)strlen(messageFin) + 1);
      afficher_message(message, strlen(messageFin) + 1);
      i++;
      }
      
      else {
        for (int i=1; i<nbMsg + 1;i++){
          NbOctetsLu = recv(second_socket_locale_TCP, message, lgMsg,0);
        if(NbOctetsLu==-1){
          exit(1);
        }
	 if(NbOctetsLu==0){
          continue;
        }

        msg = i;
        printf("PUITS: Réception no. %d ( %d)", i,(int)NbOctetsLu);
        afficher_message(message, lgMsg);
        }
        nb = NbOctetsLu;
	
      }

      free(message);
      
      // Message informatif des messages envoyés + la fermetur du nouvau socket. Après, le boucle while commence du début
      printf("Nombre de messages: %d.\n",msg);
      printf("Nombre d'octets dans un message lu sont %d.\n", nb);
      close(second_socket_locale_TCP);

    close(sockfd);
    exit(1);
    }
  }

    //****************************************************************************************************
    // Boite aux lettres
    // **************************************************************************************************

    if (Type == 'B'){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct BoitePointer * BAL_Tete=malloc(sizeof(struct BoitePointer));
    struct sockaddr_in serv_addr;

     // Affectation de memoire pour l'@ locale
    memset((char *)&serv_addr, 0, sizeof(serv_addr));

    // Affectation du domaine
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[argc-1]));

    // Affectation du socket de l'adresse au-dessus
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Binding malfunctioned");
        close(sockfd);
        exit(1);
    }

    // Message qui s'affiche pour chaque fois que le programme boucle quand il attend une nouvlle connexion
    printf("\n#####################################\n");
    printf("Dans un boite aux lettres en attente de connexion...\n\n\n");

    // La fonction qui arrête le program lorsqu'il attend une connexion + gestion d'erreur
    if (listen(sockfd, 1) == -1) {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }

    while (1){

    struct sockaddr_in client_addr;
    char client_adresse_IP[INET_ADDRSTRLEN];
    socklen_t addr_len = sizeof(client_addr);
    int socket_acceptation = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);

    // Acceptation de connexion avec un utilisateur + gestion d'erreur
    if (socket_acceptation == -1) {
        perror("Acceptance failed");
        close(sockfd);
        exit(1);
    }

    // Récupertation de l'info sur le client qui se connecte et l'afficher
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_adresse_IP, INET_ADDRSTRLEN);
    printf("Connexion avec la machine avec l'adresse IP: %s, Port: %d\n", client_adresse_IP, ntohs(client_addr.sin_port));

    // Allocation de mémoire pour le message informative + gestion d'erreur
    struct Msg_Info *Bufre = malloc(sizeof(struct Msg_Info));
    if (Bufre == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Reception du message informative + gestion d'erreur
    ssize_t bytes_received = recv(socket_acceptation, (char *)Bufre, sizeof(struct Msg_Info), 0);
    if (bytes_received == -1) {
        perror("recv failed");
        free(Bufre);
        exit(EXIT_FAILURE);
    }

    // Affiche de l'information sur le client qui se connecte
    printf("Message reçu: type=%c, lng_msg=%d, nb_msg=%d, dest=%d\n",
           Bufre->type, Bufre->lng_msg, Bufre->nb_msg, Bufre->dest);

    if (Bufre -> type == 'E'){
        int information=-1;
        int i;
        ssize_t OctetsLu;
        char * message = malloc( Bufre -> lng_msg*sizeof(char));
        for (i = 1; i < Bufre -> nb_msg + 1; i++){
          // Réception des messages + gestion d'erreur
            OctetsLu = recv(socket_acceptation, message, Bufre -> lng_msg, 0);
            if(OctetsLu==-1){
              printf("Erreur\n");
              exit(1);
            }

            // Affichages de l'info sur la reception et le stockement des messages
            printf("Message %d reçu : ", i);
            afficher_message(message, Bufre -> lng_msg);
            PlaceDansUneBoite(BAL_Tete, Bufre -> dest,message, &information);
        }
        // Message de fin
        printf("%d messages stockés avec longeur %d\n", i - 1, (int) OctetsLu);

        // Information sur les boîtes aux lettres
        if (information==0){
            printf("La boite aux lettres n'existait pas, on a créé une nouvelle\n");
        }
        if (information==1){
            printf("La boite aux lettres existait, mais aucune tablette trouvée. On a créé une nouvelle tablette.\n");
        }
        if (information==2){
            printf("La boite aux lettres existait, et il y avait des tablettes déjà remplit. On a mit le message dans une tablette.\n");
        }

        free(message);

    }

    if (Bufre -> type == 'R'){

      // Preparation des informations sur les messages à envoyer
      struct Env_Info * MessageInfo = malloc(sizeof(struct Env_Info));
      MessageInfo = TrouverMessages(BAL_Tete, Bufre -> dest);
      if (MessageInfo != NULL){
        printf("lg = %d , ng = %d\n", MessageInfo -> lngMsg, MessageInfo -> nbMsg);

        // Creation de struct à envoyer + mettre les valeurs dedans
        struct Msg_Info *M = malloc(sizeof(struct Msg_Info));
        if (M == NULL) {
          perror("malloc failed");
          exit(EXIT_FAILURE);
        }
        M->lng_msg = MessageInfo -> lngMsg;
        M->nb_msg = MessageInfo -> nbMsg;

        // L'envoi de message informative
        send(socket_acceptation, MessageInfo, sizeof(MessageInfo), 0);

        // L'envoi des messages
        EnvoyerMessages(socket_acceptation, BAL_Tete, Bufre -> dest);
      }
    }
    // Préparation pour le prochain itération du boucle
    free(Bufre);
    close(socket_acceptation);
    }
    close(sockfd);
    exit(1);
    }

    // **************************************************************************************
    // CLIENTS
    // ************************************************************************************
    
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    // Créer un socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Configurer l'adresse du serveur
    server = gethostbyname(argv[argc-2]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(argv[argc-1]));

    // Se connecter au serveur
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    struct Msg_Info *M = malloc(sizeof(struct Msg_Info));

    // Stocker les informations sur les options du client dans le struct
    if (Type == 'E'){
      if (M == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
      }
      M->type = 'E';
      M->lng_msg = lgMsg;
      M->nb_msg = nbMsg;
      M->dest = NumDest;
    }
    if(Type == 'R'){
      if (M == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
      }
      M->type = 'R';
      M->lng_msg = lgMsg;
      M->nb_msg = nbMsg;
      M->dest = NumRecepteur;
    }

    // Envoyer le message informatif
    ssize_t bytes_sent_M = send(sockfd, (char *)M, sizeof(struct Msg_Info), 0);
    if (bytes_sent_M == -1) {
      perror("send M failed");
      free(M);
      exit(EXIT_FAILURE);
    }

    printf("Message informative envoyée\n");
    
    //Emetteur
    if (Type == 'E'){
      int i;
      ssize_t bytesEnv;
      for (i = 0; i< nbMsg; i++){
        char * message = malloc(lgMsg*sizeof(char));

        construction_message(message, 65 + i%26, lgMsg, NumDest); //L'alphabet commence à 65 en ASCII

        printf("Message %d envoyé : ", i+1);
        afficher_message(message, lgMsg);

        // Envoyer le message à l'utilisateur + gestion d'erreur
        bytesEnv = send(sockfd, message, lgMsg, 0);
        if (bytesEnv == -1){
          printf("error");
        }

        free(message);
      }
    printf("Messages  envoyée : %d\n", i);
    printf("Longeur des messages : %d\n", (int) bytesEnv );

    }
    //Recepteur
    if (Type == 'R'){

      ssize_t OctetsLu;
      struct Env_Info * Info = malloc(sizeof(struct Env_Info));

      // Reception de l'info sur des messages à reçevoir
      OctetsLu = recv(sockfd, Info, sizeof(Info), 0);
      if (OctetsLu == -1){
        printf("error");
      }
      int nb = Info -> nbMsg;
      int lg = Info -> lngMsg ;
      printf("nb = %d , lg = %d\n", nb, lg);
      free(Info);

      // Reception des messages
      int i;
      for(i = 0; i < nb; i++){
        char * message = malloc(lg*sizeof(char));
        OctetsLu = recv(sockfd, message, lg, 0 );
        if (OctetsLu == -1){
          printf("error");
        }
	if (nb == 0){exit(1);}

        // Affichage des messages reçues
        printf("Message %d reçu : ", i + 1);
        afficher_message(message, lg);
        free(message);
      }

      // Message de la fin
      printf("Messages reçues : %d\n", i);
      printf("Longeur des messages : %d\n", (int)OctetsLu );

    }
    // Libration de memoire de message informative
    free(M);
    //Fermeture de socket
    close(sockfd);
    return 0;
}
