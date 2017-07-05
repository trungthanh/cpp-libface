/*
 * Simple fixed size vector with minimum memory footprint
 * Author: tngo@walmartlabs.com
 */

#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include <vector>

using namespace std;

template<typename T>
class fixed_vector {
  public:
    T* data;
    unsigned int sz;

    fixed_vector() {
      data = NULL;
      sz = 0;
    }

    fixed_vector(vector<T> &v) {
      set_vector(v);
    }

    void set_vector(vector<T> &v) {
      if (v.size()) {
        sz = v.size();
        data = new T[sz];
        for (unsigned int i=0; i<sz; ++i) {
          data[i] = v[i];
        }
      } else {
        data = NULL;
        sz = 0;
      }
    }

    ~fixed_vector() {
      if (data) {
        delete[] data;
      }
    }

    T operator[](unsigned int i) {
      if (data && i<sz) {
        return data[i];
      } else {
        throw -1;
      }
    }

    fixed_vector<T>& operator=(vector<T> &v) {
      set_vector(v);
      return *this;
    }

    unsigned int size() {
      return sz;
    }
};

#endif
