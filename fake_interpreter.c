#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 65536  // 64 КБ

int main(int argc, char *argv[]) {
    const char *filename = NULL;
    
    char *program_name = strrchr(argv[0], '/');
    program_name = program_name ? program_name + 1 : argv[0];

    if (strcmp(program_name, "bash") == 0) {
        if (argc < 3 || strcmp(argv[1], "-c") != 0) {
            fprintf(stderr, "Использование bash: %s -c \"source <файл>\"\n", argv[0]);
            return 1;
        }
        if (strncmp(argv[2], "source ", 7) == 0) {
            filename = argv[2] + 7;  // Пропускаем "source "
        } else {
            fprintf(stderr, "Ожидалась команда 'source'\n");
            return 1;
        }
    } else if (strcmp(program_name, "python") == 0) {
        if (argc < 3 || strcmp(argv[1], "-u") != 0) {
            fprintf(stderr, "Использование python: %s -u <файл>\n", argv[0]);
            return 1;
        }
        filename = argv[2];
    } else {
        fprintf(stderr, "Неизвестная программа: %s\n", program_name);
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Не удалось открыть файл: %s\n", filename);
        return 1;
    }

    char buffer[MAX_BUFFER];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }

    fclose(file);
    return 0;
}
