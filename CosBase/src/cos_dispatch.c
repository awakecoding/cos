/*
 o---------------------------------------------------------------------o
 |
 | COS dispatch caches
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
 | $Id: cos_dispatch.c,v 1.6 2010/01/16 13:58:41 ldeniau Exp $
 |
*/

#include <cos/Object.h>

void cos_method_clearCaches(void)
{
  cos_method_clearCache1();
  cos_method_clearCache2();
  cos_method_clearCache3();
  cos_method_clearCache4();
  cos_method_clearCache5();
}

/*
 * ----------------------------------------------------------------------------
 *  Debug Functions
 * ----------------------------------------------------------------------------
 */

#include <cos/debug.h>

static const double shift32 = 4294967296.0;

// ----- cache stats

void
cos_method_statCache1(FILE *fp)
{
  struct cos_method_cache1 *cache = cos_method_cache1();
  F64 miss = cache->mis2*shift32 + cache->mis;
  U32 i, n, n1=0, n2=0, n3=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot1 *slot = cache->slot[i];

    if (slot->idg) {
      ++n1;
      if (slot->nxt->idg) {
        ++n2;
        if (slot->nxt->nxt->idg) {
          ++n3;
    }}}
  }

  n = n1+n2+n3;
  fprintf(fp,"cache1: slots=%6u, cells=%4u, miss=%g, load=%.2f, depth=[%u,%u,%u]\n",
          i,n,miss,(double)n/i,n1,n2,n3);
}

void
cos_method_statCache2(FILE *fp)
{
  struct cos_method_cache2 *cache = cos_method_cache2();
  F64 miss = cache->mis2*shift32 + cache->mis;
  U32 i, n, n1=0, n2=0, n3=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot2 *slot = cache->slot[i];

    if (slot->idg) {
      ++n1;
      if (slot->nxt->idg) {
        ++n2;
        if (slot->nxt->nxt->idg) {
          ++n3;
    }}}
  }

  n = n1+n2+n3;
  fprintf(fp,"cache2: slots=%6u, cells=%4u, miss=%g, load=%.2f, depth=[%u,%u,%u]\n",
          i,n,miss,(double)n/i,n1,n2,n3);
}

void
cos_method_statCache3(FILE *fp)
{
  struct cos_method_cache3 *cache = cos_method_cache3();
  F64 miss = cache->mis2*shift32 + cache->mis;
  U32 i, n, n1=0, n2=0, n3=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot3 *slot = cache->slot[i];

    if (slot->idg) {
      ++n1;
      if (slot->nxt->idg) {
        ++n2;
        if (slot->nxt->nxt->idg) {
          ++n3;
    }}}
  }

  n = n1+n2+n3;
  fprintf(fp,"cache3: slots=%6u, cells=%4u, miss=%g, load=%.2f, depth=[%u,%u,%u]\n",
          i,n,miss,(double)n/i,n1,n2,n3);
}

void
cos_method_statCache4(FILE *fp)
{
  struct cos_method_cache4 *cache = cos_method_cache4();
  F64 miss = cache->mis2*shift32 + cache->mis;
  U32 i, n, n1=0, n2=0, n3=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot4 *slot = cache->slot[i];

    if (slot->idg) {
      ++n1;
      if (slot->nxt->idg) {
        ++n2;
        if (slot->nxt->nxt->idg) {
          ++n3;
    }}}
  }

  n = n1+n2+n3;
  fprintf(fp,"cache4: slots=%6u, cells=%4u, miss=%g, load=%.2f, depth=[%u,%u,%u]\n",
          i,n,miss,(double)n/i,n1,n2,n3);
}

void
cos_method_statCache5(FILE *fp)
{
  struct cos_method_cache5 *cache = cos_method_cache5();
  F64 miss = cache->mis2*shift32 + cache->mis;
  U32 i, n, n1=0, n2=0, n3=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot5 *slot = cache->slot[i];

    if (slot->idg) {
      ++n1;
      if (slot->nxt->idg) {
        ++n2;
        if (slot->nxt->nxt->idg) {
          ++n3;
    }}}
  }

  n = n1+n2+n3;
  fprintf(fp,"cache5: slots=%6u, cells=%4u, miss=%g, load=%.2f, depth=[%u,%u,%u]\n",
          i,n,miss,(double)n/i,n1,n2,n3);
}

// ----- cache content

void
cos_method_showCache1(FILE *fp)
{
  struct cos_method_cache1 *cache = cos_method_cache1();
  U32 i, n=0, d=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot1 *slot = cache->slot[i];

    if (slot->idg) {
      U32 j=0;

      fprintf(fp, "[%3u]", i);

      do {
        ++j;
        fprintf(fp, " %s(%s)",
                cos_generic_get(slot->idg)->str,
                cos_class_get  (slot->id1)->str);
        slot = slot->nxt;
      } while (slot->idg);

      fputc('\n', fp);

      n += j;
      if (j > d) d = j;
    }
  }

  fprintf(fp,"cache 1: slots=%u, cells=%u, load=%.2f\n, maxdepth=%u",i,n,(double)n/i,d);
}

void
cos_method_showCache2(FILE *fp)
{
  struct cos_method_cache2 *cache = cos_method_cache2();
  U32 i, n=0, d=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot2 *slot = cache->slot[i];

    if (slot->idg) {
      U32 j=0;

      fprintf(fp, "[%3u]", i);

      do {
        ++j;
        fprintf(fp, " %s(%s,%s)",
                cos_generic_get(slot->idg)->str,
                cos_class_get  (slot->id1)->str,
                cos_class_get  (slot->id2)->str);
        slot = slot->nxt;
      } while (slot->idg);

      fputc('\n', fp);

      n += j;
      if (j > d) d = j;
    }
  }

  fprintf(fp,"cache 2: slots=%u, cells=%u, load=%.2f, maxdepth=%u\n",i,n,(double)n/i,d);
}

void
cos_method_showCache3(FILE *fp)
{
  struct cos_method_cache3 *cache = cos_method_cache3();
  U32 i, n=0, d=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot3 *slot = cache->slot[i];

    if (slot->idg) {
      U32 j=0;

      fprintf(fp, "[%3u]", i);

      do {
        ++j;
        fprintf(fp, " %s(%s,%s,%s)",
                cos_generic_get(slot->idg)->str,
                cos_class_get  (slot->id1)->str,
                cos_class_get  (slot->id2)->str,
                cos_class_get  (slot->id3)->str);
        slot = slot->nxt;
      } while (slot->idg);

      fputc('\n', fp);

      n += j;
      if (j > d) d = j;
    }
  }

  fprintf(fp,"cache 3: slots=%u, cells=%u, load=%.2f, maxdepth=%u\n",i,n,(double)n/i,d);
}

void
cos_method_showCache4(FILE *fp)
{
  struct cos_method_cache4 *cache = cos_method_cache4();
  U32 i, n=0, d=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot4 *slot = cache->slot[i];

    if (slot->idg) {
      U32 j=0;

      fprintf(fp, "[%3u]", i);

      do {
        ++j;
        fprintf(fp, " %s(%s,%s,%s,%s)",
                cos_generic_get(slot->idg)->str,
                cos_class_get  (slot->id1)->str,
                cos_class_get  (slot->id2)->str,
                cos_class_get  (slot->id3)->str,
                cos_class_get  (slot->id4)->str);
        slot = slot->nxt;
      } while (slot->idg);

      fputc('\n', fp);

      n += j;
      if (j > d) d = j;
    }
  }

  fprintf(fp,"cache 4: slots=%u, cells=%u, load=%.2f, maxdepth=%u\n",i,n,(double)n/i,d);
}

void
cos_method_showCache5(FILE *fp)
{
  struct cos_method_cache5 *cache = cos_method_cache5();
  U32 i, n=0, d=0;

  if (!fp) fp = stderr;

  for (i=0; i <= cache->msk; i++) {
    struct cos_method_slot5 *slot = cache->slot[i];

    if (slot->idg) {
      U32 j=0;

      fprintf(fp, "[%3u]", i);

      do {
        ++j;
        fprintf(fp, " %s(%s,%s,%s,%s,%s)",
                cos_generic_get(slot->idg)->str,
                cos_class_get  (slot->id1)->str,
                cos_class_get  (slot->id2)->str,
                cos_class_get  (slot->id3)->str,
                cos_class_get  (slot->id4)->str,
                cos_class_get  (slot->id5)->str);
        slot = slot->nxt;
      } while (slot->idg);

      fputc('\n', fp);

      n += j;
      if (j > d) d = j;
    }
  }

  fprintf(fp,"cache 5: slots=%u, cells=%u, load=%.2f, maxdepth=%u\n",i,n,(double)n/i,d);
}
