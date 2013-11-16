CXX=g++
CXXFLAGS=-c -Wall -std=c++0x
LDFLAGS=
LDLIBS=

SOURCEDIR=src
BUILDDIR=build

SOURCES=AISDI.cpp GenericException.cpp containers/Attachment.cpp containers/Person.cpp containers/Email.cpp containers/Mail.cpp containers/Headers.cpp
EXECUTABLE=run

REAL_SOURCES=$(addprefix $(SOURCEDIR)/, $(SOURCES))
OBJECTS=$(addprefix $(BUILDDIR)/$(SOURCEDIR)/, $(SOURCES:.cpp=.o))
REAL_EXECUTABLE=$(addprefix $(BUILDDIR)/, $(EXECUTABLE))

all: $(REAL_SOURCES) $(REAL_EXECUTABLE)
	
$(REAL_EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@

$(BUILDDIR)/$(SOURCEDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(foreach d,$(subst /, ,${@D}), mkdir $d; cd $d && ):
	$(CXX) $(CXXFLAGS) $< -o $@
	
.PHONY: clean
	
clean:
	rm -rf $(BUILDDIR)