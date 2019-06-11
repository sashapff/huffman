//
// Created by Alexandra Ivanova on 06/06/2019.
//

#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <array>
#include <queue>
#include <string>

const size_t BLOCK_SIZE = 1024;
const size_t ALPHABET_SIZE = 256;
const size_t SIZE_OF_NUMBER_8 = 8;
const size_t shift = 128;

class huffman {
    struct symbol {
        char c;
        size_t cnt;
    };

    struct node {
        node *left = nullptr, *right = nullptr, *parent = nullptr;
        char c = '\0';
        size_t cnt = 0;
        bool is_leaf = true;

        node() = default;

        explicit node(node *n) {
            left = nullptr;
            right = nullptr;
            parent = n;
            n->is_leaf = false;
        }

        explicit node(symbol a) {
            c = a.c;
            cnt = a.cnt;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            is_leaf = true;
        }

        node(node *first, node *second) {
            cnt = first->cnt + second->cnt;
            left = first;
            right = second;
            left->parent = this;
            right->parent = this;
            is_leaf = false;
        }

        ~node() {
            if (this == nullptr) {
                return;
            }
            if (left != nullptr) {
                left->~node();
            }
            if (right != nullptr) {
                right->~node();
            }
            operator delete(this);
        }
    };

    node *root = nullptr;

    std::array<symbol, ALPHABET_SIZE> frequency{};
    std::vector<bool> tree_code;
    std::vector<uint8_t> alphabet_code;
    std::array<std::vector<bool>, ALPHABET_SIZE> symbol_code{};
    std::vector<bool> remaining_bits;
    size_t cnt_symbol = 0;

    void build_code(node *n, std::vector<bool> &current) {
        if (n == nullptr) {
            return;
        }
        if (n->is_leaf) {
            alphabet_code.push_back(n->c);
            symbol_code[n->c + shift] = current;
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

    std::string encode_symbol(char c) const {
        std::string result;
        for (auto i : symbol_code[c + shift]) {
            result.append(i ? "1" : "0");
        }
        return result;
    }

    std::string encode_alphabet() const {
        std::string result;
        for (auto i : alphabet_code) {
            result.append(convert_uint8_t(i));
        }
        return result;
    }

    std::string encode_tree() const {
        std::string result;
        for (auto i : tree_code) {
            result.append(i ? "1" : "0");
        }
        return result;
    }

    std::string convert_uint8_t(size_t size) const {
        std::string result;
        auto begin_size = size;
        for (size_t i = 0; i < SIZE_OF_NUMBER_8; i++) {
            result.append(std::to_string(size % 2));
            size >>= 1;
        }
        return result;
    }

public:
    huffman() = default;

    ~huffman() {
        root->~node();
    }

    template<typename iterator>
    void update_frequency(iterator begin, iterator end) {
        while (begin != end) {
            frequency[*begin + shift].cnt++;
            begin++;
        }
    }

    void count_order_symbols(std::vector<char> &data) {
        int cnt = 0;
        for (size_t i = 0; i < data.size(); i += SIZE_OF_NUMBER_8) {
            size_t number = 0;
            for (size_t j = SIZE_OF_NUMBER_8; j > 0; j--) {
                number *= 2;
                number += (data[i + j - 1] - '0');
            }
            alphabet_code.push_back(number);
        }
    }

    template<typename iterator>
    size_t decode_number(iterator begin, iterator end) {
        size_t number = 0;
        while (begin != end) {
            number <<= 1;
            number += *(end - 1) - '0';
            end--;
        }
        return number;
    }

    void encode_build() {
        for (size_t i = 0; i < ALPHABET_SIZE; i++) {
            frequency[i].c = static_cast<char>(i - shift);
        }
        auto node_comp = [](node const *first, node const *second) { return first->cnt > second->cnt; };
        std::priority_queue<node *, std::vector<node *>, decltype(node_comp)> q(node_comp);
        for (size_t i = 0; i < ALPHABET_SIZE; i++) {
            if (frequency[i].cnt != 0) {
                q.push(new node(frequency[i]));
            }
        }
        while (q.size() > 1) {
            auto first = q.top();
            q.pop();
            auto second = q.top();
            q.pop();
            q.push(new node(first, second));
        }
        if (q.size() != 0) {
            root = q.top();
            q.pop();
        }
        std::vector<bool> current_code;
        build_code(root, current_code);
    }

    void decode_build(std::vector<char> &data) {
        node *current = new node();
        for (auto i : data) {
            if (i == '0') {
                current->left = new node(current);
                current = current->left;
            } else {
                current->c = alphabet_code[cnt_symbol++];
                current = current->parent;
                while (current->right != nullptr) {
                    current = current->parent;
                }
                current->right = new node(current);
                current = current->right;
            }
        }
        if (current != nullptr) {
            if (cnt_symbol < alphabet_code.size()) {
                current->c = alphabet_code[cnt_symbol++];
            }
            while (current->parent != nullptr) {
                current = current->parent;
            }
        }
        root = current;
    }

    std::string encode_headline() {
        return convert_uint8_t(alphabet_code.size()) + convert_uint8_t(tree_code.size())
               + encode_alphabet() + encode_tree();
    }

    template<typename iterator>
    std::string encode_block(iterator begin, iterator end) {
        std::string result;
        while (begin != end) {
            result.append(encode_symbol(*begin));
            begin++;
        }
        return result;
    }

    std::string decode_block(std::vector<char> data, size_t size) {
        std::string result;
        node *current = root;
        size_t read = 0;
        size_t cnt = 0;
        for (auto i : remaining_bits) {
            if (!i) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        for (size_t i = 0; i < size; i++) {
            cnt++;
            if (data[i] == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            if (current->is_leaf) {
                result += (current->c);
                current = root;
                read = cnt;
            }
        }
        remaining_bits.resize(data.size() - read);
        for (size_t i = read; i < size; i++) {
            remaining_bits[i - read] = data[i] != '0';
        }
        return result;
    }

};


#endif //HUFFMAN_HUFFMAN_H
