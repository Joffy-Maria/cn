```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 9000
#define MAX 10

int main() {
    int s, ns, i, max, sd;
    struct sockaddr_in addr;
    fd_set fds;
    int clients[MAX] = {0};
    char names[MAX][30], buf[1024], msg[1200];

    s = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    printf("Chat Server Started\n");

    while (1) {
        FD_ZERO(&fds);
        FD_SET(s, &fds);
        max = s;

        for (i = 0; i < MAX; i++)
            if (clients[i] > 0) {
                FD_SET(clients[i], &fds);
                if (clients[i] > max) max = clients[i];
            }

        select(max + 1, &fds, NULL, NULL, NULL);

        if (FD_ISSET(s, &fds)) {
            ns = accept(s, NULL, NULL);

            for (i = 0; i < MAX; i++)
                if (!clients[i]) {
                    clients[i] = ns;
                    recv(ns, names[i], 30, 0);
                    printf("%s connected\n", names[i]);
                    break;
                }
        }

        for (i = 0; i < MAX; i++) {
            sd = clients[i];

            if (sd && FD_ISSET(sd, &fds)) {
                int n = recv(sd, buf, 1023, 0);

                if (n <= 0) {
                    close(sd);
                    clients[i] = 0;
                    continue;
                }

                buf[n] = '\0';
                sprintf(msg, "%s: %s", names[i], buf);
                printf("%s", msg);

                for (int j = 0; j < MAX; j++)
                    if (clients[j] && clients[j] != sd)
                        send(clients[j], msg, strlen(msg), 0);
            }
        }
    }
}
```