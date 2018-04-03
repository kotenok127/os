#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *const argv[]) {
    for (;;) {
        char *args[100];
        char argp[100][200];
        for (int i = 0; i < 100; i++) args[i] = NULL;
        int i = 0;
        int eosc = scanf("%s", argp[0]);
        if (eosc == -1) break;
        args[0] = argp[0];
        int c;
        while ((c = getchar()) == ' ') {
            i++;
            eosc = scanf("%s", argp[i]);
            args[i] = argp[i];
        }
        const pid_t pid = fork();
        int status;
        if (pid == -1) {
            perror("fork faled");
            exit(EXIT_FAILURE);
        }
        if (!pid) {
            execve(args[0], args, argv);
            perror("execve faled");
            exit(EXIT_FAILURE);
        } else {
            pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
            if (w == -1) {
                perror("waitpid faled");
                exit(EXIT_FAILURE);
            }
            if ((c == EOF) || (eosc == -1)) {
                break;
            }
        }
    }
    return 0;
}

