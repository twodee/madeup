#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>

namespace td {

/* ------------------------------------------------------------------------- */

template<typename T> class MinHeap {
  public:
    MinHeap();

    bool IsEmpty() const;
    int Size() const;
    T *operator[](int i) const;

    void Add(T *new_item); 
    T *Remove();

    /* void Debug() { */
      /* for (int i = 0; i < items.size(); ++i) { */
        /* std::cout << "items[" << i << "]: " << items[i].i << std::endl; */
      /* } */
    /* } */

    void ReheapUp(int i);
    void ReheapDown(int i);

  private:

    std::vector<T *> items;
};

/* ------------------------------------------------------------------------- */

template<typename T>
MinHeap<T>::MinHeap() :
  items() {
}

/* ------------------------------------------------------------------------- */

template<typename T>
bool MinHeap<T>::IsEmpty() const {
  return items.size() == 0;
}

/* ------------------------------------------------------------------------- */

template<typename T>
int MinHeap<T>::Size() const {
  return items.size();
}

/* ------------------------------------------------------------------------- */

template<typename T>
T *MinHeap<T>::operator[](int i) const {
  assert(i >= 0 && i < items.size());
  return items[i];
}

/* ------------------------------------------------------------------------- */

template<typename T>
void MinHeap<T>::Add(T *new_item) {
  items.push_back(new_item);  
  items[items.size() - 1]->heap_index = items.size() - 1;
  ReheapUp(items.size() - 1);
}

/* ------------------------------------------------------------------------- */
 
template<typename T>
T *MinHeap<T>::Remove() {
  assert(!this->IsEmpty());

  typename std::vector<T *>::iterator first = items.begin();
  T *to_return = *first;
  typename std::vector<T *>::iterator last = items.end() - 1;
  T *to_move = *last;
  items.erase(last);

  if (!this->IsEmpty()) {
    items[0] = to_move;
    ReheapDown(0);
  } 

  return to_return;
}

/* ------------------------------------------------------------------------- */

template<typename T>
void MinHeap<T>::ReheapUp(int ichild) {
  T *child = (*this)[ichild];
  int iparent = (ichild - 1) / 2;

  // If the item has no parent or is greater than its parent, we're done.
  if (iparent == ichild || Compare(*child, *((*this)[iparent])) > 0) {
    return;
  }

  // Otherwise, let's keep pushing greater parents down the tree.
  while (iparent != ichild && Compare(*child, *(*this)[iparent]) < 0) {
    // Move the greater parent into the child's spot.
    items[ichild] = (*this)[iparent];
    items[ichild]->heap_index = ichild;

    // Move a level up.
    ichild = iparent;
    iparent = (ichild - 1) / 2;
  }

  items[ichild] = child;
  items[ichild]->heap_index = ichild;
}

/* ------------------------------------------------------------------------- */

template<typename T>
void MinHeap<T>::ReheapDown(int iparent) {
  T *parent = (*this)[iparent];
  int ichild = 2 * iparent + 1;

  // Descend down the tree until the maximum level.
  while (ichild < this->Size()) {
    // Find the smaller of the two children. We've already guessed the left to
    // be smaller, but we try to oust it by the right.
    if (ichild + 1 < this->Size() && Compare(*items[ichild + 1], *items[ichild]) < 0) {
      ++ichild;
    }

    // If the parent maintains the heap property, we're done.
    if (Compare(*parent, *items[ichild]) <= 0) {
      break;
    }

    // Otherwise, if the parent is greater than the smaller child, we raise up
    // the child a level and check further levels.
    items[iparent] = items[ichild];
    items[iparent]->heap_index = iparent;
    iparent = ichild;
    ichild = 2 * iparent + 1;
  }

  // Drop the starting node it its rightful place.
  items[iparent] = parent;
  items[iparent]->heap_index = iparent;
}

/* ------------------------------------------------------------------------- */

}

#endif
