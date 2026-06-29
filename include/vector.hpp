#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <algorithm>         // std::copy, std::equal, std::fill
#include <cassert>           // assert()
#include <cstddef>           // std::size_t
#include <iostream>          // std::cout, std::endl
#include <iterator>          // std::advance, std::begin(), std::end(), std::ostream_iterator
#include <limits>            // std::numeric_limits<T>
#include <memory>            // std::unique_ptr
#include <stdexcept>
#include <initializer_list>  // std::initializer_list

/// Sequence container namespace.
namespace sc {
/// This class implements the ADT list with dynamic array.
/*!
 * sc::vector is a sequence container that encapsulates dynamic size arrays.
 *
 * The elements are stored contiguously, which means that elements can
 * be accessed not only through iterators, but also using offsets to
 * regular pointers to elements.
 * This means that a pointer to an element of a vector may be passed to
 * any function that expects a pointer to an element of an array.
 *
 * \tparam T The type of the elements.
 */
template <typename T>
class vector {
private:
  size_t resize_count{0};
  //=== Aliases
public:
  using size_type = unsigned long;   //!< The size type.
  using value_type = T;              //!< The value type.
  using pointer = value_type*;       //!< Pointer to a value in the container.
  using reference = value_type&;     //!< Reference to a value in the container.
  using const_reference = const T&;  //!< Const ref to a value in the container.
  size_t get_resize_count() const { return resize_count; }

    //!< The iterator, instantiated from a template class.
  class iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;

  private:
    pointer m_ptr;
  public:
    /**
     * @brief Regular constructor.
     * @param ptr Pointer to the element.
     */
    iterator(T* ptr = nullptr) { m_ptr = ptr; }
    
    /**
     * @brief Copy constructor.
     * @param other Another iterator to copy from.
     */
    iterator(const iterator& other) {m_ptr = other.m_ptr;}

    /**
     * @brief Dereference operator.
     * @return Reference to the element pointed to by the iterator.
     */
    reference operator*() const { return *m_ptr; }

    /**
     * @brief Equality comparison operator.
     * @param itr2 Another iterator to compare with.
     * @return True if both iterators point to the same element, false otherwise.
     */
    bool operator==(const iterator& itr2) const { 
      return this->m_ptr == itr2.m_ptr; 
    }

    /**
     * @brief Pre-increment operator.
     * @return Reference to the incremented iterator.
     */
    iterator& operator++() {
      m_ptr++;
      return *this;
    }

    /**
     * @brief Post-increment operator.
     * @return A copy of the iterator before incrementing.
     */
    iterator operator++(int) {
      iterator backup{ *this };
      m_ptr++;
      return backup;
    }

    /**
     * @brief Greater than comparison operator.
     * @param itr2 Another iterator to compare with.
     * @return True if this iterator points to an element after the one pointed by itr2.
     */
    bool operator>(const iterator& itr2) const {
      return this->m_ptr > itr2.m_ptr;
    }

    /**
     * @brief Less than comparison operator.
     * @param itr2 Another iterator to compare with.
     * @return True if this iterator points to an element before the one pointed by itr2.
     */
    bool operator<(const iterator& itr2) const {
      return this->m_ptr < itr2.m_ptr;
    }

    /**
     * @brief Less than or equal comparison operator.
     * @param itr2 Another iterator to compare with.
     * @return True if this iterator points to an element before or at the same position as itr2.
     */
    bool operator<=(const iterator& itr2) const {
      return this->m_ptr <= itr2.m_ptr;
    }

    /**
     * @brief Greater than or equal comparison operator.
     * @param itr2 Another iterator to compare with.
     * @return True if this iterator points to an element after or at the same position as itr2.
     */
    bool operator>=(const iterator& itr2) const {
      return this->m_ptr >= itr2.m_ptr;
    }

    /**
     * @brief Assignment operator.
     * @param itr2 Another iterator to assign from.
     * @return Reference to this iterator.
     */
    iterator& operator=(const iterator& itr2) { 
      m_ptr = itr2.m_ptr; 
      return *this; 
    }

    /**
     * @brief Inequality comparison operator.
     * @param itr2 Another iterator to compare with.
     * @return True if iterators point to different elements, false otherwise.
     */
    bool operator!=(const iterator& itr2) const {
      return this->m_ptr != itr2.m_ptr; 
    }

    /**
     * @brief Pre-decrement operator.
     * @return Reference to the decremented iterator.
     */
    iterator& operator--() {
      m_ptr--;
      return *this;
    }

    /**
     * @brief Post-decrement operator.
     * @return A copy of the iterator before decrementing.
     */
    iterator operator--(int) {
      iterator backup{ *this };
      m_ptr--;
      return backup;
    }

    /**
     * @brief Addition operator (n + iterator).
     * @param n Number of positions to move forward.
     * @param itr2 The iterator.
     * @return A new iterator moved forward by n positions.
     */
    friend iterator operator+(difference_type n, const iterator& itr2) {
      return iterator(n + itr2.m_ptr);
    }

    /**
     * @brief Addition operator (iterator + n).
     * @param itr2 The iterator.
     * @param n Number of positions to move forward.
     * @return A new iterator moved forward by n positions.
     */
    friend iterator operator+(const iterator& itr2, difference_type n) {
      return iterator(itr2.m_ptr + n);
    }

    /**
     * @brief Subtraction operator (iterator - iterator).
     * @param itr2 Another iterator.
     * @return The distance between the two iterators.
     */
    difference_type operator-(const iterator& itr2) const{
      return this->m_ptr - itr2.m_ptr;
    }

    /**
     * @brief Subtraction operator (iterator - n).
     * @param n Number of positions to move backward.
     * @return A new iterator moved backward by n positions.
     */
    iterator operator-(difference_type n) const {
      return iterator(this->m_ptr - n);
    }

    /**
     * @brief Subtraction assignment operator.
     * @param n Number of positions to move backward.
     * @return Reference to this iterator.
     */
    iterator& operator-=(difference_type n) {
      this->m_ptr -= n;
      return *this;
    }

    /**
     * @brief Addition assignment operator.
     * @param n Number of positions to move forward.
     * @return Reference to this iterator.
     */
    iterator& operator+=(difference_type n) {
      this->m_ptr += n;
      return *this;
    }
  };

  //!< The const_iterator, instantiated from a template class.
  class const_iterator {
    public:
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type = T;
      using difference_type = std::ptrdiff_t;
      using pointer = const T*;
      using reference = const T&;
    private:
      pointer m_ptr;
    public:
      /**
       * @brief Copy constructor.
       * @param other Another const_iterator to copy from.
       */
      const_iterator(const const_iterator& other) {m_ptr = other.m_ptr;}

      /**
       * @brief Regular constructor.
       * @param ptr Pointer to the element.
       */
      const_iterator(pointer ptr = nullptr) { m_ptr = ptr; }

      /**
       * @brief Dereference operator.
       * @return Const reference to the element pointed to by the iterator.
       */
      reference operator*() const { return *m_ptr; }

      /**
       * @brief Equality comparison operator.
       * @param itr2 Another const_iterator to compare with.
       * @return True if both iterators point to the same element, false otherwise.
       */
      bool operator==(const const_iterator& itr2) const { 
        return this->m_ptr == itr2.m_ptr; 
      }

      /**
       * @brief Pre-increment operator.
       * @return Reference to the incremented iterator.
       */
      const_iterator& operator++() {
        m_ptr++;
        return *this;
      }

      /**
       * @brief Post-increment operator.
       * @return A copy of the iterator before incrementing.
       */
      const_iterator operator++(int) {
        const_iterator backup{ *this };
        m_ptr++;
        return backup;
      }

      /**
       * @brief Greater than comparison operator.
       * @param itr2 Another const_iterator to compare with.
       * @return True if this iterator points to an element after the one pointed by itr2.
       */
      bool operator>(const const_iterator& itr2) const {
        return this->m_ptr > itr2.m_ptr;
      }

      /**
       * @brief Less than comparison operator.
       * @param itr2 Another const_iterator to compare with.
       * @return True if this iterator points to an element before the one pointed by itr2.
       */
      bool operator<(const const_iterator& itr2) const {
        return this->m_ptr < itr2.m_ptr;
      }

      /**
       * @brief Less than or equal comparison operator.
       * @param itr2 Another const_iterator to compare with.
       * @return True if this iterator points to an element before or at the same position as itr2.
       */
      bool operator<=(const const_iterator& itr2) const {
        return this->m_ptr <= itr2.m_ptr;
      }

      /**
       * @brief Greater than or equal comparison operator.
       * @param itr2 Another const_iterator to compare with.
       * @return True if this iterator points to an element after or at the same position as itr2.
       */
      bool operator>=(const const_iterator& itr2) const {
        return this->m_ptr >= itr2.m_ptr;
      }

      /**
       * @brief Assignment operator.
       * @param itr2 Another const_iterator to assign from.
       * @return Reference to this iterator.
       */
      const_iterator& operator=(const const_iterator& itr2) { 
        m_ptr = itr2.m_ptr; 
        return *this; 
      }

      /**
       * @brief Inequality comparison operator.
       * @param itr2 Another const_iterator to compare with.
       * @return True if iterators point to different elements, false otherwise.
       */
      bool operator!=(const const_iterator& itr2) const {
        return this->m_ptr != itr2.m_ptr; 
      }

      /**
       * @brief Pre-decrement operator.
       * @return Reference to the decremented iterator.
       */
      const_iterator& operator--() {
        m_ptr--;
        return *this;
      }

      /**
       * @brief Post-decrement operator.
       * @return A copy of the iterator before decrementing.
       */
      const_iterator operator--(int) {
        const_iterator backup{ *this };
        m_ptr--;
        return backup;
      }

      /**
       * @brief Addition operator (n + const_iterator).
       * @param n Number of positions to move forward.
       * @param itr2 The iterator.
       * @return A new iterator moved forward by n positions.
       */
      friend const_iterator operator+(difference_type n, const const_iterator& itr2) {
        return const_iterator(n + itr2.m_ptr);
      }

      /**
       * @brief Addition operator (const_iterator + n).
       * @param itr2 The iterator.
       * @param n Number of positions to move forward.
       * @return A new iterator moved forward by n positions.
       */
      friend const_iterator operator+(const const_iterator& itr2, difference_type n) {
        return const_iterator(itr2.m_ptr + n);
      }

      /**
       * @brief Subtraction operator (const_iterator - const_iterator).
       * @param itr2 Another iterator.
       * @return The distance between the two iterators.
       */
      difference_type operator-(const const_iterator& itr2) const{
        return this->m_ptr - itr2.m_ptr;
      }

      /**
       * @brief Subtraction operator (const_iterator - n).
       * @param n Number of positions to move backward.
       * @return A new iterator moved backward by n positions.
       */
      const_iterator operator-(difference_type n) const {
        return const_iterator(this->m_ptr - n);
      }

      /**
       * @brief Subtraction assignment operator.
       * @param n Number of positions to move backward.
       * @return Reference to this iterator.
       */
      const_iterator& operator-=(difference_type n) {
        this->m_ptr -= n;
        return *this;
      }

      /**
       * @brief Addition assignment operator.
       * @param n Number of positions to move forward.
       * @return Reference to this iterator.
       */
      const_iterator& operator+=(difference_type n) {
        this->m_ptr += n;
        return *this;
      }
  };

  //=== [I] SPECIAL MEMBERS (6 OF THEM)
  /**
   * @brief Default constructor.
   * @param cp Initial capacity of the vector.
   */
  explicit vector(size_type cp = 0) {
    m_end = cp;
    m_capacity = cp;
    m_storage = new value_type[m_capacity]();
  }

  /**
   * @brief Destructor.
   */
  virtual ~vector() {
    delete[] m_storage;
  }

  /**
   * @brief Copy constructor.
   * @param other Another vector to copy from.
   */
  vector(const vector& other) {
    m_capacity = other.m_capacity;
    m_end = other.m_end;
    m_storage = new value_type[other.m_capacity];
    std::copy(other.m_storage, other.m_storage + other.m_end, m_storage);
  }

  /**
   * @brief Constructor from an initializer list.
   * @param il The initializer list.
   */
  vector(const std::initializer_list<T>& il) {
    m_capacity = il.size();
    m_end = il.size();
    m_storage = new value_type[m_capacity];

    std::copy(il.begin(), il.end(), m_storage);
  }

  /**
   * @brief Constructor from a range [first, last).
   * @tparam InputItr Type of the input iterator.
   * @param first Iterator to the beginning of the range.
   * @param last Iterator to the end of the range.
   */
  template <typename InputItr> vector(InputItr first, InputItr last) {
    m_capacity = std::distance(first, last);
    m_end = m_capacity;
    m_storage = new T[m_capacity];
    std::copy(first, last, m_storage);
  }

  /**
   * @brief Copy assignment operator.
   * @param other Another vector to copy from.
   * @return Reference to this vector.
   */
  vector& operator=(const vector& other) { // vec1 = vec2
    vector temp(other);
    swap(*this, temp);
    return *this;
  }

  //=== [II] ITERATORS
  /**
   * @brief Returns an iterator to the beginning.
   * @return Iterator to the first element.
   */
  iterator begin() { return iterator{ m_storage }; }

  /**
   * @brief Returns an iterator to the end.
   * @return Iterator to the element following the last element.
   */
  iterator end() { return iterator{ m_storage + m_end }; }

  /**
   * @brief Returns a constant iterator to the beginning.
   * @return Constant iterator to the first element.
   */
  const_iterator cbegin() const {
    return const_iterator{ m_storage };
  }

  /**
   * @brief Returns a constant iterator to the end.
   * @return Constant iterator to the element following the last element.
   */
  const_iterator cend() const {
    return const_iterator{ m_storage + m_end };
  }

  // [III] Capacity
  /**
   * @brief Returns the number of elements in the vector.
   * @return The number of elements.
   */
  [[nodiscard]] size_type size() const {
    return m_end;
  }

  /**
   * @brief Returns the number of elements that can be held in currently allocated storage.
   * @return The capacity.
   */
  [[nodiscard]] size_type capacity() const {
    return m_capacity;
  }

  /**
   * @brief Checks whether the container is empty.
   * @return True if the container is empty, false otherwise.
   */
  [[nodiscard]] bool empty() const {
    if (m_end == 0) {
      return true;
    }
    return false;
  }

  // [IV] Modifiers
  /**
   * @brief Removes all elements from the vector.
   */
  void clear() {
    m_end = 0;
  }

  /**
   * @brief Inserts an element at the beginning.
   * @param value The value to insert.
   */
  void push_front(const_reference value) {
    if (m_end == m_capacity){ reserve(m_capacity == 0 ? 1 : m_capacity * 2); }

    for (size_t i = m_end; i > 0; --i) {
      m_storage[i] = m_storage[i - 1];
    }

    m_storage[0] = value;
    m_end++;
  };

  /**
   * @brief Appends an element to the end.
   * @param value The value to append.
   */
  void push_back(const_reference value) {
    value_type temp = value;
    if (m_capacity == m_end) {
      reserve(m_capacity == 0 ? 1 : m_capacity * 2);;
    }
    m_storage[m_end++] = temp;
  }

  /**
   * @brief Removes the last element.
   */
  void pop_back() {
    if (m_end > 0) {
      m_end--;
    }
  }

  /**
   * @brief Removes the first element.
   */
  void pop_front() {
    if (m_end == 0) return;

    for (size_t i = 0; i < m_end - 1; ++i) {
      m_storage[i] = m_storage[i + 1];
    }

    m_end--;
  };

  /**
   * @brief Inserts an element at a specific position.
   * @param pos Iterator to the position where the element will be inserted.
   * @param value The value to insert.
   * @return Iterator to the inserted element.
   */
  iterator insert(iterator pos, const_reference value) {
    size_t index = std::distance(begin(), pos);
    value_type temp = value;
    if (m_end == m_capacity){ reserve(m_capacity == 0 ? 1 : m_capacity * 2); }

    for (size_t i = m_end; i > index; --i) {
      m_storage[i] = m_storage[i - 1];
    }

    m_storage[index] = temp;
    m_end++;

    return iterator{m_storage + index};
  };

  /**
   * @brief Inserts an element at a specific position.
   * @param pos Constant iterator to the position where the element will be inserted.
   * @param value The value to insert.
   * @return Iterator to the inserted element.
   */
  iterator insert(const_iterator pos, const_reference value) {
    value_type temp = value;
    size_t index = std::distance(cbegin(), pos);

    if (m_end == m_capacity){ reserve(m_capacity == 0 ? 1 : m_capacity * 2); }

    for (size_t i = m_end; i > index; --i) {
      m_storage[i] = m_storage[i - 1];
    }

    m_storage[index] = temp;
    m_end++;

    return iterator{m_storage + index};
  };

  /**
   * @brief Inserts elements from a range [first, last) at a specific position.
   * @tparam InputItr Type of the input iterator.
   * @param pos Iterator to the position where elements will be inserted.
   * @param first Iterator to the beginning of the range.
   * @param last Iterator to the end of the range.
   * @return Iterator to the first inserted element.
   */
  template <typename InputItr>
  iterator insert(iterator pos, InputItr first, InputItr last) {
    size_t count = std::distance(first, last);
    if (count == 0) return pos;

    size_t index = std::distance(begin(), pos);

    if (m_end + count > m_capacity) { reserve((m_capacity * 2 >= m_end + count) ? m_capacity * 2 : m_end + count); }

    std::copy_backward(m_storage + index, m_storage + m_end, m_storage + m_end + count);
    std::copy(first, last, m_storage + index);

    m_end += count;

    return iterator{m_storage + index};
  };

  /**
   * @brief Inserts elements from a range [first, last) at a specific position.
   * @tparam InputItr Type of the input iterator.
   * @param pos Constant iterator to the position where elements will be inserted.
   * @param first Iterator to the beginning of the range.
   * @param last Iterator to the end of the range.
   * @return Iterator to the first inserted element.
   */
  template <typename InputItr>
  iterator insert(const_iterator pos, InputItr first, InputItr last) {
    size_t index = std::distance(cbegin(), pos);
    size_t count = std::distance(first, last);

    if (count == 0) return iterator{m_storage + index};

    if (m_end + count > m_capacity) { reserve((m_capacity * 2 >= m_end + count) ? m_capacity * 2 : m_end + count); }

    std::copy_backward(m_storage + index, m_storage + m_end, m_storage + m_end + count);
    std::copy(first, last, m_storage + index);

    m_end += count;

    return iterator{m_storage + index};
  };

  /**
   * @brief Inserts elements from an initializer list at a specific position.
   * @param pos Iterator to the position where elements will be inserted.
   * @param ilist The initializer list.
   * @return Iterator to the first inserted element.
   */
  iterator insert(iterator pos, const std::initializer_list<value_type> &ilist) {
    size_t count = ilist.size();
    if (count == 0) { return pos; }

    size_t index = std::distance(begin(), pos);

    if (m_end + count > m_capacity) { reserve((m_capacity * 2 >= m_end + count) ? m_capacity * 2 : m_end + count); }

    std::copy_backward(m_storage + index, m_storage + m_end, m_storage + m_end + count);
    std::copy(ilist.begin(), ilist.end(), m_storage + index);

    m_end += count;

    return iterator{m_storage + index};
  };

  /**
   * @brief Inserts elements from an initializer list at a specific position.
   * @param pos Constant iterator to the position where elements will be inserted.
   * @param ilist The initializer list.
   * @return Iterator to the first inserted element.
   */
  iterator insert(const_iterator pos, const std::initializer_list<value_type> &ilist) {
    size_t count = ilist.size();
    size_t index = std::distance(cbegin(), pos);

    if (count == 0) { return iterator{m_storage + index}; }

    if (m_end + count > m_capacity) { reserve((m_capacity * 2 >= m_end + count) ? m_capacity * 2 : m_end + count); }

    std::copy_backward(m_storage + index, m_storage + m_end, m_storage + m_end + count);
    std::copy(ilist.begin(), ilist.end(), m_storage + index);

    m_end += count;

    return iterator{m_storage + index};
  };

  /**
   * @brief Increases the capacity of the vector.
   * @param size_change The new capacity.
   */
  void reserve(size_type size_change) {
    if (size_change < m_capacity) {
      return;
    }
    value_type* temp_storage = new value_type[size_change];
    std::copy(m_storage, m_storage + m_end, temp_storage);
    delete[] m_storage;
    m_storage = temp_storage;
    m_capacity = size_change;

    resize_count++; // Records that the reallocation took place
  }

  /**
   * @brief Reduces capacity to fit the size.
   */
  void shrink_to_fit() {
    if(m_capacity == m_end) return; // Avoids unnecessary reallocation
    
    value_type* temp = new value_type[m_end];
    std::copy(m_storage, m_storage + m_end, temp);
    
    delete[] m_storage;
    m_storage = temp;
    m_capacity = m_end;

    resize_count++; // Records that the reallocation took place
  }

  /**
   * @brief Assigns a new value to the vector, replacing its current contents.
   * @param count The number of elements.
   * @param value The value to assign.
   */
  void assign(size_type count, const_reference value) {
    value_type temp = value;
    if (count > m_capacity) { reserve(count); }
    std::fill(m_storage, m_storage + count, temp);
    m_end = count; 
  };

  /**
   * @brief Assigns values from an initializer list to the vector.
   * @param ilist The initializer list.
   */
  void assign(const std::initializer_list<T>& ilist) {
    size_t count = ilist.size();
    m_end = 0;

    if (count > m_capacity) { reserve(count); }

    std::copy(ilist.begin(), ilist.end(), m_storage);
    m_end = count;
  };

  /**
   * @brief Assigns values from a range [first, last) to the vector.
   * @tparam InputItr Type of the input iterator.
   * @param first Iterator to the beginning of the range.
   * @param last Iterator to the end of the range.
   */
  template <typename InputItr>
  void assign(InputItr first, InputItr last) {
    size_t count = std::distance(first, last);
    if (count > m_capacity) { 
       value_type* temp = new value_type[count];
       std::copy(first, last, temp);
       delete[] m_storage;
       m_storage = temp;
       m_capacity = count;
    } else {
       std::copy(first, last, m_storage);
    }
    m_end = count;
  }

  /**
   * @brief Erases elements in the range [first, last).
   * @param first Iterator to the beginning of the range.
   * @param last Iterator to the end of the range.
   * @return Iterator following the last removed element.
   */
  iterator erase(iterator first, iterator last) {
    size_t count = std::distance(first, last);
    if (count == 0) return first;

    size_t index = std::distance(begin(), first);
    
    for (size_t i = index; i < m_end - count; ++i) {
      m_storage[i] = m_storage[i + count];
    }
    
    m_end -= count;
    return iterator{m_storage + index};
  }

  /**
   * @brief Erases elements in the range [first, last).
   * @param first Constant iterator to the beginning of the range.
   * @param last Constant iterator to the end of the range.
   * @return Iterator following the last removed element.
   */
  iterator erase(const_iterator first, const_iterator last) {
    size_t count = std::distance(first, last);
    if (count == 0) return iterator{m_storage + std::distance(cbegin(), first)};

    size_t index = std::distance(cbegin(), first);
    
    for (size_t i = index; i < m_end - count; ++i) {
      m_storage[i] = m_storage[i + count];
    }
    
    m_end -= count;
    return iterator{m_storage + index};
  }

  /**
   * @brief Erases the element at a specific position.
   * @param pos Iterator to the element to remove.
   * @return Iterator following the removed element.
   */
  iterator erase(iterator pos) {
    return erase(pos, pos + 1); 
  }

  /**
   * @brief Erases the element at a specific position.
   * @param pos Constant iterator to the element to remove.
   * @return Iterator following the removed element.
   */
  iterator erase(const_iterator pos) {
    return erase(pos, pos + 1); 
  }

  // [V] Element access
  /**
   * @brief Returns a constant reference to the last element.
   * @return Constant reference to the last element.
   */
  const_reference back() const {
    return m_storage[m_end - 1];
  }

  /**
   * @brief Returns a constant reference to the first element.
   * @return Constant reference to the first element.
   */
  const_reference front() const {
    return m_storage[0];
  }

  /**
   * @brief Returns a reference to the last element.
   * @return Reference to the last element.
   */
  reference back() {
    return m_storage[m_end - 1];
  }

  /**
   * @brief Returns a reference to the first element.
   * @return Reference to the first element.
   */
  reference front() {
    return m_storage[0];
  }

  /**
   * @brief Constant access to the element at a specific index.
   * @param idx Index of the element.
   * @return Constant reference to the element.
   */
  const_reference operator[](size_type idx) const {
    return *(m_storage + idx);
  }

  /**
   * @brief Access to the element at a specific index.
   * @param idx Index of the element.
   * @return Reference to the element.
   */
  reference operator[](size_type idx) {
    return *(m_storage + idx);
  }

  /**
   * @brief Constant access to the element at a specific index with bounds checking.
   * @param idx Index of the element.
   * @return Constant reference to the element.
   * @throw std::out_of_range If index is out of bounds.
   */
  const_reference at(size_type idx) const {
    if (idx >= m_end) {
      throw std::out_of_range("Error! Access out of memory limits");
    }
    return m_storage[idx];
  }

  /**
   * @brief Access to the element at a specific index with bounds checking.
   * @param idx Index of the element.
   * @return Reference to the element.
   * @throw std::out_of_range If index is out of bounds.
   */
  reference at(size_type idx) {
    if (idx >= m_end) {
      throw std::out_of_range("Error! Access out of memory limits");
    }
    return m_storage[idx];
  }

  /**
   * @brief Returns a pointer to the underlying array.
   * @return Pointer to the data storage.
   */
  pointer data() {
    return m_storage;
  }

  /**
   * @brief Returns a constant pointer to the underlying array.
   * @return Constant pointer to the data storage.
   */
  const_reference data() const {
    return m_storage;
  }

  // [VII] Friend functions.
  /**
   * @brief Stream insertion operator.
   * @param os The output stream.
   * @param vec The vector to print.
   * @return Reference to the output stream.
   */
  friend std::ostream& operator<<(std::ostream& os, const vector<T>& vec) {
    os << "{ ";
    for (auto i{ 0U }; i < vec.m_capacity; ++i) {
      if (i == vec.m_end) {
        os << "| ";
      }
      os << vec.m_storage[i] << " ";
    }
    os << "}, m_end=" << vec.m_end << ", m_capacity=" << vec.m_capacity;
    return os;
  }

  /**
   * @brief Swaps the contents of two vectors.
   * @param first The first vector.
   * @param second The second vector.
   */
  friend void swap(vector<T>& first, vector<T>& second) noexcept {
    using std::swap;

    swap(first.m_end, second.m_end);
    swap(first.m_capacity, second.m_capacity);
    swap(first.m_storage, second.m_storage);
  }

private:
  /**
   * @brief Checks if the container is full.
   * @return True if capacity equals size, false otherwise.
   */
  [[nodiscard]] bool full() const {
    if (m_capacity == m_end) {
      return true;
    }
    return false;
  };

  size_type m_end{ 0 };       //!< The list's current size.
  size_type m_capacity{ 0 };  //!< The list's storage capacity.
  T* m_storage{ nullptr };    //!< The list's data storage area.
};

// [VI] Operators
/**
 * @brief Equality comparison operator between two vectors.
 * @tparam T Type of the elements.
 * @param first The first vector.
 * @param second The second vector.
 * @return True if vectors are equal, false otherwise.
 */
template <typename T>
bool operator==(const vector<T>& first, const vector<T>& second) {
  if (first.size() != second.size()) return false;
  return std::equal(first.cbegin(), first.cend(), second.cbegin());
};

/**
 * @brief Inequality comparison operator between two vectors.
 * @tparam T Type of the elements.
 * @param first The first vector.
 * @param second The second vector.
 * @return True if vectors are not equal, false otherwise.
 */
template <typename T>
bool operator!=(const vector<T>& first, const vector<T>& second) {
  return !(first == second);
};

}  // namespace sc.

#endif
