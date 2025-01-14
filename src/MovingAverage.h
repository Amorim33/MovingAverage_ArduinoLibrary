/***************************************************************
  MovingAverage.h - Moving average structure manager.
  Created by Alexandre Hiroyuki Yamauchi, September 2, 2019.
***************************************************************/
#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <Arduino.h>

#include <cstring>

template <class TypeOfArray>
class MovingAverage {
 private:
  size_t _array_size;
  size_t _current_index;
  size_t _average_counter;
  TypeOfArray *_array;
  TypeOfArray _average_sum;
  TypeOfArray _initial_value;

  void _nextIndex() {
    _current_index++;
    if (_current_index >= _array_size) _current_index = 0;
  }

 public:
  // Constructor
  MovingAverage(size_t size)
      : _array_size(size),
        _current_index(0),
        _average_counter(0),
        _array((TypeOfArray *)calloc(size, sizeof(TypeOfArray))),
        _average_sum(0),
        _initial_value(0) {}

  // Destructor
  ~MovingAverage() { free(_array); }

  // Get Result and Access elements

  MovingAverage<TypeOfArray> &push(TypeOfArray input) {
    _average_sum -= _array[_current_index];

    _array[_current_index] = input;

    _average_sum += _array[_current_index];

    if (_average_counter < _array_size) {
      _average_counter++;
    }

    _nextIndex();

    return *this;
  }

  TypeOfArray get() {
    return (_average_sum / ((_average_counter == 0) ? 1 : _average_counter));
  }

  TypeOfArray front() {
    int last_index = _current_index;

    if (last_index - 1 < 0) {
      last_index = _array_size - 1;
    } else {
      last_index -= 1;
    }

    return _array[last_index];
  }

  TypeOfArray back() { return _array[_current_index]; }

  TypeOfArray operator[](size_t index) {
    if (index > _array_size) return 0;

    int final_index = (_current_index - 1) - index;

    int check_index = _array_size + final_index;

    if (final_index < 0) {
      return _array[check_index];
    }

    return _array[final_index];
  }

  TypeOfArray atIndex(size_t index) { return _array[index]; }

  // Modify array

  size_t size() { return _array_size; }

  MovingAverage<TypeOfArray> &resize(size_t new_size) {
    _array = (TypeOfArray *)realloc(_array, new_size * sizeof(TypeOfArray));

    if (_current_index == 0) {
      _current_index = _array_size;
    }

    _array_size = new_size;
    return *this;
  }
  MovingAverage<TypeOfArray> &clear() {
    memset(_array, 0, sizeof(TypeOfArray) * _array_size);

    _average_sum = 0;

    _average_counter = 0;

    return *this;
  }
  MovingAverage<TypeOfArray> &fill(TypeOfArray fill_value) {
    for (size_t i = 0; i < _array_size; i++) {
      _array[i] = fill_value;
    }

    _average_sum = fill_value * _average_counter;

    return *this;
  }

  size_t d_counter() { return _average_counter; }
};

#endif
