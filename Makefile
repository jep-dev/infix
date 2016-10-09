CXXFLAGS:=$(CXXFLAGS) -std=c++11 -Iinclude
LDFLAGS:=

BINDIR:=bin/
LIBDIR:=lib/
INCDIR:=include/
SRCDIR:=src/
TESTDIR:=test/

OBJ_EXT:=.o
EXE_EXT:=

APPLICATIONS:=main
MODULES:=load expression parse print math
SUBMODULES:=math/function\
	math/value math/variable\
	math/negative math/cosine math/sine math/tangent\
	math/secant math/cosecant math/cotangent\
	math/arccosine math/arcsine math/arctangent math/logarithm\
	math/difference math/sum\
	math/ratio math/product\
	math/power
OBJECTS:=$(foreach obj,$(APPLICATIONS) $(MODULES) $(SUBMODULES),\
	$(obj:%=$(LIBDIR)%$(OBJ_EXT)))
EXES:=$(foreach app,$(APPLICATIONS),$(app:%=$(BINDIR)%$(EXE_EXT)))

TEST_APPLICATIONS:=math
TEST_OBJECTS:=$(foreach mod,$(TEST_APPLICATIONS),\
		$(mod:%=$(TESTDIR)$(LIBDIR)%$(OBJ_EXT)))
TEST_DEPS:=$(filter-out $(LIBDIR)main$(OBJ_EXT),$(OBJECTS))
TEST_EXES:=$(foreach obj,$(TEST_APPLICATIONS),\
		$(obj:%=$(TESTDIR)$(BINDIR)%$(EXE_EXT)))

default:$(OBJECTS) $(EXES)
test:default $(TEST_OBJECTS) $(TEST_EXES)
all:default test

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCDIR)

$(LIBDIR)%$(OBJ_EXT):%.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(BINDIR)%$(EXE_EXT):$(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)
$(TESTDIR)$(LIBDIR)%$(OBJ_EXT): $(TESTDIR)$(SRCDIR)%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(TESTDIR)$(BINDIR)%$(EXE_EXT): $(TESTDIR)$(LIBDIR)%$(OBJ_EXT) $(TEST_DEPS)
	$(CXX) -o $@ $< $(TEST_DEPS) $(LDFLAGS)

clean:;@$(RM) $(OBJECTS) $(TEST_OBJECTS) $(EXES) $(TEST_EXES)

print:; @echo "$(foreach var,TEST_APPLICATIONS TEST_OBJECTS TEST_EXES,\
	\r$(var) = ${$(var)}\n\n)"

.PHONY: clean all print
