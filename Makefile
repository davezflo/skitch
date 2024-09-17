CXX=g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath,/usr/local/lib -lpthread
SDL_INCLUDE = -I/usr/local/include -I./include
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) -g
LDFLAGS=$(SDL_LIB)
OBJDIR := obj
SOURCEDIR := source
BINDIR := bin
files = main.cc action.cc actionsequence.cc actionthread.cc assignment.cc attributevariable.cc camera.cc colorvar.cc context.cc entity.cc \
          eventlistener.cc face.cc generate.cc lex.cc log.cc lvalue.cc infinimap.cc turtlemap.cc maths.cc model.cc parser.cc point.cc projection.cc regex.cc \
		  render.cc rvalue.cc shape.cc state.cc transition.cc variable.cc ticket.cc watch.cc light.cc map.cc surface.cc \
		  elementparser.cc parsecolor.cc parsemap.cc parsewatch.cc parseaction.cc parsegenerate.cc parsenumber.cc parseshape.cc parsestring.cc \
		  parseuse.cc interactlight.cc consume.cc comptransition.cc statetransition.cc parsestatevar.cc fractaltransformer.cc transformer.cc \
		  recolortransformer.cc
target = $(BINDIR)/skitch

sources = ${files:%.cc=$(SOURCEDIR)/%.cc}
objects = ${files:%.cc=$(OBJDIR)/%.o}

skitch: $(target)

$(target): $(objects) | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(objects): $(OBJDIR)/%.o: $(SOURCEDIR)/%.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

test: $(BINDIR)/tests | $(BINDIR)

$(OBJDIR)/lex_test.o: $(SOURCEDIR)/lex_test.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINDIR)/tests: $(OBJDIR)/lex_test.o $(OBJDIR)/lex.o $(OBJDIR)/regex.o $(OBJDIR)/state.o $(OBJDIR)/log.o | $(BINDIR)
	$(CXX) $^ $(LDFLAGS) -o $@

$(BINDIR):
	mkdir -p $@

$(OBJDIR):
	mkdir -p $@

clean:
	rm $(OBJDIR)/*.o && rm $(target)
