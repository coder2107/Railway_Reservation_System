# === Compiler and flags ===
CC = gcc
CFLAGS = -lpthread

# === Directories ===
BIN_DIR = bin
SERVER_DIR = server

# === Targets ===
SERVER_OBJ = $(SERVER_DIR)/server.o $(SERVER_DIR)/ticket.o

SERVER_EXEC = $(BIN_DIR)/server

# === Default Target ===
all: $(BIN_DIR) $(SERVER_EXEC)

# === Server executable ===
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# === Object files ===
$(SERVER_DIR)/%.o: $(SERVER_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# === Create bin directory if not exist ===
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# === Clean ===
.PHONY: clean
clean:
	rm -rf $(BIN_DIR) *.o $(SERVER_DIR)/*.o $(CLIENT_DIR)/*.o
