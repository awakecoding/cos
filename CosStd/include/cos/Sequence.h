#ifndef COS_SEQUENCE_H
#define COS_SEQUENCE_H

/*
 o---------------------------------------------------------------------o
 |
 | COS Sequence
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
 | $Id: Sequence.h,v 1.14 2010/06/04 23:27:21 ldeniau Exp $
 |
*/

#include <cos/Collection.h>

defclass(Sequence, Collection)
endclass

defclass(ValueSequence, Sequence)
endclass

#ifndef SEQUENCE_GROWTH_RATE
#define SEQUENCE_GROWTH_RATE 1.618034 // golden ratio
#endif

#ifndef SEQUENCE_MIN_SIZE
#define SEQUENCE_MIN_SIZE 256         // dynamic sequence starting size
#endif

U32 Sequence_enlargeCapacity(U32 capacity, U32 extra);

/***********************************************************
 * Implementation (private)
 */

/* NOTE-INFO: Sized Name
   Many sequences in COS provide "size-aware" types.
   Sequence_SizedName build a type name by suffixing the type by the size
   If the size of (__VA_ARGS__) >= S, then suffix from N is used.
   Sequence_SizedName(Array,10,N,obj1,obj2,obj3) -> Array3 (with size == 3)
   Sequence_SizedName(Array,10,N,obj1,..,obj10)  -> ArrayN (with size == 10)
   Sequence_SizedName(Array,2,Long,obj1,obj2) -> ArrayLong (with size == 2)
*/

#define Sequence_SizedName(T,S,N,...) \
        COS_PP_IF(COS_PP_GE(COS_PP_NARG(__VA_ARGS__),S)) \
                  (COS_PP_CAT(T,N), COS_PP_CAT_NARG(T,__VA_ARGS__))

#endif // COS_SEQUENCE_H

