#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000

int main() {

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    printf("Listening on port %d...\n", PORT);

    uint16_t last_seq = 0;
    uint8_t buffer[64];

    while (1) {

        ssize_t len = recv(sock, buffer, sizeof(buffer), 0);
        if (len < 8)
            continue;

        int offset = 0;

        uint16_t seq;
        memcpy(&seq, buffer + offset, 2);
        seq = ntohs(seq);
        offset += 2;

        int16_t px;
        memcpy(&px, buffer + offset, 2);
        px = ntohs(px);
        offset += 2;

        int16_t py;
        memcpy(&py, buffer + offset, 2);
        py = ntohs(py);
        offset += 2;

        int16_t hp;
        memcpy(&hp, buffer + offset, 2);
        hp = ntohs(hp);
        offset += 2;

        // --- Packet loss detection con overflow correcto ---
        if (last_seq != 0) {
            uint16_t expected = last_seq + 1;  // overflow automático
            if (seq != expected) {
                printf("⚠ Packet loss! Expected %u got %u\n",
                       expected, seq);
            }
        }

        last_seq = seq;

        printf("SEQ:%u PLAYER (%d,%d) HP:%d\n",
               seq, px, py, hp);
    }

    close(sock);
    return 0;
}
