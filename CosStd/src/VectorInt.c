/*
 o---------------------------------------------------------------------o
 |
 | COS IntVector - basic vectors
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
 | useful, but WITHOUT ANY WARRANTY; without even the implied wstranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: VectorInt.c,v 1.2 2010/06/13 20:24:46 ldeniau Exp $
 |
*/

#define INTVECTOR_ONLY

#define T     IntVector
#define TM   mIntVector
#define TP  pmIntVector
#define TN    IntVectorN
#define TF    IntVectorFix
#define TD    IntVectorDyn
// #define TL    IntVectorLzy
#define TV    IntVectorView
#define TW    IntVectorSubView
#define TS    "int vector"
#define TE    Int

#define TF1   IntFunction1
#define TF2   IntFunction2
#define TF3   IntFunction3
#define TF4   IntFunction4
#define TF5   IntFunction5

#define PF1   I32FCT1
#define PF2   I32FCT2
#define PF3   I32FCT3
#define PF4   I32FCT4
#define PF5   I32FCT5

#define VS    Vector.ValueSequence.Sequence

#define VAL       I32
#define VALOBJ(v) aInt(v)

#define aTView(v,s)    aIntVectorView(v,s)
#define aTRef(v,s)     aIntVectorRef(v,s)

#define T_alloc(s)        IntVector_alloc(s)
#define TV_init(vw,v,s,b) IntVectorView_init(vw,v,s,b)

#define TOVALPTR      gintPtr
#define TOVAL(v)      gint(v)
#define VALUE(v)      v->value
#define EQUAL(v1,v2)  (v1 == v2)

#include <cos/IntVector.h>
#include <cos/Function.h>

makclass(IntVector, IntegralVector);

// vector templates
#include "./tmpl/Vector_utl.h"

#include "./tmpl/Vector.c"

#include "./tmpl/Vector_dyn.c"
// #include "./tmpl/Vector_lzy.c"
#include "./tmpl/Vector_vw.c"

#include "./tmpl/Vector_acc.c"
#include "./tmpl/Vector_alg.c"
#include "./tmpl/Vector_fun.c"
// #include "./tmpl/Vector_fct.c"
// #include "./tmpl/Vector_vfn.c"

