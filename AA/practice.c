#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

char message[100];

void handle_alarm(int sig) {
    printf("\n%s\n", message);
}

int main(int argc, char* argv[]) {
    int seconds;
    strcpy(message, argv[1]);

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("Usage: %s <message> <seconds>\n", argv[0]);
        return 1;
    }

    if (argc > 2) {
        seconds = atoi(argv[2]);
    } else {
        seconds = 10;
    }

    signal(SIGALRM, handle_alarm);
    alarm(seconds);
    pause();
    return 0;
}