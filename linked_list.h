#pragma once

#include <cstdint>

#define DEFAULT_LIST_CAPACITY 128

struct Node {
    uint32_t m_prev{0};
    uint32_t m_next{0};
    uint32_t m_data{0};
};

class List {
public:
    explicit List(uint32_t initial_node_capacity = DEFAULT_LIST_CAPACITY);
    ~List();


    void insert_head(uint32_t data);

    void insert_tail(uint32_t data);

    void insert_after(uint32_t index, uint32_t data);

    void insert_before(uint32_t index, uint32_t data);

    void pop_head();

    void pop_tail();

    void pop_next(uint32_t index);

    void pop_prev(uint32_t index);

    void pop_node(uint32_t index);


    [[nodiscard]] bool is_empty() const;

    [[nodiscard]] uint32_t head() const;

    [[nodiscard]] uint32_t tail() const;

    [[nodiscard]] uint32_t count() const;

    [[nodiscard]] uint32_t capacity() const;


    uint32_t next(uint32_t index);

    uint32_t prev(uint32_t index);

    uint32_t data(uint32_t index);

    void print_list() const;

private:
    uint32_t m_head{0};
    uint32_t m_tail{0};

    uint32_t m_count{0};
    uint32_t m_capacity{0};

    uint32_t get_free_node();

    uint32_t m_free_nodes{0};

    Node *m_nodes;
};

#undef DEFAULT_LIST_CAPACITY
