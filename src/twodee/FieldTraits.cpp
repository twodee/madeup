#include "twodee/FieldTraits.h"

namespace td {

/* ------------------------------------------------------------------------- */

/** Floats cap out at 1.0 */
template<> const float FieldTraits<float>::max = 1.0f; 

/** Maximum unsigned int */
template<> const unsigned int FieldTraits<unsigned int>::max = std::numeric_limits<unsigned int>::max();

/** Maximum unsigned short */
template<> const unsigned short FieldTraits<unsigned short>::max = std::numeric_limits<unsigned short>::max();

/** Maximum unsigned char */
template<> const unsigned char FieldTraits<unsigned char>::max = std::numeric_limits<unsigned char>::max();

/** Maximum signed int */
template<> const int FieldTraits<int>::max = std::numeric_limits<int>::max();

/** Maximum signed short */
template<> const short FieldTraits<short>::max = std::numeric_limits<short>::max();

/** Maximum signed char */
template<> const char FieldTraits<char>::max = std::numeric_limits<char>::max();

/* ------------------------------------------------------------------------- */

template<> const int FieldTraits<float>::type = FieldType::FLOAT;
template<> const int FieldTraits<unsigned int>::type = FieldType::UNSIGNED_INT;
template<> const int FieldTraits<unsigned short>::type = FieldType::UNSIGNED_SHORT;
template<> const int FieldTraits<unsigned char>::type = FieldType::UNSIGNED_CHAR;
template<> const int FieldTraits<int>::type = FieldType::INT;
template<> const int FieldTraits<short>::type = FieldType::SHORT;
template<> const int FieldTraits<char>::type = FieldType::CHAR;

/* ------------------------------------------------------------------------- */

#ifdef USE_MAGICK
template<> const Magick::StorageType FieldTraits<unsigned char>::magick_type = Magick::CharPixel;
template<> const Magick::StorageType FieldTraits<unsigned short>::magick_type = Magick::ShortPixel;
template<> const Magick::StorageType FieldTraits<unsigned int>::magick_type = Magick::IntegerPixel;
template<> const Magick::StorageType FieldTraits<char>::magick_type = Magick::CharPixel;
template<> const Magick::StorageType FieldTraits<short>::magick_type = Magick::ShortPixel;
template<> const Magick::StorageType FieldTraits<int>::magick_type = Magick::IntegerPixel;
template<> const Magick::StorageType FieldTraits<float>::magick_type = Magick::FloatPixel;
#endif

/* ------------------------------------------------------------------------- */

}
