#include "../utils.hpp"

struct timediff t;
const char* file_path = "./bin/temp_file";


int main() {
    set_priority(-20);

    std::cout << std::fixed << std::setprecision(3);

    size_t page_size = sysconf(_SC_PAGESIZE);  // Get the system page size
    size_t num_pages = 1024 * 1024;              // Allocate space for multiple pages (adjust as needed)
    size_t array_size = page_size * num_pages; // Total array size in bytes

    uint64_t start_time, end_time;
    long long int total_time = 0;

    // Sync pending writes from file buffer cache to disk, and clear all file caches
    int status = system("sync && echo 3 > /proc/sys/vm/drop_caches");

    if (status == -1) {
        std::cerr << "Error: system() failed to create a child process or invoke the shell" << '\n';
        return -1;
    } 

    int fd = open(file_path, O_RDWR);

    // Allocate memory backed by a temp file
    char *buffer = (char *)mmap(nullptr, array_size, PROT_READ | PROT_WRITE,
                            MAP_SHARED, fd, 0);

    if (buffer == MAP_FAILED) {
        std::cout << "Memory allocation failed\n";
        return -1;
    }

    if(posix_madvise(buffer, array_size, POSIX_MADV_DONTNEED) != 0) {
        std::cerr << "Failed to set madvise usage pattern advice!\n";
        delete[] buffer;
        munmap(buffer, array_size);
        return -1; 
    }

    // Pause to give the kernel a chance to swap out the pages
    std::cout << "Pausing to allow swapping...\n";
    sleep(5);

    // Access the memory again, potentially causing a hard page fault
    // std::cout << "Re-accessing memory to cause hard page faults...\n";
    for (size_t i = 0; i < array_size; i += page_size) {
        tic(t);
        buffer[i] = 1;  // Access each page again
        toc(t);
        
        start_time = getcycles(t.cycles_high0, t.cycles_low0);
        end_time = getcycles(t.cycles_high1, t.cycles_low1);
        total_time += (end_time - start_time);
    }

    std::cout << (double)(total_time / num_pages);

    // Cleanup
    munmap(buffer, array_size);
    close(fd);
    return 0;
}
