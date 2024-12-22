#include "../utils.hpp"

#define ENSEMBLE_SIZE 10000
#define KB 1024
#define MB 1024 * KB
#define GB 1024 * MB

#define MEM_SIZE 4 * MB             // 4MB
#define ARR_SIZE (MEM_SIZE >> 2)    // Number of integers in array, divide MEM_SIZE by 4
#define CACHE_SIZE (32 * MB)        // 32MB, considerably larger than LLC size on our machine

// Clear the cache lines to make sure we touch memory only
char foo[CACHE_SIZE], bar[CACHE_SIZE];
#define CLEAR_CACHE memcpy(foo, bar, CACHE_SIZE)

struct timediff t;

uint64_t cycles_read(int* start, int* end) 
{
    uint64_t start_time, end_time, total_time = 0;
    
    int* ptr = NULL;
    double* times = new double[ENSEMBLE_SIZE];

    int timing_overhead = readTimingOverhead();

    for(int i = 0; i < ENSEMBLE_SIZE; ++i)
    {
        ptr = start;
        int sum = 0;

        // Memcpy instruction that occupies the cache
        CLEAR_CACHE;

        tic(t);

        // Hand unrolled loop to simulate read access(LOL)
        while(ptr < end)
        {
            sum = ptr[0]; sum = ptr[8]; sum = ptr[16]; sum = ptr[24]; sum = ptr[32]; sum = ptr[40]; sum = ptr[48]; sum = ptr[56]; sum = ptr[64]; 
            sum = ptr[72]; sum = ptr[80]; sum = ptr[88]; sum = ptr[96]; sum = ptr[104]; sum = ptr[112]; sum = ptr[120]; sum = ptr[128]; 
            sum = ptr[136]; sum = ptr[144]; sum = ptr[152]; sum = ptr[160]; sum = ptr[168]; sum = ptr[176]; sum = ptr[184]; sum = ptr[192]; 
            sum = ptr[200]; sum = ptr[208]; sum = ptr[216]; sum = ptr[224]; sum = ptr[232]; sum = ptr[240]; sum = ptr[248]; sum = ptr[256]; 
            sum = ptr[264]; sum = ptr[272]; sum = ptr[280]; sum = ptr[288]; sum = ptr[296]; sum = ptr[304]; sum = ptr[312]; sum = ptr[320]; 
            sum = ptr[328]; sum = ptr[336]; sum = ptr[344]; sum = ptr[352]; sum = ptr[360]; sum = ptr[368]; sum = ptr[376]; sum = ptr[384]; 
            sum = ptr[392]; sum = ptr[400]; sum = ptr[408]; sum = ptr[416]; sum = ptr[424]; sum = ptr[432]; sum = ptr[440]; sum = ptr[448]; 
            sum = ptr[456]; sum = ptr[464]; sum = ptr[472]; sum = ptr[480]; sum = ptr[488]; sum = ptr[496]; sum = ptr[504]; sum = ptr[512]; 
            sum = ptr[520]; sum = ptr[528]; sum = ptr[536]; sum = ptr[544]; sum = ptr[552]; sum = ptr[560]; sum = ptr[568]; sum = ptr[576]; 
            sum = ptr[584]; sum = ptr[592]; sum = ptr[600]; sum = ptr[608]; sum = ptr[616]; sum = ptr[624]; sum = ptr[632]; sum = ptr[640]; 
            sum = ptr[648]; sum = ptr[656]; sum = ptr[664]; sum = ptr[672]; sum = ptr[680]; sum = ptr[688]; sum = ptr[696]; sum = ptr[704]; 
            sum = ptr[712]; sum = ptr[720]; sum = ptr[728]; sum = ptr[736]; sum = ptr[744]; sum = ptr[752]; sum = ptr[760]; sum = ptr[768]; 
            sum = ptr[776]; sum = ptr[784]; sum = ptr[792]; sum = ptr[800]; sum = ptr[808]; sum = ptr[816]; sum = ptr[824]; sum = ptr[832]; 
            sum = ptr[840]; sum = ptr[848]; sum = ptr[856]; sum = ptr[864]; sum = ptr[872]; sum = ptr[880]; sum = ptr[888]; sum = ptr[896]; 
            sum = ptr[904]; sum = ptr[912]; sum = ptr[920]; sum = ptr[928]; sum = ptr[936]; sum = ptr[944]; sum = ptr[952]; sum = ptr[960]; 
            sum = ptr[968]; sum = ptr[976]; sum = ptr[984]; sum = ptr[992]; sum = ptr[1000]; sum = ptr[1008]; sum = ptr[1016];

            ptr += 1024;
            
        }
        
        toc(t);
        start_time = getcycles(t.cycles_high0, t.cycles_low0);
        end_time = getcycles(t.cycles_high1, t.cycles_low1);
        times[i] = (end_time - start_time - timing_overhead);
        total_time += end_time - start_time - timing_overhead;
    }

    double mean_time = (double)total_time/ENSEMBLE_SIZE;
    double sq_errors = 0.0;
    for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        sq_errors = (times[i] - mean_time)*(times[i] - mean_time);
    
    double stdev = sqrt(sq_errors/(ENSEMBLE_SIZE-1));
    std::cout << "Standard Deviation in reads: " << stdev << '\n';
    delete[] times;

    return total_time / ENSEMBLE_SIZE;
}

uint64_t cycles_write(int* start, int* end) 
{
    uint64_t start_time, end_time, total_time = 0;
    
    int* ptr = NULL;

    int timing_overhead = readTimingOverhead();
    double* times = new double[ENSEMBLE_SIZE];

    for(int i = 0; i < ENSEMBLE_SIZE; ++i)
    {
        ptr = start;
        int sum = 0;

        // Memcpy instruction that occupies the cache
        CLEAR_CACHE;

        tic(t);

        // Hand unrolled loop to simulate write access(LOL)
        while(ptr < end)
        {
            ptr[0] = 0; ptr[8] = 0; ptr[16] = 0; ptr[24] = 0; ptr[32] = 0; ptr[40] = 0; ptr[48] = 0; ptr[56] = 0; ptr[64] = 0;
            ptr[72] = 0; ptr[80] = 0; ptr[88] = 0; ptr[96] = 0; ptr[104] = 0; ptr[112] = 0; ptr[120] = 0; ptr[128] = 0; 
            ptr[136] = 0; ptr[144] = 0; ptr[152] = 0; ptr[160] = 0; ptr[168] = 0; ptr[176] = 0; ptr[184] = 0; ptr[192] = 0; 
            ptr[200] = 0; ptr[208] = 0; ptr[216] = 0; ptr[224] = 0; ptr[232] = 0; ptr[240] = 0; ptr[248] = 0; ptr[256] = 0; 
            ptr[264] = 0; ptr[272] = 0; ptr[280] = 0; ptr[288] = 0; ptr[296] = 0; ptr[304] = 0; ptr[312] = 0; ptr[320] = 0; 
            ptr[328] = 0; ptr[336] = 0; ptr[344] = 0; ptr[352] = 0; ptr[360] = 0; ptr[368] = 0; ptr[376] = 0; ptr[384] = 0; 
            ptr[392] = 0; ptr[400] = 0; ptr[408] = 0; ptr[416] = 0; ptr[424] = 0; ptr[432] = 0; ptr[440] = 0; ptr[448] = 0; 
            ptr[456] = 0; ptr[464] = 0; ptr[472] = 0; ptr[480] = 0; ptr[488] = 0; ptr[496] = 0; ptr[504] = 0; ptr[512] = 0; 
            ptr[520] = 0; ptr[528] = 0; ptr[536] = 0; ptr[544] = 0; ptr[552] = 0; ptr[560] = 0; ptr[568] = 0; ptr[576] = 0; 
            ptr[584] = 0; ptr[592] = 0; ptr[600] = 0; ptr[608] = 0; ptr[616] = 0; ptr[624] = 0; ptr[632] = 0; ptr[640] = 0; 
            ptr[648] = 0; ptr[656] = 0; ptr[664] = 0; ptr[672] = 0; ptr[680] = 0; ptr[688] = 0; ptr[696] = 0; ptr[704] = 0; 
            ptr[712] = 0; ptr[720] = 0; ptr[728] = 0; ptr[736] = 0; ptr[744] = 0; ptr[752] = 0; ptr[760] = 0; ptr[768] = 0; 
            ptr[776] = 0; ptr[784] = 0; ptr[792] = 0; ptr[800] = 0; ptr[808] = 0; ptr[816] = 0; ptr[824] = 0; ptr[832] = 0; 
            ptr[840] = 0; ptr[848] = 0; ptr[856] = 0; ptr[864] = 0; ptr[872] = 0; ptr[880] = 0; ptr[888] = 0; ptr[896] = 0; 
            ptr[904] = 0; ptr[912] = 0; ptr[920] = 0; ptr[928] = 0; ptr[936] = 0; ptr[944] = 0; ptr[952] = 0; ptr[960] = 0; 
            ptr[968] = 0; ptr[976] = 0; ptr[984] = 0; ptr[992] = 0; ptr[1000] = 0; ptr[1008] = 0; ptr[1016] = 0;

            ptr += 1024;
        }
        
        toc(t);
        start_time = getcycles(t.cycles_high0, t.cycles_low0);
        end_time = getcycles(t.cycles_high1, t.cycles_low1);
        times[i] = (end_time - start_time - timing_overhead);
        total_time += (end_time - start_time - timing_overhead);
    }

    double mean_time = (double)total_time/ENSEMBLE_SIZE;
    double sq_errors = 0.0;
    for(int i = 0; i < ENSEMBLE_SIZE; ++i)
        sq_errors = (times[i] - mean_time)*(times[i] - mean_time);
    
    double stdev = sqrt(sq_errors/(ENSEMBLE_SIZE-1));
    std::cout << "Standard Deviation in writes: " << stdev << '\n';
    delete[] times;

    return total_time / ENSEMBLE_SIZE;
}



int main() {
    set_priority(-20);
    uint64_t tw, tr;
    int arr[ARR_SIZE];

    tr = cycles_read(arr, arr + ARR_SIZE);
    tw = cycles_write(arr, arr + ARR_SIZE);

    // bandwidth: GB/s = GB * cycles/s * 1/cycles
    #define t2b(t) (MEM_SIZE*1.0 / (GB) * 2.6e9 / (t))

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Read Bandwidth: " << t2b(tr) << "  Write Bandwidth: " << t2b(tw) << '\n';

    return 0; 
}