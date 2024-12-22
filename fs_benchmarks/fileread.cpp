#include "../utils.hpp"
#define BUF_SIZE 65536

long MB = 1024*1024;
long GB = 1024*MB;
long CPU_SPEED = 2.6e9;
struct timediff t;
const char* file_path = "./bin/temp_file";

uint64_t sequential_reads(int fd, long double fileSize, char * buffer) {
    //Seek to 64K before the end of the file
    lseek(fd, -1*BUF_SIZE, SEEK_END);

    int status = posix_fadvise64(fd, 0, fileSize, POSIX_FADV_DONTNEED);
    if(status != 0) {
        std::cerr << "Failed to set fadvise usage pattern advice! Returned : " << status <<  "\n";
        return -1;
    }

    uint64_t total_cycles = 0;
    
    for (long double i = 2; i < fileSize/BUF_SIZE; ++i) {
        tic(t);
        read(fd, buffer, BUF_SIZE);
        toc(t);
         uint64_t start_time = getcycles(t.cycles_high0, t.cycles_low0);
        uint64_t end_time = getcycles(t.cycles_high1, t.cycles_low1);
        uint64_t cycles_taken = end_time - start_time;
        total_cycles += cycles_taken;
        lseek(fd, -i*BUF_SIZE, SEEK_END);
    }
    

   
    return total_cycles;
}

static int inline generate_random_number(long int upper){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distr(1, upper);
    return distr(rng);
}

uint64_t random_reads(int fd, long double fileSize, char* buffer) {
    //Seek to 64K before the end of the file
    lseek(fd, -1*BUF_SIZE, SEEK_END);

    int status = posix_fadvise64(fd, 0, fileSize, POSIX_FADV_DONTNEED);
    if(status != 0) {
        std::cerr << "Failed to set fadvise usage pattern advice! Returned : " << status <<  "\n";
        return -1;
    }

    long int numBlocks = fileSize/BUF_SIZE;

    uint64_t total_cycles = 0;
    for (long double i = 2; i < fileSize/BUF_SIZE; ++i) {
        tic(t);
        read(fd, buffer, BUF_SIZE);
        lseek(fd, -generate_random_number(numBlocks)*BUF_SIZE, SEEK_END);
        toc(t);
        uint64_t start_time = getcycles(t.cycles_high0, t.cycles_low0);
        uint64_t end_time = getcycles(t.cycles_high1, t.cycles_low1);
        uint64_t cycles_taken = end_time - start_time;
        total_cycles += cycles_taken;
        
    }
    return total_cycles;
}

int main()
{
    int SAMPLES = 16;
    long long int sizes[] = {128*MB, 256*MB, 512*MB, 1*GB, 2*GB, 4*GB};
    int SIZES = sizeof(sizes)/sizeof(long long int);
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

    std::cout << "!!!!!!!!!!!!!!SEQUENTIAL READ!!!!!!!!!!!!!!\n";

    char * buffer = (char *) malloc(BUF_SIZE*sizeof(char));

    for (int i = 0; i < SIZES; i++) {
        long double fSize = sizes[i];
        std::cout << "The current size of the file is: " << fSize << "bytes\n";
        for (int j = 0; j < SAMPLES; j++) {
            long double bytesForFile = sizes[i];

            //File read access 
            long double time_taken = sequential_reads(fd, bytesForFile, buffer);

            std::cout << "time_taken" << time_taken << '\n';

            //Store into the array to calculate average later.
            timesForCurrFile[j] = time_taken;
        }

        long double totaltime = 0;
        for (int i = 1; i < SAMPLES; i++) {
            totaltime += timesForCurrFile[i];
        }
        long double average_cycles = (double)totaltime / (SAMPLES-1);
        std::cout << "Average cycles: "<< average_cycles << "\n";

        long double avg_time = (double)(average_cycles*1e3 / (CPU_SPEED));
        std::cout << "Average milliseconds:" << avg_time << "\n";

        averages[i] = avg_time;
    }

    close(fd);
    // Close then re-open file for part 2 
    
    fd = open(file_path, O_RDONLY);

    std::cout << "!!!!!!!!!!!!!!RANDOM READ!!!!!!!!!!!!!!\n";

    // Sync pending writes from file buffer cache to disk, and clear all file caches
    status = system("sync && echo 3 > /proc/sys/vm/drop_caches");
    if (status != 0) {
        std::cerr << "Error: system() failed to create a child process or invoke the shell" << '\n';
        return -1;
    }


    for (int i = 0; i < SIZES; i++) {
        long double fSize = sizes[i];
        std::cout << "The current size of the file is: " << fSize << "bytes\n";
        for (int j = 0; j < SAMPLES; j++) {
            long double bytesForFile = sizes[i];

            //File read access 
            long double time_taken = random_reads(fd, bytesForFile, buffer);
            std::cout << "time_taken" << time_taken << '\n';

            //Store into the array to calculate average later.
            timesForCurrFile[j] = time_taken;
        }

        long double totaltime = 0;
        for (int i = 1; i < SAMPLES; i++) {
            totaltime += timesForCurrFile[i];
        }
        long double average_cycles = (double) totaltime / (SAMPLES-1);
        std::cout << "Average cycles: "<< average_cycles << "\n";

        long double avg_time = (double)(average_cycles*1e3 / (CPU_SPEED));
        std::cout << "Average milliseconds:" << avg_time << "\n";

        averages[i] = avg_time;
    }

    close(fd);

    
    free(buffer);

    return 0;
}