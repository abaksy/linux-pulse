#include "../utils.hpp"
#define FS_BLOCK_SIZE 4096 // stat -fc %s .

long MB = 1024*1024;
long GB = 1024*MB;
long CPU_SPEED = 2.6e9;

struct timediff t;

uint64_t contention_read(int fd, long double fileSize, char * buf) {

    /* set file pos indicator to beginning */
    lseek(fd, 0, SEEK_SET);

    // long double cycles_taken = 0;
    int status = posix_fadvise64(fd, 0, fileSize, POSIX_FADV_DONTNEED);
    if(status != 0) {
        std::cerr << "Failed to set fadvise usage pattern advice! Returned : " << status <<  "\n";
        return -1;
    }

    tic(t);
    for (long double i = 0.0; i < fileSize; i += FS_BLOCK_SIZE) {
        read(fd, buf, FS_BLOCK_SIZE);
    }
    toc(t);
    uint64_t start_time = getcycles(t.cycles_high0, t.cycles_low0);
    uint64_t end_time = getcycles(t.cycles_high1, t.cycles_low1);
    uint64_t cycles_taken = end_time - start_time;
    return cycles_taken;
}



int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        printf("No valid file was passed in!");
        return 1;
    }

    set_priority(-20);

    // Sync pending writes from file buffer cache to disk, and clear all file caches
    int status = system("sync && echo 3 > /proc/sys/vm/drop_caches");
    if (status != 0) {
        std::cerr << "Error: system() failed to create a child process or invoke the shell" << '\n';
        return -1;
    }

    char* file_name = argv[1];
    int fd = open(file_name, O_RDONLY);
    char* buf = (char *) malloc(sizeof(char)*FS_BLOCK_SIZE);

    long double file_size = 64*MB;
    long double num_blocks = file_size/(double)FS_BLOCK_SIZE;

    uint64_t cycles_read = contention_read(fd, file_size, buf);
    long double ms_read = (double)((cycles_read*1e3 / (CPU_SPEED))/num_blocks);

    char* id = argv[2];
    int pid = atoi(id);
    char outfilename[30];
    sprintf(outfilename, "contention_res_%d.txt", pid);

    char output[32];
    FILE * result_file = fopen(outfilename, "a");
    sprintf(output, "%Lf", ms_read);
    fprintf(result_file,"%s\n", output);
    fclose(result_file);

    close(fd);
    free(buf);

    return 0;
}
