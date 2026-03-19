#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MEMORY 1024

// ================= STRUCT =================
typedef struct {
    char name[256];
    int priority;
    int pid;
    int address;
    int memory;
    int runtime;
    bool suspended;
} proc;

// ================= QUEUE =================
typedef struct {
    proc items[100];
    int front;
    int rear;
    int count;
} queue;

void init(queue* q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

bool is_empty(queue* q) {
    return q->count == 0;
}

void push(queue* q, proc p) {
    if (q->count == 100) {
        fprintf(stderr, "Queue overflow while pushing process %s\n", p.name);
        exit(1);
    }
    q->items[q->rear] = p;
    q->rear = (q->rear + 1) % 100;
    q->count++;
}

proc pop(queue* q) {
    if (q->count == 0) {
        fprintf(stderr, "Queue underflow while popping process\n");
        exit(1);
    }
    proc p = q->items[q->front];
    q->front = (q->front + 1) % 100;
    q->count--;
    return p;
}

// ================= MEMORY =================
int avail_mem[MEMORY] = {0};

int allocate_memory(int size) {
    int count = 0;

    for (int i = 0; i < MEMORY; i++) {
        if (avail_mem[i] == 0) {
            count++;
            if (count == size) {
                int start = i - size + 1;
                for (int j = start; j <= i; j++)
                    avail_mem[j] = 1;
                return start;
            }
        } else {
            count = 0;
        }
    }
    return -1;
}

void free_memory(int start, int size) {
    for (int i = start; i < start + size; i++)
        avail_mem[i] = 0;
}

// ================= MAIN =================
int main() {

    FILE *file = fopen("processes_q2.txt", "r");
    if (!file) {
        perror("File error");
        return 1;
    }

    queue priority, secondary;
    init(&priority);
    init(&secondary);

    char line[256];

    // ===== READ FILE =====
    while (fgets(line, sizeof(line), file)) {
        proc p;

        sscanf(line, "%[^,], %d, %d, %d",
               p.name, &p.priority, &p.memory, &p.runtime);

        p.pid = 0;
        p.address = 0;
        p.suspended = false;

        if (p.priority == 0)
            push(&priority, p);
        else
            push(&secondary, p);
    }

    fclose(file);

    // ================= PRIORITY QUEUE =================
    while (!is_empty(&priority)) {
        proc p = pop(&priority);

        int addr = allocate_memory(p.memory);
        if (addr == -1) {
            printf("Not enough memory for %s\n", p.name);
            continue;
        }

        p.address = addr;

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            free_memory(addr, p.memory);
            continue;
        }

        if (pid == 0) {
            execl("./process", "process", NULL);
            perror("exec failed");
            exit(1);
        } else {
            p.pid = pid;
            printf("START %s | Priority:%d | PID:%d | Mem:%d | Time:%d\n",
                   p.name, p.priority, p.pid, p.memory, p.runtime);
            sleep(p.runtime);
            kill(pid, SIGTSTP);
            kill(pid, SIGINT);
            waitpid(pid, NULL, 0);
            free_memory(addr, p.memory);
        }
    }

    // ================= SECONDARY QUEUE =================
    while (!is_empty(&secondary)) {
        proc p = pop(&secondary);

        if (!p.suspended) {
            int addr = allocate_memory(p.memory);
            if (addr == -1) {
                push(&secondary, p);
                continue;
            }
            p.address = addr;
        }

        if (p.runtime == 1) {
            if (!p.suspended) {
                pid_t pid = fork();
                if (pid < 0) {
                    perror("fork failed");
                    free_memory(p.address, p.memory);
                    continue;
                }
                if (pid == 0) {
                    execl("./process", "process", NULL);
                    perror("exec failed");
                    exit(1);
                }
                p.pid = pid;
                printf("START %s | Priority:%d | PID:%d | Mem:%d | Time:%d\n",
                       p.name, p.priority, p.pid, p.memory, p.runtime);
            } else {
                printf("START %s | Priority:%d | PID:%d | Mem:%d | Time:%d\n",
                       p.name, p.priority, p.pid, p.memory, p.runtime);
                kill(p.pid, SIGCONT);
            }

            sleep(1);
            kill(p.pid, SIGINT);
            waitpid(p.pid, NULL, 0);
            free_memory(p.address, p.memory);
            continue;
        }

        if (!p.suspended) {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork failed");
                free_memory(p.address, p.memory);
                continue;
            }
            if (pid == 0) {
                execl("./process", "process", NULL);
                perror("exec failed");
                exit(1);
            }
            p.pid = pid;
            printf("START %s | Priority:%d | PID:%d | Mem:%d | Time:%d\n",
                   p.name, p.priority, p.pid, p.memory, p.runtime);
        } else {
            printf("START %s | Priority:%d | PID:%d | Mem:%d | Time:%d\n",
                   p.name, p.priority, p.pid, p.memory, p.runtime);
            kill(p.pid, SIGCONT);
        }

        sleep(1);
        kill(p.pid, SIGTSTP);

        p.runtime--;
        p.suspended = true;

        push(&secondary, p);
    }

    return 0;
}
