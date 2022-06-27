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

# Ubicación de los *.l, *.c y *h (excepto los generados por FLEX y BISON)
SRCDIR:=src
# Ubicación de los objetos generados por FLEX y BISON
OBJDIR:=obj
# Ubicación del build
BINDIR:=bin

# Analizador sintáctico a usar
YACC:=bison
# Agrega los flags que se le pasan a YACC (los flags -v y -d sí que son necesarios)
YFLAGS:=-v -d

# Analizador léxico a usar
LEX:=flex
# Agrega los flags que se le pasan a Flex
LFLAGS+=

# Compilador a usar
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

# Para generar todos los objetos necesarios, se usa la sig estructura:

# 	objeto_a_generar: dependencia1 dependencia2 ... dependenciaM
# 	<tab> comando_a_ejecutar1
# 	<tab> comando_a_ejecutar2
# 	...
# 	<tab> comando_a_ejecutarN

# Para compilar todos los *.l que fueron modificados con 'make'
all: $(BINS)

# Para "limpiar" (eliminar) todos los objetos generados
clean:
	@echo ""
	@echo "<<< Eliminando el directorio $(OBJDIR) recursivamente si existiera >>> "
	rm -rf $(OBJDIR)
	@echo "<<< Directorio $(OBJDIR) eliminado recursivamente. >>> "
	@echo ""
	@echo "<<< Eliminando todos los archivos *.$(BINSEXT) en el directorio $(BINDIR) si existiera >>> "
	rm -rf $(BINDIR)/*.$(BINSEXT)
	@echo "<<< Archivos $(BINDIR)/*.$(BINSEXT) eliminados. >>> "
	@echo ""
	@echo "<<< Eliminando el directorio $(BINDIR) si existiera y estuviera vacio >>> "
	if [ -d "$(BINDIR)" ]; then rmdir --ignore-fail-on-non-empty $(BINDIR); fi
	@echo "<<< Directorio $(BINDIR) eliminado en caso de haber estado vacio. >>> "

# Para agregar el flag de debug a YACC
debug: YFLAGS += --debug
# Para agregar el flag de debug a LEX
debug: LFLAGS += -d
# Para agregar el flag de debug para YACC a CC
debug: CFLAGS4Y += -DBISON_DEBUG -DDEBUG -g
# Para "debuggear" YACC y LEX
debug: clean all
	
# Para crear reglas que no generen objetos de ese mismo nombre
.PHONY: all clean debug

# Para YACC + LEX + CC: Para buildear el binario
$(BINDIR)/%.$(BINSEXT): $(SRCDIR)/%.l $(SRCDIR)/%.y $(OBJDIR) $(BINDIR) $(COBJS) $(OBJDIR)/%.tab.o $(OBJDIR)/%.lex+yacc.yy.o
	@echo ""
	@echo "<<< Build $(YACC)+$(LEX)+$(CC): Compilando y enlazando todo con libfl.a y liby.a >>>"
	$(CC) $(CFLAGS) $(CFLAGS4Y) $(filter %.o, $^) $(LIBS) -o $@
	@echo "<<< Archivo $@ buildeado. >>>"

# Para LEX + CC: Para buildear el binario
$(BINDIR)/%.$(BINSEXT): $(SRCDIR)/%.l $(OBJDIR) $(BINDIR) $(COBJS) $(OBJDIR)/%.lex.yy.o
	@echo ""
	@echo "<<< Build $(LEX)+$(CC): Compilando y enlazando todo con libfl.a >>>"
	$(CC) $(CFLAGS) $(filter %.o, $^) $(LIBS) -o $@
	@echo "<<< Archivo $@ buildeado. >>>"

# Para crear los directorios si no existieran
$(OBJDIR) $(BINDIR):
	@echo ""
	@echo "<<< Creando el directorio $@ >>>"
	mkdir -p $@
	@echo "<<< Directorio $@ creado. >>>"
	
# Para que no elimine los archivos secundarios (aquellos que son creados por regla prerequisito de otra regla) una vez buildeado
.SECONDARY: $(COBJS) $(YOBJS) $(YOBJS:%.o=%.c) $(LOBJS:%.yy.o=%.lex+yacc.yy.o) $(LOBJS:%.yy.o=%.lex+yacc.yy.c) $(LOBJS:%.yy.o=%.lex.yy.o) $(LOBJS:%.yy.o=%.lex.yy.c)

# Para CC: Para generar los objetos desde $(SRCDIR)/%.c
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo ""
	@echo "<<< $(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para YACC + CC: Para generar los objetos desde $(OBJDIR)/%.tab.c
$(OBJDIR)/%.tab.o: $(OBJDIR)/%.tab.c
	@echo ""
	@echo "<<< $(YACC)+$(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para YACC: Para generar el archivo $(OBJDIR)/%.tab.c (junto a $(OBJDIR)/%.tab.h y $(OBJDIR)/%.tab.output) desde $(SRCDIR)/%.y
$(OBJDIR)/%.tab.c: $(SRCDIR)/%.y
	@echo ""
	@echo "<<< $(YACC): Generando los archivos intermedios $(patsubst $(OBJDIR)/%.tab.c,$(OBJDIR)/%.tab.h,$@), $@ y $(patsubst $(OBJDIR)/%.tab.c,$(OBJDIR)/%.output,$@) >>>"
	$(YACC) $(YFLAGS) -o"$@" $<
	@echo "<<< Archivos intermedios $(patsubst $(OBJDIR)/%.tab.c,$(OBJDIR)/%.tab.h,$@), $@ y $(patsubst $(OBJDIR)/%.tab.c,$(OBJDIR)/%.output,$@) generados. >>>"

# Para LEX + CC (y sí hay YACC): Para generar los objetos desde $(OBJDIR)/%.lex+yacc.yy.c
$(OBJDIR)/%.lex+yacc.yy.o: $(OBJDIR)/%.lex+yacc.yy.c
	@echo ""
	@echo "<<< $(LEX)+$(CC): Generando el archivo intermedio $@ >>>"
	$(CC) $(CFLAGS) -c -o "$@" $<
	@echo "<<< Archivo intermedio $@ generado. >>>"

# Para LEX (y sí hay YACC): Para generar los archivos $(OBJDIR)/%.lex+yacc.yy.c desde $(SRCDIR)/%.l
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