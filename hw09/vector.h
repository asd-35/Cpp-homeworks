#pragma once

#include <algorithm>
#include <memory>
#include <ostream>
#include <stdexcept>

template <typename T>
class Vector {
public:
    /**
     * TODO: Define the following associated types (i.e. `using blabla = T`, then blabla is an associated type)
     *       - `size_type`: should be an unsigned integer type
     *       - `difference_type`: should be a signed integer type
     *       - `reference`: reference to value_type
     *       - `const_reference`: const reference to value_type
     *       - `pointer`: pointer to value_type
     *       - `const_pointer`: const pointer to value_type
     *       - `iterator`: a random access iterator to the underlying data
     *       - `const_iterator`: a random access iterator to the underlying data
     *       - `reverse_iterator`: a random access iterator to the underlying data
     *       - `const_reverse_iterator`: a random access iterator to the underlying data
     *
     *       All of these are types aliases, and all of them are easy. Check e.g. the reference
     *       of `std::vector` (hint, pointers are iterators too), check reverse
     */
    using value_type = T;
    using size_type = unsigned int;
    using difference_type = signed int;
    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() noexcept { return _data.get(); }

    const_iterator begin() const noexcept { return _data.get(); }

    const_iterator cbegin() const noexcept { return _data.get(); }

    iterator end() noexcept { return _data.get() + _size; }

    const_iterator end() const noexcept { return _data.get() + _size; }

    const_iterator cend() const noexcept { return _data.get() + _size; }
    
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }

    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
    
    Vector() = default;

    /**
     * Creates a vector of size n with values default_val.
     */
    Vector(size_type n, const_reference default_val) {
        _data = std::make_unique<value_type[]>(n);
        _size = n;
        _capacity = n;
        
        for (size_type i = 0; i < _size; i++)
        {
            _data[i] = default_val;
        }
    
    }

    /**
     * Creates a vector containing the elements in l.
     */
    Vector(std::initializer_list<value_type> l) {
        _data = std::make_unique<value_type[]>(l.size());
        _size = l.size();
        _capacity = l.size();
        //this is a wonky loop :)
        int i = 0;
        for(auto x: l){
            _data[i] = x;
            ++i;
        }
    }

    Vector(const Vector& copy) : _size(copy._size), _capacity(copy._capacity) {
        _data = std::make_unique<value_type[]>(copy._capacity);
        
        for(size_type i = 0; i < _size;i++){
            _data[i] = copy._data[i];
        }

    }

    Vector(Vector&& move) : _size(move._size), _capacity(move._capacity) {
        _data = std::move(move._data);
        move._data = nullptr; 
    }

    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(const Vector& copy) {
        
        if (this != &copy) {
            _data.reset();

            _data = std::make_unique<value_type[]>(copy._capacity);

            for (size_type i = 0; i < copy._size; ++i) {
                _data[i] = copy._data[i];
            }

            _size = copy._size;
            _capacity = copy._capacity;
        }

        return *this;
    }


    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(Vector&& move) noexcept {
        
        if (this != &move) {
        
            _data.reset();
     
            _data = std::move(move._data);
            move._data = nullptr;

            _size = move._size;
            _capacity = move._capacity;
        }

        return *this; 
    }

    size_type size() const noexcept { return _size; }

    size_type capacity() const noexcept { return _capacity; }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(const_reference value) {
        size_type newCap = calculate_capacity(_size + 1);
        resize(newCap);
        _data[_size] = value; 
        _size++; 
    }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(T&& value) {
        size_type newCap = calculate_capacity(_size + 1);
        resize(newCap);
        _data[_size] = std::move(value);
        _size++; 
    }

    /**
     * Removes the last element of the vector.
     */
    void pop_back() {
        _size = _size - 1;
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    reference at(const size_type pos) const {
        if (pos >= _size)
            throw std::out_of_range("oor");
        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    reference at(const size_type pos) {
        if (pos >= _size)
            throw std::out_of_range("oor");
        return _data[pos];
    }

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    const_reference operator[](size_type index) const {return _data[index];}

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    reference operator[](const size_type index) {return _data[index];}

    // TODO: implement: 
    //           - begin, end (both const and non-const)
    //           - cbegin, cend,
    //           - rbegin, rend (const and non-const)
    //           - crbegin, crend (see e.g. https://en.cppreference.com/w/cpp/iterator/rbegin)
    //       This isn't necessarily very complicated, but a little tiresome, but that's part of C++ (yeah....)

    /**
     * Stream a Vector to an output stream textually.
     */
    friend std::ostream& operator<<(std::ostream& o, Vector<value_type> v) {
        o << "Size: " << v._size << ", Capacity: " << v._capacity << std::endl;
        for (size_type i = 0; i < v._size; ++i) {
            if (i > 0)
                o << ", ";
            o << v._data[i];
        }
        o << std::endl;
        return o;
    }

    ~Vector() = default;

private:
    // Defines how the `_capacity` is increased.
    size_type growth_factor = 2;
    size_type _size;
    size_type _capacity;

    // Holds vector's data.
    std::unique_ptr<value_type[]> _data;

    /**
     * Calculates the necessary capacity for new_size.
     * If necessary, double `_capacity` using `growth_factor`.
     */
    size_type calculate_capacity(size_type new_size) {
        if(_capacity == 0){
            return new_size;
        }else if(new_size <= _capacity){
            return _capacity;
        }
        return _capacity * growth_factor;
    }

    /**
     * Resizes the vector to new_capacity.
     * If the current capacity is less than new_capacity
     * the vector moves all elements to a new array.
     */
    void resize(size_type new_capacity) {
        std::unique_ptr<T[]> temp = std::make_unique<T[]>(new_capacity);
        for (size_type i{0}; i < _size; ++i){temp[i] = std::move(_data[i]);}
        _data = std::move(temp);
        _capacity = new_capacity;
    }
};
