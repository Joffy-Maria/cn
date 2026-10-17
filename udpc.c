```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in server = {0};
    socklen_t len = sizeof(server);
    char buffer[1024], msg[1024];

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);

    sendto(s, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server, len);

    int n = recvfrom(s, msg, sizeof(msg)-1, 0, NULL, NULL);

    if (n > 0) {
        msg[n] = '\0';
        printf("Server: %s\n", msg);
    }

    close(s);
    return 0;
}
```