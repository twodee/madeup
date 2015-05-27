#ifndef FIELDTRAITS_H
#define FIELDTRAITS_H

#include <limits>

/* #include "Configuration.h" */
#include "FieldType.h"

#ifdef USE_MAGICK
#include <ImageMagick/Magick++.h>
#endif

namespace td {

/* ------------------------------------------------------------------------- */

/**
 Type-specific field attributes, used to generalize algorithms where values
 differ depending on the underlying type.
 */
template<class T>
class FieldTraits {
  public:
    /** A numeric identifier for this data type */
    static const int type;

    /** The maximum value for this data type */
    static const T max;

#ifdef USE_MAGICK
    static const Magick::StorageType magick_type;
#endif
};

/* ------------------------------------------------------------------------- */

}

#endif
