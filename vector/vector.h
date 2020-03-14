#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

namespace SSTL {
template <typename T>
class vector {
   public:
    using iterator = T*;
    using reference = T&;
    using const_iterator = const T*;
    using const_reference = const T&;
    using value_type = T;
    using size_type = size_t;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    // the initialization should follow the order of
    // declaration
    vector() : sz{0}, cap{sz} { vec_ptr = nullptr; };
    ~vector() { delete[] vec_ptr; };
    explicit vector(size_t n) : sz{n}, cap{sz}, vec_ptr(new T[sz]) {
        std::fill(vec_ptr, (vec_ptr + sz), 0);
    }
    explicit vector(size_t n, T value) : sz{n}, cap{sz}, vec_ptr(new T[sz]) {
        std::fill(vec_ptr, (vec_ptr + sz), value);
    }
    vector(std::initializer_list<T> ini_list)
        : sz{ini_list.size()}, cap{sz}, vec_ptr(new T[sz]) {
        /* std::cout << "Initializer list constructer \n"; */
        std::copy(ini_list.begin(), ini_list.end(), vec_ptr);
    }

    // Copy constructor
    vector(const vector& from_vec) {
        /* std::cout << "Copy constructor  \n"; */
        sz = from_vec.size();
        cap = from_vec.cap;
        vec_ptr = new T[sz];
        std::copy(from_vec.cbegin(), from_vec.cend(), vec_ptr);
    }
    // move constructor
    vector(vector&& from_vec) noexcept : sz{0}, cap{sz}, vec_ptr(new T[sz]) {
        // call move assignment now
        /* std::cout << "Move constructer \n"; */
        *this = std::move(from_vec);
    }

    vector& operator=(const vector& from_vec);
    vector& operator=(vector&& from_vec) noexcept;
    template <typename type>
    friend std::ostream& operator<<(std::ostream& os,
                                    const vector<type>& rhs_vec);
    bool operator==(const vector& rhs_vec) const;

    size_t size() const;
    size_t capacity() const;

    reference operator[](size_t i);

    const_reference operator[](size_t i) const;

    const_iterator data() const;

    iterator data();

    reference at(size_t loc);
    const_reference at(size_t loc) const;
    reference back();
    const_reference back() const;
    reference front();
    const_reference front() const;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_iterator end() const noexcept;
    iterator end() noexcept;
    const_iterator cend() const noexcept;
    size_type max_size() const;
    void push_back(const_reference value);
    void push_back(T&& value);
    void reallocate();
    void shrink_to_fit();
    template <typename... Arg>
    void emplace_back(Arg&&... args);
    template <typename... Arg>
    iterator emplace(const_iterator args_iter, Arg&&... args);
    void reserve(size_t new_cap);
    bool empty() const noexcept;
    void clear() noexcept;
    void swap(vector& rhs) noexcept;
    void resize(size_type count);
    void resize(size_type count, const_reference value);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    iterator insert(const_iterator pos, const_reference value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_t count, const_reference value);
    template <
        typename input_iterator,
        std::enable_if_t<
            std::is_base_of<std::input_iterator_tag,
                            typename std::iterator_traits<
                                input_iterator>::iterator_category>::value,
            bool> = true>
    iterator insert(const_iterator pos, input_iterator first,
                    input_iterator last);

   private:
    size_t sz;
    size_t cap;
    T* vec_ptr;
    const size_t maxsize = 100000;

    // private helper functions
    void shift_right(iterator first_loc, size_t count);
};

// Copy assignment
template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& from_vec) {
    /* std::cout << "Copy assignment  \n"; */
    sz = from_vec.size();
    cap = from_vec.cap;
    delete[] vec_ptr;
    vec_ptr = new T[sz];
    std::copy(from_vec.data(), (from_vec.data() + sz), vec_ptr);
    return *this;
}
// move constructor
template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& from_vec) noexcept {
    /* std::cout << "Move assignment  \n"; */
    if (vec_ptr != from_vec.vec_ptr) {
        delete[] vec_ptr;
        vec_ptr = from_vec.data();
        sz = from_vec.size();
        cap = from_vec.cap;
        from_vec.vec_ptr = nullptr;
        from_vec.sz = 0;
    }

    return *this;
}
template <typename T>
bool vector<T>::operator==(const vector<T>& rhs_vec) const {
    bool is_equal = false;
    if (sz == rhs_vec.sz) {
        is_equal = true;
        for (size_t pos = 0; pos != sz && is_equal; ++pos) {
            if (vec_ptr[pos] != rhs_vec[pos]) {
                is_equal = false;
            }
        }
    }
    return is_equal;
}
template <typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& rhs_vec) {
    for (const auto& elem : rhs_vec) {
        os << elem << ' ';
    }
    return os;
}
template <typename T>
T& vector<T>::front() {
    return at(0);
}

template <typename T>
const T& vector<T>::front() const {
    return at(0);
}

template <typename T>
T& vector<T>::back() {
    return at(sz - 1);
}

template <typename T>
const T& vector<T>::back() const {
    return at(sz - 1);
}

template <typename T>
T* vector<T>::data() {
    return vec_ptr;
}

template <typename T>
const T* vector<T>::data() const {
    return vec_ptr;
}

template <typename T>
size_t vector<T>::size() const {
    return sz;
}

template <typename T>
size_t vector<T>::capacity() const {
    return cap;
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() noexcept {
    return vec_ptr;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
    return vec_ptr;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cbegin() const noexcept {
    return vec_ptr;
}

// reverse iterator points to one element
// preceeding it
template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::crend() const noexcept {
    return const_reverse_iterator(begin());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <typename T>
typename vector<T>::reverse_iterator vector<T>::rend() noexcept {
    return reverse_iterator(begin());
}

template <typename T>
typename vector<T>::iterator vector<T>::end() noexcept {
    return (vec_ptr + sz);
}

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
    return (vec_ptr + sz);
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cend() const noexcept {
    return (vec_ptr + sz);
}
template <typename T>
typename vector<T>::size_type vector<T>::max_size() const {
    return maxsize;
}

template <typename T>
T& vector<T>::at(size_t loc) {
    if (loc >= sz) {
        throw std::out_of_range("Accessing out of bounds!");
    }
    return operator[](loc);
}

template <typename T>
const T& vector<T>::at(size_t loc) const {
    if (loc >= sz) {
        throw std::out_of_range("Accessing out of bounds!");
    }
    return operator[](loc);
}
template <typename T>
T& vector<T>::operator[](size_t i) {
    return *(vec_ptr + i);
}

// const overload is required to access the value
// and not change the value. Hence not passing
// the reference of the pointer
template <typename T>
const T& vector<T>::operator[](size_t i) const {
    return *(vec_ptr + i);
}
template <typename T>
void vector<T>::reallocate() {
    // Paranthesis at the end value initializes
    // the heap allocation.
    auto temp_vectr = new T[cap]();
    std::copy(begin(), end(), temp_vectr);
    delete[] vec_ptr;
    vec_ptr = temp_vectr;
}
template <typename T>
void vector<T>::push_back(const T& value) {
    if (vec_ptr == nullptr) {
        vec_ptr = new T[1];
        cap = 1;
    }
    if (cap == sz) {
        cap = 2 * sz;
        reallocate();
    }
    *(vec_ptr + sz) = value;
    ++sz;
}
template <typename T>
void vector<T>::push_back(T&& value) {
    if (vec_ptr == nullptr) {
        vec_ptr = new T[1];
        cap = 1;
    }
    if (cap == sz) {
        cap = 2 * sz;
        reallocate();
    }
    *(vec_ptr + sz) = value;
    ++sz;
}
template <typename T>
void vector<T>::shrink_to_fit() {
    cap = sz;
    reallocate();
}
template <typename T>
template <typename... Arg>
void vector<T>::emplace_back(Arg&&... args) {
    if (vec_ptr == nullptr) {
        vec_ptr = new T[1];
        cap = 1;
    }
    if (cap == sz) {
        cap = 2 * sz;
        reallocate();
    }
    *(vec_ptr + sz) = std::move(T{std::forward<Arg>(args)...});
    ++sz;
}
template <typename T>
void vector<T>::reserve(size_t new_cap) {
    if (new_cap > sz) {
        cap = new_cap;
        reallocate();
    }
}

template <typename T>
template <typename... Arg>
T* vector<T>::emplace(const T* args_iter, Arg&&... args) {
    // Important to calculate the pos before reallocating
    // as the memory address might change.
    size_t pos = (args_iter - begin());

    if (vec_ptr == nullptr) {
        vec_ptr = new T[1];
        cap = 1;
    }

    if (cap == sz) {
        cap = 2 * sz;
        reallocate();
    }

    std::copy(begin() + pos, end(), begin() + pos + 1);
    *(vec_ptr + pos) = std::move(T{std::forward<Arg>(args)...});
    ++sz;
    return (vec_ptr + pos);
}
template <typename T>
bool vector<T>::empty() const noexcept {
    return (cbegin() == cend());
}
template <typename T>
void vector<T>::clear() noexcept {
    sz = 0;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(
    typename vector<T>::const_iterator pos) {
    auto loc = (pos - begin());
    // shift the vector to the left from pos+1
    std::copy(begin() + loc + 1, end(), begin() + loc);
    --sz;
    return begin() + loc;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(
    typename vector<T>::const_iterator first,
    typename vector<T>::const_iterator last) {
    auto first_loc = (first - begin());
    auto last_loc = (last - begin());

    std::copy(begin() + last_loc, end(), begin() + first_loc);
    sz -= (last_loc - first_loc);

    return begin() + first_loc;
}

template <typename T>
void vector<T>::shift_right(typename vector<T>::iterator first_loc,
                            size_t count) {
    for (auto iter = end() - 1; iter != first_loc - 1; iter--) {
        *(iter + count) = *iter;
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator pos, const T& value) {
    auto loc = (pos - begin());

    if (cap == sz) {
        cap = 2 * sz;
        reallocate();
    }

    auto first_loc = begin() + loc;
    shift_right(first_loc, 1);
    vec_ptr[loc] = value;
    ++sz;
    return first_loc;
}
template <typename T>
typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator pos, T&& value) {
    auto loc = (pos - begin());
    if (cap == sz) {
        cap = 2 * sz;
        reallocate();
    }

    auto first_loc = begin() + loc;
    shift_right(first_loc, 1);
    vec_ptr[loc] = value;
    ++sz;

    return first_loc;
}
template <typename T>
typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator pos, size_t count, const T& value)

{
    auto loc = (pos - begin());

    if (cap < (sz + count)) {
        // reallocating two times the needed memory
        cap = (sz + count) * 2;
        reallocate();
    }
    auto first_loc = begin() + loc;
    auto last_loc = begin() + loc + count;

    shift_right(first_loc, count);
    std::fill(first_loc, last_loc, value);
    sz += count;
    return first_loc;
}

template <typename T>
template <typename input_iterator,
          std::enable_if_t<
              std::is_base_of<std::input_iterator_tag,
                              typename std::iterator_traits<
                                  input_iterator>::iterator_category>::value,
              bool>>
typename vector<T>::iterator vector<T>::insert(
    typename vector<T>::const_iterator pos, input_iterator first,
    input_iterator last) {
    auto loc = pos - begin();
    auto size_of_input = last - first;
    if (cap < (sz + size_of_input)) {
        // reallocating two times the needed memory
        cap = 2 * (sz + size_of_input);
        reallocate();
    }
    auto first_loc = begin() + loc;
    auto last_loc = begin() + loc + size_of_input;
    std::copy(first_loc, end(), last_loc);
    std::copy(first, last, first_loc);
    sz += size_of_input;
    return first_loc;
}
template <typename T>
void vector<T>::swap(vector& rhs) noexcept {
    using std::swap;
    swap(vec_ptr, rhs.vec_ptr);
    swap(sz, rhs.sz);
    swap(cap, rhs.cap);
}
template <typename T>
void vector<T>::resize(typename vector<T>::size_type count) {
    if (count > sz) {
        cap = count;
        reallocate();
    }
    sz = count;
}
template <typename T>
void vector<T>::resize(typename vector<T>::size_type count,
                       typename vector<T>::const_reference value) {
    if (count > sz) {
        cap = count;
        reallocate();
        using std::fill_n;
        fill_n(end(), cap - sz, value);
    }
    sz = count;
}
}  // namespace SSTL
