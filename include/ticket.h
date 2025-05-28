#ifndef TICKET_H
#define TICKET_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

typedef struct Ticket
{
    int pnr;
    char name[50];
    int age;
    int seat_no;
    struct Ticket *next;
} Ticket;

// External variables
extern Ticket *head;
extern int next_seat;

Ticket* book_ticket(const char *name, int age);
void print_ticket(int pnr, int client_sock);
void view_available_seats(int client_sock);
void save_ticket_to_file();
void load_tickets_from_file();

#endif
