CXX=g++
CXXFLAGS=-c -Wall -std=gnu++0x
LDFLAGS=
LDLIBS=

SOURCEDIR=src
BUILDDIR=build

SOURCES=AISDI.cpp utils/GenericException.cpp models/Attachment.cpp models/Person.cpp models/Email.cpp models/Mail.cpp models/Headers.cpp parser/FileParser.cpp
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