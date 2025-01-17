#ifndef COS_COS_DISPATCH_H
#define COS_COS_DISPATCH_H

/*
 o---------------------------------------------------------------------o
 |
 | COS message
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
 | $Id: dispatch.h,v 1.8 2010/05/20 15:48:16 ldeniau Exp $
 |
*/

#ifndef COS_COS_COS_H
#error "COS: use <cos/cos/cos.h> instead of <cos/cos/dispatch.h>"
#endif

// --------------------------------------------------

static cos_inline U32
cos_method_hkey1(U32 idg, U32 id1)
{
  return idg +id1;
}

static cos_inline IMP1
cos_method_fastLookup1(SEL restrict _sel, U32 id1)
{
  U32 key = cos_method_hkey1(_sel->Behavior.id,id1);
  struct cos_method_cache1 *restrict cache = cos_method_cache1();
  struct cos_method_slot1 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1
    ? (*slot)->fct
    : cos_method_fastLookup1_(slot,_sel,id1);

  COS_UNUSED(cos_method_fastLookup1);
}

static cos_inline BOOL
cos_method_understand1(SEL restrict _sel, U32 id1)
{
  U32 key = cos_method_hkey1(_sel->Behavior.id,id1);
  struct cos_method_cache1 *restrict cache = cos_method_cache1();
  struct cos_method_slot1 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1
    ? COS_YES
    : cos_method_understand1_(slot,_sel,id1);

  COS_UNUSED(cos_method_understand1);
}

// --------------------------------------------------

static cos_inline U32
cos_method_hkey2(U32 idg, U32 id1, U32 id2)
{
  return idg +id1 +id2/2;
}

static cos_inline IMP2
cos_method_fastLookup2(SEL restrict _sel, U32 id1, U32 id2)
{
  U32 key = cos_method_hkey2(_sel->Behavior.id,id1,id2);
  struct cos_method_cache2 *restrict cache = cos_method_cache2();
  struct cos_method_slot2 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2
    ? (*slot)->fct
    : cos_method_fastLookup2_(slot,_sel,id1,id2);

  COS_UNUSED(cos_method_fastLookup2);
}

static cos_inline BOOL
cos_method_understand2(SEL restrict _sel, U32 id1, U32 id2)
{
  U32 key = cos_method_hkey2(_sel->Behavior.id,id1,id2);
  struct cos_method_cache2 *restrict cache = cos_method_cache2();
  struct cos_method_slot2 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2
    ? COS_YES
    : cos_method_understand2_(slot,_sel,id1,id2);

  COS_UNUSED(cos_method_understand2);
}

// --------------------------------------------------

static cos_inline U32
cos_method_hkey3(U32 idg, U32 id1, U32 id2, U32 id3)
{
  return idg +id1 +id2/2 +id3/4;
}

static cos_inline IMP3
cos_method_fastLookup3(SEL restrict _sel, U32 id1, U32 id2, U32 id3)
{
  U32 key = cos_method_hkey3(_sel->Behavior.id,id1,id2,id3);
  struct cos_method_cache3 *restrict cache = cos_method_cache3();
  struct cos_method_slot3 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2 &&
                   id3 == (*slot)->id3
    ? (*slot)->fct
    : cos_method_fastLookup3_(slot,_sel,id1,id2,id3);

  COS_UNUSED(cos_method_fastLookup3);
}

static cos_inline BOOL
cos_method_understand3(SEL restrict _sel, U32 id1, U32 id2, U32 id3)
{
  U32 key = cos_method_hkey3(_sel->Behavior.id,id1,id2,id3);
  struct cos_method_cache3 *restrict cache = cos_method_cache3();
  struct cos_method_slot3 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2 &&
                   id3 == (*slot)->id3
    ? COS_YES
    : cos_method_understand3_(slot,_sel,id1,id2,id3);

  COS_UNUSED(cos_method_understand3);
}

// --------------------------------------------------

static cos_inline U32
cos_method_hkey4(U32 idg, U32 id1, U32 id2, U32 id3, U32 id4)
{
  return idg +id1 +id2/2 +id3/4 +id4/8;
}

static cos_inline IMP4
cos_method_fastLookup4(SEL restrict _sel, U32 id1, U32 id2, U32 id3, U32 id4)
{
  U32 key = cos_method_hkey4(_sel->Behavior.id,id1,id2,id3,id4);
  struct cos_method_cache4 *restrict cache = cos_method_cache4();
  struct cos_method_slot4 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2 &&
                   id3 == (*slot)->id3 &&
                   id4 == (*slot)->id4
    ? (*slot)->fct
    : cos_method_fastLookup4_(slot,_sel,id1,id2,id3,id4);

  COS_UNUSED(cos_method_fastLookup4);
}

static cos_inline BOOL
cos_method_understand4(SEL restrict _sel, U32 id1, U32 id2, U32 id3, U32 id4)
{
  U32 key = cos_method_hkey4(_sel->Behavior.id,id1,id2,id3,id4);
  struct cos_method_cache4 *restrict cache = cos_method_cache4();
  struct cos_method_slot4 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2 &&
                   id3 == (*slot)->id3 &&
                   id4 == (*slot)->id4
    ? COS_YES
    : cos_method_understand4_(slot,_sel,id1,id2,id3,id4);

  COS_UNUSED(cos_method_understand4);
}

// --------------------------------------------------

static cos_inline U32
cos_method_hkey5(U32 idg, U32 id1, U32 id2, U32 id3, U32 id4, U32 id5)
{
  return idg +id1 +id2/2 +id3/4 +id4/8 +id5/16;
}

static cos_inline IMP5
cos_method_fastLookup5(SEL restrict _sel, U32 id1, U32 id2, U32 id3, U32 id4, U32 id5)
{
  U32 key = cos_method_hkey5(_sel->Behavior.id,id1,id2,id3,id4,id5);
  struct cos_method_cache5 *restrict cache = cos_method_cache5();
  struct cos_method_slot5 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2 &&
                   id3 == (*slot)->id3 &&
                   id4 == (*slot)->id4 &&
                   id5 == (*slot)->id5
    ? (*slot)->fct
    : cos_method_fastLookup5_(slot,_sel,id1,id2,id3,id4,id5);

  COS_UNUSED(cos_method_fastLookup5);
}

static cos_inline BOOL
cos_method_understand5(SEL restrict _sel, U32 id1, U32 id2, U32 id3, U32 id4, U32 id5)
{
  U32 key = cos_method_hkey5(_sel->Behavior.id,id1,id2,id3,id4,id5);
  struct cos_method_cache5 *restrict cache = cos_method_cache5();
  struct cos_method_slot5 *restrict*restrict slot = cache->slot + (key & cache->msk);

  return
    _sel->Behavior.id  == (*slot)->idg &&
                   id1 == (*slot)->id1 &&
                   id2 == (*slot)->id2 &&
                   id3 == (*slot)->id3 &&
                   id4 == (*slot)->id4 &&
                   id5 == (*slot)->id5
    ? COS_YES
    : cos_method_understand5_(slot,_sel,id1,id2,id3,id4,id5);

  COS_UNUSED(cos_method_understand5);
}

#endif // COS_COS_DISPATCH_H
