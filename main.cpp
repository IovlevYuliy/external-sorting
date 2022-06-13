#include <iostream>
#include <chrono>

#include "src/sorter.hpp"

int main(int argc, char *argv[]) {
    auto t_start = std::chrono::system_clock::now();

    string unsorted_filename = "input";
    string output_filename = "output";
    size_t free_mem_bytes = 128 * 1024 * 1024; // 128 MB

    if (argc > 1) {
        unsorted_filename = string(argv[1]);
    }
    if (argc > 2) {
        output_filename = string(argv[2]);
    }
    if (argc > 3) {
        free_mem_bytes = atoi(argv[3]);
    }

    if (free_mem_bytes < sizeof(uint32_t)) {
        std::cerr << "Available memory should be more than size of uint_32t (4 bytes)\n";
        return 1;
    }

    Sorter sorter(unsorted_filename, output_filename, free_mem_bytes);
    sorter.SortParallel();
    // sorter.SortSync();
    // sorter.EnsureResult();
    auto t_end = std::chrono::system_clock::now();

    std::clog << "Done." << std::endl;
    std::clog << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count() << " ms" << std::endl;

    return 0;
}