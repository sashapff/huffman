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
const size_t SIZE_OF_NUMBER = 16;
const size_t SHIFT = 128;

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
            delete left;
            delete right;
        }
    };

    node *root = nullptr;
    std::array<symbol, ALPHABET_SIZE> frequency{};
    std::vector<bool> tree_code;
    std::vector<uint16_t> alphabet_code;
    std::array<std::vector<bool>, ALPHABET_SIZE> symbol_code{};
    std::vector<bool> remaining_bits;
    size_t cnt_symbol = 0;

    void build_code(node *n, std::vector<bool> &current);

    std::string encode_symbol(char c) const;

    std::string encode_alphabet() const;

    std::string encode_tree() const;

    std::string convert_uint16_t(size_t size) const;

public:
    huffman() = default;

    ~huffman() {
        delete root;
    }

    template<typename iterator>
    void update_frequency(iterator begin, iterator end) {
        while (begin != end) {
            frequency[*begin + SHIFT].cnt++;
            begin++;
        }
    }

    template<typename iterator>
    void count_order_symbols(iterator begin, iterator end) {
        while (begin != end) {
            size_t number = 0;
            for (size_t j = SIZE_OF_NUMBER; j > 0; j--) {
                number *= 2;
                number += (*(begin + j - 1) - '0');
            }
            alphabet_code.push_back(number);
            begin += SIZE_OF_NUMBER;
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
            frequency[i].c = static_cast<char>(static_cast<int>(i) - static_cast<int>(SHIFT));
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

    template<typename iterator>
    void decode_build(iterator begin, iterator end) {
        node *current = new node();
        while (begin != end) {
            if (*begin == '0') {
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
            begin++;
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
        std::string s = "";
        s.append(convert_uint16_t(alphabet_code.size()));
        s.append(convert_uint16_t(tree_code.size()));
        s.append(encode_alphabet());
        s.append(encode_tree());
        return s;
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
