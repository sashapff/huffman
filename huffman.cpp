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

std::string huffman::encode_symbol(char c) const {
    std::string result;
    for (auto i : symbol_code[c + SHIFT]) {
        result.append(i ? "1" : "0");
    }
    return result;
}

std::string huffman::encode_alphabet() const {
    std::string result;
    for (auto i : alphabet_code) {
        result.append(convert_uint16_t(i));
    }
    return result;
}

std::string huffman::encode_tree() const {
    std::string result;
    for (auto i : tree_code) {
        result.append(i ? "1" : "0");
    }
    return result;
}

std::string huffman::convert_uint16_t(size_t size) const {
    std::string result;
    for (size_t i = 0; i < SIZE_OF_NUMBER; i++) {
        result.append(std::to_string(size % 2));
        size >>= 1;
    }
    return result;
}
