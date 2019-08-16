#include "libalgebra.h"

#include <iostream>
#include <random>
#include <chrono>

uint8_t popcnt_lookup8bit[256] = {
	/* 0 */ 0, /* 1 */ 1, /* 2 */ 1, /* 3 */ 2,
	/* 4 */ 1, /* 5 */ 2, /* 6 */ 2, /* 7 */ 3,
	/* 8 */ 1, /* 9 */ 2, /* a */ 2, /* b */ 3,
	/* c */ 2, /* d */ 3, /* e */ 3, /* f */ 4,
	/* 10 */ 1, /* 11 */ 2, /* 12 */ 2, /* 13 */ 3,
	/* 14 */ 2, /* 15 */ 3, /* 16 */ 3, /* 17 */ 4,
	/* 18 */ 2, /* 19 */ 3, /* 1a */ 3, /* 1b */ 4,
	/* 1c */ 3, /* 1d */ 4, /* 1e */ 4, /* 1f */ 5,
	/* 20 */ 1, /* 21 */ 2, /* 22 */ 2, /* 23 */ 3,
	/* 24 */ 2, /* 25 */ 3, /* 26 */ 3, /* 27 */ 4,
	/* 28 */ 2, /* 29 */ 3, /* 2a */ 3, /* 2b */ 4,
	/* 2c */ 3, /* 2d */ 4, /* 2e */ 4, /* 2f */ 5,
	/* 30 */ 2, /* 31 */ 3, /* 32 */ 3, /* 33 */ 4,
	/* 34 */ 3, /* 35 */ 4, /* 36 */ 4, /* 37 */ 5,
	/* 38 */ 3, /* 39 */ 4, /* 3a */ 4, /* 3b */ 5,
	/* 3c */ 4, /* 3d */ 5, /* 3e */ 5, /* 3f */ 6,
	/* 40 */ 1, /* 41 */ 2, /* 42 */ 2, /* 43 */ 3,
	/* 44 */ 2, /* 45 */ 3, /* 46 */ 3, /* 47 */ 4,
	/* 48 */ 2, /* 49 */ 3, /* 4a */ 3, /* 4b */ 4,
	/* 4c */ 3, /* 4d */ 4, /* 4e */ 4, /* 4f */ 5,
	/* 50 */ 2, /* 51 */ 3, /* 52 */ 3, /* 53 */ 4,
	/* 54 */ 3, /* 55 */ 4, /* 56 */ 4, /* 57 */ 5,
	/* 58 */ 3, /* 59 */ 4, /* 5a */ 4, /* 5b */ 5,
	/* 5c */ 4, /* 5d */ 5, /* 5e */ 5, /* 5f */ 6,
	/* 60 */ 2, /* 61 */ 3, /* 62 */ 3, /* 63 */ 4,
	/* 64 */ 3, /* 65 */ 4, /* 66 */ 4, /* 67 */ 5,
	/* 68 */ 3, /* 69 */ 4, /* 6a */ 4, /* 6b */ 5,
	/* 6c */ 4, /* 6d */ 5, /* 6e */ 5, /* 6f */ 6,
	/* 70 */ 3, /* 71 */ 4, /* 72 */ 4, /* 73 */ 5,
	/* 74 */ 4, /* 75 */ 5, /* 76 */ 5, /* 77 */ 6,
	/* 78 */ 4, /* 79 */ 5, /* 7a */ 5, /* 7b */ 6,
	/* 7c */ 5, /* 7d */ 6, /* 7e */ 6, /* 7f */ 7,
	/* 80 */ 1, /* 81 */ 2, /* 82 */ 2, /* 83 */ 3,
	/* 84 */ 2, /* 85 */ 3, /* 86 */ 3, /* 87 */ 4,
	/* 88 */ 2, /* 89 */ 3, /* 8a */ 3, /* 8b */ 4,
	/* 8c */ 3, /* 8d */ 4, /* 8e */ 4, /* 8f */ 5,
	/* 90 */ 2, /* 91 */ 3, /* 92 */ 3, /* 93 */ 4,
	/* 94 */ 3, /* 95 */ 4, /* 96 */ 4, /* 97 */ 5,
	/* 98 */ 3, /* 99 */ 4, /* 9a */ 4, /* 9b */ 5,
	/* 9c */ 4, /* 9d */ 5, /* 9e */ 5, /* 9f */ 6,
	/* a0 */ 2, /* a1 */ 3, /* a2 */ 3, /* a3 */ 4,
	/* a4 */ 3, /* a5 */ 4, /* a6 */ 4, /* a7 */ 5,
	/* a8 */ 3, /* a9 */ 4, /* aa */ 4, /* ab */ 5,
	/* ac */ 4, /* ad */ 5, /* ae */ 5, /* af */ 6,
	/* b0 */ 3, /* b1 */ 4, /* b2 */ 4, /* b3 */ 5,
	/* b4 */ 4, /* b5 */ 5, /* b6 */ 5, /* b7 */ 6,
	/* b8 */ 4, /* b9 */ 5, /* ba */ 5, /* bb */ 6,
	/* bc */ 5, /* bd */ 6, /* be */ 6, /* bf */ 7,
	/* c0 */ 2, /* c1 */ 3, /* c2 */ 3, /* c3 */ 4,
	/* c4 */ 3, /* c5 */ 4, /* c6 */ 4, /* c7 */ 5,
	/* c8 */ 3, /* c9 */ 4, /* ca */ 4, /* cb */ 5,
	/* cc */ 4, /* cd */ 5, /* ce */ 5, /* cf */ 6,
	/* d0 */ 3, /* d1 */ 4, /* d2 */ 4, /* d3 */ 5,
	/* d4 */ 4, /* d5 */ 5, /* d6 */ 5, /* d7 */ 6,
	/* d8 */ 4, /* d9 */ 5, /* da */ 5, /* db */ 6,
	/* dc */ 5, /* dd */ 6, /* de */ 6, /* df */ 7,
	/* e0 */ 3, /* e1 */ 4, /* e2 */ 4, /* e3 */ 5,
	/* e4 */ 4, /* e5 */ 5, /* e6 */ 5, /* e7 */ 6,
	/* e8 */ 4, /* e9 */ 5, /* ea */ 5, /* eb */ 6,
	/* ec */ 5, /* ed */ 6, /* ee */ 6, /* ef */ 7,
	/* f0 */ 4, /* f1 */ 5, /* f2 */ 5, /* f3 */ 6,
	/* f4 */ 5, /* f5 */ 6, /* f6 */ 6, /* f7 */ 7,
	/* f8 */ 5, /* f9 */ 6, /* fa */ 6, /* fb */ 7,
	/* fc */ 6, /* fd */ 7, /* fe */ 7, /* ff */ 8
};

struct bench_unit {
    bench_unit() : valid(false), cycles(0), cycles_local(0), times(0), times_local(0){}

    bool valid;
    float cycles;
    float cycles_local;
    uint64_t times;
    uint64_t times_local;
};

uint64_t get_cpu_cycles() {
    uint64_t result;
#ifndef _MSC_VER
    __asm__ volatile(".byte 15;.byte 49;shlq $32,%%rdx;orq %%rdx,%%rax":"=a"
                     (result)::"%rdx");
#else
    result = __rdtsc();
#endif
    return result;
};

void generate_random_data(uint64_t* data, uint32_t range, uint32_t n) {
    // PRNG
    std::uniform_int_distribution<uint32_t> distr(0, range-1); // right inclusive
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

     // Generate some random data.
    uint32_t n_unique = 0;
    // while (n_unique < n) {
    for (int i = 0; i < n; ++i) {
        uint32_t val = distr(eng);
        n_unique += (data[val / 64] & (1ULL << (val % 64))) == 0;
        data[val / 64] |= 1ULL << (val % 64);
    }
}

#if !defined(__clang__) && !defined(_MSC_VER)
__attribute__((optimize("no-tree-vectorize")))
#endif
uint64_t popcount_scalar_naive_nosimd(const uint8_t* data, uint32_t len) {
    uint64_t total = 0;
    // for (int i = 0; i < len; ++i) {
    //     total += STORM_popcount64(data1[i] & data2[i]);
    // }

    // uint64_t diff;
    // uint8_t* b8 = (uint8_t*)&data;
    for (int i = 0, j = 0; i < len; ++i, j += 8) {
        // total += STORM_popcount64(data[i]);
        // diff = data1[i] & data2[i];
        total += popcnt_lookup8bit[data[j+0]];
        total += popcnt_lookup8bit[data[j+1]];
        total += popcnt_lookup8bit[data[j+2]];
        total += popcnt_lookup8bit[data[j+3]];
        total += popcnt_lookup8bit[data[j+4]];
        total += popcnt_lookup8bit[data[j+5]];
        total += popcnt_lookup8bit[data[j+6]];
        total += popcnt_lookup8bit[data[j+7]];
    }

    return total;
}

#if !defined(__clang__) && !defined(_MSC_VER)
__attribute__((optimize("no-tree-vectorize")))
#endif
uint64_t intersect_scalar_naive_nosimd(const uint64_t* STORM_RESTRICT data1,const uint64_t* STORM_RESTRICT data2, uint32_t len) {
    uint64_t total = 0;
    // for (int i = 0; i < len; ++i) {
    //     total += STORM_popcount64(data1[i] & data2[i]);
    // }

    uint64_t diff;
    uint8_t* b8 = (uint8_t*)&diff;
    for (int i = 0; i < len; ++i) {
        // total += STORM_popcount64(data1[i] & data2[i]);
        diff = data1[i] & data2[i];
        total += popcnt_lookup8bit[b8[0]];
        total += popcnt_lookup8bit[b8[1]];
        total += popcnt_lookup8bit[b8[2]];
        total += popcnt_lookup8bit[b8[3]];
        total += popcnt_lookup8bit[b8[4]];
        total += popcnt_lookup8bit[b8[5]];
        total += popcnt_lookup8bit[b8[6]];
        total += popcnt_lookup8bit[b8[7]];
    }

    return total;
}

#if !defined(__clang__) && !defined(_MSC_VER)
__attribute__((optimize("no-tree-vectorize")))
#endif
uint64_t union_scalar_naive_nosimd(const uint64_t* STORM_RESTRICT data1,const uint64_t* STORM_RESTRICT data2, uint32_t len) {
    uint64_t total = 0;
    // for (int i = 0; i < len; ++i) {
    //     total += STORM_popcount64(data1[i] | data2[i]);
    // }

    uint64_t diff;
    uint8_t* b8 = (uint8_t*)&diff;
    for (int i = 0; i < len; ++i) {
        // total += STORM_popcount64(data1[i] | data2[i]);
        diff = data1[i] | data2[i];
        total += popcnt_lookup8bit[b8[0]];
        total += popcnt_lookup8bit[b8[1]];
        total += popcnt_lookup8bit[b8[2]];
        total += popcnt_lookup8bit[b8[3]];
        total += popcnt_lookup8bit[b8[4]];
        total += popcnt_lookup8bit[b8[5]];
        total += popcnt_lookup8bit[b8[6]];
        total += popcnt_lookup8bit[b8[7]];
    }

    return total;
}

#if !defined(__clang__) && !defined(_MSC_VER)
__attribute__((optimize("no-tree-vectorize")))
#endif
uint64_t diff_scalar_naive_nosimd(const uint64_t* STORM_RESTRICT data1,const uint64_t* STORM_RESTRICT data2, uint32_t len) {
    uint64_t total = 0;
    // for (int i = 0; i < len; ++i) {
    //     total += STORM_popcount64(data1[i] ^ data2[i]);
    // }

    uint64_t diff;
    uint8_t* b8 = (uint8_t*)&diff;
    for (int i = 0; i < len; ++i) {
        // total += STORM_popcount64(data1[i] ^ data2[i]);
        diff = data1[i] ^ data2[i];
        total += popcnt_lookup8bit[b8[0]];
        total += popcnt_lookup8bit[b8[1]];
        total += popcnt_lookup8bit[b8[2]];
        total += popcnt_lookup8bit[b8[3]];
        total += popcnt_lookup8bit[b8[4]];
        total += popcnt_lookup8bit[b8[5]];
        total += popcnt_lookup8bit[b8[6]];
        total += popcnt_lookup8bit[b8[7]];
    }

    return total;
}

// Definition for microsecond timer.
typedef std::chrono::high_resolution_clock::time_point clockdef;

int set_algebra_wrapper(std::string name,
    STORM_compute_func f, 
    int iterations,
    uint64_t* STORM_RESTRICT data1, 
    uint64_t* STORM_RESTRICT data2, 
    uint32_t range,
    uint32_t n_values,
    uint32_t n_bitmaps, 
    bench_unit& unit) 
{
    uint32_t cycles_low = 0, cycles_high = 0;
    uint32_t cycles_low1 = 0, cycles_high1 = 0;
    // Start timer.

    std::vector<uint64_t> clocks;
    std::vector<uint32_t> times;

#ifndef _MSC_VER
// Intel guide:
// @see: https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
asm   volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx"); 
asm   volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx"); 
asm   volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx"); 
asm   volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
#endif
    generate_random_data(data1, range, n_values);
    generate_random_data(data2, range, n_values);

    volatile uint64_t total = 0; // voltatile to prevent compiler to remove work through optimization
    clockdef t1 = std::chrono::high_resolution_clock::now();

#ifdef __linux__ 
    // unsigned long flags;
    // preempt_disable(); /*we disable preemption on our CPU*/
    // raw_local_irq_save(flags); /*we disable hard interrupts on our CPU*/  
    /*at this stage we exclusively own the CPU*/ 
#endif

#ifndef _MSC_VER 
    asm   volatile ("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
#endif

    for (int i = 0; i < iterations; ++i) {
        // Call argument subroutine pointer.
        total += (*f)(data1, data2, n_bitmaps);
    }

#ifndef _MSC_VER 
    asm   volatile("RDTSCP\n\t"
                   "mov %%edx, %0\n\t"
                   "mov %%eax, %1\n\t"
                   "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
#endif
#ifdef __linux__ 
        // raw_local_irq_restore(flags);/*we enable hard interrupts on our CPU*/
        // preempt_enable();/*we enable preemption*/
#endif

    clockdef t2 = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

    uint64_t start = ( ((uint64_t)cycles_high  << 32) | cycles_low  );
    uint64_t end   = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );

    double mean_cycles = (end - start) / (double)iterations;
    uint32_t mean_time = time_span.count() / (double)iterations;

    std::cout << name << "\t" << n_bitmaps << "\t" << total << "\t" << 
        mean_cycles << "\t" <<
        mean_time << "\t" << 
        mean_cycles / n_bitmaps << "\t" << 
        ((n_bitmaps*2*sizeof(uint64_t)) / (1024*1024.0)) / (mean_time / 1000000000.0) << std::endl;
    
    unit.times += mean_time;
    unit.times_local = mean_time;
    unit.cycles += mean_cycles;
    unit.cycles_local = mean_cycles;
    unit.valid = 1;

    return 0;
}

int popcount_wrapper(std::string name,
    STORM_popcnt_func f, 
    int iterations,
    uint64_t* data,
    uint32_t range,
    uint32_t n_values,
    uint32_t n_bitmaps, 
    bench_unit& unit) 
{
    uint32_t cycles_low = 0, cycles_high = 0;
    uint32_t cycles_low1 = 0, cycles_high1 = 0;
    // Start timer.

    std::vector<uint64_t> clocks;
    std::vector<uint32_t> times;

#ifndef _MSC_VER
// Intel guide:
// @see: https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
asm   volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx"); 
asm   volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx"); 
asm   volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx"); 
asm   volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
#endif
    generate_random_data(data, range, n_values);

    volatile uint64_t total = 0; // voltatile to prevent compiler to remove work through optimization
    clockdef t1 = std::chrono::high_resolution_clock::now();

#ifdef __linux__ 
    // unsigned long flags;
    // preempt_disable(); /*we disable preemption on our CPU*/
    // raw_local_irq_save(flags); /*we disable hard interrupts on our CPU*/  
    /*at this stage we exclusively own the CPU*/ 
#endif

#ifndef _MSC_VER 
    asm   volatile ("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax", "%rbx", "%rcx", "%rdx");
#endif

    for (int i = 0; i < iterations; ++i) {
        // Call argument subroutine pointer.
        total += (*f)((uint8_t*)data, n_bitmaps);
    }

#ifndef _MSC_VER 
    asm   volatile("RDTSCP\n\t"
                   "mov %%edx, %0\n\t"
                   "mov %%eax, %1\n\t"
                   "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax", "%rbx", "%rcx", "%rdx");
#endif
#ifdef __linux__ 
        // raw_local_irq_restore(flags);/*we enable hard interrupts on our CPU*/
        // preempt_enable();/*we enable preemption*/
#endif

    clockdef t2 = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);

    uint64_t start = ( ((uint64_t)cycles_high  << 32) | cycles_low  );
    uint64_t end   = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );

    double mean_cycles = (end - start) / (double)iterations;
    uint32_t mean_time = time_span.count() / (double)iterations;

    std::cout << name << "\t" << n_bitmaps << "\t" << total << "\t" << 
        mean_cycles << "\t" <<
        mean_time << "\t" << 
        mean_cycles / n_bitmaps << "\t" << 
        ((n_bitmaps*2*sizeof(uint64_t)) / (1024*1024.0)) / (mean_time / 1000000000.0) << std::endl;
    
    unit.times += mean_time;
    unit.times_local = mean_time;
    unit.cycles += mean_cycles;
    unit.cycles_local = mean_cycles;
    unit.valid = 1;

    return 0;
}

int benchmark(uint32_t range, uint32_t n_values) {
    std::cout << "Range = [0," << range << ") with bits=" << n_values << std::endl;

    // Align some bitmaps.
    uint32_t n_bitmaps = ceil(range / 64.0);
    uint64_t* bitmaps  = (uint64_t*)STORM_aligned_malloc(STORM_get_alignment(), n_bitmaps*sizeof(uint64_t));
    uint64_t* bitmaps2 = (uint64_t*)STORM_aligned_malloc(STORM_get_alignment(), n_bitmaps*sizeof(uint64_t));
    
    generate_random_data(bitmaps, range, n_values);
    generate_random_data(bitmaps2, range, n_values);

    // Compute the population bit count.
    uint64_t bitcnt = STORM_popcnt((uint8_t*)bitmaps, n_bitmaps*8);
    std::cout << "POPCNT=" << bitcnt << std::endl;
    
    // Compute the positional bit count.
    uint32_t flag_count[16];
    int pospopcnt = STORM_pospopcnt_u16((uint16_t*)bitmaps, n_bitmaps*4, &flag_count[0]);
    uint32_t pospopcnt_total = flag_count[0];
    std::cout << "POSPOPCNT=" << flag_count[0];
    for (int i = 1; i < 16; ++i) {
        std::cout << "," << flag_count[i];
        pospopcnt_total += flag_count[i];
    }
    std::cout << std::endl;
    std::cout << "POSPOPCNT total=" << pospopcnt_total << std::endl;

    // Compute intersect count
    uint64_t intersect_count = STORM_intersect_count(bitmaps, bitmaps2, n_bitmaps);
    std::cout << "intersect count=" << intersect_count << std::endl;

    // Compute union count
    uint64_t union_count = STORM_union_count(bitmaps, bitmaps2, n_bitmaps);
    std::cout << "union count=" << union_count << std::endl;

    // Compute diff count
    uint64_t diff_count = STORM_diff_count(bitmaps, bitmaps2, n_bitmaps);
    std::cout << "diff count=" << diff_count << std::endl;

    {
        // Align some bitmaps.
        uint32_t n_bitmaps = ceil(range / 64.0);
        uint64_t* bitmaps  = (uint64_t*)STORM_aligned_malloc(STORM_get_alignment(), 1048576*sizeof(uint64_t));
        uint64_t* bitmaps2 = (uint64_t*)STORM_aligned_malloc(STORM_get_alignment(), 1048576*sizeof(uint64_t));

        std::vector<uint32_t> ranges = {4,8,32,128,256,512,1024,2048,4096,8192,65536,262144,1048576};
        std::vector<uint32_t> iterations = {1000000, 1000000, 1000000, 1000000, 1000000, 50000, 50000, 50000, 50000, 2000, 2000, 2000, 2000};
        assert(ranges.size() == iterations.size());

        for (int i = 0; i < ranges.size(); ++i) {
            bench_unit unit_intsec, unit_union, unit_diff;
            popcount_wrapper("popcount-naive",&popcount_scalar_naive_nosimd, iterations[i], bitmaps, range, n_values, ranges[i], unit_intsec);
            popcount_wrapper("popcount",&STORM_popcnt, iterations[i], bitmaps, range, n_values, ranges[i], unit_intsec);
            
            set_algebra_wrapper("intersect-naive",&intersect_scalar_naive_nosimd, iterations[i], bitmaps, bitmaps2, range, n_values, ranges[i], unit_intsec);
            set_algebra_wrapper("intersect",STORM_get_intersect_count_func(ranges[i]), iterations[i], bitmaps, bitmaps2, range, n_values, ranges[i], unit_intsec);
            set_algebra_wrapper("union-naive",&union_scalar_naive_nosimd, iterations[i], bitmaps, bitmaps2, range, n_values, ranges[i], unit_intsec);
            set_algebra_wrapper("union",STORM_get_union_count_func(ranges[i]), iterations[i], bitmaps, bitmaps2, range, n_values, ranges[i], unit_union);
            set_algebra_wrapper("diff-naive",&diff_scalar_naive_nosimd, iterations[i], bitmaps, bitmaps2, range, n_values, ranges[i], unit_intsec);
            set_algebra_wrapper("diff",STORM_get_diff_count_func(ranges[i]), iterations[i], bitmaps, bitmaps2, range, n_values, ranges[i], unit_diff);
        }

        // Clean up.
        STORM_aligned_free(bitmaps);
        STORM_aligned_free(bitmaps2);
    }

    // Clean up.
    STORM_aligned_free(bitmaps);
    STORM_aligned_free(bitmaps2);
    return 1;
}

int main(int argc, char **argv) { 
    benchmark(2048000, 50000);

    return EXIT_SUCCESS;
}