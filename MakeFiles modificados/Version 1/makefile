# Agregar bibliotecas necesarias acá (por ejemplo, -lm para incluir <math.h>) (-L lib se agrega sólo para Windows más abajo)
LIBS+=-lfl -ly -lm

# Detecta el SO
ifeq ($(OS),Windows_NT)
# Para Windows, genera un ejecutable *.exe
BINSEXT:=exe
# Para Windows, se agregan las bibliotecas compatibles que están en ./lib
LIBS+=-L lib
# Para Windows, agrega la bandera -m32 para el compilador de C
CFLAGS+=-m32
else
# En cualquier otro sistema operativo (GNU/Linux, Mac OS, etc.) se genera un archivo *.out
BINSEXT:=out
endif

# Ubicación de los *.l, *.c y *.h fuente (excepto los generados por LEX, YACC y CC)
SRCDIR:=src
# Ubicación de los objetos generados por LEX, YACC y CC
OBJDIR:=obj
# Ubicación de los binarios buildeados
BINDIR:=bin

# Analizador sintáctico a usar
YACC:=bison
# Agrega los flags que se le pasan a YACC (los flags -v y -d sí que son necesarios)
YFLAGS:=-v -d

# Analizador léxico a usar
LEX:=flex
# Agrega los flags que se le pasan a LEX
LFLAGS+=

# Compilador de C a usar
CC:=gcc
# Banderas para el compilador de C. -I agrega un directorio donde se encuentran includes. Opcionalmente se puede agregar -Wall, que sirve para mostrar los warnings (los flags -I$(SRCDIR) y -I$(OBJDIR) sí que son necesarios)
CFLAGS+=-I$(SRCDIR) -I$(OBJDIR)
# Banderas para el compilador de C que se suman a las demás sólo cuando haya YACC
CFLAGS4Y+=

# wildcard me busca todos los archivos que cumplan con la condición y los concatena
LSRCS:= $(wildcard $(SRCDIR)/*.l)
# Ej: src/Ejemplo1.l src/Ejemplo2.l ... src/Ejemplo7.l
DEPS:= $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.h)
# Ej: src/misfunciones.h

# $(patsubst param1, param2, param3) agarra param3 y le reemplaza una parte del string (param1) por otra (param2)
BINS:=$(patsubst $(SRCDIR)/%.l,$(BINDIR)/%.$(BINSEXT),$(LSRCS))
# Ej: src/Ejemplo1.l --> Ejemplo1.exe

# Mensaje de aviso que se muestra cada vez que se ejecute este makefile
WARNING:=IMPORTANTE: el comando make debe ejecutarse en una consola/terminal bash para que este makefile funcione correctamente (no anda en powershell, cmd, etc.)
$(info $(WARNING))

# Para crear reglas que no generen objetos de ese mismo nombre
.PHONY: all run open close clean debug

# Para generar todos los objetos necesarios, se usa la sig estructura:

# 	objeto_a_generar: dependencia1 dependencia2 ... dependenciaM
# 	<tab> comando_a_ejecutar1
# 	<tab> comando_a_ejecutar2
# 	...
# 	<tab> comando_a_ejecutarN

# Para buildear todos los binarios con sus SRCDIR/*.l como fuentes. Se ejecuta con 'make' o 'make all'
all: $(BINS)

# Para ejecutar los binarios que se buildean sucesivamente desde la misma consola
run:
	@echo ""
	@echo "=================[ Ejecutar sucesivamente si existiera/n el/los binario/s: $(BINS) ]================="
	@for BIN in $(BINS) ; do \
		if [ -f "$$BIN" ]; then \
			echo "" ; \
			echo "<<< Ejecutando el binario: $$BIN >>> " ; \
			cd $(BINDIR) && ./$${BIN/#'$(BINDIR)/'/} && cd .. ; \
			echo "<<< Binario $$BIN ejecutado. >>> " ; \
		fi \
	done
	@echo ""
	@echo "=================[ Completado ]============="
	
# Para abrir los binarios que se buildean sucesivamente en ventanas nuevas (por ahora sólo funciona en Windows)
open:
	@echo ""
	@echo "=================[ Abrir sucesivamente si existiera/n el/los binario/s: $(BINS) ]================="
	@if [ "$(OS)" == "Windows_NT" ]; then \
		for BIN in $(BINS) ; do \
			if [ -f "$$BIN" ]; then \
				echo "" ; \
				echo "<<< Abriendo el binario: $$BIN >>> " ; \
				cd $(BINDIR) && start $${BIN/#'$(BINDIR)/'/} && cd .. ; \
				echo "<<< Binario $$BIN abierto. >>> " ; \
			fi \
		done \
	fi
	@echo ""
	@echo "=================[ Completado ]============="
	
# Para cerrar los procesos de los binarios que se buildean sucesivamente (por ahora sólo funciona en Windows)
close:
	@echo ""
	@echo "=================[ Cerrar sucesivamente si hubiera/n proceso/s de: $(BINS) ]================="
	@if [ "$(OS)" == "Windows_NT" ]; then \
		for BIN in $(BINS) ; do \
			for PID in $$(wmic process where "ExecutablePath='$$(realPath $$BIN | sed 's/\/c\//C:\//' | sed 's/\//\\\\\\\\/g')'" get ProcessId /FORMAT:list 2>&1 | grep -o -E '[0-9]+' | tr '\n' ' ') ; do \
				echo "" ; \
				echo "<<< Cerrando el proceso: $$BIN con PID $$PID >>> " ; \
				taskkill //PID $$PID ; \
				echo "<<< Proceso $$BIN con PID $$PID cerrado. >>> " ; \
			done \
		done \
	fi
	@echo ""
	@echo "=================[ Completado ]============="

# Para "limpiar" (eliminar) todos los objetos y binarios generados al compilar
clean: close
	@echo ""
	@echo "=================[ Eliminar todo lo que se genera al compilar ]================="
	@echo ""
	@echo "<<< Eliminando si existiera/n el/los archivo/s: $(BINS) >>> "
	rm -f $(BINS)
	@echo "<<< Archivo/s $(BINS) existente/s eliminado/s. >>> "
	@echo ""
	@echo "<<< Eliminando si existiera/n el/los archivo/s: $(foreach BIN,$(BINS),$(BIN:$(BINDIR)/%.$(BINSEXT)=$(OBJDIR)/%.*)) >>> "
	rm -f $(foreach BIN,$(BINS),$(BIN:$(BINDIR)/%.$(BINSEXT)=$(OBJDIR)/%.*))
	@echo "<<< Archivo/s $(foreach BIN,$(BINS),$(BIN:$(BINDIR)/%.$(BINSEXT)=$(OBJDIR)/%.*)) existente/s eliminado/s. >>> "
	@echo ""
	@echo "<<< Eliminando los directorios $(BINDIR) y $(OBJDIR) si existieran y estuvieran vacios >>> "
	if [ -d "$(BINDIR)" ]; then rmdir --ignore-fail-on-non-empty $(BINDIR); fi
	if [ -d "$(OBJDIR)" ]; then rmdir --ignore-fail-on-non-empty $(OBJDIR); fi
	@echo "<<< Directorios $(BINDIR) y $(OBJDIR) eliminados en caso de haber estado vacios. >>> "
	@echo ""
	@echo "=================[ Completado ]============="

# Para agregar el flag de debug a YACC
debug: YFLAGS += --debug
# Para agregar el flag de debug a LEX
debug: LFLAGS += -d
# Para agregar los flags de debug a CC haya o no haya YACC
debug: CFLAGS += -DDEBUG -g
# Para agregar los flags de debug a CC sólo cuando haya YACC
debug: CFLAGS4Y += -DBISON_DEBUG
# Primero hace un make clean y luego un make all con todos los nuevos valores asignados a las variables de arriba
debug: clean all

# Para YACC + LEX + CC: Para buildear el binario
$(BINDIR)/%.$(BINSEXT): $(SRCDIR)/%.l $(SRCDIR)/%.y $(OBJDIR) $(BINDIR)
	@echo ""
	@echo "=================[ Build con $(YACC)+$(LEX)+$(CC): $@ ]================="
	@echo ""
	@echo "<<< $(YACC): Generando los archivos intermedios $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.tab.c,$@), $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.tab.h,$@) y $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.output,$@) >>>"
	$(YACC) $(YFLAGS) -o"$(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.tab.c,$@)" $(<:%.l=%.y)
	@echo "<<< Archivos intermedios $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.tab.c,$@), $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.tab.h,$@) y $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.output,$@) generados. >>>"
	@echo ""
	@echo "<<< $(LEX): Generando el archivo intermedio $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex+yacc.yy.c,$@) >>>"
	$(LEX) $(LFLAGS) -o"$(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex+yacc.yy.c,$@)" $<
	@echo "<<< Archivo intermedio $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex+yacc.yy.c,$@) generado. >>>"
	@echo ""
	@echo "<<< $(CC): Compilando y enlazando todo con libfl.a y liby.a >>>"
	$(CC) $(CFLAGS) $(CFLAGS4Y) $(patsubst $(SRCDIR)/%.l,$(OBJDIR)/%.tab.c,$<) $(patsubst $(SRCDIR)/%.l,$(OBJDIR)/%.tab.h,$<) $(DEPS) $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex+yacc.yy.c,$@) $(LIBS) -o $@
	@echo "<<< Archivo $@ buildeado. >>>"
	@echo ""
	@echo "=================[ Completado ]================="

# Para LEX + CC: Para buildear el binario
$(BINDIR)/%.$(BINSEXT): $(SRCDIR)/%.l $(OBJDIR) $(BINDIR)
	@echo ""
	@echo "=================[ Build con $(LEX)+$(CC): $@ ]================="
	@echo ""
	@echo "<<< $(LEX): Generando el archivo intermedio $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex.yy.c,$@) >>>"
	$(LEX) $(LFLAGS) -o"$(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex.yy.c,$@)" $<
	@echo "<<< Archivo $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex.yy.c,$@) generado. >>>"
	@echo ""
	@echo "<<< $(CC): Compilando y enlazando todo con libfl.a >>>"
	$(CC) $(CFLAGS) $(DEPS) $(patsubst $(BINDIR)/%.$(BINSEXT),$(OBJDIR)/%.lex.yy.c,$@) $(LIBS) -o $@
	@echo "<<< Archivo $@ buildeado. >>>"
	@echo ""
	@echo "=================[ Completado ]================="
	
# Para crear los directorios si no existieran
$(OBJDIR) $(BINDIR):
	@echo ""
	@echo "<<< Creando el directorio $@ >>>"
	mkdir -p $@
	@echo "<<< Directorio $@ creado. >>>"