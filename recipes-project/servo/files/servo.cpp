#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main() {
    const char *port = "/dev/ttyACM0"; 
    int serial_port = open(port, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "Error opening serial port " << port << std::endl;
        return 1;
    }

    termios tty{};
    tcgetattr(serial_port, &tty);
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tcsetattr(serial_port, TCSANOW, &tty);

    char buf[256];
    while (true) {
        int n = read(serial_port, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            std::string line(buf);

            std::cout << "Received: " << line;

            std::ofstream csv("/home/weston/data.csv", std::ios::trunc);
            csv << line;
            csv.close();
        }
    }

    close(serial_port);
    return 0;
}
