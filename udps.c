```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {0}, client;
    socklen_t len = sizeof(client);
    char buffer[1024];

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    int n = recvfrom(s, buffer, sizeof(buffer)-1, 0,
                     (struct sockaddr *)&client, &len);

    if (n > 0) {
        buffer[n] = '\0';
        printf("Client: %s\n", buffer);

        char msg[] = "Message received";
        sendto(s, msg, strlen(msg), 0,
               (struct sockaddr *)&client, len);
    }

    close(s);
    return 0;
}
```