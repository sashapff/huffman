//
// Created by Alexandra Ivanova on 08/06/2019.
//

#include <iostream>
#include <fstream>
#include "huffman.h"
#include <vector>
#include <cstring>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "please enter key -e/-d for encoding/decoding" << '\n';
        return 0;
    }
    if (argc == 2) {
        std::cout << "please enter key and than input and output files" << '\n';
        return 0;
    }
    if (strcmp(argv[1], "-e") == 0) {
        std::ifstream in;
        in.open(argv[2], std::ios::in | std::ios::binary);
        if (!in.is_open()) {
            std::cout << "error in opening input file" << '\n';
            return -1;
        }
        std::ofstream out;
        out.open(argv[3], std::ios::out);
        if (!out.is_open()) {
            std::cout << "error in opening output file" << '\n';
            return -1;
        }
        huffman tree;
        std::vector<char> data(BLOCK_SIZE);
        size_t read;
        while (!in.eof()) {
            in.read(data.data(), data.size());
            read = static_cast<size_t>(in.gcount());
            if (read == 0 && in.bad()) {
                std::cout << "error in reading\n";
                return -1;
            }
            tree.update_frequency(data.begin(), data.begin() + read);
        }
        in.clear();
        in.seekg(0);
        tree.encode_build();
        std::string headline = tree.encode_headline();
        out.write(headline.data(), headline.size());
        while (!in.eof()) {
            in.read(data.data(), data.size());
            read = static_cast<size_t>(in.gcount());
            if (read == 0 && in.bad()) {
                std::cout << "error in reading\n";
                return -1;
            }
            auto block = tree.encode_block(data.begin(), data.begin() + read);
            auto length = tree.convert_uint16_t(block.first.size());
            auto extra_bits = tree.convert_uint16_t(block.second);
            out.write(length.data(), length.size());
            out.write(extra_bits.data(), extra_bits.size());
            out.write(block.first.data(), block.first.size());
        }
    } else if (strcmp(argv[1], "-d") == 0) {
        std::ifstream in;
        in.open(argv[2], std::ios::in);
        if (!in.is_open()) {
            std::cout << "error in opening input file" << '\n';
            return -1;
        }
        std::ofstream out;
        out.open(argv[3], std::ios::out);
        if (!out.is_open()) {
            std::cout << "error in opening output file" << '\n';
            return -1;
        }
        size_t read;
        huffman tree;
        std::vector<char> data(2);
        in.read(data.data(), data.size());
        read = in.gcount();
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return -1;
        }
        auto alphabet_size = tree.decode_number(data.begin(), data.begin() + data.size());
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return -1;
        }
        auto tree_size = tree.decode_number(data.begin(), data.begin() + data.size());
        data.resize(alphabet_size);
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return -1;
        }
        tree.count_order_symbols(data.begin(), data.begin() + data.size());
        data.resize((tree_size + SIZE_OF_CHAR - 1) / SIZE_OF_CHAR);
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return -1;
        }
        tree.decode_build(data.data(), data.data() + data.size(), tree_size);
        data.resize(BLOCK_SIZE);
        std::vector<char> number(2);
        while (!in.eof()) {
            in.read(number.data(), number.size());
            size_t length = tree.decode_number(number.begin(), number.begin() + number.size());
            in.read(number.data(), number.size());
            size_t extra_bits = tree.decode_number(number.begin(), number.begin() + number.size());
            data.resize(length);
            in.read(data.data(), data.size());
            read = static_cast<size_t>(in.gcount());
            if (read == 0 && in.bad()) {
                std::cout << "error in reading\n";
                return -1;
            }
            auto block = tree.decode_block(data, read, extra_bits);
            out.write(block.data(), block.size());
        }
    } else {
        std::cout << "please enter key -e/-d for encoding/decoding" << '\n';
        return 0;
    }
}