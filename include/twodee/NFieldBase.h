#ifndef NFIELDBASE_H
#define NFIELDBASE_H

#include <sstream>

#include "FieldType.h"
#include "QVector.h"

using std::stringstream;

namespace td {

/* ------------------------------------------------------------------------- */

template<class T, int ndims> class NField;

/* ------------------------------------------------------------------------- */

/**
 Represents an n-dimensional field of data. This abstract class does not impose
 any particular data type, but manages only typeless information like size,
 data labels, etc.
 */
template<int ndims> class NFieldBase {
  public:
    /**
     Creates a new field with no spatial extent.
     */
    NFieldBase();

    /**
     Creates a new field with the specified dimensions and the specified
     number of channels.
     @param dims Dimensions of field
     @param nchannels Number of channels (variables) per element
     */
    NFieldBase(const QVector<int, ndims>& dims,
               int nchannels = 1);

    /**
     Frees resources consumed by this field.
     */
    virtual ~NFieldBase();

    /**
     Gets the field's dimensions.
     @returns Dimensions of field
     */
    const QVector<int, ndims>& GetDimensions() const;

    /**
     Get the number of channels in field.
     @return Number of channels in field
     */
    int GetChannelCount() const;

    /**
     Get the number of elements in field.
     @return Number of elements in field
     */
    int GetElementCount() const;

    /**
     Get the number of values in field.
     @return Number of values in field
     */
    int GetValueCount() const;

    /**
     Normalize the field such the range [min, max] linearly maps to [type-min,
     type-max]. If the minima and maxima had not previously been computed, they
     are computed now on a per-channel basis.
     */
    virtual void Normalize() = 0;

    /**
     Compute the minima and maxima of the field. If <code>per_channel</code>
     is true, the minimum and maximum are computed within each channel
     separately. If false, a global minimum and maximum is computed across all
     channels. Any previously forced minima or maxima are lost.
     @param per_channel If false, compute multichannel minimum and maximum.
     */
    virtual void ComputeMinMax(bool per_channel = true) = 0;

    const std::string& GetChannelName(int i) const;

    static NField<float, ndims> *ToFloat(NFieldBase<ndims> *base);

    const std::string& GetDimensionName(int d) const;
    float GetDimensionMinimum(int d) const;
    float GetDimensionMaximum(int d) const;
    const QVector<float, ndims>& GetDimensionMinima() const;
    const QVector<float, ndims>& GetDimensionMaxima() const;
    const QVector<float, ndims>& GetDimensionSpans() const;

    static const int CASE_TO_EDGE[];
    static const int CASE_TO_PRIMITIVES[1 << ((1 << ndims))][((ndims - 1) * (ndims - 1) + 1) * ndims + 1];

  protected:
    /**
     Alter dimensions and optionally the number of channels in this field.  Any
     previous data is overwritten, but not explicitly deallocated. (Use Wipe to
     do that.) This method is meant to be used internally only, otherwise
     memory leaks may ensue. Don't ever make it public, Chris.
     @param dims New dimensions of field
     @param nchannels New number of channels, -1 to retain current number
     @see Wipe
     */
    virtual void Resize(const QVector<int, ndims>& dims,
                        int nchannels = -1);

    /**
     Zero out this field. Free its resources.
     */
    virtual void Wipe();

    /** Spatial dimensions */
    QVector<int, ndims> dims;

    /** Number of channels (values per cell) in field */
    int nchannels;

    /** Number of elements (addressable cells) in field */
    int nelements;

    /** Total number of values in field */
    int nvalues;

    /** Whether user forced min on channel */
    bool *is_min_forced;

    /** Whether used forced max on channel */
    bool *is_max_forced;

    /** Descriptive names of channels */
    std::string *channel_names;
    
    std::string dimension_names[ndims];
    QVector<float, ndims> dimension_minima;
    QVector<float, ndims> dimension_maxima;
    QVector<float, ndims> dimension_spans;

  private:
    /**
     Factors out code common to constructors.
    */
    void Initialize();
};

/* ------------------------------------------------------------------------- */

template<int ndims>
NFieldBase<ndims>::NFieldBase()
  : dims(0),
    nchannels(0),
    nelements(0),
    nvalues(0) {
  Initialize();

  for (int d = 0; d < ndims; ++d) {
    dimension_minima[d] = 0.0f;
    dimension_maxima[d] = 0.0f;
  }
  dimension_spans = dimension_maxima - dimension_minima;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
NFieldBase<ndims>::NFieldBase(const QVector<int, ndims>& dims,
                              int nchannels) {
  Initialize();
  Resize(dims, nchannels);

  for (int d = 0; d < ndims; ++d) {
    dimension_minima[d] = 0.0f;
    dimension_maxima[d] = dims[d];
  }
  dimension_spans = dimension_maxima - dimension_minima;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
NFieldBase<ndims>::~NFieldBase() {
  Wipe();
}

/* ------------------------------------------------------------------------- */

template<int ndims>
void NFieldBase<ndims>::Initialize() {
  is_min_forced = NULL;
  is_max_forced = NULL;
  channel_names = NULL;

  // Name first three dimensions x, y, and z by default.
  for (int d = 0; d < 3 && d < ndims; ++d) {
    stringstream ss;
    ss << 'x' + d;
    dimension_names[d] = ss.str();
  }

  // Any remaining dimensions are named dim3, dim4, ...
  for (int d = 3; d < ndims; ++d) {
    stringstream ss;
    ss << "dim" << d;
    dimension_names[d] = ss.str();
  }
}

/* ------------------------------------------------------------------------- */

template<int ndims>
const QVector<int, ndims>& NFieldBase<ndims>::GetDimensions() const {
  return dims;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
int NFieldBase<ndims>::GetChannelCount() const {
  return nchannels;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
int NFieldBase<ndims>::GetElementCount() const {
  return nelements;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
int NFieldBase<ndims>::GetValueCount() const {
  return nvalues;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
void NFieldBase<ndims>::Wipe() {
  dims = QVector<int, ndims>(0); 
  nchannels = 0; 
  nelements = 0;
  nvalues = 0;

  delete[] is_min_forced;
  delete[] is_max_forced;
  delete[] channel_names;
  is_min_forced = NULL;
  is_max_forced = NULL;
  channel_names = NULL;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
void NFieldBase<ndims>::Resize(const QVector<int, ndims>& dims,
                               int nchannels) {
  Wipe();
  this->dims = dims; 
  if (nchannels >= 0) {
    this->nchannels = nchannels; 
  }
  nelements = dims.SelfProduct();
  nvalues = nelements * this->nchannels;
  channel_names = new std::string[this->nchannels];
  for (int i = 0; i < this->nchannels; ++i) {
    std::stringstream ss;
    ss << "channel" << (char) ('A' + i);
    channel_names[i] = ss.str();
  }
}

/* ------------------------------------------------------------------------- */

template<int ndims>
const std::string& NFieldBase<ndims>::GetChannelName(int i) const {
  assert(i >= 0 && i < nchannels);
  return channel_names[i];
}

/* ------------------------------------------------------------------------- */

template<int ndims>
NField<float, ndims> *NFieldBase<ndims>::ToFloat(NFieldBase<ndims> *base) {
  NField<float, ndims> *field;
  if (base->GetType() == FieldType::FLOAT) {
     field = dynamic_cast<NField<float, ndims> *>(base);
  } else {
     field = dynamic_cast<NField<float, ndims> *>(base->Cast(FieldType::FLOAT));
     delete base;
  }
  return field;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
const std::string& NFieldBase<ndims>::GetDimensionName(int d) const {
  assert(0 <= d && d < ndims);
  return dimension_names[d];  
}

/* ------------------------------------------------------------------------- */

template<int ndims>
float NFieldBase<ndims>::GetDimensionMinimum(int d) const {
  return dimension_minima[d];
}

/* ------------------------------------------------------------------------- */

template<int ndims>
float NFieldBase<ndims>::GetDimensionMaximum(int d) const {
  return dimension_maxima[d];
}

/* ------------------------------------------------------------------------- */

template<int ndims>
const QVector<float, ndims>& NFieldBase<ndims>::GetDimensionMinima() const {
  return dimension_minima;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
const QVector<float, ndims>& NFieldBase<ndims>::GetDimensionMaxima() const {
  return dimension_maxima;
}

/* ------------------------------------------------------------------------- */

template<int ndims>
const QVector<float, ndims>& NFieldBase<ndims>::GetDimensionSpans() const {
  return dimension_spans;
}

/* ------------------------------------------------------------------------- */

}

#endif
