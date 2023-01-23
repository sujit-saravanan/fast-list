#include "linked_list.h"
#include <iostream>

List::List(uint32_t initial_node_capacity) {
    m_nodes    = (Node *) malloc(initial_node_capacity * sizeof(Node));
    m_capacity = initial_node_capacity;

    for (uint32_t i = 1; i < m_capacity - 1; i++)
        m_nodes[i] = {i - 1, i + 1, i};

    m_nodes[0]              = {0, 1, 0};
    m_nodes[m_capacity - 1] = {m_capacity - 2, m_capacity - 1, m_capacity - 1};
}


uint32_t List::get_free_node() {
    uint32_t free_node = m_free_nodes;
    m_free_nodes = next(m_free_nodes);
    return free_node;
}

void List::insert_head(uint32_t data) {
    uint32_t position = get_free_node();
    m_nodes[position] = {position, m_head, data};
    m_nodes[m_head].m_prev = position;
    m_head = position;
    m_count++;
}

void List::insert_tail(uint32_t data) {
    uint32_t position = get_free_node();
    m_nodes[position] = {m_tail, position, data};
    m_nodes[m_tail].m_next = position;
    m_tail = position;
    m_count++;
}

void List::insert_after(uint32_t index, uint32_t data) {
    uint32_t position = get_free_node();
    m_nodes[position] = {index, m_nodes[index].m_next, data};
    m_nodes[prev(position)].m_next = position;
    m_nodes[next(position)].m_prev = position;
    m_count++;
}

void List::insert_before(uint32_t index, uint32_t data) {
    uint32_t position = get_free_node();
    m_nodes[position] = {m_nodes[index].m_prev, index, data};
    m_nodes[prev(position)].m_next = position;
    m_nodes[next(position)].m_prev = position;
    m_count++;
}


void List::pop_head() {
    pop_node(m_head);
}

void List::pop_tail() {
    pop_node(m_tail);
}

void List::pop_next(uint32_t index) {
    pop_node(m_nodes[index].m_next);
}

void List::pop_prev(uint32_t index) {
    pop_node(m_nodes[index].m_prev);
}

void List::pop_node(uint32_t index) {
    if (index == m_head) {
        m_head = m_nodes[m_head].m_next;
        m_nodes[m_head].m_prev = m_head;
    } else if (index == m_tail) {
        m_tail = m_nodes[m_tail].m_prev;
        m_nodes[m_tail].m_next = m_tail;
    } else {
        m_nodes[prev(index)].m_next = m_nodes[index].m_next;
        m_nodes[next(index)].m_prev = m_nodes[index].m_prev;
    }

    m_nodes[index] = {0, m_free_nodes, 0};
    m_free_nodes = index;
    m_count--;
}


bool List::is_empty() const { return m_count == 0; }

uint32_t List::head() const { return m_head; }

uint32_t List::tail() const { return m_tail; }

uint32_t List::count() const { return m_count; }

uint32_t List::capacity() const { return m_capacity; }


uint32_t List::next(uint32_t index) { return m_nodes[index].m_next; }

uint32_t List::prev(uint32_t index) { return m_nodes[index].m_prev; }

uint32_t List::data(uint32_t index) { return m_nodes[index].m_data; }

void List::print_list() const {
    uint32_t position = m_head;

    while (position != m_tail){
        std::cout << "(" << position << "){" << m_nodes[position].m_prev << ", " << m_nodes[position].m_next << ", " << m_nodes[position].m_data << "} -> ";
        position = m_nodes[position].m_next;
    }
    std::cout << "(" << position << "){" << m_nodes[position].m_prev << ", " << m_nodes[position].m_next << ", " << m_nodes[position].m_data << "}\n";
}

List::~List() {
    free(m_nodes);
}
