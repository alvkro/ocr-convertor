#ifndef LINKEDQUEUE_HPP
#define LINKEDQUEUE_HPP

#include "./vector.hpp"
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

struct Node {
  sc::vector<uint8_t> m_data{};
  std::unique_ptr<Node> m_next{};

  Node(sc::vector<uint8_t> data) : m_data(std::move(data)), m_next(nullptr) {}
};

class LinkedQueue {
private:
  std::unique_ptr<Node> m_head;
  Node *m_tail;

public:
  LinkedQueue() : m_head(nullptr), m_tail(nullptr) {}
  ~LinkedQueue() {
    while (!empty())
      m_head = std::move(m_head->m_next);
  }

  void enqueue(sc::vector<uint8_t> data) {
    auto new_node = std::make_unique<Node>(std::move(data));

    Node *new_address = new_node.get();

    if (empty()) {
      m_head = std::move(new_node);
    } else {
      m_tail->m_next = std::move(new_node);
    }

    m_tail = new_address;
  }
  sc::vector<uint8_t> dequeue() {
    if (empty())
      throw std::out_of_range("Empty queue.");

    sc::vector<uint8_t> current_data = std::move(m_head->m_data);

    m_head = std::move(m_head->m_next);

    if (empty())
      m_tail = nullptr;

    return current_data;
  }

  bool empty() const { return m_head == nullptr; }
};

#endif
