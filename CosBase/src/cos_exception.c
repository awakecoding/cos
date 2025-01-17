/*
 o---------------------------------------------------------------------o
 |
 | COS exception
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
 | $Id: cos_exception.c,v 1.33 2010/06/08 09:05:28 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/Exception.h>
#include <cos/debug.h>

#include <cos/gen/object.h>
#include <cos/gen/value.h>
#include <cos/gen/message.h>

#include <stdlib.h>

static struct cos_exception_context _cxt0;

#if COS_HAVE_TLS || !COS_HAVE_POSIX // -----------------------------

__thread struct cos_exception_context *cos_exception_cxt_ = &_cxt0;

static inline void
cxt_set(struct cos_exception_context *cxt)
{
  cos_exception_cxt_ = cxt;
}

#else // COS_HAVE_POSIX && !COS_HAVE_TLS ---------------------------

static int            cxt_key_init = 0;
static pthread_key_t  cxt_key;
static pthread_once_t cxt_key_once = PTHREAD_ONCE_INIT;

static void
cxt_set(struct cos_exception_context *cxt)
{
	if( pthread_setspecific(cxt_key, cxt) )
    cos_abort("unable to set exception context");
}

static void
make_key(void)
{
  if ( pthread_key_create(&cxt_key, 0) )
    cos_abort("unable to initialize exceptions");
}

static struct cos_exception_context*
cxt_init(void)
{
  pthread_once(&cxt_key_once, make_key);
  cxt_key_init = 1;
  cxt_set(&_cxt0);
  return &_cxt0;
}

struct cos_exception_context*
cos_exception_context(void)
{
  struct cos_exception_context *cxt;
  
  if (! cxt_key_init ||
      !(cxt = pthread_getspecific(cxt_key)))
      cxt = cxt_init();

  return cxt;
}
  
#endif // ------------------------------------------------

static void
unwind_stack(struct cos_exception_context *cxt)
{
  struct cos_exception_protect *p = cxt->stk;

  cxt->unstk = YES;

  for (; p; p = p->prv) {
    if (p->obj == (OBJ*)YES) {
      struct cos_exception_extendedProtect *ep = (void*)p;

      if (*ep->alt && ep->fct)
        ep->fct(*ep->alt);
    }
    else if (*p->obj)
      grelease(*p->obj);
  }

  cxt->unstk = NO;
  cxt->stk   = 0;
}

static void
unwind_funstack(struct cos_exception_context *cxt)
{
  struct cos_functor_context *fcxt = cos_functor_context();

  fcxt->top = fcxt->stk + cxt->fss;
}

static void
terminate_default(OBJ ex, STR file, int line)
{
  STR reason = cos_object_isKindOf(ex, classref(Exception)) ? gstr(ex) : ".";

  if (cos_exception_uncaught() == YES)
    cos_abort("exception %s (%s) thrown at (%s:%d) during stack unwinding "
              "leading to an undefined behavior",
              cos_object_className(ex), reason, file, line);
  else
    cos_info ("exiting with uncaught exception %s '%s' thrown at (%s:%d)",
              cos_object_className(ex), reason, file, line);
}

static cos_exception_handler handler = terminate_default;

static void __attribute__((__noreturn__))
terminate(void)
{
  if (handler) {
    struct cos_exception_context *cxt = cos_exception_context();
    handler(cxt->ex, cxt->file, cxt->line);
  }
	
  exit(EXIT_FAILURE);
}

// ----- low-level API

BOOL
cos_exception_catch(OBJ ex, OBJ cls)
{
  return gisKindOf(ex,cls) == True;
}

void
cos_exception_throw(OBJ ex, STR file, int line)
{
  if (!ex) ex = Nil;
  
  gthrow(ex, file, line);

  terminate(); // safer if an overridden gthrow returns.
}

BOOL
cos_exception_uncaught(void)
{
  return cos_exception_context()->unstk;
}

void
cos_exception_initContext(struct cos_exception_context *cxt)
{
  struct cos_functor_context *fcxt = cos_functor_context();

  cxt->prv   = cos_exception_context();
  cxt->stk   = 0;
  cxt->unstk = 0;
  cxt->ex    = 0;
  cxt->fss   = fcxt->top - fcxt->stk;

  cxt_set(cxt);
}

void
cos_exception_deinitContext(struct cos_exception_context *cxt)
{
	cxt_set(cxt->prv);

	if ( (cxt->tag & cos_tag_throw) ) {
    cxt->prv->ex = cxt->ex;
		cos_exception_throw(cxt->ex, cxt->file, cxt->line); // rethrow
  }

	if (cxt->ex)
	  grelease(cxt->ex), cxt->ex = 0;
}

// -----

cos_exception_handler
cos_exception_setTerminate(cos_exception_handler new)
{
  cos_exception_handler old = handler;
  handler = new;
  return old;
}

// ----- methods

defmethod(void, gthrow, Object, (STR)file, (int)line)

  struct cos_exception_context *cxt = cos_exception_context();

  OBJ ex = cxt->ex;
  cxt->ex = gretain(_1);
  if (ex) grelease(ex);
  
  cxt->file = file ? file : "";
  cxt->line = line;

  if (cxt->unstk == YES)
    terminate();

  unwind_stack(cxt);
  unwind_funstack(cxt);

  if (cxt == &_cxt0)
    terminate();

  cos_exception_lngjmp(cxt->buf, cxt->tag | cos_tag_throw);
endmethod

defmethod(void, gdeinitialize, pmException)
  struct cos_exception_context *cxt = cos_exception_context();

  if (cxt->ex)
    grelease(cxt->ex), cxt->ex = 0;
endmethod

/*
 * ----------------------------------------------------------------------------
 *  Debug Functions
 * ----------------------------------------------------------------------------
 */

void
cos_exception_showProtectionStack(FILE *fp)
{
  struct cos_exception_protect *p = cos_exception_context()->stk;
  U32 i = 0;

  if (!fp) fp = stderr;

  while (p) {
    fprintf(fp, "prt[%4u] = %-25s\n",
            i, *p->obj ? cos_object_className(*p->obj) : "NIL");
    ++i, p = p->prv;
  }
}
