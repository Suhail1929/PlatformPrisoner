#
# MAIN CONFIGURATION
#

EXEC = editor client #server
OBJECTS = window.o interface.o fonction.o entity.o couleur.o data_table.o interface_client.o item.o liste.o cellule.o
PROJECT_NAME = PlateformPrisoner

SRC_DIR = src
OBJECTS_DIR = obj
INCLUDE_DIR = includes
BIN_DIR = bin

#
# SUFFIXES (must not change it)
#

.SUFFIXES: .c .o

#
# OBJECTS (must not change it)
#

EXEC_O = $(EXEC:=.o)
OBJECTS_O = $(OBJECTS) $(EXEC_O)

#
# ARGUMENTS AND COMPILER (to configure)
#

CC = gcc
GDBFLAGS = -g
CCFLAGS_STD = -Wall -O3 -Werror
CCFLAGS_DEBUG = -D _DEBUG_ 
CCFLAGS = $(CCFLAGS_STD)
CCLIBS = -lncurses -pthread

#
# RULES (must not change it)
#

all: msg $(addprefix $(OBJECTS_DIR)/,$(OBJECTS)) $(addprefix $(OBJECTS_DIR)/,$(EXEC_O))
	@echo "Create executables..."
	@for i in $(EXEC); do \
	$(CC) -o $(addprefix $(BIN_DIR)/,$$i) $(addprefix $(OBJECTS_DIR)/,$$i.o) $(addprefix $(OBJECTS_DIR)/,$(OBJECTS)) $(CCLIBS) -I../$(INCLUDE_DIR)/; \
	done
	@echo "Done."

msg:
	@echo "Create objects..."

debug: CCFLAGS = $(CCFLAGS_STD) $(CCFLAGS_DEBUG) $(GDBFLAGS)
debug: all

#
# DEFAULT RULES (must not change it)
#

$(addprefix obj/,%.o) : $(addprefix src/,%.c)
	@${CC} ${CCFLAGS} -c $< -o $@ -I$(INCLUDE_DIR)/

#
# MAIN RULES (must not change it)
#

# You can add your own commands
clean:
	@echo "Delete objects, temporary files..."
	@rm -f $(addprefix $(OBJECTS_DIR)/,$(OBJECTS_O))
	@rm -f $(addprefix $(OBJECTS_DIR)/,$(EXEC_O))
	@rm -f $(addprefix $(OBJECTS_DIR)/,*~) $(addprefix $(OBJECTS_DIR)/,*#)
	@rm -f $(addprefix $(INCLUDE_DIR)/,*~) $(addprefix $(INCLUDE_DIR)/,*#)
	@rm -f $(addprefix $(BIN_DIR)/,$(EXEC))
	@rm -f dependancies
	@echo "Done."

depend:
	@echo "Create dependancies..."
	@sed -e "/^# DEPENDANCIES/,$$ d" makefile > dependancies
	@echo "# DEPENDANCIES" >> dependancies
	@for i in $(OBJECTS_O); do \
	$(CC) -MM -MT $(addprefix $(OBJECTS_DIR)/,$$i) $(CCFLAGS) $(SRC_DIR)/`echo $$i | sed "s/\(.*\)\\.o$$/\1.c/"` -I$(INCLUDE_DIR) >> dependancies; \
	done
	@cat dependancies > makefile
	@rm dependancies
	@echo "Done."

#
# CREATE ARCHIVE (must not modify)
#

ARCHIVE_FILES = *

archive: clean
	@echo "Create archive $(PROJECT_NAME)_$(shell date '+%y%m%d.tar.gz')..."
	@REP=`basename "$$PWD"`; cd .. && tar zcf $(PROJECT_NAME)_$(shell date '+%y%m%d.tar.gz') $(addprefix "$$REP"/,$(ARCHIVE_FILES))
	@echo "Done."

# DEPENDANCIES
obj/window.o: src/window.c includes/window.h
obj/interface.o: src/interface.c includes/fonction.h includes/couleur.h \
  includes/window.h includes/data_table.h includes/item.h \
  includes/liste.h includes/cellule.h includes/interface.h \
  includes/entity.h
obj/fonction.o: src/fonction.c includes/fonction.h
obj/entity.o: src/entity.c includes/couleur.h includes/window.h \
  includes/data_table.h includes/item.h includes/liste.h \
  includes/cellule.h includes/interface.h includes/entity.h
obj/couleur.o: src/couleur.c includes/couleur.h
obj/data_table.o: src/data_table.c includes/data_table.h
obj/interface_client.o: src/interface_client.c includes/fonction.h \
  includes/couleur.h includes/window.h includes/data_table.h \
  includes/item.h includes/liste.h includes/cellule.h \
  includes/interface.h includes/entity.h
obj/item.o: src/item.c includes/couleur.h includes/window.h \
  includes/item.h includes/liste.h includes/cellule.h \
  includes/data_table.h includes/interface.h
obj/liste.o: src/liste.c includes/window.h includes/item.h \
  includes/cellule.h includes/liste.h
obj/cellule.o: src/cellule.c includes/window.h includes/item.h \
  includes/liste.h includes/cellule.h
obj/editor.o: src/editor.c includes/fonction.h includes/couleur.h \
  includes/window.h includes/data_table.h includes/item.h \
  includes/liste.h includes/cellule.h includes/interface.h
obj/client.o: src/client.c includes/fonction.h includes/couleur.h \
  includes/window.h includes/data_table.h includes/item.h \
  includes/liste.h includes/cellule.h includes/interface.h
