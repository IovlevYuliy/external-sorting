#include <cstdio>
#include <thread>
#include <fstream>
#include <sstream>
#include <ios>
#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <cassert>

#include "sorter.hpp"

using std::ifstream;
using std::ofstream;
using std::ios;
using std::thread;
using std::vector;
using std::sort;
using std::cerr;
using std::endl;
using std::priority_queue;
using std::pair;
using std::greater;

typedef pair<uint32_t, size_t> QueueItem;

Sorter::Sorter(const string &input_file_name, const string &ouput_file_name, size_t free_mem)
    : input_name_(input_file_name), output_name_(ouput_file_name), free_mem_(free_mem) {
        // align available memory bytes to sizeof uint32_t
        free_mem_ -= free_mem_ % sizeof(uint32_t);
    };

void Sorter::SortParallel() {
    ifstream ifs;
    ifs.open(input_name_, ios::in | ios::binary);

    ifs.seekg(0, ifs.end);

    uint64_t file_size = ifs.tellg();
    chunk_cnt_ = (file_size + free_mem_ - 1) / free_mem_;

    vector<thread> threads(chunk_cnt_);
    for (size_t i = 0; i < chunk_cnt_; ++i) {
        threads[i] = thread(&Sorter::ReadAndSortChunk, this, i, file_size);
    }

    for (size_t i = 0; i < chunk_cnt_; ++i){
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }

    MergeChunks();
    RemoveChunkFiles();
}

void Sorter::SortSync() {
    ifstream ifs;
    ifs.open(input_name_, ios::in | ios::binary);

    ifs.seekg(0, ifs.end);

    uint64_t file_size = ifs.tellg();
    chunk_cnt_ = (file_size + free_mem_ - 1) / free_mem_;

    for (size_t i = 0; i < chunk_cnt_; ++i) {
        ReadAndSortChunk(i, file_size);
    }

    MergeChunks();
    RemoveChunkFiles();
}

void Sorter::ReadAndSortChunk(const size_t chunk_id, const u_int64_t file_size) {
    ifstream ifs(input_name_, ios::in | ios::binary);
    ifs.seekg(chunk_id * free_mem_, ios::beg);

    size_t buffer_size = std::min(file_size - ifs.tellg(), free_mem_);

    vector<uint32_t> buffer(buffer_size / sizeof(uint32_t));
    ifs.read(reinterpret_cast<char *>(&buffer[0]), buffer_size);

    sort(buffer.begin(), buffer.end());

    string out_file_name = std::to_string(chunk_id) + "_sorted";
    WriteToFile(out_file_name, buffer);
}

void Sorter::MergeChunks() {
    priority_queue<QueueItem, vector<QueueItem>, greater<QueueItem>> min_q;
    vector<ifstream> ifs_chunk(chunk_cnt_);

    uint32_t val;
    for (size_t chunk_id = 0; chunk_id < chunk_cnt_; ++chunk_id) {
        ifs_chunk[chunk_id].open(std::to_string(chunk_id) + "_sorted", ios::in | ios::binary);
        ifs_chunk[chunk_id].read(reinterpret_cast<char *>(&val), sizeof(val));
        min_q.push({ val, chunk_id });
    }

    ofstream ofs(output_name_, ios::out | ios::binary);
    while(!min_q.empty()) {
        uint32_t min_val = min_q.top().first;
        size_t chunk_id = min_q.top().second;
        min_q.pop();

        // write minimum value to output file
        ofs.write(reinterpret_cast<char *>(&min_val), sizeof(min_val));
        // read the next one number from chunk file
        ifs_chunk[chunk_id].read(reinterpret_cast<char *>(&val), sizeof(val));

        if (ifs_chunk[chunk_id].eof()) {
            ifs_chunk[chunk_id].close();
        } else {
            min_q.push({ val, chunk_id });
        }
    }
}

void Sorter::WriteToFile(const string &output_file_name, vector<uint32_t> &buffer) {
    ofstream ofs(output_file_name, ios::out | ios::binary);
    ofs.write(reinterpret_cast<char *>(&buffer[0]), sizeof(uint32_t) * buffer.size());
}

void Sorter::RemoveChunkFiles() {
    for (size_t chunk_id = 0; chunk_id < chunk_cnt_; ++chunk_id) {
        string chunk_filename = std::to_string(chunk_id) + "_sorted";
        remove(chunk_filename.c_str());
    }
}

void Sorter::EnsureResult() {
    uint64_t nums_count = 0;

    ifstream ifs(output_name_, ios::in | ios::binary);

    int64_t prev_val = -1;
    uint32_t cur_val;

    while (!ifs.eof()) {
        if (!ifs.read(reinterpret_cast<char *>(&cur_val), sizeof(cur_val))) {
            break;
        }

        assert(prev_val <= cur_val);

        prev_val = cur_val;
        nums_count++;
    }

    std::clog << "Processed " << nums_count << " numbers" << std::endl;
}
