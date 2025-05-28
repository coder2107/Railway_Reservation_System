# 🚂 Railway Reservation System

A full-stack railway reservation system combining the performance of C with modern web technologies. The backend uses HTTP server implementation in C with pthread support for concurrent bookings, while the frontend provides a sleek, mobile-responsive interface for ticket booking, printing, and seat availability checking. Features persistent file-based storage, CORS-enabled API, and real-time server status monitoring.

## ✨ Features

- **🎫 Book Tickets**: Reserve seats with passenger details
- **🔍 Print Tickets**: Retrieve and display ticket information using PNR
- **💺 View Available Seats**: Check real-time seat availability
- **📱 Responsive Design**: Works seamlessly on desktop and mobile devices
- **🔄 Real-time Status**: Live server connection status indicator
- **💾 Persistent Storage**: File-based ticket storage system
- **🔒 Thread Safety**: Multi-client support with mutex protection

## 🏗️ Architecture

### Backend (C)
- **HTTP Server**: Handles web requests and serves API endpoints
- **Ticket Management**: Core booking logic with linked list data structure
- **File I/O**: Persistent storage using binary files
- **Threading**: Multi-client support using pthreads
- **Linkedlist Implementation**: Implemented a linked list to track reserved ticket details.

### Frontend (Web)
- **Modern UI**: Clean, gradient-based design with animations
- **JavaScript API**: RESTful communication with C backend
- **Error Handling**: User-friendly error messages and loading states
- **Responsive Layout**: Mobile-first design approach

## 📋 Prerequisites

- **GCC Compiler** with pthread support
- **Linux/Unix Environment** (tested on Ubuntu/Debian)
- **Modern Web Browser** (Chrome, Firefox, Safari, Edge)
- **Make** (optional, for easier compilation)

## How to run

### Navingate to project directory
Type following command in terminal
```bash 
make
```
Navigate to the bin folder
Type folowing command in terminal
```bash
./server
```

## Access the Web Application
Navingate to your html file folder and open the file in the browser

## 🖥️ Usage

### Booking a Ticket
1. Navigate to the "Book Ticket" tab
2. Enter passenger name and age
3. Click "Book Ticket"
4. Note the PNR and seat number from the response

### Printing a Ticket
1. Go to the "Print Ticket" tab
2. Enter the PNR number
3. Click "Print Ticket" to view details

### Checking Available Seats
1. Switch to the "Available Seats" tab
2. Click "Check Available Seats"
3. View the current seat availability

## 📁 Project Structure

```
railway-reservation/
├──include/
│    ├── ticket.h       # Function declarations and structures
├── data/               # Data storage directory
│   └── tickets.dat     # Binary file for ticket storage
├──server/
│    ├── server.c       # HTTP server with API endpoints
│    ├── ticket.c       # Ticket booking logic and linkedlist implementation
├──web/
│    ├── index.html     # Web interface
├── Makefile            # Build configuration
└── README.md           # Project documentation
```

### Seat Capacity
Default maximum seats is 100. To modify:

```c
#define MAX_SEATS 150  // Change in ticket.c
```

## 🔌 API Endpoints

### POST /api
Main API endpoint for all operations.

**Book Ticket:**
```
Body: BOOK <name> <age>
Response: BOOKED | PNR: 1001 | SEAT: 1
```

**Print Ticket:**
```
Body: PRINT <pnr>
Response: PNR: 1001 | Name: John | Age: 25 | Seat No: 1
```

**View Available Seats:**
```
Body: VIEW
Response: Available seats: 99
```

## 🔒 Security Considerations

- Input validation for all user inputs
- Buffer overflow protection with bounded string operations
- Thread-safe operations using mutex locks

**Happy Booking! 🚂✨**
