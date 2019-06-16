//
// Created by Alexandra Ivanova on 12/06/2019.
//

#include "huffman.h"

void huffman::build_code(huffman::node *n, std::vector<bool> &current) {
    if (n == nullptr) {
        return;
    }
    if (n->is_leaf) {
        if (n == root) {
            tree_code.push_back(false);
            root->left = new node(root);
            root->left->c = root->c;
            root->left->cnt = root->cnt;
            root->is_leaf = false;
            current.push_back(false);
            alphabet_code.push_back(n->c);
            symbol_code[n->c + SHIFT] = current;
        } else {
            alphabet_code.push_back(n->c);
            symbol_code[n->c + SHIFT] = current;
        }
        return;
    }
    tree_code.push_back(false);
    current.push_back(false);
    build_code(n->left, current);
    current.pop_back();
    tree_code.push_back(true);
    current.push_back(true);
    build_code(n->right, current);
    current.pop_back();
}

void huffman::encode_symbol(char c) {
    for (auto i : symbol_code[c + SHIFT]) {
        if (carry_cnt == SIZE_OF_CHAR) {
            carry_cnt = 0;
            now_bits.push_back(carry - SHIFT);
            carry = 0;
        }
        carry <<= 1;
        carry += i ? 1 : 0;
        carry_cnt++;
    }
}

std::string huffman::encode_alphabet() const {
    std::string result;
    for (auto i : alphabet_code) {
        result.append(convert_uint8_t(i));
    }
    return result;
}

std::string huffman::encode_tree() {
    while (tree_code.size() % SIZE_OF_CHAR != 0) {
        tree_code.push_back(false);
    }
    std::string result;
    for (size_t i = 0; i < tree_code.size(); i += SIZE_OF_CHAR) {
        int c = 0;
        for (size_t j = SIZE_OF_CHAR; j > 0; j--) {
            c <<= 1;
            c += tree_code[i + j - 1] ? 1: 0;
        }
        result.append(1, static_cast<char>(c - SHIFT));
    }
    return result;
}

std::string huffman::convert_uint8_t(size_t size) const {
    char c = static_cast<char>(static_cast<int>(size));
    return std::string(1, c);
}

std::string huffman::convert_uint16_t(size_t size) const {
    std::string result;
    char first = static_cast<char>(static_cast<int>(size / SIZE_OF_CHAR) - SHIFT);
    char second = static_cast<char>(static_cast<int>(size % SIZE_OF_CHAR) - SHIFT);
    return std::string(1, first) + std::string(1, second);
}