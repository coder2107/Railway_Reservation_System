#include "../include/ticket.h"

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 4096

pthread_mutex_t lock;

void send_http_response(int sock, const char* content, const char* content_type) {
    char response[BUFFER_SIZE];
    int content_length = strlen(content);
    
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %d\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        content_type, content_length, content);
    
    write(sock, response, strlen(response));
}

void* handle_client(void* client_sock) {
    int sock = *(int *)client_sock;
    free(client_sock);

    char buffer[BUFFER_SIZE];
    char method[16], path[256], version[16];
    char *body_start;
    int content_length = 0;
    
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = read(sock, buffer, BUFFER_SIZE - 1);
    
    if (bytes_read <= 0) {
        close(sock);
        return NULL;
    }
    
    // Parse HTTP request line
    sscanf(buffer, "%s %s %s", method, path, version);
    
    printf("Received: %s %s\n", method, path);

    // Handle POST requests to /api
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api") == 0) {
        // Find Content-Length header
        char *content_length_header = strstr(buffer, "Content-Length:");
        if (content_length_header) {
            sscanf(content_length_header, "Content-Length: %d", &content_length);
        }
        
        // Find the body (after \r\n\r\n)
        body_start = strstr(buffer, "\r\n\r\n");
        if (body_start) {
            body_start += 4; // Skip \r\n\r\n
        } else {
            close(sock);
            return NULL;
        }
        
        // Process the command from the body
        char command[256];
        strncpy(command, body_start, sizeof(command) - 1);
        command[sizeof(command) - 1] = '\0';
        
        // Remove any trailing newlines
        command[strcspn(command, "\r\n")] = '\0';
        
        char response_content[512] = {0};
        
        pthread_mutex_lock(&lock);
        
        if (strncmp(command, "BOOK", 4) == 0) {
            char name[50];
            int age;
            if (sscanf(command + 5, "%s %d", name, &age) == 2) {
                Ticket* ticket = book_ticket(name, age);
                if (ticket) {
                    snprintf(response_content, sizeof(response_content),
                        "BOOKED | PNR: %d | SEAT: %d", ticket->pnr, ticket->seat_no);
                } else {
                    snprintf(response_content, sizeof(response_content), "Booking Failed - No seats available");
                }
            } else {
                snprintf(response_content, sizeof(response_content), "Invalid command format");
            }
        } 
        else if (strncmp(command, "VIEW", 4) == 0) {
            int available_seats = 100 - (next_seat - 1);
            snprintf(response_content, sizeof(response_content), "Available seats: %d", available_seats);
        } 
        else if (strncmp(command, "PRINT", 5) == 0) {
            int pnr;
            if (sscanf(command + 6, "%d", &pnr) == 1) {
                Ticket *temp = head;
                int found = 0;
                while (temp != NULL) {
                    if (temp->pnr == pnr) {
                        snprintf(response_content, sizeof(response_content),
                            "PNR: %d | Name: %s | Age: %d | Seat No: %d", 
                            temp->pnr, temp->name, temp->age, temp->seat_no);
                        found = 1;
                        break;
                    }
                    temp = temp->next;
                }
                if (!found) {
                    snprintf(response_content, sizeof(response_content), "Ticket not found");
                }
            } else {
                snprintf(response_content, sizeof(response_content), "Invalid PNR format");
            }
        } 
        else {
            snprintf(response_content, sizeof(response_content), "Invalid command");
        }
        
        pthread_mutex_unlock(&lock);
        
        send_http_response(sock, response_content, "text/plain");
    } 
    
    close(sock);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1, addrlen = sizeof(address);

    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("HTTP Server running on http://localhost:%d\n", PORT);
    printf("Web interface available at: http://localhost:%d\n", PORT);

    load_tickets_from_file();

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }
        
        int *client_sock = malloc(sizeof(int));
        *client_sock = new_socket;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)client_sock) != 0) {
            perror("pthread_create failed");
            free(client_sock);
            close(new_socket);
            continue;
        }
        pthread_detach(thread_id);
    }

    pthread_mutex_destroy(&lock);
    close(server_fd);

    return 0;
}
