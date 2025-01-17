/*
 o---------------------------------------------------------------------o
 |
 | COS Vector template - Vector view
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
 | $Id: Vector_vw.c,v 1.14 2010/06/03 15:27:50 ldeniau Exp $
 |
*/

#ifndef VECTOR_TMPL
#error "this template file requires tmpl/Vector.c"
#endif

// -----

makclass(TV, T);
makclass(TW, TV);

// -----

useclass(TV, TW);

// ----- initializer

struct T*
TV_init(struct TV *vecv, struct T *vec, struct Slice *slc, BOOL isSub)
{
  test_assert( Slice_first(slc) < vec->size &&
               Slice_last (slc) < vec->size, "slice out of range" );

  struct T* vw = &vecv->T;

  vw->value  = Slice_start (slc)*vec->stride + vec->value;
  vw->size   = Slice_size  (slc);
  vw->stride = isSub ? Slice_stride(slc) : Slice_stride(slc)*vec->stride;
  vecv->ref  = vec;

  return vw;
}

// ----- view constructors

defalias (OBJ, (ginitWith2)gnewWith2, mView, T, Slice);
defmethod(OBJ,  ginitWith2          , mView, T, Slice) // vector view
  retmethod(ginitWith3(galloc(TV),_2,_3,aInt(NO)));
endmethod

defalias (OBJ, (ginitWith2)gnewWith2, mView, T, Range);
defmethod(OBJ,  ginitWith2          , mView, T, Range) // vector view
  struct Slice *slice = Slice_fromRange(atSlice(0), self3, &self2->size);
  
  retmethod(ginitWith3(galloc(TV),_2,(OBJ)slice,aInt(NO)));
endmethod

defalias (OBJ, (ginitWith2)gnewWith2, mSubView, T, Slice);
defmethod(OBJ,  ginitWith2          , mSubView, T, Slice) // vector view
  retmethod(ginitWith3(galloc(TW),_2,_3,aInt(YES)));
endmethod

defalias (OBJ, (ginitWith2)gnewWith2, mSubView, T, Range);
defmethod(OBJ,  ginitWith2          , mSubView, T, Range) // vector view
  struct Slice *slice = Slice_fromRange(atSlice(0), self3, &self2->size);

  retmethod(ginitWith3(galloc(TW),_2,(OBJ)slice,aInt(YES)));
endmethod

defmethod(OBJ, ginitWith3, TV, T, Slice, Int) // vector view
  PRT(_1);
  self->ref = 0;
  
  if (cos_object_isKindOf(_2, classref(TD)))
    gadjust(_2);

  OBJ ref = gretain(_2); PRT(ref);
  
  TV_init(self, CAST(struct T*, ref), self3, self4->value);

  UNPRT(_1);
  retmethod(_1);
endmethod

// ----- destructor

defmethod(OBJ, gdeinit, TV)
  if (self->ref)              // take care of protection cases
    grelease( (OBJ)self->ref ), self->ref = 0;
  retmethod(_1);
endmethod

// ----- invariant

defmethod(void, ginvariant, TV, (STR)file, (int)line)
  test_assert( cos_object_isKindOf((OBJ)self->ref, classref(T)),
               TS " view points to something not a " TS, file, line);

  test_assert( !cos_object_isKindOf((OBJ)self->ref, classref(TD)) ||
               cos_object_rc((OBJ)self->ref) == COS_RC_AUTO ,
               TS " view points to a dynamic " TS, file, line);

  struct T *vec = self->ref;

  I32 start  = (vec->value - self->T.value)/vec->stride;
  U32 size   = self->T.size;
  I32 stride = self->T.stride/vec->stride;

  struct Slice *slc = atSlice(start, size, stride);

  U32 first = Slice_first(slc);
  U32 last  = Slice_last (slc);

  test_assert( first < self->ref->size &&
               last  < self->ref->size, TS " view is out of range", file, line);

  if (next_method_p)
    next_method(self, file, line);
endmethod

