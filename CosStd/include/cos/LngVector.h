#ifndef COS_LNGVECTOR_H
#define COS_LNGVECTOR_H

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
 | $Id: LngVector.h,v 1.10 2010/05/31 14:02:23 ldeniau Exp $
 |
*/

#include <cos/Vector.h>

/* NOTE-USER: LngVector class cluster constructors

   see cos/Vector.h for description
*/

defclass(LngVector, IntegralVector)
  I64 *value;
  U32  size;
  I32  stride;
endclass

// ----- automatic constructors

#define aLngVector(...)        ( (OBJ)atLngVector       (__VA_ARGS__) )
#define aLngVectorRef(...)     ( (OBJ)atLngVectorRef    (__VA_ARGS__) )
#define aLngVectorView(...)    ( (OBJ)atLngVectorView   (__VA_ARGS__) )
#define aLngVectorSubView(...) ( (OBJ)atLngVectorSubView(__VA_ARGS__) )

// --- shortcuts

#ifndef COS_NOSHORTCUT

#define aLVec(...)  aLngVector(__VA_ARGS__)

#endif

/***********************************************************
 * Implementation (private)
 */

// ----- Block vector

defclass(LngVector0, LngVector) I64 _value[]; endclass
defclass(LngVector1, LngVector) I64 _value[]; endclass
defclass(LngVector2, LngVector) I64 _value[]; endclass
defclass(LngVector3, LngVector) I64 _value[]; endclass
defclass(LngVector4, LngVector) I64 _value[]; endclass
defclass(LngVector5, LngVector) I64 _value[]; endclass
defclass(LngVector6, LngVector) I64 _value[]; endclass
defclass(LngVector7, LngVector) I64 _value[]; endclass
defclass(LngVector8, LngVector) I64 _value[]; endclass
defclass(LngVector9, LngVector) I64 _value[]; endclass
defclass(LngVectorN, LngVector) I64 _value[]; endclass

// ----- Fixed vector, Dynamic vector and Lazy vector

defclass(LngVectorFix, LngVector)
  I64 *_value;
  U32  capacity;
endclass

defclass(LngVectorDyn, LngVectorFix)
endclass

// ----- LngVector view and subview

defclass(LngVectorView, LngVector)
  struct LngVector *ref;
endclass

defclass(LngVectorSubView, LngVectorView)
endclass

// ----- allocators and initializers

struct Slice;
struct LngVector* LngVector_alloc(U32);
struct LngVector* LngVectorView_init(struct LngVectorView*, struct LngVector*,
                                     struct Slice*, BOOL);

// ----- automatic constructors

#define atLngVector(...)                 atVector    (Lng,I64,__VA_ARGS__)
#define atLngVectorRef(...)              atVectorRef (Lng,    __VA_ARGS__)
#define atLngVectorView(vector,slice)    atVectorView(Lng,vector,slice,COS_NO)
#define atLngVectorSubView(vector,slice) atVectorView(Lng,vector,slice,COS_YES)

#endif // COS_LNGVECTOR_H
