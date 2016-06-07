#ifndef NFIELD_H
#define NFIELD_H

#include <fstream>

#include "twodee/Configuration.h"

#ifdef USE_MAGICK
#include <Magick++.h>
#endif

#include "twodee/ContourMarcher.h"
#include "twodee/FieldTraits.h"
#include "twodee/LineSegment.h"
#include "twodee/MessagedException.h"
#include "twodee/NFieldBase.h"
#include "twodee/NFieldIterator.h"
#include "twodee/Scanner.h"
#include "twodee/Utilities.h"

namespace td {

/* ------------------------------------------------------------------------- */

template<class T, int ndims> class NField;

template<class T, int ndims>
std::ostream& operator<<(std::ostream& out,
                         const td::NField<T, ndims>& field);

/* ------------------------------------------------------------------------- */

/**
 A typed n-dimensional field.
 */
template<class T, int ndims> class NField : public NFieldBase<ndims> {
  friend std::ostream& operator<<<>(std::ostream& out,
                                    const td::NField<T, ndims>& field);

  public:
    /**
     Creates a new field containing no elements.
     */
    NField();

    /**
     Creates a new field with the specified dimensions and with the specified
     number of channels.
     @param dims Dimensions of field
     @param nchannels Number of channels in field
     */
    NField(const QVector<int, ndims>& dims,
           int nchannels = 1);

    /**
     Loads a field from the specified path.
     @param path Path to field file
     */
    NField(const std::string& path);

    /**
     Frees resources consumed by this field.
     */
    ~NField();

#ifdef USE_MAGICK
    /**
     Loads image stored in the specified file into this field.
     @param path File containing field
     */
    void ReadImage(const std::string& path);

    /**
     Writes the field to the specified image file in a format determined by
     the file's extension.
     @param path Path to file
     @param channel_id ID of channel to write, -1 for all
     @param flip_red_blue True to write BGR or BGRA
     */
    void WriteImage(const std::string& path,
                    int channel_id = -1,
                    bool flip_red_blue = false) const;
#endif

    /**
     Read field from a plain text file with the following format:
     <pre>
     F20
     ndims
     width [height [depth [...]]]
     nchannels
     channel-0-name nfactors
     channel-1-name nfactors
     channel-2-name nfactors
     observations
     </pre>
     @param path File containing field
     */
    void ReadF20(const std::string& path);

    /**
     Read field from a plain text file with the following format:
     <pre>
     F20
     ndims
     name0
     min0 max0 nsamples0
     name1
     min1 max1 nsamples1
     ...
     namen
     minn maxn nsamplesn
     nchannels
     channel-0-name nfactors
     channel-1-name nfactors
     channel-2-name nfactors
     observations
     </pre>
     @param path File containing field
     */
    void ReadF21(const std::string& path);

    /**
     Read single channel 3-D field from a binary file with the following
     format:
     <pre>
     [width][height][depth] (all as ints)
     observations
     </pre>
     @param path File containing field
     */
    void ReadF0(const std::string& path);
    void ReadF4(const std::string& path);

    /**
     Writes the field to the specified file in the format inferred by the
     file's extension. For example, to write in the F20 format, the file's
     extension must be ".f20".
     @param path Path to file, whose extension is used to determine format
     */
    void Write(const std::string& path) const;

    /**
     Writes the field to the specified file in the F20 format.
     @param path Path to file
     */
    void WriteF20(const std::string& path) const;

    /**
     Get the data buffer for this field.
     @return Field's data buffer
     */
    const T *GetData() const;

    /**
     Gets the element at the specified coordinate.
     @param c Position of element
     */
    T *operator()(const QVector<int, ndims>& c);

    /**
     Gets the element at the specified coordinate.
     @param c Position of element
     */
    const T *operator()(const QVector<int, ndims>& c) const;

    /**
     Gets the specified element.
     @param element Serial index of element
     */
    T *operator()(int element);

    /**
     Gets the specified element.
     @param element Serial index of element
     */
    const T *operator()(int element) const;


    /**
     Linearly interpolate the value at the specified position in the specified
     channel.
     @param position Position at which value is desired, likely non-integral
     @param channel_id Channel whose values are interpolated
     */
    template<class U>
    T LinearInterpolate(const QVector<U, ndims>& position,
                        int channel_id = 0) const;

    /**
     Get the minimum for the specified channel. Assumes statistics have been
     computed.
     @param channel Channel whose minimum is to be retrieved
     @see ComputeMinMax
     */
    T GetMinimum(int channel) const;

    /**
     Get the maximum for the specified channel. Assumes statistics have been
     computed previously.
     @param channel Channel whose maximum is to be retrieved
     @see ComputeMinMax
     */
    T GetMaximum(int channel) const;

    /**
     Draws a line from the start to end point.
     @param
     @param
     @param
     */
    void DrawLine(const QVector<int, ndims>& start,
                  const QVector<int, ndims>& end,
                  T intensity,
                  int channel_id = 0);

    /**
     Sets all values in the specified channel to the specified value.
     @param clear_value Value to clear to
     @param channel_id Channel to clear, -1 for all
     */
    void Clear(T clear_value = 0,
               int channel_id = 0);

    void Log(int channel_id = 0);

    /**
     Extracts a solitary channel from the field. Caller must free buffer when
     it is no longer needed.
     @param channel_id Channel to extract
     */
    T *ExtractChannel(int channel_id) const;

    /**
     Gets a new field built from the channels of this field. Call must release
     field when it is no longer needed.
     @param nchannels Number of channels enumerated in <code>channels</code>
     @param channels Mapping of source channels to destination channels, -1 for zeroed channel
     @return New field with reordered channels
     */
    NField<T, ndims> *Rechannel(int nchannels,
                                int *channels) const;

    void ForceMin(T min, int channel_id = 0);
    void ForceMax(T max, int channel_id = 0);

    /**
     Gets a contour fitted around the specified intensity.
     */
    void GetIsocontour(T isovalue,
                       int& nprimitives,
                       float*& positions,
                       int channel = 0);

    /**
     Gets a portion of this field.
     @param from The starting location of the subfield, inclusive.
     @param to The end location of the subfield, inclusive.
     @return The specified portion of the field, to be freed by the caller.
     */
    NField<T, ndims> *GetSubfield(const QVector<int, ndims>& from, const QVector<int, ndims>& to) const;

    NField<T, ndims - 1> *GetSlice(const QVector<int, ndims - 1>& axis, int slice_of, int slice_id) const;

    NField<T, ndims> *GetSeamless() const;

    void Transpose();

    NField<T, ndims - 1> *Flatten() const;

    T GetUnnormalizedValue(const QVector<int, ndims>& i, int ci) const;
                               

    /* INHERITED ----------------------------------------------------------- */
    void Normalize();
    void ComputeMinMax(bool per_channel = true);

  protected:
    /* INHERITED ----------------------------------------------------------- */
    void Wipe();
    void Resize(const QVector<int, ndims>& dims,
                int nchannels = -1);

  private:
    /**
     Factors out code common to constructors.
    */
    void Initialize();

    /** Values of field, stored with x increasing fastest */
    T *data;

    /** The minimum values for each channel */
    T *minima;

    /** The maximum values for each channel */
    T *maxima;

    /** The spans between minima and maxima */
    T *spans;

    T *prenormalized_minima;
    T *prenormalized_maxima;
    T *prenormalized_spans;
};

/* ------------------------------------------------------------------------- */

template<int ndims>
int VecToIdx(const QVector<int, ndims>& dims,
             const QVector<int, ndims>& c) {
  int idx = c[ndims - 1];
  for (int d = ndims - 2; d >= 0; --d) {
    idx = c[d] + dims[d] * idx;
  }
  return idx;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims>::NField()
  : NFieldBase<ndims>(),
    data(NULL) {
  Initialize();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims>::NField(const QVector<int, ndims>& dims,
                         int nchannels)
  : NFieldBase<ndims>(dims, nchannels),
    data(new T[this->GetValueCount()]) {
  Initialize();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims>::NField(const std::string& path)
  : data(NULL) {
  Initialize();

  std::string ext = path.substr(path.find_last_of('.')); 

  if (ext.compare(".f20") == 0) {
    ReadF20(path);
  } else if (ext.compare(".f21") == 0) {
    ReadF21(path);
  } else if (ext.compare(".f0") == 0) {
    ReadF0(path);
  } else if (ext.compare(".f4") == 0) {
    ReadF4(path);
  }

#ifdef USE_MAGICK
  else if (ext.compare(".png") == 0 ||
           ext.compare(".bmp") == 0 ||
           ext.compare(".ppm") == 0 ||
           ext.compare(".pgm") == 0 ||
           ext.compare(".jpg") == 0) {
    ReadImage(path);
  }
#endif

  else {
    throw MessagedException(std::string("illegal field format"));
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Initialize() {
  minima = NULL;
  maxima = NULL;
  spans = NULL;
  prenormalized_minima = NULL;
  prenormalized_maxima = NULL;
  prenormalized_spans = NULL;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims>::~NField() {
  Wipe();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Wipe() {
  NFieldBase<ndims>::Wipe();
  delete[] data;
  delete[] minima;
  delete[] maxima;
  delete[] spans;
  delete[] prenormalized_minima;
  delete[] prenormalized_maxima;
  delete[] prenormalized_spans;
  data = NULL;
  minima = NULL;
  maxima = NULL;
  spans = NULL;
  prenormalized_minima = NULL;
  prenormalized_maxima = NULL;
  prenormalized_spans = NULL;
}

/* ------------------------------------------------------------------------- */

#ifdef USE_MAGICK
template<class T, int ndims>
void NField<T, ndims>::ReadImage(const std::string& path) {
  assert(ndims >= 2);

  try {
    Magick::Image image;
    image.read(path);
    int magick_nchannels;

    if (image.type() == Magick::GrayscaleType ||
        image.type() == Magick::BilevelType) {
      magick_nchannels = 1;
    }

    else if (image.type() == Magick::GrayscaleMatteType) {
      magick_nchannels = 2;
    }

    else if (image.type() == Magick::PaletteType ||
             image.type() == Magick::TrueColorType) {
      magick_nchannels = 3;
    }

    else {
      magick_nchannels = 4;
    } 

    // Get new dimensions. X and Y change for sure. Others may need 1'd out,
    // since we may be loading a 2-D field into a >2-D field.
    QVector<int, ndims> new_dims(1);
    new_dims[0] = image.columns();
    new_dims[1] = image.rows();

    // Now we can resize.
    Resize(new_dims, magick_nchannels);

    // ImageMagick has a different notion of the origin than I do.
    image.flip();

    // We read in from ImageMagick byte data.
    unsigned char *uc_data = new unsigned char[this->nvalues];
    image.getConstPixels(0, 0, image.columns(), image.rows());
    image.depth(sizeof(unsigned char) * 8);

    if (this->nchannels == 1) {
      image.writePixels(Magick::RedQuantum, uc_data);
    } else if (this->nchannels == 2) {
      image.writePixels(Magick::GrayAlphaQuantum, uc_data);
    } else if (this->nchannels == 3) {
      image.writePixels(Magick::RGBQuantum, uc_data);
    } else if (this->nchannels == 4) {
      image.writePixels(Magick::RGBAQuantum, uc_data);
    }

    // Now we 
    Utilities::ConvertArray(data, uc_data, this->GetValueCount());
    Normalize();

    delete[] uc_data;
  } catch (Magick::Exception& error) {
    throw MessagedException(std::string("magick exception"));
  } 
}
#endif

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ReadF20(const std::string& path) {
  Scanner in(path, Scanner::SOURCE_FILE);
  assert(in.Next().compare("F20") == 0);

  // Get dimensional extents
  int file_ndims = in.NextInt();
  QVector<int, ndims> new_dims(1);
  for (int i = 0; i < file_ndims; ++i) {
    new_dims[i] = in.NextInt();
    NFieldBase<ndims>::dimension_maxima[i] = new_dims[i];
  }

  NFieldBase<ndims>::dimension_spans = NFieldBase<ndims>::dimension_maxima - NFieldBase<ndims>::dimension_minima;

  // Get channel information
  int new_nchannels = in.NextInt();

  // Now we can resize
  Resize(new_dims, new_nchannels);

  for (int i = 0; i < new_nchannels; ++i) {
    NFieldBase<ndims>::channel_names[i] = in.Next();
    /*int nfactors =*/ in.NextInt();
  }
 
  NFieldIterator<ndims> c(this->GetDimensions());
  while (c.HasNext()) {
    c.Next();
    float *element = (*this)(c);
    for (int i = 0; i < this->nchannels; ++i) {
      element[i] = (T) in.NextFloat();
    }
  }

  in.Close();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ReadF21(const std::string& path) {
  Scanner in(path, Scanner::SOURCE_FILE);
  assert(in.Next().compare("F21") == 0);

  // Get dimensional extents
  int file_ndims = in.NextInt();
  in.NextLine(); // advance to next line

  // Get dimension metadata
  QVector<int, ndims> new_dims(1);
  for (int i = 0; i < file_ndims; ++i) {
    NFieldBase<ndims>::dimension_names[i] = in.NextLine();
    NFieldBase<ndims>::dimension_minima[i] = in.NextFloat();
    NFieldBase<ndims>::dimension_maxima[i] = in.NextFloat();
    new_dims[i] = in.NextInt();
    in.NextLine(); // advance to next line
  }

  NFieldBase<ndims>::dimension_spans = NFieldBase<ndims>::dimension_maxima - NFieldBase<ndims>::dimension_minima;

  // Get channel information
  int new_nchannels = in.NextInt();

  // Now we can resize
  Resize(new_dims, new_nchannels);

  for (int i = 0; i < new_nchannels; ++i) {
    NFieldBase<ndims>::channel_names[i] = in.Next();
    /*int nfactors =*/ in.NextInt();
  }
 
  NFieldIterator<ndims> c(this->GetDimensions());
  while (c.HasNext()) {
    c.Next();
    float *element = (*this)(c);
    for (int i = 0; i < this->nchannels; ++i) {
      element[i] = (T) in.NextFloat();
    }
  }

  in.Close();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ReadF0(const std::string& path) {
  assert(ndims == 3); // F0 is 3-D only

  std::ifstream in(path.c_str(), std::ios::binary | std::ios::in);

  // Get dimensions
  QVector<int, ndims> new_dims;
  /* for (int i = 2; i >= 0; --i) { */
  for (int i = 0; i < 3; ++i) {
    in.read((char *) &new_dims[i], sizeof(int));
  }
  Resize(new_dims, 1);

  // If we've got a byte-based volume, we can just read the data and use it
  // as is.
  if (FieldTraits<T>::type == FieldType::UNSIGNED_CHAR) {
    in.read((char *) data, this->GetValueCount());
  } 

  // For any other field type, we've got to convert the char data.
  else {
    unsigned char *voxels = new unsigned char[this->GetValueCount()];
    in.read((char *) voxels, this->GetValueCount());
    Utilities::ConvertArray(data, voxels, this->GetValueCount());
    delete[] voxels;
    voxels = NULL;
  }

  in.close();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ReadF4(const std::string& path) {
  assert(ndims == 3); // F0 is 3-D only

  std::ifstream in(path.c_str(), std::ios::binary | std::ios::in);

  // Get dimensions
  QVector<int, ndims> new_dims;
  for (int i = 2; i >= 0; --i) {
    in.read((char *) &new_dims[i], sizeof(int));
  }
  Resize(new_dims, 1);

  // If we've got a byte-based volume, we can just read the data and use it
  // as is.
  if (FieldTraits<T>::type == FieldType::FLOAT) {
    in.read((char *) data, this->GetValueCount() * sizeof(T));
  } 

  // For any other field type, we've got to convert the char data.
  else {
    float *voxels = new float[this->GetValueCount()];
    in.read((char *) voxels, this->GetValueCount());
    Utilities::ConvertArray(data, voxels, this->GetValueCount() * sizeof(float));
    delete[] voxels;
    voxels = NULL;
  }

  in.close();
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Write(const std::string& path) const {
  std::string ext = path.substr(path.find_last_of('.')); 

  if (ext.compare(".f20") == 0) {
    WriteF20(path);
  }

#ifdef USE_MAGICK
  else if (ext.compare(".jpg") == 0 ||
           ext.compare(".png") == 0 ||
           ext.compare(".pgm") == 0 ||
           ext.compare(".ppm") == 0 ||
           ext.compare(".bmp") == 0) {
    WriteImage(path);
  }
#endif

  else {
    throw MessagedException(std::string("illegal field format"));
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::WriteF20(const std::string& path) const {
  std::ofstream out(path.c_str());

  out.setf(std::ios::fixed);

  // Header
  out << "F20" << std::endl;
  out << ndims << std::endl;
  out << this->GetDimensions() << std::endl;
  out << this->GetChannelCount() << std::endl;

  // TODO handle factors!
 
  for (int i = 0; i < this->GetChannelCount(); ++i) {
    out << NFieldBase<ndims>::channel_names[i] << " " << 0 << std::endl;
  }

  // Data
  NFieldIterator<ndims> c(this->GetDimensions());
  while (c.HasNext()) {
    c.Next();
    const T *element = (*this)(c);
    out << element[0];
    for (int i = 1; i < this->GetChannelCount(); ++i) {
      out << " " << element[i]; 
    }
    out << std::endl;
  }

  out.close();
}

/* ------------------------------------------------------------------------- */

#ifdef USE_MAGICK
template<class T, int ndims>
void NField<T, ndims>::WriteImage(const std::string& path,
                                  int channel_id,
                                  bool flip_red_blue) const {
  // Only works for 2-D.
  assert(ndims == 2);

  // Either we've selected a channel or we have few enough to convert to image.
  assert(channel_id != -1 || this->GetChannelCount() <= 4);

  // Either we're not flipping channels, or we have red and blue to flip.
  assert(!flip_red_blue || (this->GetChannelCount() >= 3 && this->GetChannelCount() <= 4 && channel_id == -1));

  const char *labels[] = {
    "I",
    "RA",
    "RGB",
    "RGBA",
    "BGR",
    "BGRA"
  };

  const char *label;
  if (channel_id != -1) {
    label = labels[0];
  } else if (flip_red_blue) {
    label = labels[2 + this->GetChannelCount() - 1];
  } else {
    label = labels[this->GetChannelCount() - 1];
  }
  
  const QVector<int, ndims>& dims = this->GetDimensions();
  Magick::Image image;

  // If we're selecting out one channel and we have many to choose from, we've got to pull out
  // a contiguous chunk of memory. Otherwise, we just grab it all.
  // TODO untested
  if (channel_id != -1 && this->GetChannelCount() > 1) {
    T *buffer = this->ExtractChannel(channel_id);
    image = Magick::Image(dims[0], dims[1], label, FieldTraits<T>::magick_type, buffer);
    delete[] buffer;
  } else if (strcmp(label, "RA") == 0) {
    int channels[] = {0, 0, 0, 1};
    NField<T, ndims> *expanded = Rechannel(4, channels);
    image = Magick::Image(dims[0], dims[1], "RGBA", FieldTraits<T>::magick_type, expanded->GetData());
    delete expanded;
    /* image.type(MagickCore::GrayscaleMatteType); // not needed for some reason? */
  } else {
    image = Magick::Image(dims[0], dims[1], label, FieldTraits<T>::magick_type, this->GetData());
  }

  image.flip();
  image.write(path);
}
#endif

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const T *NField<T, ndims>::GetData() const {
  return data;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T *NField<T, ndims>::operator()(const QVector<int, ndims>& c) {
  for (int i = 0; i < ndims; ++i) {
    assert(c[i] >= 0 && c[i] < this->GetDimensions()[i]);
  }
  return &data[VecToIdx(this->dims, c) * this->nchannels];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const T *NField<T, ndims>::operator()(const QVector<int, ndims>& c) const {
  for (int i = 0; i < ndims; ++i) {
    assert(c[i] >= 0 && c[i] < this->GetDimensions()[i]);
  }
  return &data[VecToIdx(this->dims, c) * this->nchannels];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T *NField<T, ndims>::operator()(int element) {
  assert(element >= 0 && element < this->GetElementCount());
  return &data[element * this->nchannels];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
const T *NField<T, ndims>::operator()(int element) const {
  assert(element >= 0 && element < this->GetElementCount());
  return &data[element * this->nchannels];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ComputeMinMax(bool per_channel) {
  // Make space for the min/max.
  if (!minima) {
    minima = new T[this->nchannels];
  }

  if (!maxima) {
    maxima = new T[this->nchannels];
  }

  if (!NFieldBase<ndims>::is_min_forced) {
    NFieldBase<ndims>::is_min_forced = new bool[this->nchannels];
  }

  if (!NFieldBase<ndims>::is_max_forced) {
    NFieldBase<ndims>::is_max_forced = new bool[this->nchannels];
  }

  // Prime the min/max with the first observations.
  for (int ci = 0; ci < this->nchannels; ++ci) {
    minima[ci] = data[ci];
    maxima[ci] = data[ci];
    NFieldBase<ndims>::is_min_forced[ci] = false;
    NFieldBase<ndims>::is_max_forced[ci] = false;
  }

  // Go ahead and calculate per channel min/max initially.
  NFieldIterator<ndims> c(this->GetDimensions());
  while (c.HasNext()) {
    c.Next();
    const T *element = (*this)(c);
    for (int ci = 0; ci < this->nchannels; ++ci) {
      if (minima[ci] > element[ci]) {
        minima[ci] = element[ci];
      } else if (maxima[ci] < element[ci]) {
        maxima[ci] = element[ci];
      }
    }
  }

  // Now, if we wanted only the global minimum and maximum across all
  // channels, we find the minimum of the minima and the maximum of the
  // maxima and smear those across the minima and maxima list.
  if (!per_channel) {
    float global_minimum = minima[0];
    float global_maximum = maxima[0];
    for (int ci = 1; ci < this->nchannels; ++ci) {
      if (global_minimum > minima[ci]) {
        global_minimum = minima[ci];
      } else if (global_maximum < maxima[ci]) {
        global_maximum = maxima[ci];
      }
    }

    for (int ci = 0; ci < this->nchannels; ++ci) {
      minima[ci] = global_minimum;
      maxima[ci] = global_maximum;
    }
  }
 
  // With minima and maxima computed, we can find the span.
  if (!spans) {
    spans = new T[this->nchannels];
  }

  for (int ci = 0; ci < this->nchannels; ++ci) {
    spans[ci] = maxima[ci] - minima[ci];
    spans[ci] = maxima[ci] - minima[ci];
  }

  if (!prenormalized_minima) {
    prenormalized_minima = new T[this->GetChannelCount()];
  }
  memcpy(prenormalized_minima, minima, sizeof(T) * this->GetChannelCount());

  if (!prenormalized_maxima) {
    prenormalized_maxima = new T[this->GetChannelCount()];
  }
  memcpy(prenormalized_maxima, maxima, sizeof(T) * this->GetChannelCount());

  if (!prenormalized_spans) {
    prenormalized_spans = new T[this->GetChannelCount()];
  }
  memcpy(prenormalized_spans, spans, sizeof(T) * this->GetChannelCount());
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Normalize() {
  // We need minima and maxima to do this.
  if (!minima || !maxima) {
    ComputeMinMax();
  } 

  // Now bias and scale to get in the type range.
  NFieldIterator<ndims> c(this->GetDimensions());
  while (c.HasNext()) {
    c.Next();
    T *element = (*this)(c);
    for (int ci = 0; ci < this->nchannels; ++ci) {
      element[ci] = FieldTraits<T>::max * (element[ci] - minima[ci]) / spans[ci];
    }
  }

  // Update minima and maxima now.
  for (int i = 0; i < this->GetChannelCount(); ++i) {
    minima[i] = 0;
    maxima[i] = FieldTraits<T>::max;
    spans[i] = maxima[i] - minima[i];
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T NField<T, ndims>::GetMinimum(int channel) const {
  assert(minima);
  return minima[channel];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T NField<T, ndims>::GetMaximum(int channel) const {
  assert(maxima);
  return maxima[channel];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Resize(const QVector<int, ndims>& dims,
                              int nchannels) {
  NFieldBase<ndims>::Resize(dims, nchannels); 
  data = new T[this->GetValueCount()];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
template<class U>
T NField<T, ndims>::LinearInterpolate(const QVector<U, ndims>& position,
                                      int channel_id) const {
  // Find integral base grid position.
  QVector<int, ndims> base;
  for (int d = 0; d < ndims; ++d) {
    base[d] = (int) floor(position[d]);
  }

  // Collect values of surrounding samples.
  int nneighbors = 1 << ndims;
  T values[1 << ndims];
  NFieldIterator<ndims> c(base + 2, base);
  int ni = 0;
  while (c.HasNext()) {
    c.Next();
    values[ni] = (*this)(c)[channel_id];
    ++ni;
  }

  // Interpolate, one dimension at a time.
  for (int d = 0; d < ndims; ++d) {
    float weight = position[d] - base[d];
    int jump = 1 << d;
    for (int i = 0; i + jump < nneighbors; ++i) {
      values[i] = (1 - weight) * values[i] + weight * values[i + jump];
    }
  }

  return values[0];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::DrawLine(const QVector<int, ndims>& start,
                                const QVector<int, ndims>& end,
                                T intensity,
                                int channel_id) {
  LineSegmentIterator<int, ndims> c(LineSegment<int, ndims>(start, end));
  while (c.HasNext()) {
    c.Next();
    (*this)(c)[channel_id] = intensity;
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Clear(T clear_value,
                             int channel_id) {
  if (channel_id == -1) {
    for (int i = 0; i < this->GetValueCount(); ++i) {
      data[i] = clear_value;
    }
  } else {
    for (int i = channel_id, ei = 0;
         ei < this->GetElementCount();
         ++ei, i += this->GetChannelCount()) {
      data[i] = clear_value;
    }
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T *NField<T, ndims>::ExtractChannel(int channel_id) const {
  // TODO untested
  T *buffer = new T[this->GetElementCount()]; 
  for (int ei = 0; ei < this->GetElementCount(); ++ei) {
    buffer[ei] = (*this)(ei)[channel_id];
  }
  return buffer;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims> *NField<T, ndims>::Rechannel(int nchannels,
                                              int *channels) const {
  for (int i = 0; i < nchannels; ++i) {
    assert(channels[i] >= -1 && channels[i] < this->GetChannelCount());
  }

  NField<T, ndims> *field = new NField<T, ndims>(this->GetDimensions(), nchannels);
  field->Clear(0);

  NFieldIterator<ndims> c(this->GetDimensions());
  while (c.HasNext()) {
    c.Next();
    for (int i = 0; i < nchannels; ++i) {
      if (channels[i] >= 0) {
        (*field)(c)[i] = (*this)(c)[channels[i]];
      }
    }
  }

  return field;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
std::ostream& operator<<(std::ostream& out,
                         const td::NField<T, ndims>& field) {
  out << "dimensions: " << field.GetDimensions() << std::endl;
  out << "channels (" << field.GetChannelCount() << "): " << std::endl;
  for (int i = 0; i < field.GetChannelCount(); ++i) {
    out << "  " << i << ". " << field.channel_names[i] << ": ";
    if (field.minima) {
      out << "[" << field.minima[i] << " " << (field.minima[i] + field.spans[i]) << "]";
    }
    out << std::endl;
  }
  return out;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ForceMin(T min, int channel_id) {
  if (!minima || !maxima) {
    ComputeMinMax(); 
  }

  NFieldBase<ndims>::is_min_forced[channel_id] = true;
  minima[channel_id] = min;
  spans[channel_id] = maxima[channel_id] - minima[channel_id];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::ForceMax(T max, int channel_id) {
  if (!minima || !maxima) {
    ComputeMinMax(); 
  }

  NFieldBase<ndims>::is_max_forced[channel_id] = true;
  maxima[channel_id] = max;
  spans[channel_id] = maxima[channel_id] - minima[channel_id];
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Log(int channel_id) {
  int stride = channel_id == -1 ? 1 : this->GetChannelCount();
  int start = channel_id == -1 ? 0 : channel_id;

  for (int i = start; i < this->GetValueCount(); i += stride) {
    data[i] = (T) log(data[i] + 1);
  }
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::GetIsocontour(T isovalue,
                                     int& nprimitives,
                                     float*& positions,
                                     int channel) {
  ContourMarcher<T, ndims>::GetIsocontour(*this, isovalue, nprimitives, positions, channel);
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims> *NField<T, ndims>::GetSubfield(const QVector<int, ndims>& from,
                                                const QVector<int, ndims>& to) const {
  QVector<int, ndims> new_dimensions = (to - from).Abs() + 1;
  NField<T, ndims> *subfield = new NField<T, ndims>(new_dimensions, this->GetChannelCount());

  // It'd be wasteful to copy over one element at a time. Instead, let's
  // figure out how many elements are contiguous. The span of the first
  // dimension for sure can be grabbed in one fell swoop. If that span is 
  // the entirety of that dimension, then we can also read some span of the
  // second dimension. And so on, into all dimensions.
  QVector<int, ndims> bite_dims = QVector<int, ndims>(1);
  for (int d = 0; d < ndims; ++d) {
    bite_dims[d] = Utilities::ClampAbove(to[d] - from[d], 0) + 1;
    if (bite_dims[d] != this->GetDimensions()[d]) {
      break;
    }
  }

  // Let's figure out how many elements we need to jump between bites. We
  // almost have the answer in bite_dims, but if from[i] > to[i], our offset
  // will have to be negative.
  QVector<int, ndims> delta = bite_dims;
  QVector<int, ndims> xmax;
  for (int d = 0; d < ndims; ++d) {
    if (to[d] < from[d]) {
      delta[d] = -delta[d];
      xmax[d] = to[d] - 1;
    } else {
      xmax[d] = to[d] + 1;
    }
  }

  // The number of bytes copied per bit depends on the size of each value, the
  // number of channels, and the dimensions of each bite.
  int nbytes = sizeof(T) * this->GetChannelCount() * bite_dims.SelfProduct();

  // Walk through the source and destination fields, copying bites over.
  NFieldIterator<ndims> c(xmax, from, delta);
  NFieldIterator<ndims> subc(subfield->GetDimensions(), QVector<int, ndims>(0), bite_dims);
  while (c.HasNext() && subc.HasNext()) {
    c.Next();
    subc.Next();
    memcpy((*subfield)(subc), (*this)(c), nbytes);
  }

  return subfield; 
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims - 1> *NField<T, ndims>::GetSlice(const QVector<int, ndims - 1>& axis, int slice_of, int slice_id) const {
  // Assert no repeated axes.
  for (int i = 0; i < ndims - 1; ++i) {
    for (int j = i + 1; j < ndims - 1; ++j) {
      assert(axis[i] != axis[j]);
    }
  }

   // Find the dimensions of the slice and create a new field to hold the
   // slice.
   QVector<int, ndims - 1> slice_dims;
   for (int d = 0; d < ndims - 1; ++d) {
      slice_dims[d] = this->GetDimensions()[axis[d]];
   }
   NField<T, ndims - 1> *slice = new NField<T, ndims - 1>(slice_dims, this->nchannels);

   // Figure out how many voxels we can copy in each swipe.  Images in XY
   // can be done all at once, ones in XZ can be done line by line, and
   // anything else is done voxel by voxel.

   // Span is the untransformed dimensions of the maximally-sized chunk of
   // memory that can be copied.
   QVector<int, ndims> span(1);
   for (int d = 0; d < ndims - 1 && axis[d] == d; ++d) {
      span[d] = this->dims[d];
   }

   QVector<int, ndims> bounds;
   for (int d = 0; d < ndims - 1; ++d) {
      bounds[axis[d]] = this->dims[axis[d]];
   }

   int chunk_size = span.SelfProduct() * sizeof(T) * this->nchannels;

   QVector<int, ndims> max(this->dims);
   QVector<int, ndims> min(0);
   min[slice_of] = slice_id;
   max[slice_of] = slice_id + 1;

   QVector<int, ndims - 1> sc;

   NFieldIterator<ndims> c(max, min, span);
   while (c.HasNext()) {
     c.Next();
     for (int d = 0; d < ndims - 1; ++d) {
       sc[d] = c[axis[d]];
     }
     memcpy((*slice)(sc), (*this)(c), chunk_size);
   }

   return slice;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
void NField<T, ndims>::Transpose() {
  int nbytes = sizeof(T) * this->GetChannelCount();
  QVector<int, ndims> rdims = this->GetDimensions().GetReverse();
  T *rdata = new T[this->GetValueCount()];

  // Swap values. No symmetry to exploit, as field may not be square.
  NFieldIterator<ndims> c(this->GetDimensions()); 
  while (c.HasNext()) {
    c.Next();
    int i = VecToIdx(this->GetDimensions(), c);
    int ri = VecToIdx(rdims, c.GetReverse());
    memcpy(&rdata[ri], (*this)(i), nbytes);
  }

  // Reorder dimensions.
  NFieldBase<ndims>::dims = rdims;

  delete[] data;
  data = rdata;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims - 1> *NField<T, ndims>::Flatten() const {

  int dim_to_go = -1;
  for (int d = ndims - 1; d >= 0; --d) {
    if (this->GetDimensions()[d] == 1) {
      dim_to_go = d;
    }
  }
  assert(dim_to_go >= 0);

  QVector<int, ndims - 1> new_dims;
  for (int d = 0, new_d = 0; d < ndims; ++d) {
    if (d != dim_to_go) {
      new_dims[new_d] = this->GetDimensions()[d];
      ++new_d;
    }
  } 


  NField<T, ndims - 1> *flattened = new NField<T, ndims - 1>(new_dims, this->GetChannelCount());
  memcpy((*flattened)(0), (*this)(0), this->GetValueCount() * sizeof(T));

  return flattened;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
NField<T, ndims> *NField<T, ndims>::GetSeamless() const {
  NField<T, ndims> *seamless = new NField<T, ndims>(this->GetDimensions(), this->GetChannelCount());  

  QVector<int, ndims> half_dims = this->GetDimensions() / 2;
  QVector<float, ndims> midpoint = QVector<float, ndims>(this->GetDimensions()) * 0.5f;

  float radius = this->GetDimensions()[0];
  for (int i = 1; i < ndims; ++i) {
    if (this->GetDimensions()[i] < radius) {
      radius = this->GetDimensions()[i];
    }
  }
  radius *= 0.5f;

  NFieldIterator<ndims> c(seamless->GetDimensions());
  while (c.HasNext()) {
    c.Next();

    QVector<float, ndims> diff = (midpoint - QVector<float, ndims>(c)).Abs();
    int dmax = 0;
    for (int d = 1; d < ndims; ++d) {
      if (diff[dmax] < diff[d]) {
        dmax = d;
      }
    }

    float mix = diff[dmax] / midpoint[dmax];

    QVector<int, ndims> oppc = (c + half_dims) % this->GetDimensions();
    mix = powf(1.0f - mix, 0.5);
    for (int i = 0; i < this->GetChannelCount(); ++i) {
      (*seamless)(c)[i] = mix * (*this)(c)[i] + (1.0f - mix) * (*this)(oppc)[i];
    }
  }

  return seamless;
}

/* ------------------------------------------------------------------------- */

template<class T, int ndims>
T NField<T, ndims>::GetUnnormalizedValue(const QVector<int, ndims>& i, int ci) const {
  return (*this)(i)[ci] * prenormalized_spans[ci] + prenormalized_minima[ci];
}

/* ------------------------------------------------------------------------- */

}

#endif
