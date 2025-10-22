# Hospital Management System

A modern, full-stack hospital management system built with C++ backend and HTML/JavaScript frontend.

## Features

- **Patient Management**: Add, view, search, and discharge patients
- **Real-time Dashboard**: Live statistics and patient counts
- **Search Functionality**: Find patients by ID or name
- **Data Persistence**: Patient data saved to local file
- **Modern UI**: Beautiful, responsive web interface
- **RESTful API**: Clean API endpoints for all operations

## Quick Start

### Option 1: Using Make (Recommended)
```bash
# Full setup: download dependencies, build, and run
make setup

# In another terminal, open the web interface
make web
```

### Option 2: Manual Setup
```bash
# 1. Download dependencies
make deps

# 2. Build the server
make all

# 3. Run the server
./server

# 4. Open index.html in your browser
open index.html
```

## API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/patients` | Get all patients |
| GET | `/api/patients/:id` | Get patient by ID |
| POST | `/api/patients` | Add new patient |
| DELETE | `/api/patients/:id` | Discharge patient |
| GET | `/api/patients/search?q=` | Search patients |
| GET | `/api/statistics` | Get hospital statistics |

## Dependencies

- **cpp-httplib**: HTTP server library ([GitHub](https://github.com/yhirose/cpp-httplib))
- **nlohmann/json**: JSON library for C++ ([GitHub](https://github.com/nlohmann/json))

## File Structure

```
cpp/
├── server.cpp          # C++ backend server
├── index.html          # Frontend web interface
├── httplib.h           # HTTP library header
├── json.hpp            # JSON library header
├── Makefile            # Build configuration
├── README.md           # This file
└── hospital_data.txt   # Patient data (created at runtime)
```

## Usage

1. **Start the server**: The server runs on `http://localhost:8080`
2. **Open the web interface**: Open `index.html` in your browser
3. **Add patients**: Use the "Add Patient" form to register new patients
4. **View patients**: Browse all patients in the "View Patients" section
5. **Search**: Find specific patients using the search functionality
6. **Statistics**: View hospital statistics and patient demographics

## Data Format

Patient data is stored in `hospital_data.txt` with the following format:
```
ID|Name|Age|Gender|Contact|Disease|AdmitDate|Doctor|Room
```

## Make Targets

- `make deps` - Download required dependencies
- `make all` - Build the server (default)
- `make run` - Build and run the server
- `make clean` - Remove build artifacts and data
- `make web` - Open the web interface
- `make setup` - Full setup: deps + build + run
- `make help` - Show help message

## Requirements

- C++17 compatible compiler (clang++, g++)
- Internet connection (for downloading dependencies)
- Modern web browser (for the frontend)

## License

MIT License - Feel free to use and modify as needed.

## Implementation
<img width="452" height="224" alt="image" src="https://github.com/user-attachments/assets/5b6db85c-81d7-4d64-be4c-53edfe54b4ea" />


---

**Note**: Make sure to keep the server running while using the web interface. The frontend communicates with the C++ backend via HTTP requests.
