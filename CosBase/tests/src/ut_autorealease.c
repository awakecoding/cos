/*
 o---------------------------------------------------------------------o
 |
 | COS testsuites - autorelease
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
 | useful, but WITHOUTEST ANY WARRANTY; without even the implied warranty
 | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 |
 | See <http://www.gnu.org/licenses> for more details.
 |
 o---------------------------------------------------------------------o
 |
 | $Id: ut_autorealease.c,v 1.13 2010/05/28 08:53:08 ldeniau Exp $
 |
*/

#include <cos/Object.h>
#include <cos/gen/object.h>
#include <cos/gen/value.h>
#include <cos/signal.h>
#include <cos/utest.h>

#include <string.h>

#include "tests.h"
#include "generics.h"

void
ut_autorelease(void)
{
  useclass(A, AutoRelease);
  OBJ a, c;
  volatile size_t i;
  volatile OBJ ar, ar1, ar2, ar3, ar4;
  static OBJ arr[1000];
  
  UTEST_START("reference counting & autorelease")

    // ----
    a = gnew(A);
    c = gclone(a);
    UTEST( gretainCount(a) == 1 );
    UTEST( gretainCount(c) == 1 );
    UTEST( (grelease(a), 1) );
    UTEST( (grelease(c), 1) );

    // ----
    a = gretain(gnew(A));
    UTEST( gretainCount(a) == 2  );
    UTEST( (grelease(a), 1) );
    UTEST( gretainCount(a) == 1  );
    UTEST( (grelease(a), 1) );

    // ----
    a = gretain(gnew(A));
    UTEST( gretainCount(a) == 2  );
    UTEST( (grelease(a), 1) );
    UTEST( gretainCount(a) == 1  );
    UTEST( (grelease(a), 1) );

    // ----
    ar = gnew(AutoRelease);
    a = gretain(gnew(A));
    UTEST( gretainCount(a) == 2  );
    gautoRelease(a);
    UTEST( gretainCount(a) == 2  );
    grelease(ar);
    UTEST( gretainCount(a) == 1  );
    UTEST( (grelease(a), 1) );

    // ----
    ar = gnew(AutoRelease);
    a = gretain(gnew(A));
    UTEST( gretainCount(a) == 2  );
    gautoRelease(a);
    UTEST( gretainCount(a) == 2  );
    grelease(ar);
    UTEST( gretainCount(a) == 1  );
    UTEST( (grelease(a), 1) );

    // ----
    ar = gnew(AutoRelease);
    a = gautoRelease(gnew(A));
    UTEST( gsize(ar) == 1 );
    UTEST( gretainCount(a) == 1  );
    a = gretain(a);
    UTEST( gsize(ar) == 1 );
    UTEST( gretainCount(a) == 2  );
    a = gretain(a);
    UTEST( gsize(ar) == 1 );
    UTEST( gretainCount(a) == 3  );
    grelease(ar);
    UTEST( gretainCount(a) == 2  );
    UTEST( (grelease(a), 1) );
    UTEST( gretainCount(a) == 1  );
    UTEST( (grelease(a), 1) );

    // ----
    ar = gnew(AutoRelease);
    a = gautoRelease(gnew(A));
    c = gautoRelease(gnew(A));
    UTEST( gsize(ar) == 2 );
    a = gretain(a);
    UTEST( gsize(ar) == 2 );
    UTEST( gretainCount(a) == 2  );
    c = gretain(c);
    UTEST( gsize(ar) == 2 );
    UTEST( gretainCount(c) == 2  );
    grelease(ar);
    UTEST( gretainCount(a) == 1  );
    UTEST( gretainCount(c) == 1  );
    UTEST( (grelease(a), 1) );
    UTEST( (grelease(c), 1) );

    // ----
    ar = gnew(AutoRelease);
    for (i = 1; i <= 1000; i++) {
      size_t s;
      gautoRelease(gnew(A));
      s = gsize(ar);
      if (!(s & (s-1))) // check every 2^n
        UTEST( s == i );
    }
    grelease(ar);

    // ----
    ar = gnew(AutoRelease);
    ar1 = ar2 = ar3 = ar4 = 0;
    TRY
      ar1 = gnew(AutoRelease);
      for (i = 0; i < 100; i++)
        arr[i] = gautoRelease(gnew(A));
      TRY
        ar2 = gnew(AutoRelease);
        for (; i < 300; i++)
          arr[i] = gautoRelease(gnew(A));
        TRY
          ar3 = gnew(AutoRelease);
          for (; i < 600; i++)
            arr[i] = gautoRelease(gnew(A));
          TRY
             ar4 = gnew(AutoRelease);
             for (; i < 700; i++)
               gautoRelease(gnew(A));
             THROW(Nil);
          FINALLY
            UTEST( gsize(ar4) == 100 );
            grelease(ar4), ar4 = 0;
          ENDTRY
        FINALLY
          UTEST( gsize(ar3) == 300 );
        ENDTRY // rethrow Nil
      FINALLY
        UTEST( gsize(ar3) == 300 );
        UTEST( gsize(ar2) == 200 );
      ENDTRY // rethrow Nil
    CATCH_ANY() // catch Nil
    FINALLY
      UTEST( gsize(ar1) == 100 );
      UTEST( gsize(ar2) == 200 );
      UTEST( gsize(ar3) == 300 );
    ENDTRY
    UTEST( gsize(ar) == 0 );
    grelease(ar); // destroy all chained pools
    ar = ar1 = ar2 = ar3 = ar4 = 0;

  UTEST_END
}
