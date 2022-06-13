#ifndef THDB_DEFINE_H_
#define THDB_DEFINE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace thdb {

using std::cout;
using std::endl;

typedef uint32_t PageID;
typedef uint16_t PageOffset;
typedef uint16_t SlotID;
typedef uint16_t FieldID;
typedef std::string String;
typedef uint32_t Size;

typedef std::pair<PageID, SlotID> PageSlotID;

// 为 C++11 添加 make_unique
// copy from https://isocpp.org/files/papers/N3656.txt
#ifdef __cpp_lib_make_unique
using std::make_unique;
#else
#define __cpp_lib_make_unique
template <class T>
struct _Unique_if {
  typedef std::unique_ptr<T> _Single_object;
};

template <class T>
struct _Unique_if<T[]> {
  typedef std::unique_ptr<T[]> _Unknown_bound;
};

template <class T, size_t N>
struct _Unique_if<T[N]> {
  typedef void _Known_bound;
};

template <class T, class... Args>
typename _Unique_if<T>::_Single_object make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {
  typedef typename std::remove_extent<T>::type U;
  return std::unique_ptr<T>(new U[n]());
}

template <class T, class... Args>
typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;
#endif
typedef uint32_t TxnID;

}  // namespace thdb

#endif
