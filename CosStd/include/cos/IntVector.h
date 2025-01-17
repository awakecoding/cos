#ifndef COS_INTVECTOR_H
#define COS_INTVECTOR_H

/*
 o---------------------------------------------------------------------o
 |
 | COS Vector, Dynamic Vector and Vector View
 |
 o---------------------------------------------------------------------o
 |
 | C Object System
 |
 | Copyright (c) 2006+ Laurent Deniau, laurent.deniau@cern.ch
 |
 | For more information, see:
 | http://cern.ch/laurent.deniau/cos.html
 |
 o---------------------------------------------------------------------o
 |
 | This file is part of the C Object System framework.
 |
 | The C Object System is free software; you can redistribute it and/or
 | modify it under the terms of the GNU Lesser General Public License
 | as published by the Free Software Foundation; either version 3 of
 | the License, or (at your option) any later version.
 |
 | The C Object System is distributed in the hope that it will be
 | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: IntVector.h,v 1.11 2010/05/31 14:02:23 ldeniau Exp $
 |
*/

#include <cos/Vector.h>

/* NOTE-USER: IntVector class cluster constructors

   see cos/Vector.h for description
*/

defclass(IntVector, IntegralVector)
  I32 *value;
  U32  size;
  I32  stride;
endclass

// ----- automatic constructors

#define aIntVector(...)        ( (OBJ)atIntVector       (__VA_ARGS__) )
#define aIntVectorRef(...)     ( (OBJ)atIntVectorRef    (__VA_ARGS__) )
#define aIntVectorView(...)    ( (OBJ)atIntVectorView   (__VA_ARGS__) )
#define aIntVectorSubView(...) ( (OBJ)atIntVectorSubView(__VA_ARGS__) )

// --- shortcuts

#ifndef COS_NOSHORTCUT

#define aIVec(...)  aIntVector(__VA_ARGS__)

#endif

/***********************************************************
 * Implementation (private)
 */

// ----- Block vector

defclass(IntVector0, IntVector) I32 _value[]; endclass
defclass(IntVector1, IntVector) I32 _value[]; endclass
defclass(IntVector2, IntVector) I32 _value[]; endclass
defclass(IntVector3, IntVector) I32 _value[]; endclass
defclass(IntVector4, IntVector) I32 _value[]; endclass
defclass(IntVector5, IntVector) I32 _value[]; endclass
defclass(IntVector6, IntVector) I32 _value[]; endclass
defclass(IntVector7, IntVector) I32 _value[]; endclass
defclass(IntVector8, IntVector) I32 _value[]; endclass
defclass(IntVector9, IntVector) I32 _value[]; endclass
defclass(IntVectorN, IntVector) I32 _value[]; endclass

// ----- Fixed vector, Dynamic vector and Lazy vector

defclass(IntVectorFix, IntVector)
  I32 *_value;
  U32  capacity;
endclass

defclass(IntVectorDyn, IntVectorFix)
endclass

// ----- IntVector view and subview

defclass(IntVectorView, IntVector)
  struct IntVector *ref;
endclass

defclass(IntVectorSubView, IntVectorView)
endclass

// ----- initializers, allocators (for the class cluster)

struct Slice;
struct IntVector* IntVector_alloc(U32);
struct IntVector* IntVectorView_init(struct IntVectorView*, struct IntVector*,
                                     struct Slice*, BOOL);

// ----- automatic constructors

#define atIntVector(...)                 atVector    (Int,I32,__VA_ARGS__)
#define atIntVectorRef(...)              atVectorRef (Int,    __VA_ARGS__)
#define atIntVectorView(vector,slice)    atVectorView(Int,vector,slice,COS_NO)
#define atIntVectorSubView(vector,slice) atVectorView(Int,vector,slice,COS_YES)

#endif // COS_INTVECTOR_H
