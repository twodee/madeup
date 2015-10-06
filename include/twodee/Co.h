#ifndef CO_H
#define CO_H

namespace td {

/* ------------------------------------------------------------------------- */

template<typename T> class Co {
  public:
    Co();
    Co(T *src);
    template<typename U> Co(Co<U> src);
    Co(const Co<T>& other);

    ~Co();

    T *operator->();
    const T *operator->() const;
    T &operator*();
    const T &operator*() const;

    bool IsNull() const;

    T *GetPointer();
    const T *GetPointer() const;

    Co<T> &operator=(const Co<T>& other);

    int GetReferenceCount() {
      return *nreferences;
    }

  public:
    void Withdraw();

    T *p;
    int *nreferences;
};

/* ------------------------------------------------------------------------- */

template<typename T>
Co<T>::Co() :
  p(NULL),
  nreferences(NULL) {
}

/* ------------------------------------------------------------------------- */

template<typename T>
Co<T>::Co(T *src) :
  p(src),
  nreferences(new int(1)) {
}

/* ------------------------------------------------------------------------- */

template<typename T>
template<typename U> Co<T>::Co(Co<U> src) :
  p((T *) src.GetPointer()),
  nreferences(src.nreferences) {
  if (!IsNull()) {
    ++*nreferences;
  }
}

/* ------------------------------------------------------------------------- */

template<typename T>
Co<T>::Co(const Co<T>& other) :
  p(other.p),
  nreferences(other.nreferences) {
  if (!IsNull()) {
    ++*nreferences;
  }
}

/* ------------------------------------------------------------------------- */

template<typename T>
Co<T>::~Co() {
  Withdraw();
}

/* ------------------------------------------------------------------------- */

template<typename T>
void Co<T>::Withdraw() {
  if (!IsNull()) {
    --*nreferences; 
    if (0 == *nreferences) {
      delete p;
      delete nreferences;
      p = NULL;
      nreferences = NULL;
    }
  }
}

/* ------------------------------------------------------------------------- */

template<typename T>
T *Co<T>::operator->() {
  return p;
}

/* ------------------------------------------------------------------------- */

template<typename T>
const T *Co<T>::operator->() const {
  return p;
}

/* ------------------------------------------------------------------------- */

template<typename T>
T &Co<T>::operator*() {
  return *p;
}

/* ------------------------------------------------------------------------- */

template<typename T>
const T &Co<T>::operator*() const {
  return *p;
}

/* ------------------------------------------------------------------------- */

template<typename T>
Co<T> &Co<T>::operator=(const Co<T>& other) {
  // a = b
  if (this != &other) {
    Withdraw(); // a's getting overwritten, loses a ref
    p = other.p;
    nreferences = other.nreferences;
    ++*nreferences;
  } 

  return *this;
}

/* ------------------------------------------------------------------------- */

template<typename T>
T *Co<T>::GetPointer() {
  return p;
}

/* ------------------------------------------------------------------------- */

template<typename T>
const T *Co<T>::GetPointer() const {
  return p;
}

/* ------------------------------------------------------------------------- */

template<typename T>
bool Co<T>::IsNull() const {
  return p == NULL;
}

/* ------------------------------------------------------------------------- */

}

#endif
