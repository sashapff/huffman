//
// Created by Alexandra Ivanova on 11/06/2019.
//

#include "../gtest/gtest.h"
#include "../huffman.h"
#include <fstream>
#include <cstdlib>
#include <ctime>

void encode(std::string input, std::string output) {
    std::ifstream in;
    in.open(input, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cout << "error in opening input file" << '\n';
        return;
    }
    std::ofstream out;
    out.open(output, std::ios::out);
    if (!out.is_open()) {
        std::cout << "error in opening output file" << '\n';
        return;
    }
    huffman tree;
    std::vector<char> data(BLOCK_SIZE);
    size_t read;
    while (!in.eof()) {
        in.read(data.data(), data.size());
        read = static_cast<size_t>(in.gcount());
        if (read == 0 && in.bad()) {
            std::cout << "error in reading\n";
            return;
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
            return;
        }
        auto block = tree.encode_block(data.begin(), data.begin() + read);
        auto length = tree.convert_uint16_t(block.first.size());
        auto extra_bits = tree.convert_uint16_t(block.second);
        out.write(length.data(), length.size());
        out.write(extra_bits.data(), extra_bits.size());
        out.write(block.first.data(), block.first.size());
    }
}

void decode(std::string input, std::string output) {
    std::ifstream in;
    in.open(input, std::ios::in);
    if (!in.is_open()) {
        std::cout << "error in opening input file" << '\n';
        return;
    }
    std::ofstream out;
    out.open(output, std::ios::out);
    if (!out.is_open()) {
        std::cout << "error in opening output file" << '\n';
        return;
    }
    size_t read;
    huffman tree;
    std::vector<char> data(2);
    in.read(data.data(), data.size());
    read = in.gcount();
    if (read != data.size()) {
        std::cout << "file is damaged\n";
        return;
    }
    auto alphabet_size = tree.decode_number(data.begin(), data.begin() + data.size());
    in.read(data.data(), data.size());
    read = static_cast<size_t>(in.gcount());
    if (read != data.size()) {
        std::cout << "file is damaged\n";
        return;
    }
    auto tree_size = tree.decode_number(data.begin(), data.begin() + data.size());
    data.resize(alphabet_size);
    in.read(data.data(), data.size());
    read = static_cast<size_t>(in.gcount());
    if (read != data.size()) {
        std::cout << "file is damaged\n";
        return;
    }
    tree.count_order_symbols(data.begin(), data.begin() + data.size());
    data.resize((tree_size + SIZE_OF_CHAR - 1) / SIZE_OF_CHAR);
    in.read(data.data(), data.size());
    read = static_cast<size_t>(in.gcount());
    if (read != data.size()) {
        std::cout << "file is damaged\n";
        return;
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
            return;
        }
        auto block = tree.decode_block(data, read, extra_bits);
        out.write(block.data(), block.size());
    }
}

bool is_equals(std::string first, std::string second) {
    std::ifstream in_first, in_second;
    in_first.open(first, std::ios::in);
    if (!in_first.is_open()) {
        std::cerr << "error in opening first file\n";
        return false;
    }
    in_second.open(second, std::ios::in);
    if (!in_first.is_open()) {
        std::cerr << "error in opening second file\n";
        return false;
    }
    std::vector<char> first_data(BLOCK_SIZE), second_data(BLOCK_SIZE);
    while (!in_first.eof() && !in_second.eof()) {
        in_first.read(first_data.data(), BLOCK_SIZE);
        in_second.read(second_data.data(), BLOCK_SIZE);
        auto read_first = in_first.gcount();
        auto read_second = in_second.gcount();
        if (read_first != read_second) {
            return false;
        }
        for (size_t i = 0; i < read_first; i++) {
            if (first_data[i] != second_data[i]) {
                return false;
            }
        }
    }
    return in_first.eof() && in_second.eof();
}

TEST(correctness, empty) {
    encode("test/empty", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/empty", "test/output"));
}

TEST(correctness, empty_with_enter) {
    encode("test/empty_with_enter", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/empty_with_enter", "test/output"));
}

TEST(correctness, palata6) {
    encode("test/palata6", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/palata6", "test/output"));
}

TEST(correctness, mars) {
    encode("test/mars", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/mars", "test/output"));
}

TEST(correctness, mars_many_times) {
    encode("test/mars", "test/buffer");
    decode("test/buffer", "test/output");
    for (size_t i = 0; i < 10; i++) {
        encode("test/output", "test/buffer");
        decode("test/buffer", "test/output");
    }
    ASSERT_TRUE(is_equals("test/mars", "test/output"));
}

TEST(correctness, homework) {
    encode("test/homework", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/homework", "test/output"));
}

void generate(std::string file) {
    srand(time(NULL));
    std::ofstream out;
    out.open(file, std::ios::out);
    std::string s = "";
    for (size_t i = 0; i < 1e5; i++) {
        char c = (char) ((rand() % SHIFT * 2) - SHIFT);
        s.append(1, c);
    }
    out.write(s.data(), s.size());
}

TEST(correctness, random_) {
    generate("test/random");
    encode("test/random", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/random", "test/output"));
}

//void generate_all() {
//    std::ofstream out;
//    out.open("test/all", std::ios::out);
//    std::string s = "";
//    for (char i = -128; i < 127; i++) {
//        s.append(1, i);
//    }
//    s.append(1, 127);
//    out.write(s.data(), s.size());
//}

TEST(correctness, all) {
//    generate_all();
    encode("test/all", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/all", "test/output"));
}

TEST(correctness, photo) {
    encode("test/photo.png", "test/buffer");
    decode("test/buffer", "test/output");
    ASSERT_TRUE(is_equals("test/photo.png", "test/output"));
}
