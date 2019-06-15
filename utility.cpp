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
        in.open(argv[2], std::ios::in);
        if (!in.is_open()) {
            std::cout << "error in opening input file" << '\n';
            return 0;
        }
        std::ofstream out;
        out.open(argv[3], std::ios::out);
        if (!out.is_open()) {
            std::cout << "error in opening output file" << '\n';
            return 0;
        }
        huffman tree;
        std::vector<char> data(BLOCK_SIZE);
        size_t read;
        while (!in.eof()) {
            in.read(data.data(), data.size());
            read = static_cast<size_t>(in.gcount());
            if (read == 0) {
                std::cout << "error in reading\n";
                return 0;
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
            if (read == 0) {
                std::cout << "error in reading\n";
                return 0;
            }
            auto block = tree.encode_block(data.begin(), data.begin() + read);
            out.write(block.data(), block.size());
        }
    } else if (strcmp(argv[1], "-d") == 0) {
        std::ifstream in;
        in.open(argv[2], std::ios::in);
        if (!in.is_open()) {
            std::cout << "error in opening input file" << '\n';
            return 0;
        }
        std::ofstream out;
        out.open(argv[3], std::ios::out);
        if (!out.is_open()) {
            std::cout << "error in opening output file" << '\n';
            return 0;
        }
        std::vector<char> data(SIZE_OF_NUMBER);
        size_t read;
        huffman tree;
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return 0;
        }
        auto alphabet_size = tree.decode_number(data.begin(), data.begin() + data.size()) * SIZE_OF_NUMBER;
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return 0;
        }
        auto tree_size = tree.decode_number(data.begin(), data.begin() + data.size());
        data.resize(alphabet_size);
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return 0;
        }
        tree.count_order_symbols(data.begin(), data.begin() + data.size());
        data.resize(tree_size);
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read != data.size()) {
            std::cout << "file is damaged\n";
            return 0;
        }
        tree.decode_build(data.data(), data.data() + data.size());
        data.resize(BLOCK_SIZE);
        while (!in.eof()) {
            in.read(data.data(), data.size());
            read = static_cast<size_t>(in.gcount());
            if (read == 0) {
                std::cout << "error in reading\n";
                return 0;
            }
            auto block = tree.decode_block(data, read);
            out.write(block.data(), block.size());
        }
    } else {
        std::cout << "please enter key -e/-d for encoding/decoding" << '\n';
        return 0;
    }
}