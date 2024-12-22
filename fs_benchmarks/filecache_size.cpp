#include "../utils.hpp"
#define BUF_SIZE 65536

long MB = 1024*1024;
long GB = 1024*MB;
long CPU_SPEED = 2.6e9;
struct timediff t;
const char* file_path = "./bin/temp_file";

uint64_t file_cache_size_read(int fd, long double fileSize, char * buffer) {
    //Go to the beginning of the file
    lseek(fd, 0, SEEK_SET);

    tic(t);

    for (long double i = 0.0; i < fileSize; i += BUF_SIZE) {
        // int status = posix_fadvise64(fd, 0, fileSize, POSIX_FADV_DONTNEED);
        // if(status != 0) {
        //     std::cerr << "Failed to set fadvise usage pattern advice! Returned : " << status <<  "\n";
        //     return -1;
        // }
        read(fd, buffer, BUF_SIZE);
    }

    toc(t);

    uint64_t start_time = getcycles(t.cycles_high0, t.cycles_low0);
    uint64_t end_time = getcycles(t.cycles_high1, t.cycles_low1);
    uint64_t cycles_taken = end_time - start_time;
    return cycles_taken;
}


int main()
{
    int SAMPLES = 16;

    long long int sizes[] = {128*MB, 256*MB, 512*MB, 1*GB, 2*GB, 4*GB, 8*GB, 16*GB};
    int SIZES = sizeof(sizes)/sizeof(long long int);
    // long long int sizes[] = {12*GB, 13*GB, 14*GB, 15*GB};
    long double averages[SIZES];
    long double timesForCurrFile[SAMPLES];

    set_priority(-20);
    
    // Sync pending writes from file buffer cache to disk, and clear all file caches
    int status = system("sync && echo 3 > /proc/sys/vm/drop_caches");
    if (status != 0) {
        std::cerr << "Error: system() failed to create a child process or invoke the shell" << '\n';
        return -1;
    }

    // Open large file for reading
    int fd = open(file_path, O_RDONLY);

    char * buffer = (char *) malloc(BUF_SIZE*sizeof(char));

    for (int i = 0; i < SIZES; i++) {
        long double fSize = sizes[i];
        std::cout << "The current size of the file is: " << fSize << "bytes\n";
        for (int j = 0; j < SAMPLES; j++) {
            long double bytesForFile = sizes[i];

            //First time of loading in the data
            file_cache_size_read(fd, bytesForFile, buffer);

            //Do a second read now, see if it's fitted into the cache.
            long double time_taken = file_cache_size_read(fd, bytesForFile, buffer);

            //Store into the array to calculate average later.
            timesForCurrFile[j] = time_taken;
        }

        long double totaltime = 0;
        for (int i = 0; i < SAMPLES; i++) {
            totaltime += timesForCurrFile[i];
        }
        long double average_cycles = totaltime / SAMPLES;
        std::cout << "Average cycles: "<< average_cycles << "\n";

        long double avg_time = (double)(average_cycles*1e3 / (CPU_SPEED));
        std::cout << "Average milliseconds:" << avg_time << "\n";

        averages[i] = avg_time;
    }

    close(fd);
    free(buffer);

    return 0;
}