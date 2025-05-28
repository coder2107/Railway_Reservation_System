#include "../include/ticket.h"

#define MAX_SEATS 100
#define DATA_FILE "../data/tickets.dat"

Ticket *head = NULL;
int next_pnr = 1001;
int next_seat = 1;

void send_message(int sock, const char *message) {
    write(sock, message, strlen(message));
}

Ticket* book_ticket(const char *name, int age) {
    if (next_seat > MAX_SEATS) {
        return NULL; // No seats available
    }

    Ticket *new_ticket = (Ticket *)malloc(sizeof(Ticket));

    new_ticket->pnr = next_pnr++;
    new_ticket->seat_no = next_seat++;
    strncpy(new_ticket->name, name, sizeof(new_ticket->name) - 1);
    new_ticket->name[sizeof(new_ticket->name) - 1] = '\0';
    new_ticket->age = age;
    new_ticket->next = NULL;

    if(head == NULL) {
        head = new_ticket;
    } else {
        Ticket *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_ticket;
    }

    save_ticket_to_file();

    return new_ticket;
}

void print_ticket(int pnr, int sock) {
    Ticket *temp = head;
    char response[256] = {0};
    while (temp != NULL ) {
        if (temp->pnr == pnr) {
            char response[256];
            sprintf(response, "PNR: %d | Name: %s | Age: %d | Seat No: %d\n", temp->pnr, temp->name, temp->age, temp->seat_no);
            send_message(sock, response);
            return;
        }
        temp = temp->next;
    }
    send_message(sock, "Ticket not found\n");
}

void view_available_seats(int sock) {
    char response[64] = {0};
    int available_seats = MAX_SEATS - (next_seat - 1);
    sprintf(response, "Available seats: %d\n", available_seats);
    send_message(sock, response);
}

void save_ticket_to_file() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        perror("Failed to open file for saving tickets");
        return;
    }

    Ticket *temp = head;
    while (temp != NULL) {
        fwrite(temp, sizeof(Ticket), 1, file);
        temp = temp->next;
    }

    fclose(file);
}

void load_tickets_from_file() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        perror("Failed to open file for loading tickets");
        return;
    }

    Ticket temp;
    while (fread(&temp, sizeof(Ticket), 1, file) == 1) {
        Ticket *new_ticket = (Ticket *)malloc(sizeof(Ticket));
        *new_ticket = temp;
        new_ticket->next = NULL;

        if (head == NULL) {
            head = new_ticket;
        } else {
            Ticket *last = head;
            while (last->next != NULL) {
                last = last->next;
            }
            last->next = new_ticket;
        }

        if (temp.pnr >= next_pnr) {
            next_pnr = temp.pnr + 1;
        }
        if (temp.seat_no >= next_seat) {
            next_seat = temp.seat_no + 1;
        }
    }

    fclose(file);
}
