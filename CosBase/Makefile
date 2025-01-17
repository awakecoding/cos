#
# o---------------------------------------------------------------------o
# |
# | COS Base Library
# |
# o---------------------------------------------------------------------o
# |
# | C Object System
# |
# | Copyright (c) 2007+ Laurent Deniau, laurent.deniau@cern.ch
# |
# | For more information, see:
# | http://cern.ch/laurent.deniau/cos.html
# |
# o---------------------------------------------------------------------o
# |
# | This file is part of the C Object System framework.
# |
# | The C Object System is free software; you can redistribute it and/or
# | modify it under the terms of the GNU Lesser General Public License
# | as published by the Free Software Foundation; either version 3 of
# | the License, or (at your option) any later version.
# |
# | The C Object System is distributed in the hope that it will be
# | useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# | of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# |
# | See <http://www.gnu.org/licenses> for more details.
# |
# o---------------------------------------------------------------------o
# |
# | $Id: Makefile,v 1.37 2011/11/03 09:32:51 ldeniau Exp $
# |
#

# bootstrap, normally $(cos) = cos
cos := include/cos

#
# standard COS makefile
#

include $(cos)/prologue

# project
library := CosBase
version := 0.9
release := 201003
package := $(library)-$(version)_$(release)

# targets
targets := all

# files and modules
sources := src/*.c
headers := include/cos/*.h include/cos/*/*.h
defgens := include/cos/gen/*.h
defprps := include/cos/prp/*.h

# project dependencies (as with -lname)
libdeps := $(SYSLIBS)
moddeps :=

# project dependencies (as with -Ipath or -Lpath)
incdirs := src include
bindirs :=
libdirs :=
moddirs :=

# documentation
auxdocs :=

# patterns (clean, install-excluded, tar-excluded)
cleanpat :=
tarexpat :=
insexpat :=

# reload makefile (bootstrap)
ifeq ($(filter clean% tarball% uninstall%,$(MAKECMDGOALS)),)
DOIT := $(if $(BOOTSTRAP),,$(shell $(MAKE) -R bootstrap BOOTSTRAP=yes))
endif

include $(cos)/epilogue

# end of (standard) makefile

#
# COS boostrap
# install tools cosgen, cosprp, coscmt and cossym before anything else is done.
#
.PHONY: bootstrap

bootstrap: $(OSNAME)/bin/coscmt$(BINEXT)
	$_ cp -fP bin/cosgen$(BINEXT) $(dir $<)
	$_ cp -fP bin/cosprp$(BINEXT) $(dir $<)
	$_ cp -fP bin/cossym$(BINEXT) $(dir $<)

$(OSNAME)/bin/coscmt$(BINEXT): CCFLAGS := $(CCFLAGS)
$(OSNAME)/bin/coscmt$(BINEXT): tools/src/coscmt.c
	$_ $(call mkdir,$(dir $@)) 
	$_ $(CC) $(CCFLAGS) $< -o $@
	$_ $(if $(STRIP),$(STRIP) $@)

