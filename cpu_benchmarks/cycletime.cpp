#include "../utils.hpp"
#include <time.h>


int main()
{
    uint64_t start_time, end_time;
    struct timediff t;
    long int tt = 0;
    for(int i = 0; i<10; ++i)
    {
        tic(t);

        sleep(10);

        toc(t);
        start_time = getcycles(t.cycles_high0, t.cycles_low0);
        end_time = getcycles(t.cycles_high1, t.cycles_low1);

        assert(start_time <= end_time);
        // std::cout<< (end_time - start_time);

        tt += (end_time - start_time);
        std::cout<< "Cycle Time:" << (end_time - start_time) << '\n';
    }

    std::cout<< "Average time:" << (double)tt/100 << "Hz\n";

    return 0;
}
