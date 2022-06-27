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
	@echo "=================[ Eliminar todo lo generado al compilar ]================="
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
	
# Para crear reglas que no generen objetos de ese mismo nombre
.PHONY: all clean debug

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