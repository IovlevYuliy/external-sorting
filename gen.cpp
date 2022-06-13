#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <ios>

using std::ios;
using std::ofstream;
using std::string;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./gen {output_file_name} {number_count} \n";
        return 0;
    }

    string output_name(argv[1]);
    size_t nums_count = atoi(argv[2]);

    ofstream ofs(output_name, ios::out | ios::binary);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint32_t> dist;

    for (size_t i = 0; i < nums_count; ++i) {
        uint32_t rand_val = dist(rng);
        ofs.write(reinterpret_cast<char *>(&rand_val), sizeof(rand_val));
    }
}
