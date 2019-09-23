#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <regex.h>
#include <string.h>
#include <ftw.h>
#include <queue>
#include <iostream>
#include <time.h>
#include "PriorityString.hpp"

using namespace std;

regex_t reg;
pthread_t *threads;
pthread_mutex_t thread_lock;

priority_queue<
    PriorityString, vector<PriorityString>, less<PriorityString>
> file_pq;

int threads_count, has_ended;

char *dirname;

regex_t initRegex(char *pattern) {
    regex_t _reg;
    if (regcomp(&_reg, pattern, 0) != 0) {
        fprintf(stderr, "Erro ao compilar o regex!\n");
        exit(1);
    }
    return _reg;
}

void *processFile(void *arg);
void *producer(void *arg);

int initThreads() {
    pthread_mutex_init(&thread_lock, NULL);

    threads = (pthread_t *)malloc((threads_count) * sizeof(pthread_t));

    // thread for producer
    pthread_create(&threads[0], NULL, &producer, NULL);

    // thread for consumer
    for (int i = 1; i < threads_count; i++) {
        pthread_create(&threads[i], NULL, &processFile, NULL);
    }

    return 0;
}

void killThreads() {
    free(threads);
    pthread_mutex_destroy(&thread_lock);
}

int isMatch(const char *line, regex_t *reg) {
    return regexec(reg, line, 0, NULL, 0) == 0;
}

int checkMatch(char *fpath) {
    FILE *fileToRead = fopen(fpath, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int linenum = 0;

    while ((read = getline(&line, &len, fileToRead)) != -1) {
        if (isMatch(line, &reg))
            printf("%s: %d\n", fpath, linenum);
        linenum++;
    }

    free(line);
    fclose(fileToRead);

    return 0;
}

void *processFile(void *arg) {
    // Consumer
    while (true) {
        // sleep if queue is empty
        while (file_pq.empty() && !has_ended) {
            nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
            continue;
        }
        if (file_pq.empty() && has_ended)
            break;

        // <seção crítica>
        pthread_mutex_lock(&thread_lock);
        if (file_pq.empty()) {
            pthread_mutex_unlock(&thread_lock);
            continue;
        }

        string fpath_str = file_pq.top().value;
        file_pq.pop();
        pthread_mutex_unlock(&thread_lock);
        // </seção crítica>

        char *fpath = &fpath_str[0];
        checkMatch(fpath);
    }
    return 0;
}

int walk(const char *fpath, const struct stat *sb, int tflag,
        struct FTW *ftwbuf) {

    if (tflag != FTW_F)
        return 0;

    // Producer: put the file on a queue

    // sleep if queue is too big
    while (file_pq.size() > 1000) {
        nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
        continue;
    }

    // <seção crítica>
    pthread_mutex_lock(&thread_lock);
    file_pq.push({sb->st_size, fpath});
    pthread_mutex_unlock(&thread_lock);
    // </seção crítica>

    return 0;
}

void *producer(void *arg) {
    has_ended = false;
    nftw(dirname, walk, 20, 0);
    has_ended = true;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("USAGE:   pgrep [max_threads] [pattern] [dir]\n");
        return 1;
    }

    // read args
    threads_count = atoi(argv[1]);
    char *pattern = argv[2];
    dirname = argv[3];

    if (threads_count < 3) {
        fprintf(stderr, "Please specify at least 3 threads.\n");
        return 1;
    }

    // since main thread is also a thread, that's one
    threads_count--;

    // init
    reg = initRegex(pattern);
    initThreads();

    // process
    for (int i = 0; i < threads_count; i++) {
        int *a = (int *)malloc(sizeof(int));
        pthread_join(threads[i], (void**)&a);
        free(a);
    }

    // free memory
    killThreads();
    regfree(&reg);

    return 0;
}
