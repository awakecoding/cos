/*
 o---------------------------------------------------------------------o
 |
 | COS cache for method2
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
 | $Id: cos_dispatch2.c,v 1.13 2010/05/26 23:35:06 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/gen/message.h>

#include <stdlib.h>

/* NOTE-CONF: Method dispatcher cache max slots (Method lookup)
 * The slots specifies the maximum number of entries for each cache.
 * This number must be of the form 2^n > 512.
 * Number of slots increase upon collision until maxslot is reached.
 * The maxdepth (e.g. memory) of each slot is fixed to 3.
 */

#ifndef COS_METHOD_MAXSLOT2
#define COS_METHOD_MAXSLOT2 262144
#endif

// test for 2^n > 512
STATIC_ASSERT(COS_METHOD_MAXSLOT2_must_be_a_pow2_greater_than_512,
              COS_METHOD_MAXSLOT2 > 512 &&
              (COS_METHOD_MAXSLOT2 & (COS_METHOD_MAXSLOT2-1)) == 0);

static void init(SEL,OBJ,OBJ,void*,void*);

static struct cos_method_slot2 sentinel = { &sentinel,init,0,0,0 };
static struct cos_method_slot2 *cache_empty = &sentinel;

#if COS_HAVE_TLS || !COS_HAVE_POSIX // -----------------------------

__thread struct cos_method_cache2 cos_method_cache2_ = { &cache_empty, 0, 0, 0 };

#else // COS_HAVE_POSIX && !COS_HAVE_TLS ---------------------------

       int            cos_method_cache2_key_init = 0;
       pthread_key_t  cos_method_cache2_key;
static pthread_once_t cos_method_cache2_key_once = PTHREAD_ONCE_INIT;

static void
make_key(void)
{
  if ( pthread_key_create(&cos_method_cache2_key, free) )
    cos_abort("unable to initialize dispatcher cache2");
}

struct cos_method_cache2*
cos_method_cache2_init(void)
{
  struct cos_method_cache2 *cache;

  pthread_once(&cos_method_cache2_key_once, make_key);
  cos_method_cache2_key_init = 1;
 
  if (!(cache = malloc(sizeof *cache)))
	  cos_abort("out of memory while creating dispatcher cache2");

	cache->slot = &cache_empty;
	cache->msk  = 0;
	cache->mis  = 0;
	cache->mis2 = 0;

  if ( pthread_setspecific(cos_method_cache2_key, cache) )
	  cos_abort("unable to initialize dispatcher cache2");

  return cache;
}

#endif // ------------------------------------------------

static void
init(SEL _sel, OBJ _1, OBJ _2, void *_arg, void *_ret)
{
  cos_init();
  forward_message(_1,_2);
}

static inline U32
first_cell(struct cos_method_slot2 **slot)
{
  struct cos_method_slot2 *sl = *slot;
  U32 n = 0;

  while (sl != &sentinel) {
    if (sl < *slot) *slot = sl;
    sl = sl->nxt, ++n;
  }

  return n;
}

static void
enlarge_slot(struct cos_method_slot2 **slot)
{
  U32 i, n = first_cell(slot);

  *slot = realloc(n ? *slot : 0, (n+1) * sizeof **slot);
  if (!*slot)
    cos_abort("method2_lookup: out of memory");

  for (i = 0; i < n+1; i++) {
    (*slot)[i].nxt = *slot+i+1; // Ok: see C99 6.5.6-8
    (*slot)[i].fct = init;
    (*slot)[i].idg = 0;
    (*slot)[i].id1 = 0;
    (*slot)[i].id2 = 0;
  }

  (*slot)[n].nxt = &sentinel;
}

static void
enlarge_cache(void)
{
	struct cos_method_cache2 *cache = cos_method_cache2();
  U32 i, n;

  n = cache->msk ? (cache->msk+1)*2 : 512;
  cos_method_clearCache2();

  cache->slot = malloc(n * sizeof *cache->slot);
  if (!cache->slot)
    cos_abort("method2_lookup: out of memory");

  for (i = 0; i < n; i++)
    cache->slot[i] = &sentinel;

  cache->msk = n-1;
}

static struct cos_method_slot2**
load_method(SEL _sel, U32 id1, U32 id2, BOOL load)
{
	struct cos_method_cache2 *cache;
  struct cos_method_slot2 **slot;
  IMP2 fct;
  U32 key;

  fct = cos_method_get2(_sel,id1,id2);
  if (!fct) {
    if (!load) return 0;
    fct = cos_method_get2(genericref(gunrecognizedMessage2), id1, id2);
    if (!fct)
      cos_abort("method2_lookup: %s not found for classes (%s,%s)",
                "gunrecognizedMessage2",
                cos_class_get(id1)->str, cos_class_get(id2)->str);
  }

  // get slot
  key  = cos_method_hkey2(_sel->Behavior.id,id1,id2);
	cache = cos_method_cache2();
  slot  = cache->slot + (key & cache->msk);

  // get cell
  if (cache->msk < COS_METHOD_MAXSLOT2-1) {
    if (*slot != &sentinel || slot == &cache_empty) {
      // try to ensure O(1) until MAXSLOT is reached
      enlarge_cache();
      slot = cache->slot + (key & cache->msk);
    }
    // allocate 1st cell
    enlarge_slot(slot);

  } else {
    // 1st cell is never free
    if ((*slot)->nxt->fct == 0) {
      // 2nd cell exists and is not used
      struct cos_method_slot2 *tmp = *slot;
      *slot = tmp->nxt, tmp->nxt = (*slot)->nxt, (*slot)->nxt = tmp;

    } else if ((*slot)->nxt->nxt != &sentinel) {
      // 3rd cell exists and may be used (then forget)
      struct cos_method_slot2 *tmp = *slot;
      *slot = tmp->nxt->nxt, tmp->nxt->nxt = (*slot)->nxt, (*slot)->nxt = tmp;
      if (!++cache->mis) cache->mis2++;

    } else
      // allocate one more cell
      enlarge_slot(slot);
  }

  // load fct in the cache
  (*slot)->fct = fct;
  (*slot)->idg = _sel->Behavior.id;
  (*slot)->id1 = id1;
  (*slot)->id2 = id2;

  return slot;
}

#define CACHE_GET_SLOT() \
  U32 key = cos_method_hkey2(_sel->Behavior.id,id1,id2); \
	struct cos_method_cache2 *restrict cache = cos_method_cache2(); \
  struct cos_method_slot2 *restrict*restrict slot = cache->slot + (key & cache->msk);

#define CACHE_MTH_LOAD(LOAD) \
  slot = load_method(_sel,id1,id2,LOAD);

#define CACHE_TST_LEVEL1() \
  if (_sel->Behavior.id  == (*slot)->idg && \
                     id1 == (*slot)->id1 && \
                     id2 == (*slot)->id2)   \
    goto ret;

#define CACHE_TST_LEVEL2() \
  if (_sel->Behavior.id  == (*slot)->nxt->idg && \
                     id1 == (*slot)->nxt->id1 && \
                     id2 == (*slot)->nxt->id2) { \
    struct cos_method_slot2 *tmp = *slot; \
    *slot = tmp->nxt, tmp->nxt = (*slot)->nxt, (*slot)->nxt = tmp; \
    goto ret; \
  }

#define CACHE_TST_LEVEL3() \
  if (_sel->Behavior.id  == (*slot)->nxt->nxt->idg && \
                     id1 == (*slot)->nxt->nxt->id1 && \
                     id2 == (*slot)->nxt->nxt->id2) { \
    struct cos_method_slot2 *tmp = *slot; \
    *slot = tmp->nxt->nxt, tmp->nxt->nxt = (*slot)->nxt, (*slot)->nxt = tmp; \
    goto ret; \
  }

IMP2
cos_method_lookup2(SEL restrict _sel,
                   U32 id1, U32 id2)
{
  CACHE_GET_SLOT();
  CACHE_TST_LEVEL1();
  CACHE_TST_LEVEL2();
  CACHE_TST_LEVEL3();
  CACHE_MTH_LOAD(YES);

 ret:
  return (*slot)->fct;
}

IMP2
cos_method_fastLookup2_(struct cos_method_slot2 *restrict*restrict slot,
                        SEL restrict _sel,
			                  U32 id1, U32 id2)
{
  CACHE_TST_LEVEL2();
  CACHE_TST_LEVEL3();
  CACHE_MTH_LOAD(YES);

 ret:
  return (*slot)->fct;
}

BOOL
cos_method_understand2_(struct cos_method_slot2 *restrict*restrict slot,
                        SEL restrict _sel,
			                  U32 id1, U32 id2)
{
  CACHE_TST_LEVEL2();
  CACHE_TST_LEVEL3();
  CACHE_MTH_LOAD(NO);

 ret:
  return slot != 0;
}

void
cos_method_clearCache2(void)
{
	struct cos_method_cache2 *cache = cos_method_cache2();
  U32 i;

  if (cache->slot != &cache_empty) {
    for (i = 0; i <= cache->msk; i++) {
      struct cos_method_slot2 **slot = cache->slot+i;

      if (*slot != &sentinel)
        first_cell(slot), free(*slot);
    }

    free(cache->slot);
    cache->slot = &cache_empty;
    cache->msk  = 0;
    cache->mis  = 0;
    cache->mis2 = 0;
  }
}
