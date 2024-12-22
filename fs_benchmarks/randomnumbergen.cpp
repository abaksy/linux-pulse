#include "../utils.hpp"


static int inline generate_random_number(long int upper){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distr(1, upper);
    return distr(rng);
}

struct timediff t;

int main()
{
    tic(t);

    generate_random_number(1000);

    toc(t);

    uint64_t start_time = getcycles(t.cycles_high0, t.cycles_low0);
    uint64_t end_time = getcycles(t.cycles_high1, t.cycles_low1);
    uint64_t cycles_taken = end_time - start_time;

    std::cout << cycles_taken << '\n';

    return 0;
}