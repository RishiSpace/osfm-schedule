# OSFM Schedule: Distributed Task Server

## Overview

**OSFM Schedule** is a simple distributed server-client system for managing and dispatching computational tasks to multiple clients over a network using UDP. The server registers clients, maintains their available resources, and dispatches tasks to them. Clients register themselves and listen for tasks to execute.

---

## Components

### Server

- **Registers clients** via UDP.
- **Maintains a list** of available clients and their resources (cores).
- **Dispatches tasks** to clients with available resources.
- **Multithreaded:** One thread for listening to client registrations, one for dispatching tasks.

### Client

- **Registers itself** with the server, reporting its address and available cores.
- **Listens for tasks** from the server.
- **Executes received tasks** (as shell commands).

---

## File Structure

- [`server.h`](server.h) / [`server.cpp`](server.cpp): Server class and logic.
- [`client.h`](client.h) / [`client.cpp`](client.cpp): Client class and logic.
- [`network_utils.h`](network_utils.h) / [`network_utils.cpp`](network_utils.cpp): UDP networking utilities and message parsing.
- [`task.h`](task.h): Task structure definition.
- [`client_info.h`](client_info.h): Client information structure.

---

## Build Dependencies

- **C++17 or later**
- **nlohmann/json** (for JSON parsing)
  - Install on Ubuntu:  
    ```sh
    sudo apt-get install nlohmann-json3-dev
    ```
  - Or download [`json.hpp`](https://github.com/nlohmann/json) and place it in your include path.

---

## Building

Use your preferred C++ build system. Example with `g++`:

```sh
g++ -std=c++17 -o server server.cpp network_utils.cpp -lpthread
g++ -std=c++17 -o client client.cpp network_utils.cpp -lpthread
```

---

## Usage

### Start the Server

```sh
./server
```

### Start a Client

```sh
./client
```

Clients will automatically register with the server and listen for tasks.

---

## Code Structure

### Server

- **`Server::run()`**: Starts threads for listening and dispatching.
- **`Server::listenForClients()`**: Registers clients as they connect.
- **`Server::dispatchTasks()`**: Assigns tasks to clients with free cores.

### Client

- **`Client::run()`**: Registers with the server and listens for tasks.
- **`Client::executeTask()`**: Executes received tasks.

### Networking

- **UDP** is used for all communication.
- **JSON** is used for message serialization.

---

## License

This project is licensed under the GNU GPL v3. See [LICENSE](LICENSE) for details.

---

## Notes

- This is a minimal demonstration and does not include authentication, error recovery, or advanced scheduling.
- Tasks are executed as shell commands; ensure commands are safe and trusted.