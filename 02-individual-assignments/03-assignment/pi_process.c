#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#define VALUE 2
#define PI 3.1415926535

int main(int argc, char* argv[]) {
    int num_process = atoi(argv[1]),
        num_points = atoi(argv[2]);
    double *quarter_pi = mmap(NULL, sizeof(double), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0); 
    pid_t process_id;
    sem_t *semaphore = sem_open ("sem", O_CREAT | O_EXCL, 0644, VALUE); 
    double interval_size = 1.0 / num_points;
    int points_per_process;

    if (num_points >= num_process)
        points_per_process = num_points / num_process;
    else
        points_per_process = 1;

    *quarter_pi = 0;
    int i;
    for (i = 0; i < num_process; i++) {
        process_id = fork();
        if (process_id < 0) {
            sem_unlink("sem");
            sem_close(semaphore);
            printf("Error\n");
        }
        else if(process_id == 0) {
            break;
        }
    }

    /* PARENT PROCESS */
    if (process_id != 0) {
        while (process_id = waitpid(-1, NULL, 0)) {
            if (errno == ECHILD)
                break;
        }
        printf("%.20lf\n", *quarter_pi * 4);

        sem_unlink ("sem");   
        sem_close(semaphore);

        munmap(quarter_pi, sizeof(double));
        exit(0);
    }
    /* CHILD PROCESSES */
    else {
        int j;
        double interval_sum = 0;

        if (i >= num_points)
            exit(0);

        for (j = 0; j < points_per_process; j++) {
            double x = (i * points_per_process + j) * interval_size + interval_size / 2;
            interval_sum += sqrt(1.0 - x * x) * interval_size;
        }

        sem_wait(semaphore);
        *quarter_pi += interval_sum;
        sem_post(semaphore);

        exit(0);
    }
}