# Multithreaded external sort
Own implementation of external sorting algorithm.

## Current Approach
1. Split initial large file into chunks with maximum available size.
2. Read and sort all chunks one by one. Save each chunk in individual temporary file.
3. Perform k-way merge algorithm. Use Priority Queue for storing minimum value of all chunks.

# How to build and run
```
git clone git@github.com:IovlevYuliy/external-sorting.git
cd external-sorting
mkdir build && cd build
cmake .. && make
```
To run program use the following command:

```
./sorter {path_to_input_file} {path_to_output_file} {available_memory_bytes}

{path_to_input_file} — path to binary file with unsorted uint32_t numbers. Default value "input".
{path_to_output_file} — path to output file. Default value "output".
{available_memory_bytes} — available RAM in bytes. Default value 128MB.
```
