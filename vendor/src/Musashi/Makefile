# SPDX-FileCopyrightText: 2023-present pfSimulator contributors
#
# SPDX-License-Identifier: GPL-3.0-or-later

MUSASHIFILES     = m68kcpu.c m68kdasm.c softfloat/softfloat.c
MUSASHIGENCFILES = m68kops.c
MUSASHIGENHFILES = m68kops.h
MUSASHIGENERATOR = m68kmake

EXE =
EXEPATH = ./

.CFILES   = $(MUSASHIFILES) $(MUSASHIGENCFILES)

CC        = gcc
WARNINGS  = -Wall -Wextra -pedantic
CFLAGS    = $(WARNINGS)
LFLAGS    = $(WARNINGS)

DELETEFILES = $(MUSASHIGENCFILES) $(MUSASHIGENHFILES) $(MUSASHIGENERATOR)$(EXE)

all: files
	# -- We can delete the generator once it's done its job.
	rm -f $(MUSASHIGENERATOR)$(EXE)

files: $(.CFILES)

clean:
	rm -f $(DELETEFILES)

install:
	# -- This is a dummy install target which is required by Xcode when archiving.
	$(info Done.)

m68kcpu.o: $(MUSASHIGENHFILES) m68kfpu.c m68kmmu.h softfloat/softfloat.c softfloat/softfloat.h

$(MUSASHIGENCFILES) $(MUSASHIGENHFILES): $(MUSASHIGENERATOR)$(EXE)
	$(EXEPATH)$(MUSASHIGENERATOR)$(EXE)

$(MUSASHIGENERATOR)$(EXE):  $(MUSASHIGENERATOR).c
	$(CC) -o  $(MUSASHIGENERATOR)$(EXE)  $(MUSASHIGENERATOR).c
