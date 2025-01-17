/*
 o---------------------------------------------------------------------o
 |
 | COS example01 -- File class
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
 | $Id: File.c,v 1.2 2009/03/25 16:50:08 ldeniau Exp $
 |
*/

#include "File.h"
#include "String.h"
#include "generics.h"
#include <cos/errno.h>

makclass(File, Stream);

defmethod(OBJ, ginitWithStr, File, (STR)str)
  self->file = str ? fopen(str,"r+") : stdout;
  if (!self->file)
    test_errno(); // THROW
  
  retmethod(_1);
endmethod

defmethod(OBJ, gdeinit, File)
  if (self->file)
    fclose(self->file);

  retmethod(_1);
endmethod

defmethod(void, gprint, File, String)
  fputs(self2->str, self1->file);
endmethod

// extend the class Stream to support File creation

useclass(File);

defmethod(OBJ, ginitWithStr, pmStream, (STR)str)
  retmethod( gnewWithStr(File,str) );
endmethod

