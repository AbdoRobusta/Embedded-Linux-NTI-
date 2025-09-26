#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define LED_PATH   "/sys/class/leds/input4::capslock/brightness"  // Adjust if needed

class LedController {
public:
    void setLedState(bool state) {
        std::ofstream ledFile(LED_PATH);
        if (!ledFile) {
            std::cerr << "Error: Cannot open LED path\n";
            return;
        }
        ledFile << (state ? "1" : "0");
        ledFile.close();
    }
};

class SocketServer {
private:
    int server_fd, client_fd;
    sockaddr_in server_addr{}, client_addr{};
    LedController led;

public:
    SocketServer(int port) {
        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }

        // Setup address
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        // Bind
        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Bind failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Listen
        if (listen(server_fd, 1) < 0) {
            perror("Listen failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        std::cout << "Server listening on port " << port << "...\n";
    }

    void run() {
        socklen_t client_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("Accept failed");
            return;
        }

        std::cout << "Client connected!\n";

        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytes = read(client_fd, buffer, sizeof(buffer));
            if (bytes <= 0) break;  // client disconnected

            if (buffer[0] == '1') {
                std::cout << "Turning LED ON\n";
                led.setLedState(true);
            } else if (buffer[0] == '0') {
                std::cout << "Turning LED OFF\n";
                led.setLedState(false);
            } 
            else if (buffer[0] == 'q') {
                break;
            }
            else {
                std::cout << "Unknown command: " << buffer[0] << "\n";
            }
        }

        close(client_fd);
        close(server_fd);
    }
};

int main() {
    SocketServer server(8080);  // Use port 8080
    server.run();
    return 0;
}
