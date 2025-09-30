#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

class Server {
private:
    int server_fd, client_fd;
    sockaddr_in server_addr{}, client_addr{};

public:
    Server(int port) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd < 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
        server_addr.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Bind failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

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

        std::thread recvThread([&]() {
            char buffer[1024];
            while (true) {
                memset(buffer, 0, sizeof(buffer));
                int bytes = read(client_fd, buffer, sizeof(buffer));
                if (bytes <= 0) {
                    std::cout << "Client disconnected.\n";
                    break;
                }
                std::cout << "Client: " << buffer << std::endl;
            }
        });

        std::string msg;
        while (true) {
            std::getline(std::cin, msg);
            if (msg == "exit") break;
            send(client_fd, msg.c_str(), msg.size(), 0);
        }

        close(client_fd);
        close(server_fd);
        recvThread.join();
    }
};

int main() {
    Server server(8080); // Listen on port 8080
    server.run();
    return 0;
}
