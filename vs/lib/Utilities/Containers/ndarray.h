#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <iomanip>
#include <numeric>
#ifdef _DEBUG
#include "Utilities/ErrorUtils.h"
#endif

namespace Utilities
{

  /**
   * @brief A simple N-dimensional array in Column Major
   * @class ndarray
   * @tparam T data type
   */
  template <typename T>
  class ndarray
  {
  public:
    // #### constructors ####

    /// Default constructor to construct an empty ndarray
    ndarray(void) = default;

    /**
     * @brief N-Dimension constructor
     *
     * @param list A list to specify dimension of the ndarray
     */
    ndarray(std::initializer_list<uint32_t> list);

    /**
     * @brief 1-D constructor
     *
     * @param size number of element
     */
    ndarray(uint32_t size);

    /// Copy constructor
    ndarray(const ndarray<T> &rhs);

    /// Move constructor
    ndarray(ndarray<T> &&rhs);

    /// destructor
    ~ndarray(void);

    // #### methods ####

    /**
     * @brief Overload stream output operator
     *
     * @param out ostream object
     * @param s array to output
     * @return ostream& reference to the ostream object
     */
    template <class U>
    friend std::ostream &operator<<(std::ostream &out, const ndarray<U> &s);

    /**
     * @brief Setup a new ndarray object
     *
     * @param list A list to specify dimension of the ndarray
     */
    void Setup(std::initializer_list<uint32_t> list);

    /**
     * @brief Setup a new ndarray object
     *
     * @param list A list to specify dimension of the ndarray
     */
    void Setup(const ndarray<uint32_t> &list);

    /**
     * @brief Setup a new 1-D ndarray object
     *
     * @param size number of elements
     */
    void Setup(uint32_t size);

    void Clear();

    /// Assignment
    ndarray<T> &operator=(const ndarray<T> &rhs); //!< copy assignment
    ndarray<T> &operator=(ndarray<T> &&rhs);      //!< move assignment

    /// fill with value
    ndarray<T> &operator=(const T &val);

    /// Access/set ndarray element
    inline T &operator()(std::initializer_list<uint32_t> list);
    inline const T &operator()(std::initializer_list<uint32_t> list) const;

    /// 1-D access/set
    inline T &operator()(uint32_t idx);
    inline const T &operator()(uint32_t idx) const;

    /// Return pointer of ndarray element
    inline T *GetPointer(std::initializer_list<uint32_t> list);
    inline const T *GetPointer(std::initializer_list<uint32_t> list) const;

    /// Return pointer of ndarray element in 1-D
    inline T *GetPointer(uint32_t idx = 0);
    inline const T *GetPointer(uint32_t idx = 0) const;

    /// Get number of elements along one axis
    inline uint32_t GetDim(uint32_t n) const;

    /// Get number of elements along all axis
    ndarray<uint32_t> GetShape() const;

    /// Get number of dimension of the ndarray
    inline uint32_t GetNumDim(void) const;

    /// get the length of the ndarray
    inline uint32_t GetLength(void) const;

    /// Method to get maximum value of ndarray
    inline T GetMax(void) const;

    /// Method to get minimum value of ndarray
    inline T GetMin(void) const;

    /// Reshape the array
    void Reshape(std::initializer_list<uint32_t> list);

    /// Get the index of each dimension given the 1-D index
    ndarray<uint32_t> GetIndex(uint32_t idx) const;

    T *begin() { return p_data; }
    const T *begin() const { return p_data; }

    T *end() { return p_data + m_length; }
    const T *end() const { return p_data + m_length; }

  protected:
    uint32_t m_nDim = 1;
    uint32_t m_length = 0;
    uint32_t *p_shape = nullptr;
    uint32_t *p_offset = nullptr; // cumprod of the previous dimension sizes
    T *p_data = nullptr;

  private:
    /// helper method to calculate length of ndarray
    void CalcLength(void);
    void CalcOffset(void);
  };

  // #### constructors ####

  // constructor 1

  template <typename T>
  ndarray<T>::ndarray(std::initializer_list<uint32_t> list)
  {
    // store dimension array
    m_nDim = list.size();
    p_shape = new uint32_t[m_nDim];

    uint32_t i = 0;
    for (auto &l : list)
      p_shape[i++] = l;

    CalcLength();
    CalcOffset();
    p_data = new T[m_length];
  }

  // constructor 2

  template <typename T>
  ndarray<T>::ndarray(uint32_t size)
  {
    // store dimension array
    m_nDim = 1;
    p_shape = new uint32_t[m_nDim];
    p_shape[0] = size;
    CalcLength();
    CalcOffset();
    p_data = new T[m_length];
  }

  // copy constructor

  template <typename T>
  ndarray<T>::ndarray(const ndarray<T> &rhs)
  {
    m_nDim = rhs.m_nDim;
    p_shape = new uint32_t[m_nDim];
    std::copy(rhs.p_shape, rhs.p_shape + m_nDim, this->p_shape);
    p_offset = new uint32_t[m_nDim];
    std::copy(rhs.p_offset, rhs.p_offset + m_nDim, this->p_offset);
    m_length = rhs.m_length;
    p_data = new T[m_length];
    std::copy(rhs.p_data, rhs.p_data + m_length, this->p_data);
  }

  /// move constructor
  template <typename T>
  ndarray<T>::ndarray(ndarray<T> &&rhs)
  {
    m_nDim = rhs.m_nDim;
    rhs.m_nDim = 1;
    m_length = rhs.m_length;
    rhs.m_length = 0;

    p_shape = rhs.p_shape;
    rhs.p_shape = nullptr;
    p_offset = rhs.p_offset;
    rhs.p_offset = nullptr;
    p_data = rhs.p_data;
    rhs.p_data = nullptr;
  }

  // assignment

  template <typename T>
  ndarray<T> &ndarray<T>::operator=(const ndarray<T> &rhs)
  {
    if (this == &rhs)
    {
      return (*this);
    }
    else
    {
      delete[] p_data;
      delete[] p_shape;
      delete[] p_offset;

      m_nDim = rhs.m_nDim;
      p_shape = new uint32_t[m_nDim];
      std::copy(rhs.p_shape, rhs.p_shape + m_nDim, this->p_shape);
      p_offset = new uint32_t[m_nDim];
      std::copy(rhs.p_offset, rhs.p_offset + m_nDim, this->p_offset);
      m_length = rhs.m_length;
      p_data = new T[m_length];
      std::copy(rhs.p_data, rhs.p_data + m_length, this->p_data);
      return (*this);
    }
  }

  /// move assignment
  template <typename T>
  ndarray<T> &ndarray<T>::operator=(ndarray<T> &&rhs)
  {
    delete[] p_data;
    delete[] p_shape;
    delete[] p_offset;
    m_nDim = rhs.m_nDim;
    rhs.m_nDim = 1;
    m_length = rhs.m_length;
    rhs.m_length = 0;

    p_shape = rhs.p_shape;
    rhs.p_shape = nullptr;
    p_offset=rhs.p_offset;
    rhs.p_offset=nullptr;
    p_data = rhs.p_data;
    rhs.p_data = nullptr;
    return (*this);
  }

  template <typename T>
  ndarray<T> &ndarray<T>::operator=(const T &val)
  {
    std::fill_n(this->p_data, m_length, val);
    return (*this);
  }
  // destructor

  template <typename T>
  ndarray<T>::~ndarray()
  {
    delete[] p_data;
    delete[] p_shape;
    delete[] p_offset;
  }

  // #### methods ####

  // Setup

  template <typename T>
  void ndarray<T>::Setup(std::initializer_list<uint32_t> list)
  {
    // delete previous p_data
    delete[] p_shape;
    delete[] p_data;
    delete[] p_offset;

    // store dimension array
    m_nDim = list.size();
    p_shape = new uint32_t[m_nDim];

    uint32_t i = 0;
    for (auto &l : list)
      p_shape[i++] = l;

    CalcLength();
    CalcOffset();
    p_data = new T[m_length];
  }

  template <typename T>
  void ndarray<T>::Setup(const ndarray<uint32_t> &list)
  {
    // delete previous p_data
    delete[] p_shape;
    delete[] p_data;
    delete[] p_offset;
    // store dimension array
    m_nDim = list.GetLength();
    p_shape = new uint32_t[m_nDim];

    uint32_t i = 0;
    for (auto &l : list)
      p_shape[i++] = l;

    CalcLength();
    CalcOffset();
    p_data = new T[m_length];
  }

  // 1-D Setup
  template <typename T>
  void ndarray<T>::Setup(uint32_t size)
  {
    // delete previous p_data
    delete[] p_shape;
    delete[] p_data;
    delete[] p_offset;
    // store dimension array
    m_nDim = 1;
    p_shape = new uint32_t[m_nDim];
    p_shape[0] = size;
    CalcLength();
    CalcOffset();
    p_data = new T[m_length];
  }

  template <typename T>
  void ndarray<T>::Clear()
  {
    delete[] p_data;
    delete[] p_shape;
    delete[] p_offset;
    p_data = nullptr;
    p_shape = nullptr;
    p_offset = nullptr;
    m_nDim = 1;
    m_length = 0;
  }

  template <typename T>
  inline T &ndarray<T>::operator()(uint32_t idx)
  {
#ifdef _DEBUG
    if (idx >= m_length)
      FATAL_ERROR("ndarray out of bound");
#endif
    return p_data[idx];
  }

  template <typename T>
  inline const T &ndarray<T>::operator()(uint32_t idx) const
  {
#ifdef _DEBUG
    if (idx >= m_length)
      FATAL_ERROR("ndarray out of bound");
#endif
    return p_data[idx];
  }

  template <typename T>
  inline T &ndarray<T>::operator()(std::initializer_list<uint32_t> list)
  {
#ifdef _DEBUG
    ASSERT(list.size() == m_nDim, "Unmatched ndarray dimension");
    for (uint32_t i = 0; i < list.size(); i++)
      ASSERT(list.begin()[i] < p_shape[i], "ndarray access out of bound");
#endif
    uint32_t idx;
    idx = std::inner_product(list.begin(), list.end(), p_offset, 0);
    return p_data[idx];
  }

  template <typename T>
  inline const T &ndarray<T>::operator()(std::initializer_list<uint32_t> list) const
  {
#ifdef _DEBUG
    ASSERT(list.size() == m_nDim, "Unmatched ndarray dimension");
    for (uint32_t i = 0; i < list.size(); i++)
      ASSERT(list.begin()[i] < p_shape[i], "ndarray access out of bound");
#endif
    uint32_t idx;
    idx = std::inner_product(list.begin(), list.end(), p_offset, 0);
    return p_data[idx];
  }

  // return pointer

  template <typename T>
  inline T *ndarray<T>::GetPointer(uint32_t idx)
  {
    return p_data + idx;
  }

  template <typename T>
  inline const T *ndarray<T>::GetPointer(uint32_t idx) const
  {
    return p_data + idx;
  }

  template <typename T>
  inline T *ndarray<T>::GetPointer(std::initializer_list<uint32_t> list)
  {
    if (!p_data)
      return nullptr;
#ifdef _DEBUG
    ASSERT(list.size() == m_nDim, "Unmatched ndarray dimension");
#endif
    uint32_t idx;
    idx = std::inner_product(list.begin(), list.end(), p_offset, 0);
    return p_data + idx;
  }

  template <typename T>
  inline const T *ndarray<T>::GetPointer(std::initializer_list<uint32_t> list) const
  {
    if (!p_data)
      return nullptr;
#ifdef _DEBUG
    ASSERT(list.size() == m_nDim, "Unmatched ndarray dimension");
#endif
    uint32_t idx;
    idx = std::inner_product(list.begin(), list.end(), p_offset, 0);
    return p_data + idx;
  }

  // obtain dimension

  template <typename T>
  inline uint32_t ndarray<T>::GetDim(uint32_t n) const
  {
    if (n < m_nDim)
      return p_shape[n];
    else
      return 0;
  }

  template <typename T>
  ndarray<uint32_t> ndarray<T>::GetShape() const
  {
    ndarray<uint32_t> out_shape(m_nDim);
    if (m_length == 0)
      out_shape = 0;
    else
      std::copy_n(p_shape, m_nDim, out_shape.begin());
    return out_shape;
  }

  template <typename T>
  ndarray<uint32_t> ndarray<T>::GetIndex(uint32_t idx) const
  {
    uint32_t dim_bf = 1;
    ndarray<uint32_t> out_idx(m_nDim);
    for (uint32_t i = 0; i < m_nDim; i++)
    {
      out_idx(i) = idx / dim_bf;
      dim_bf *= p_shape[i];
      out_idx(i) = out_idx(i) % p_shape[i];
    }
    return out_idx;
  }

  template <typename T>
  inline uint32_t ndarray<T>::GetNumDim() const
  {
    return m_nDim;
  }

  template <typename T>
  void ndarray<T>::CalcLength(void)
  {
    m_length = 1;
    for (uint32_t i = 0; i < m_nDim; i++)
      m_length *= p_shape[i];
  }

  template <typename T>
  void ndarray<T>::CalcOffset(void)
  {
    p_offset = new uint32_t[m_nDim];
    uint32_t acc = 1;
    for (uint32_t i = 0; i < m_nDim; i++)
    {
      p_offset[i] = acc;
      acc *= p_shape[i];
    }
  }

  template <typename T>
  inline uint32_t ndarray<T>::GetLength(void) const
  {
    return m_length;
  }
  // method to calculate maximum value of ndarray
  // Template specialization
  template <typename T>
  T ndarray<T>::GetMax(void) const
  {
    return *std::max_element(p_data, p_data + m_length);
  }

  // method to calculate minimum value of ndarray
  // Template specialization
  template <typename T>
  inline T ndarray<T>::GetMin(void) const
  {
    return *std::min_element(p_data, p_data + m_length);
  }

  template <typename T>
  void ndarray<T>::Reshape(std::initializer_list<uint32_t> list)
  {

    uint32_t i = 0;

    if (m_nDim != list.size())
    {
      m_nDim = list.size();
      // delete p_shape
      delete[] p_shape;
      p_shape = new uint32_t[m_nDim];
    }

#ifdef _DEBUG
    uint32_t acc = 1;
#endif

    for (auto &l : list)
    {
#ifdef _DEBUG
      acc *= l;
#endif
      p_shape[i++] = l;
    }

    delete[] p_offset;
    CalcOffset();

#ifdef _DEBUG
    ASSERT (acc == m_length,"Total number of element doesn't agree");
#endif
  }

  //-------------friend---------------------
  // output
  template <typename U>
  std::ostream &operator<<(std::ostream &out, const ndarray<U> &s)
  {
    if (s.m_length == 0)
      return out;
    if (s.m_nDim == 1) // 1-D output
    {
      for (uint32_t i = 0; i < s.p_shape[0]; i++)
        out << std::setprecision(4) << std::setw(15) << s(i);
      out << std::endl;
    }
    else if (s.m_nDim == 2) // 2d output
    {
      for (uint32_t i = 0; i < s.p_shape[0]; i++)
      {
        for (uint32_t j = 0; j < s.p_shape[1]; j++)
        {
          out << std::setprecision(4) << std::setw(15) << s({i, j});
        }
        out << std::endl;
      }
    }
    else if (s.m_nDim == 3) // 3d output
    {
      for (uint32_t k = 0; k < s.p_shape[2]; k++)
      {
        out << "slice: (:,:," << k << ")" << std::endl;
        for (uint32_t i = 0; i < s.p_shape[0]; i++)
        {
          for (uint32_t j = 0; j < s.p_shape[1]; j++)
          {
            out << std::setprecision(4) << std::setw(15) << s({i, j, k});
          }
          out << std::endl;
        }
      }
    }
    else
    {
      std::cerr << "ERROR: Output not supported" << std::endl;
      exit(EXIT_FAILURE);
    }

    return out;
  }
}