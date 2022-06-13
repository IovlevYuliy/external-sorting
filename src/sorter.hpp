#include <string>
#include <vector>

using std::string;
using std::vector;

class Sorter {
    public:
        Sorter(const string &input_file_name, const string &ouput_file_name, size_t free_mem_);
        void SortParallel();
        void SortSync();
        void EnsureResult();

    private:
        void ReadAndSortChunk(const size_t chunk_id, const u_int64_t file_size);
        void WriteToFile(const string& output_file_name, vector<uint32_t>& buffer);
        void MergeChunks();
        void RemoveChunkFiles();

        size_t free_mem_;
        size_t chunk_cnt_;

        string input_name_;
        string output_name_;
};
