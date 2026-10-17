``` c
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>

#define SIZE 65536

int main()
{
    int sockfd, n;
    unsigned char buffer[SIZE];
    struct sockaddr addr;
    socklen_t len = sizeof(addr);

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sockfd < 0) {
        perror("Socket");
        return 1;
    }

    printf("Raw Socket Packet Capture Started\n");

    while (1) {
        n = recvfrom(sockfd, buffer, SIZE, 0,
                    &addr, &len);

        if (n < 0) {
            perror("recvfrom");
            break;
        }

        struct iphdr *ip = (struct iphdr *)buffer;

        struct in_addr src = { .s_addr = ip->saddr };
        struct in_addr dst = { .s_addr = ip->daddr };

        printf("\nPacket Length: %d Bytes\n", n);
        printf("IP Version: %d\n", ip->version);
        printf("Header Length: %d Bytes\n", ip->ihl * 4);
        printf("TTL: %d\n", ip->ttl);
        printf("Protocol: %d\n", ip->protocol);
        printf("Source IP: %s\n", inet_ntoa(src));
        printf("Destination IP: %s\n", inet_ntoa(dst));
    }

    close(sockfd);
    return 0;
}
```