/*
 o---------------------------------------------------------------------o
 |
 | COS Array
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
 | $Id: Array.c,v 1.3 2008/10/17 18:12:21 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/Array.h>
#include <cos/Size.h>
#include <cos/Index.h>
#include <cos/Range.h>
#include <cos/Slice.h>
#include <cos/Vector.h>
#include <cos/Functor.h>
#include <cos/gen/init.h>
#include <cos/gen/access.h>
#include <cos/gen/object.h>
#include <cos/gen/functor.h>
#include <cos/gen/value.h>

#include <stdlib.h>

useclass(Array);

makclass(Array    , Collection);
makclass(Array0   , Array);
makclass(Array1   , Array);
makclass(Array2   , Array);
makclass(Array3   , Array);
makclass(Array4   , Array);
makclass(Array5   , Array);
makclass(Array6   , Array);
makclass(Array7   , Array);
makclass(Array8   , Array);
makclass(Array9   , Array);
makclass(ArrayN   , Array);
makclass(SubArray , Array);
makclass(DynArrayN, Array);
makclass(DynArray , DynArrayN);

// ----- local allocators

static inline struct Array*
array_alloc(U32 size)
{
  enum { N = 10 };
  static struct Class* cls[N] = {
    classref(Array0,Array1,Array2,Array3,Array4),
    classref(Array5,Array6,Array7,Array8,Array9) }; 

  useclass(ArrayN);

  OBJ _cls = size >= N ? ArrayN : (OBJ)cls[size];
  OBJ _arr = gallocWithSize(_cls, size * sizeof(OBJ));
  struct ArrayN *narr = STATIC_CAST(struct ArrayN*, _arr);
  struct Array  * arr = &narr->Array;

  arr->size   = size;
  arr->object = narr->_object;

  return arr;
}

static inline struct Array*
subarray_alloc(struct Array *ref, U32 start, U32 size)
{
  useclass(SubArray);

  OBJ _arr = gallocWithSize(SubArray, 0);
  struct SubArray *sarr = STATIC_CAST(struct SubArray*, _arr);
  struct Array    * arr = &sarr->Array;

  arr->size   = size;
  arr->object = ref->object + start;
  sarr->array = (OBJ)ref;

  return arr;
}

static inline struct Array*
dynarray_alloc(U32 size)
{
  useclass(DynArray, ExBadAlloc);

  OBJ _arr = gallocWithSize(DynArray, 0);
  struct DynArray *darr = STATIC_CAST(struct DynArray*, _arr);
  struct Array    * arr = &darr->DynArrayN.Array;

  arr->object = malloc(size * sizeof *arr->object);
  if (!arr->object) THROW(ExBadAlloc);
  darr->capacity = size;

  return arr;
}

static void
dynarray_resizeBy(struct DynArray *darr, double factor)
{
  useclass(ExBadAlloc);

  struct Array *arr = &darr->DynArrayN.Array;
  U32  size = darr->capacity * factor;
  OBJ *object = realloc(arr->object, size * sizeof *arr->object);

  if (!object) THROW(ExBadAlloc);
  arr->object = object;
  darr->capacity = size;
}

// ----- ctor/dtor of a (dynamic) array

defmethod(OBJ, galloc, mArray)
  retmethod(_1); // lazy alloc
endmethod

defmethod(OBJ, ginit, mArray)
  retmethod( (OBJ)dynarray_alloc(10) );
endmethod

defmethod(OBJ, ginitWith, mArray, Size1)
  retmethod( (OBJ)dynarray_alloc(self2->size) );
endmethod

defmethod(OBJ, ginitWith, mArray, Array) // clone
  retmethod( ginitWith((OBJ)array_alloc(self2->size),_2) );
endmethod

defmethod(OBJ, ginitWith, Array, Array) // copy
  test_assert(self->size == self2->size);

  for (U32 i = 0; i < self->size; i++)
    self->object[i] = gclone(self2->object[i]);
  
  retmethod(_1);
endmethod

defmethod(OBJ, ginitWith2, mArray, Any, Size1)
  struct Array* arr = array_alloc(self3->size);

  for (U32 i = 0; i < arr->size; i++)
    arr->object[i] = gclone(_2);

  retmethod((OBJ)arr);
endmethod

defmethod(OBJ, ginitWith2, mArray, Array, Range1)
  struct Slice1 slice[1];
  OBJ s = Slice1_range(slice, self3, self2->size);

  retmethod( ginitWith2(_1,_2,s) );  
endmethod

defmethod(OBJ, ginitWith2, mArray, Array, Slice1)
  struct Array* arr = array_alloc(self3->size);

  test_assert( self3->start < self2->size && Slice1_last(self3) < self2->size );

  for (U32 i = 0; i < arr->size; i++) {
    OBJ obj = self2->object[ Slice1_eval(self3,i) ];
    arr->object[i] = gclone(obj);
  }
  
  retmethod((OBJ)arr);
endmethod

defmethod(OBJ, ginitWith2, mArray, Array, IntVector)
  struct Array* arr = array_alloc(self3->size);

  for (U32 i = 0; i < arr->size; i++) {
    U32 j = index_abs(self3->value[i], self2->size);
    test_assert( j < self2->size );
    arr->object[i] = gclone(self2->object[j]);
  }

  retmethod((OBJ)arr);
endmethod

defmethod(OBJ, ginitWithObjPtr, mArray, (U32)n, (OBJ*)obj)
  retmethod( ginitWith(_1, aArrRef(n,obj)) ); // clone
endmethod

defmethod(OBJ, gdeinit, Array)
  for (U32 i = 0; i < self->size; i++) {
    OBJ obj = self->object[i];
    if (obj) grelease(obj);
  }

  retmethod(_1);
endmethod

defmethod(OBJ, gdeinit, DynArrayN)
  next_method(self);
  free(self->Array.object);
  
  retmethod(_1);
endmethod

// ----- ctor/dtor of a subarray

defmethod(OBJ, ginitWith2, mSubArray, Array, Range1)
  struct Slice1 slice[1];
  OBJ s = Slice1_range(slice, self3, self2->size);

  retmethod( ginitWith2(_1,_2,s) );
endmethod

defmethod(OBJ, ginitWith2, mSubArray, Array, Slice1)
  struct Array *arr = STATIC_CAST(struct Array*, gretain(_2));

  test_assert( cos_any_id((OBJ)arr) != cos_class_id(classref(DynArray)) );
  test_assert( Slice1_iscontiguous(self3) && Slice1_last(self3) < self2->size );

  retmethod( (OBJ)subarray_alloc(arr, self3->start, self3->size) );
endmethod

defmethod(OBJ, gdeinit, SubArray)
  grelease(self->array);
  retmethod(_1);
endmethod
 
// ----- equality

defmethod(OBJ, gequal, Array, Array)
  BOOL res = self->size == self2->size;

  for (U32 i = 0; res && i < self->size; i++)
    res = gequal(self->object[i], self2->object[i]) == True;

  retmethod( res ? True : False );
endmethod

// ----- setters

defmethod(OBJ, gput, Array, Array)
  test_assert(self->size == self2->size);

  for (U32 i = 0; i < self->size; i++) {
    OBJ old = self->object[i];
    self->object[i] = gretain(self2->object[i]);
    if (old) grelease(old);
  }

  retmethod(_1);
endmethod

defmethod(OBJ, gputAt, Array, Any, Index1)
  U32 i = index_abs(self3->index, self->size);

  test_assert( i < self->size );
  OBJ old = self->object[i];
  self->object[i] = gretain(_2);
  if (old) grelease(old);
  
  retmethod(_1);
endmethod

defmethod(OBJ, gputAt, Array, Array, Index1)
  U32 i = index_abs(self3->index, self->size);

  test_assert( i+self2->size <= self->size );

  for (U32 j = 0; j < self2->size; i++, j++) {
    OBJ old = self->object[i];
    self->object[i] = gretain(self2->object[j]);
    if (old) grelease(old);
  }
  
  retmethod(_1);
endmethod

defmethod(OBJ, gputAt, Array, Array, Range1)
  struct Slice1 slice[1];
  OBJ s = Slice1_range(slice, self3, self->size);

  retmethod( gputAt(_1,_2,s) );  
endmethod

defmethod(OBJ, gputAt, Array, Array, Slice1)
  test_assert( self3->start < self->size && Slice1_last(self3) < self->size );

  for (U32 j = 0; j < self3->size; j++) {
    U32 i = Slice1_eval(self3,j);
    OBJ old = self->object[i];
    self->object[i] = gretain(self2->object[j]);
    if (old) grelease(old);
  }

  retmethod(_1);
endmethod

defmethod(OBJ, gputAt, Array, Array, IntVector)
  for (U32 j = 0; j < self3->size; j++) {
    U32 i = index_abs(self3->value[j], self->size);
    test_assert( i < self->size );
    OBJ old = self->object[i];
    self->object[i] = gretain(self2->object[j]);
    if (old) grelease(old);
  }

  retmethod(_1);  
endmethod

// ----- getter

defmethod(U32, gsize, Array)
  retmethod(self->size);
endmethod

defmethod(OBJ, ggetAt, Array, Index1)
  U32 i = index_abs(self2->index, self->size);
  test_assert( i < self->size );
  retmethod( self->object[i] );
endmethod

defmethod(OBJ, ggetAt, Array, Range1)
  struct Slice1 slice[1];
  OBJ s = Slice1_range(slice, self2, self->size);

  retmethod( ggetAt(_1,s) );
endmethod

defmethod(OBJ, ggetAt, Array, Slice1)
  retmethod( gautoRelease( gputAt((OBJ)array_alloc(self2->size),_1,_2) ) );
endmethod

defmethod(OBJ, ggetAt, Array, IntVector)
  retmethod( gautoRelease( gputAt((OBJ)array_alloc(self2->size),_1,_2) ) );
endmethod

// ----- stack-like accessors and adjustment

defalias (OBJ, (gput)gconcat, DynArray, Any);
defalias (OBJ, (gput)gpush  , DynArray, Any);
defmethod(OBJ,  gput        , DynArray, Any)
  struct Array *arr = &self->DynArrayN.Array;

  if (arr->size == self->capacity)
    dynarray_resizeBy(self, 1.75);
    
  arr->object[arr->size++] = gretain(_2);

  retmethod(_1);
endmethod

defalias (OBJ, (gget)glast, DynArray);
defalias (OBJ, (gget)gtop , DynArray);
defmethod(OBJ,  gget      , DynArray)
  struct Array *arr = &self->DynArrayN.Array;

  if (!arr->size) retmethod(Nil);

  retmethod( arr->object[arr->size-1] );
endmethod

defalias (OBJ, (gdrop)gpop, DynArray);
defmethod(OBJ,  gdrop     , DynArray)
  struct Array *arr = &self->DynArrayN.Array;

  if (!arr->size) retmethod(False);

  grelease(arr->object[--arr->size]);

  retmethod(True);
endmethod

defmethod(OBJ, gadjust, DynArray)
  struct Class *cls = &COS_CLS_NAME(DynArrayN);

  if (self->DynArrayN.Array.size < self->capacity)
    dynarray_resizeBy(self, 1.0);

  test_assert( cos_any_changeClass(_1, cls) );

  retmethod(_1);
endmethod

// ----- concat

defmethod(OBJ, gconcat, DynArray, Array)
  struct Array *arr = &self->DynArrayN.Array;

  if (self->capacity - arr->size < self2->size) {
    double size = arr->size;

    do size *= 1.75;
    while (self->capacity - size < self2->size);

    dynarray_resizeBy(self, size);
  }

  for (U32 i = 0; i < self2->size; i++)
    arr->object[arr->size++] = gretain(self2->object[i]);

  retmethod(_1);
endmethod

defmethod(OBJ, gconcat, Array, Array)
  struct Array *arr = array_alloc(self->size + self2->size);
  U32 i, j = 0;

  for (i = 0; i < self1->size; i++, j++)
    arr->object[j] = gretain(self1->object[i]);

  for (i = 0; i < self2->size; i++, j++)
    arr->object[j] = gretain(self2->object[i]);

  retmethod( gautoRelease((OBJ)arr) );
endmethod

// ----- map, fold, ...

defmethod(void, gapply, Array, Functor)
  for (U32 i = 0; i < self->size; i++)
     geval1(_2, self->object[i]);
endmethod

defmethod(OBJ, gmap, Array, Functor)
  struct Array* arr = array_alloc(self->size);

  for (U32 i = 0; i < arr->size; i++) {
      OBJ res = geval1(_2, self->object[i]);
      arr->object[i] = gretain(res);
  }

  retmethod( gautoRelease((OBJ)arr) );
endmethod

defmethod(OBJ, gmap2, Array, Array, Functor)
  test_assert(self1->size == self2->size);

  struct Array* arr = array_alloc(self->size);

  for (U32 i = 0; i < arr->size; i++) {
    OBJ res = geval2(_3, self1->object[i], self2->object[i]);
    arr->object[i] = gretain(res);
  }

  retmethod( gautoRelease((OBJ)arr) );
endmethod

defmethod(OBJ, gmap3, Array, Array, Array, Functor)
  test_assert(self1->size == self2->size && self1->size == self3->size);

  struct Array* arr = array_alloc(self->size);

  for (U32 i = 0; i < arr->size; i++) {
    OBJ res = geval3(_3, self1->object[i], self2->object[i], self3->object[i]);
    arr->object[i] = gretain(res);
  }

  retmethod( gautoRelease((OBJ)arr) );
endmethod

defmethod(OBJ, gfold, Array, Any, Functor)
  OBJ res = _2;
  
  for (U32 i = 0; i < self->size; i++)
    res = geval2(_3, res, self->object[i]);

  retmethod(res);
endmethod

defmethod(OBJ, gscan, Array, Any, Functor)
  struct Array* arr = array_alloc(self->size+1);
  OBJ res = _2;

  for (U32 i = 0; i < self->size; i++) {
    arr->object[i] = gclone(res);
    res = geval2(_3, res, self->object[i]);
  }
  arr->object[self->size] = gclone(res);

  retmethod( gautoRelease((OBJ)arr) );
endmethod

defmethod(OBJ, gfilter, Array, Functor)
  struct Array* arr = dynarray_alloc(self->size);

  for (U32 i = 0; i < arr->size; i++) {
    OBJ obj = self->object[i];
    if (geval1(_2, obj) == True)
      arr->object[arr->size++] = gretain(obj);
  }

  retmethod( gadjust(gautoRelease((OBJ)arr)) );
endmethod
