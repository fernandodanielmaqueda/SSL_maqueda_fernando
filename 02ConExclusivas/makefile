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

# wildcard me busca todos los archivos que cumplan con la condición y los concatena
YOBJS=$(patsubst $(SRCDIR)/%.y, $(OBJDIR)/%.tab.o, $(wildcard $(SRCDIR)/*.y))
LOBJS=$(patsubst $(SRCDIR)/%.l, $(OBJDIR)/%.yy.o, $(wildcard $(SRCDIR)/*.l))
# Ej: src/Ejemplo1.l src/Ejemplo2.l ... src/Ejemplo7.l
COBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.c))

# $(patsubst param1, param2, param3) agarra param3 y le reemplaza una parte del string (param1) por otra (param2)
BINS:=$(patsubst $(SRCDIR)/%.l,$(BINDIR)/%.$(BINSEXT),$(wildcard $(SRCDIR)/*.l))
# Ej: src/Ejemplo1.l --> Ejemplo1.exe

# Mensaje de aviso que se muestra cada vez que se ejecute este makefile
WARNING:=IMPORTANTE: el comando make debe ejecutarse en una consola/terminal bash para que este makefile funcione correctamente (no anda en powershell, cmd, etc.)
$(info $(WARNING))

# Para crear reglas que no generen objetos de ese mismo nombre
.PHONY: warnings-on warnings-off all run open close clean debug

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
	@echo "<<< Eliminando si existiera/n el/los archivo/s: $(COBJS) $(foreach BIN,$(BINS),$(BIN:$(BINDIR)/%.$(BINSEXT)=$(OBJDIR)/%.*)) >>> "
	rm -f $(COBJS) $(foreach BIN,$(BINS),$(BIN:$(BINDIR)/%.$(BINSEXT)=$(OBJDIR)/%.*))
	@echo "<<< Archivo/s $(COBJS) $(foreach BIN,$(BINS),$(BIN:$(BINDIR)/%.$(BINSEXT)=$(OBJDIR)/%.*)) existentes eliminado/s. >>> "
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

# Para que no elimine los archivos secundarios (aquellos que son creados por regla prerequisito de otra regla) una vez buildeado
.SECONDARY: $(BINS) $(COBJS) $(YOBJS) $(YOBJS:%.o=%.c) $(YOBJS:%.o=%.h) $(YOBJS:%.tab.o=%.output) $(LOBJS:%.yy.o=%.lex+yacc.yy.o) $(LOBJS:%.yy.o=%.lex+yacc.yy.c) $(LOBJS:%.yy.o=%.lex.yy.o) $(LOBJS:%.yy.o=%.lex.yy.c)

# Para cada binario: verifica que su archivo $(SRCDIR)/%.l exista, crea los directorios si no existen, cierra los procesos abiertos del binario si los hubiera, anuncia que se va a buildear, lo buildea y por último muestra que se completó 
$(BINDIR)/%.$(BINSEXT): $(SRCDIR)/%.l $(OBJDIR) $(BINDIR) $(OBJDIR)/%.close.temp $(OBJDIR)/%.build.temp $(OBJDIR)/%.bin.temp $(OBJDIR)/%.done.temp ;

# Para crear los directorios si no existieran
$(OBJDIR) $(BINDIR):
	@echo ""
	@echo "<<< Creando el directorio $@ >>>"
	mkdir -p $@
	@echo "<<< Directorio $@ creado. >>>"
	
# Para cerrar los procesos abiertos del binario si los hubiera
$(OBJDIR)/%.close.temp: $(SRCDIR)/%.l
	@echo ""
	@echo "=================[ Cerrar sucesivamente si hubiera/n proceso/s de: $(@:$(OBJDIR)/%.close.temp=$(BINDIR)/%.$(BINSEXT)) ]================="
	@if [ "$(OS)" == "Windows_NT" ]; then \
		for PID in $$(wmic process where "ExecutablePath='$$(realPath $(@:$(OBJDIR)/%.close.temp=$(BINDIR)/%.$(BINSEXT)) | sed 's/\/c\//C:\//' | sed 's/\//\\\\\\\\/g')'" get ProcessId /FORMAT:list 2>&1 | grep -o -E '[0-9]+' | tr '\n' ' ') ; do \
			echo "" ; \
			echo "<<< Cerrando el proceso: $(@:$(OBJDIR)/%.close.temp=$(BINDIR)/%.$(BINSEXT)) con PID $$PID >>> " ; \
			taskkill //PID $$PID ; \
			echo "<<< Proceso $(@:$(OBJDIR)/%.close.temp=$(BINDIR)/%.$(BINSEXT)) con PID $$PID cerrado. >>> " ; \
		done \
	fi
	@echo ""
	@echo "=================[ Completado ]================="

# Para YACC + LEX + CC: Para anunciar que se va a iniciar el build
$(OBJDIR)/%.build.temp: $(SRCDIR)/%.l $(SRCDIR)/%.y $(SRCDIR)/%.l $(OBJDIR)/%.close.temp
	@echo ""
	@echo "=================[ Build con $(YACC)+$(LEX)+$(CC): $(@:$(OBJDIR)/%.build.temp=$(BINDIR)/%.$(BINSEXT)) ]================="

# Para LEX + CC: Para anunciar que se va a iniciar el build
$(OBJDIR)/%.build.temp: $(SRCDIR)/%.l $(OBJDIR)/%.close.temp
	@echo ""
	@echo "=================[ Build con $(LEX)+$(CC): $(@:$(OBJDIR)/%.build.temp=$(BINDIR)/%.$(BINSEXT)) ]================="

# Para YACC + LEX + CC: Para buildear el binario
$(OBJDIR)/%.bin.temp: $(SRCDIR)/%.l $(SRCDIR)/%.y $(OBJDIR)/%.build.temp $(COBJS) $(OBJDIR)/%.tab.o $(OBJDIR)/%.lex+yacc.yy.o
	@echo ""
	@echo "<<< Build $(YACC)+$(LEX)+$(CC): Compilando y enlazando todo con libfl.a y liby.a >>>"
	$(CC) $(CFLAGS) $(CFLAGS4Y) $(filter %.o, $^) $(LIBS) -o $(@:$(OBJDIR)/%.bin.temp=$(BINDIR)/%.$(BINSEXT))
	@echo "<<< Archivo $(@:$(OBJDIR)/%.bin.temp=$(BINDIR)/%.$(BINSEXT)) buildeado. >>>"

# Para LEX + CC: Para buildear el binario
$(OBJDIR)/%.bin.temp: $(SRCDIR)/%.l $(OBJDIR)/%.build.temp $(COBJS) $(OBJDIR)/%.lex.yy.o
	@echo ""
	@echo "<<< Build $(LEX)+$(CC): Compilando y enlazando todo con libfl.a >>>"
	$(CC) $(CFLAGS) $(filter %.o, $^) $(LIBS) -o $(@:$(OBJDIR)/%.bin.temp=$(BINDIR)/%.$(BINSEXT))
	@echo "<<< Archivo $(@:$(OBJDIR)/%.bin.temp=$(BINDIR)/%.$(BINSEXT)) buildeado. >>>"
	
# Para anunciar que se completó el build
$(OBJDIR)/%.done.temp: $(OBJDIR)/%.build.temp $(OBJDIR)/%.bin.temp
	@echo ""
	@echo "=================[ Completado ]================="

# Para CC: Para generar los objetos desde $(SRCDIR)/%.c con sus dependencias $(SRCDIR)/*.h)
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(wildcard $(SRCDIR)/*.h)
	@echo ""
	@echo "<<< $(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para YACC + CC: Para generar los objetos desde $(OBJDIR)/%.tab.c con sus $(OBJDIR)/%.tab.h $(OBJDIR)/%.tab.output
$(OBJDIR)/%.tab.o: $(OBJDIR)/%.tab.c
	@echo ""
	@echo "<<< $(YACC)+$(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para producir una dependencia en el makefile entre el archivo $(OBJDIR)/%.tab.c con su $(OBJDIR)/%.tab.h
$(OBJDIR)/%.tab.c: $(OBJDIR)/%.tab.h ;
	
# Para YACC: Para generar el archivo $(OBJDIR)/%.tab.c (junto a $(OBJDIR)/%.tab.h y $(OBJDIR)/%.tab.output) desde $(SRCDIR)/%.y
$(OBJDIR)/%.tab.h: $(SRCDIR)/%.y
	@echo ""
	@echo "<<< $(YACC): Generando los archivos intermedios $(<:$(SRCDIR)/%.y=$(OBJDIR)/%.tab.c), $(<:$(SRCDIR)/%.y=$(OBJDIR)/%.tab.h) y $(<:$(SRCDIR)/%.y=$(OBJDIR)/%.output) >>>"
	$(YACC) $(YFLAGS) -o"$(<:$(SRCDIR)/%.y=$(OBJDIR)/%.tab.c)" $<
	@echo "<<< Archivos intermedios $(<:$(SRCDIR)/%.y=$(OBJDIR)/%.c), $(<:$(SRCDIR)/%.y=$(OBJDIR)/%.h) y $(<:$(SRCDIR)/%.y=$(OBJDIR)/%.output) generados. >>>"

# Para LEX + CC (y sí hay YACC): Para generar los objetos desde $(OBJDIR)/%.lex+yacc.yy.c
$(OBJDIR)/%.lex+yacc.yy.o: $(OBJDIR)/%.lex+yacc.yy.c
	@echo ""
	@echo "<<< $(LEX)+$(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para LEX (y sí hay YACC): Para generar los archivos $(OBJDIR)/%.lex+yacc.yy.c desde $(SRCDIR)/%.l y $(OBJDIR)/%.tab.o
$(OBJDIR)/%.lex+yacc.yy.c: $(SRCDIR)/%.l $(OBJDIR)/%.tab.o
	@echo ""
	@echo "<<< $(LEX): Generando el archivo intermedio $@ >>>"
	$(LEX) $(LFLAGS) -o"$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"
	
# Para LEX + CC (y no hay YACC): Para generar los objetos desde $(OBJDIR)/%.lex.yy.c
$(OBJDIR)/%.lex.yy.o: $(OBJDIR)/%.lex.yy.c
	@echo ""
	@echo "<<< $(LEX)+$(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para LEX (y no hay YACC): Para generar los archivos $(OBJDIR)/%.lex.yy.c desde $(SRCDIR)/%.l
$(OBJDIR)/%.lex.yy.c: $(SRCDIR)/%.l
	@echo ""
	@echo "<<< $(LEX): Generando el archivo intermedio $@ >>>"
	$(LEX) $(LFLAGS) -o"$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"