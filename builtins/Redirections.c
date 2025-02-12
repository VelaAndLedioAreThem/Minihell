#include "builtins.h"


void handle_redirections(char **args) {
    printf("handle_redirections called\n");
    for (int i = 0; args[i]; i++) {
        if (!my_strcmp(args[i], ">")) {
            printf("Redirection: output to file: %s\n", args[i+1]);
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) { perror("open"); exit(EXIT_FAILURE); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        } else if (!my_strcmp(args[i], ">>")) {
            printf("Redirection: append output to file: %s\n", args[i+1]);
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) { perror("open"); exit(EXIT_FAILURE); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        } else if (!my_strcmp(args[i], "<")) {
            printf("Redirection: input from file: %s\n", args[i+1]);
            int fd = open(args[i+1], O_RDONLY);
            if (fd == -1) { perror("open"); exit(EXIT_FAILURE); }
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
            break;
        } else if (!my_strcmp(args[i], "<<")) {
            printf("Redirection: here document with delimiter: %s\n", args[i+1]);
            char *delim = args[i+1];
            
            // Create temporary file
            char tmpname[] = "/tmp/heredocXXXXXX";
            int tmpfd = mkstemp(tmpname);
            if (tmpfd == -1) { perror("mkstemp"); exit(EXIT_FAILURE); }
            unlink(tmpname);
            
            // Read input until delimiter
            char *line = NULL;
            size_t len = 0;
            ssize_t read;
            while ((read = getline(&line, &len, stdin)) != -1) {
                line[strcspn(line, "\n")] = '\0';  // Remove newline
                if (my_strcmp(line, delim) == 0) break;
                line[strlen(line)] = '\n';  // Restore newline for writing
                if (write(tmpfd, line, strlen(line)) == -1) {
                    perror("write"); exit(EXIT_FAILURE);
                }
            }
            free(line);
            
            // Redirect stdin
            lseek(tmpfd, 0, SEEK_SET);
            dup2(tmpfd, STDIN_FILENO);
            close(tmpfd);
            
            args[i] = NULL;
            break;
        }
    }
}
