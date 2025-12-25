#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Размер страницы памяти (4 KiB)
#define PAGE_SIZE 4096

// Размер блока, после которого делаем паузу (128 MiB)
#define CHUNK_SIZE (128 * 1024 * 1024)

// Общий объем памяти для выделения (например, 4 GiB)
#define TOTAL_MEMORY (4L * 1024L * 1024L * 1024L)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <r|w>\n", argv[0]);
        printf("  r: Read mode\n");
        printf("  w: Write mode\n");
        return 1;
    }

    char mode = argv[1][0];
    if (mode != 'r' && mode != 'w') {
        printf("Invalid mode. Use 'r' or 'w'.\n");
        return 1;
    }

    printf("Attempting to allocate %lu GiB of virtual memory...\n", TOTAL_MEMORY / 1024 / 1024 / 1024);

    // Выделяем память (malloc)
    char *memory = (char *)malloc(TOTAL_MEMORY);

    if (memory == NULL) {
        perror("Malloc failed");
        return 1;
    }

    printf("Allocation successful!\n");
    printf("Check your task manager (top/htop).\n");
    printf("Look at VIRT vs RES.\n");
    printf("Press ENTER to start %s process...\n", (mode == 'w' ? "WRITING" : "READING"));
    getchar(); // Ждем нажатия Enter

    size_t processed = 0;
    
    volatile char temp_char; 

    for (size_t i = 0; i < TOTAL_MEMORY; i += PAGE_SIZE) {
        
        if (mode == 'w') {
            memory[i] = 1; 
        } else {
            temp_char = memory[i];
        }

        // Логика паузы каждые 128 MiB
        if ((i > 0) && (i % CHUNK_SIZE == 0)) {
            processed += CHUNK_SIZE;
            printf("Processed %lu MiB. Press ENTER to continue...", processed / 1024 / 1024);
            while(getchar() != '\n'); 
        }
    }

    printf("\nDone. Press ENTER to exit and free memory.\n");
    getchar();

    free(memory);
    return 0;
}