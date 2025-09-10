#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>

typedef struct {
    int valid;
    unsigned int tag;
    unsigned int time_stamp;
} cacheLine;


void printHelpMessage();
void printTraces(char, unsigned int, int);
void findCache(cacheLine **cache, unsigned int address, int *hits, int *misses, int *evictions, int set_bits, int block_bits, int lines, bool verbose);

static unsigned int time_stamp = 0;

int main(int argc, char*argv[])
{
    bool verbose = false;
    int opt;
    int set_bits, lines, block_bits;
    int sets;
    char *trace_file;

    int hits = 0;
    int misses = 0;
    int evictions = 0;

    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (opt) {
            case 'h':
                printHelpMessage();
                return 0;
            case 'v':
                verbose = true;
                break;
            case 's':
                set_bits = atoi(optarg);
                sets = 1 << set_bits;
                break;
            case 'E':
                lines = atoi(optarg);
                break;
            case 'b':
                block_bits = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case '?':
                printHelpMessage();
                return 0;
            case ':':
                printf("./csim: option requires an argument -- '%c'\n", optopt);
                printHelpMessage();
                return 0;
            default:
                break;
        }
    }

    cacheLine **cache = (cacheLine **)malloc(sets * sizeof(cacheLine *));
    cacheLine *data = (cacheLine *)malloc(sets * lines * sizeof(cacheLine));

    for (int i = 0; i < sets; i++) {
        cache[i] = data + i * lines;
    }

    for (int i = 0; i < sets; i++) {
        for (int j = 0; j < lines; j++) {
            (cache[i][j]).valid = 0;
        }
    }

    FILE *trace = fopen(trace_file, "r");
    char operation;
    unsigned int address;
    int size;

    while (fscanf(trace, " %c %x,%d", &operation, &address, &size) == 3) {
        if (verbose) {
            printf("%c %x,%d ", operation, address, size);
        }

        switch (operation) {
            case 'I':
                break;
            case 'L':
            case 'S':
                findCache(cache, address, &hits, &misses, &evictions, set_bits, block_bits, lines, verbose);
                break;
            case 'M':
                findCache(cache, address, &hits, &misses, &evictions, set_bits, block_bits, lines, verbose);
                findCache(cache, address, &hits, &misses, &evictions, set_bits, block_bits, lines, verbose);
                break;
        }

        if (verbose) {
            printf("\n");
        }
    }

    fclose(trace);

    printSummary(hits, misses, evictions);
    return 0;
}

void printHelpMessage() {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf(" -h          Print this help message.\n");
    printf(" -v          Optional verbose flag.\n");
    printf(" -s <num>    Number of set index bits.\n");
    printf(" -E <num>    Number of lines per set.\n");
    printf(" -b <num>    Number of block offest bits.\n");
    printf(" -t <file>   Trace file.\n");
    printf("\nExamples\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

void printTraces(char operation, unsigned int address, int size) {
    printf("%c %x,%d ", operation, address, size);
}

void findCache(cacheLine **cache, unsigned int address, int *hits, int *misses, int *evictions, int set_bits, int block_bits, int lines, bool verbose){
    int sets_ind = (address >> block_bits) & ((1 << set_bits) - 1);
    unsigned int tag = address >> (set_bits + block_bits);

    cacheLine *cacheSet = cache[sets_ind];

    time_stamp++;
    unsigned int min_time_stamp = time_stamp;
    int replace_index;
    for (int i = 0; i < lines; i++) {
        if ((cacheSet[i]).valid == 0) {
            (cacheSet[i]).valid = 1;
            (cacheSet[i]).tag = tag;
            (cacheSet[i]).time_stamp = time_stamp;
            (*misses)++;

            if (verbose) {
                printf("miss ");
            }

            return;
        }

        if (min_time_stamp > (cacheSet[i]).time_stamp) {
            min_time_stamp = (cacheSet[i]).time_stamp;
            replace_index = i;
        }

        if ((cacheSet[i]).tag == tag) {
            (cacheSet[i]).time_stamp = time_stamp;
            (*hits)++;

            if (verbose) {
                printf("hit ");
            }

            return;
        }
    }

    (cacheSet[replace_index]).tag = tag;
    (cacheSet[replace_index]).time_stamp = time_stamp;
    (*misses)++;
    (*evictions)++;

    if (verbose) {
        printf("miss evictions ");
    }
}
