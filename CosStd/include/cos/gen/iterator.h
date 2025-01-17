#ifndef COS_GEN_ITERATOR_H
#define COS_GEN_ITERATOR_H

/*
 o---------------------------------------------------------------------o
 |
 | COS generics for iterators
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
 | $Id: iterator.h,v 1.2 2010/05/21 14:59:09 ldeniau Exp $
 |
*/

#include <cos/Object.h>

defgeneric(OBJ, giterator   , _1);
defgeneric(OBJ, grevIterator, _1); // reverse iterator
defgeneric(OBJ, gnext       , _1);
defgeneric(OBJ, gallNext    , _1); // return an array of remaining objects

#endif // COS_GEN_ITERATOR_H
