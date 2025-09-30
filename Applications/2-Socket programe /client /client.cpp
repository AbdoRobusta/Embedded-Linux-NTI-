#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

class Client {
private:
    int sock;
    sockaddr_in server_addr{};

public:
    Client(const std::string& ip, int port) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
            perror("Invalid address");
            exit(EXIT_FAILURE);
        }

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            perror("Connection failed");
            exit(EXIT_FAILURE);
        }

        std::cout << "Connected to server!\n";
    }

    void run() {
        std::thread recvThread([&]() {
            char buffer[1024];
            while (true) {
                memset(buffer, 0, sizeof(buffer));
                int bytes = read(sock, buffer, sizeof(buffer));
                if (bytes <= 0) {
                    std::cout << "Server disconnected.\n";
                    break;
                }
                std::cout << "Server: " << buffer << std::endl;
            }
        });

        std::string msg;
        while (true) {
            std::getline(std::cin, msg);
            if (msg == "exit") break;
            send(sock, msg.c_str(), msg.size(), 0);
        }

        close(sock);
        recvThread.join();
    }
};

int main() {
    Client client("127.0.0.1", 8080); // Change IP to Server's IP
    client.run();
    return 0;
}
