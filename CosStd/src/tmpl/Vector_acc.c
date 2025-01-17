/*
 o---------------------------------------------------------------------o
 |
 | COS Vector template - accessors
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
 | $Id: Vector_acc.c,v 1.13 2010/06/02 22:47:26 ldeniau Exp $
 |
*/

#ifndef VECTOR_TMPL
#error "this template file requires tmpl/Vector.c"
#endif

// --- first, last

defmethod(OBJ, glast, T)
  retmethod( self->size
           ? gautoRelease(VALOBJ(self->value[(self->size-1)*self->stride]))
           : Nil );
endmethod

defmethod(OBJ, gfirst, T)
  retmethod( self->size ? gautoRelease(VALOBJ(self->value[0])) : Nil );
endmethod

// ----- getters (index, slice, range, intvector)

defmethod(OBJ, ggetAtIdx, T, (I32)idx)
  U32 i = Range_index(idx, self->size);
  test_assert( i < self->size, "index out of range" );

  retmethod( gautoRelease(VALOBJ(self->value[i*self->stride])) );
endmethod

defmethod(OBJ, ggetAt, T, Int)
  U32 i = Range_index(self2->value, self->size);
  test_assert( i < self->size, "index out of range" );

  retmethod( gautoRelease(VALOBJ(self->value[i*self->stride])) );
endmethod

defmethod(OBJ, ggetAt, T, Slice)
  retmethod( gautoRelease(gnewWith2(T,_1,_2)) );
endmethod

defmethod(OBJ, ggetAt, T, Range)
  retmethod( gautoRelease(gnewWith2(T,_1,_2)) );
endmethod

defmethod(OBJ, ggetAt, T, IntVector)
  retmethod( gautoRelease(gnewWith2(T,_1,_2)) );
endmethod

// ----- object setters (index, slice, range, intvector)

defmethod(OBJ, gputAtIdx, T, (I32)idx, Object)
  U32 i = Range_index(idx, self->size);
  test_assert( i < self->size, "index out of range" );

  VAL  val = TOVAL(_2);
  VAL *dst = self->value + i*self->stride;
  
  *dst = val;
  
  retmethod(_1);
endmethod

defmethod(OBJ, gputAt, T, Int, Object)
  U32 i = Range_index(self2->value, self->size);
  test_assert( i < self->size, "index out of range" );

  VAL  val = TOVAL(_3);
  VAL *dst = self->value + i*self->stride;
  
  *dst = val;
  
  retmethod(_1);
endmethod

defmethod(OBJ, gputAt, T, Slice, Object)
  PRE
    test_assert( Slice_first(self2) < self->size &&
                 Slice_last (self2) < self->size, "slice out of range" );

  BODY
    U32  dst_n = Slice_size  (self2);
    I32  dst_s = Slice_stride(self2)*self->stride;
    VAL *dst   = Slice_start (self2)*self->stride + self->value;
    VAL *end   = dst + dst_s*dst_n;
    VAL  val   = TOVAL(_3);

    while (dst != end) {
      *dst = val;
      dst += dst_s;
    }
    
    retmethod(_1);
endmethod

defmethod(OBJ, gputAt, T, Range, Object)
  struct Slice *slice = Slice_fromRange(atSlice(0), self2, &self->size);
  
  retmethod( gputAt(_1,(OBJ)slice,_3) );
endmethod

defmethod(OBJ, gputAt, T, IntVector, Object)
  U32  dst_n = self->size;
  I32  dst_s = self->stride;
  VAL *dst   = self->value;
  U32  idx_n = self2->size;
  I32  idx_s = self2->stride;
  I32 *idx   = self2->value;
  I32 *end   = idx + idx_s*idx_n;
  VAL  val   = TOVAL(_3);

  while (idx != end) {
    U32 i = Range_index(*idx, dst_n);
    test_assert( i < dst_n, "index out of range" );
    dst[i*dst_s] = val;
    idx += idx_s;
  }

  retmethod(_1);
endmethod

// ----- array setters (slice, range, intvector)

defmethod(OBJ, gputAt, T, Slice, T)
  PRE
    test_assert( Slice_first(self2) < self->size &&
                 Slice_last (self2) < self->size, "slice out of range" );
    test_assert( Slice_size (self2) <= self3->size, "source " TS " is too small" );

  BODY
    U32  dst_n = Slice_size  (self2);
    I32  dst_s = Slice_stride(self2)*self->stride;
    VAL *dst   = Slice_start (self2)*self->stride + self->value;
    I32  src_s = self3->stride;
    VAL *src   = self3->value;
    VAL *end   = dst + dst_s*dst_n;

    while (dst != end) {
      *dst = *src;
      src += src_s;
      dst += dst_s;
    }
    
    retmethod(_1);
endmethod

defmethod(OBJ, gputAt, T, IntVector, T)
  PRE
    test_assert( self2->size <= self3->size, "source " TS " is too small" );

  BODY
    U32  dst_n = self->size;
    I32  dst_s = self->stride;
    VAL *dst   = self->value;
    U32  idx_n = self2->size;
    I32  idx_s = self2->stride;
    I32 *idx   = self2->value;
    I32  src_s = self3->stride;
    VAL *src   = self3->value;
    I32 *end   = idx + idx_s*idx_n;

    while (idx != end) {
      U32 i = Range_index(*idx, dst_n);
      test_assert( i < dst_n, "index out of range" );
      dst[i*dst_s] = *src;
      src += src_s;
      idx += idx_s;
    }
    
    retmethod(_1);
endmethod

// ----- value getters

#if defined(CHRVECTOR_ONLY)
defalias(I32, (gintAt)gchrAt, T, Int);
#endif

#if defined(CHRVECTOR_ONLY) || defined(SHTVECTOR_ONLY)
defalias(I32, (gintAt)gshtAt, T, Int);
#endif

#if defined(CHRVECTOR_ONLY) || defined(SHTVECTOR_ONLY) || \
    defined(INTVECTOR_ONLY)

defmethod(I32, gintAt, T, Int)
  U32 i;

  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    retmethod( self->value[i*self->stride] );
endmethod

#endif

#if defined(CHRVECTOR_ONLY) || defined(SHTVECTOR_ONLY) || \
    defined(INTVECTOR_ONLY) || defined(LNGVECTOR_ONLY)

defmethod(I64, glngAt, T, Int)
  U32 i;

  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);

    retmethod( self->value[i*self->stride] );
endmethod

#endif

#if defined(CHRVECTOR_ONLY) || defined(SHTVECTOR_ONLY) || \
    defined(INTVECTOR_ONLY) || defined(LNGVECTOR_ONLY) || \
    defined(FLTVECTOR_ONLY)

defmethod(F64, gfltAt, T, Int)
  U32 i;

  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    retmethod( self->value[i*self->stride] );
endmethod

#endif

#if defined(CHRVECTOR_ONLY) || defined(SHTVECTOR_ONLY) || \
    defined(INTVECTOR_ONLY) || defined(LNGVECTOR_ONLY) || \
    defined(FLTVECTOR_ONLY) || defined(CPXVECTOR_ONLY)

defmethod(C64, gcpxAt, T, Int)
  U32 i;

  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    retmethod( self->value[i*self->stride] );
endmethod


#endif

// ----- value setters

#if defined(CHRVECTOR_ONLY) || defined(SHTVECTOR_ONLY) || \
    defined(INTVECTOR_ONLY) || defined(LNGVECTOR_ONLY) || \
    defined(FLTVECTOR_ONLY) || defined(CPXVECTOR_ONLY)

defmethod(OBJ, gputAt, T, Int, Char)
  U32 i;
  
  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    self->value[i*self->stride] = self3->Int.value;
        
    retmethod(_1);
endmethod

#endif

#if defined(SHTVECTOR_ONLY) || defined(INTVECTOR_ONLY) || \
    defined(LNGVECTOR_ONLY) || defined(FLTVECTOR_ONLY) || \
    defined(CPXVECTOR_ONLY)

defmethod(OBJ, gputAt, T, Int, Short)
  U32 i;
  
  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    self->value[i*self->stride] = self3->Int.value;
    
    retmethod(_1);
endmethod

#endif

#if defined(INTVECTOR_ONLY) || defined(LNGVECTOR_ONLY) || \
    defined(FLTVECTOR_ONLY) || defined(CPXVECTOR_ONLY)

defmethod(OBJ, gputAt, T, Int, Int)
  U32 i;
  
  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    self->value[i*self->stride] = self3->value;
    
    retmethod(_1);
endmethod

#endif

#if defined(LNGVECTOR_ONLY) || defined(FLTVECTOR_ONLY) || \
    defined(CPXVECTOR_ONLY)

defmethod(OBJ, gputAt, T, Int, Long)
  U32 i;
  
  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    self->value[i*self->stride] = self3->value;
    
    retmethod(_1);
endmethod

#endif

#if defined(FLTVECTOR_ONLY) || defined(CPXVECTOR_ONLY)

defmethod(OBJ, gputAt, T, Int, Float)
  U32 i;
  
  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    self->value[i*self->stride] = self3->value;
    
    retmethod(_1);
endmethod

#endif

#if defined(CPXVECTOR_ONLY)

defmethod(OBJ, gputAt, T, Int, Complex)
  U32 i;
  
  PRE
    i = Range_index(self2->value, self->size);
    test_assert( i < self->size, "index out of range" );

  BODY
    if (!COS_CONTRACT) // no PRE
      i = Range_index(self2->value, self->size);
      
    self->value[i*self->stride] = self3->value;
    
    retmethod(_1);
endmethod

#endif

