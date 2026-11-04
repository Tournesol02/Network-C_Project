# C Language and Networking Design Project

**Authors:** Oskar ORVIK & Brage JOHNSEN

This project is part of the networking course. It also applies C language programming.
The project is divided into two parts: the source-sink link and the Mailbox Server application, coupled with sender and receiver clients.
We were also asked to implement the option for the user to choose the number of messages and the message length.

## Created Functions

All our functions are in the file tsockSource.c, which serves as our source file.
In tsock.h, we placed the function definitions, and finally, our main function is written in tsock.c.
We also created a Makefile to compile our program.

struct BoiteContenuP * CreeBoiteContenu(char * MsgEntry);
>>Places a message in a mailbox and returns the linked list with the newly added node.

>struct Env_Info * TrouverMessages(struct BoitePointer ListeDesBoites, int NumeroDest);
>>Finds all messages and returns a struct containing the number of messages and their sizes for the recipient’s mailbox.

>void EnvoyerMessages(int client_socket, struct BoitePointer ListeDesBoites, int NumeroDest);
>>Sends the messages from the recipient’s mailbox using the send() function.

>void construire_message(char * message, char motif, int lg, int numero);
>>Creates a message with a pattern, length, and recipient number.

**To compile the program:** 
>make clean

>make
## Options:
* -s: Sets the machine in source mode to send messages.
* -p: Sets the machine in sink mode to receive messages.
* -u: Chooses the UDP protocol; TCP is selected by default (when not specified).

## Mailbox–Sender/Receiver Communication
* -b: Sets the machine as the mailbox server.
* -e: Sets the machine as a sender; this option requires a recipient number.
* -r: Sets the machine as a receiver; this option requires the receiver number that it represents.

## Common Options
* -n: Sets the number of messages to send.
* -l: Sets the length of each message to send.

## How to Execute:

> **End-to-End:**
>>**Source:** 
>>./tsock.exe -s [-n ##] [-l ##] \<port-number>

>>**Sink:**
>>./tsock.exe -p [-n ##] [-l ##] \<IP-adress> \<port-number>

> **Mailbox Server / Sender / Receiver:**
>>**Mailbox:**
>>./tsock.exe -b [-n ##] [-l ##] \<port-number>

>**Reciever:**
>>./tsock.exe -r [-n ##] [-l ##] \<IP-adress-BAL> \<port-number>

>**Sender:**
>>./tsock.exe -e [-n ##] [-l ##] \<IP-adress-BAL> \<port-number>




