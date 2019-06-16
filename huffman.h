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
const size_t SIZE_OF_CHAR = 8;
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
    std::vector<uint8_t> alphabet_code;
    std::array<std::vector<bool>, ALPHABET_SIZE> symbol_code{};
    std::vector<bool> remaining_bits;
    std::vector<char> now_bits;
    int carry;
    size_t cnt_symbol = 0;
    size_t carry_cnt = 0;

    void build_code(node *n, std::vector<bool> &current);

    void encode_symbol(char c);

    std::string encode_alphabet() const;

    std::string encode_tree();


public:

    std::string convert_uint8_t(size_t size) const;

    std::string convert_uint16_t(size_t size) const;

    huffman() = default;

    ~huffman() {
        delete root;
    }

    template<typename iterator>
    void update_frequency(iterator begin, iterator end) {
        while (begin != end) {
            frequency[static_cast<int>(*begin) + SHIFT].cnt++;
            begin++;
        }
    }

    template<typename iterator>
    void count_order_symbols(iterator begin, iterator end) {
        while (begin != end) {
            alphabet_code.push_back(*begin);
            begin++;
        }
    }

    template<typename iterator>
    size_t decode_number(iterator begin, iterator end) {
        if (end - begin == 2) {
            return (static_cast<int>(*begin) + SHIFT) * SIZE_OF_CHAR + (static_cast<int>(*(begin + 1)) + SHIFT);
        }
        return static_cast<int>(*begin) + SHIFT;
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
    void decode_build(iterator begin, iterator end, size_t len) {
        int l = len % SIZE_OF_CHAR;
        if (l == 0) {
            l = SIZE_OF_CHAR;
        }
        node *current = new node();
        while (begin != end) {
            int t = static_cast<int>(*begin) + SHIFT;
            for (size_t i = 0; i < SIZE_OF_CHAR && !((begin + 1 == end) && i >= l); i++) {
                if (t % 2 == 0) {
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
                t /= 2;
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
    std::pair<std::vector<char>, size_t> encode_block(iterator begin, iterator end) {
        now_bits.clear();
        carry = 0;
        carry_cnt = 0;
        while (begin != end) {
            encode_symbol(*begin);
            begin++;
        }
        size_t last_len = carry_cnt;
        if (carry_cnt != 0) {
            while (carry_cnt != 8) {
                carry <<= 1;
                carry_cnt++;
            }
            now_bits.push_back(carry - SHIFT);
        }
        if (last_len == 0) {
            last_len = SIZE_OF_CHAR;
        }
        return std::make_pair(now_bits, last_len);
    }

    std::string decode_block(std::vector<char> &data, size_t size, size_t last_len) {
        std::string result;
        node *current = root;
        size_t cnt = 0;
        for (auto i : remaining_bits) {
            if (!i) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        remaining_bits.clear();
        for (size_t i = 0; i < size; i++) {
            cnt++;
            for (size_t j = 0; j < SIZE_OF_CHAR && !(i == size - 1 && last_len <= j); j++) {
                int t = static_cast<int>(data[i]) + SHIFT;
                if ((t >> (SIZE_OF_CHAR - j - 1)) % 2 == 0) {
                    if (current->left == nullptr) {
                        return 0;
                    }
                    current = current->left;
                    remaining_bits.push_back(false);
                } else {
                    if (current->right == nullptr) {
                        return 0;
                    }
                    current = current->right;
                    remaining_bits.push_back(true);
                }
                if (current->is_leaf) {
                    result += (current->c);
                    current = root;
                    remaining_bits.clear();
                }
            }
        }
        return result;
    }

};


#endif //HUFFMAN_HUFFMAN_H
