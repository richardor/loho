CROSS_COMPILER:=
CC:=$(CROSS_COMPILER)g++
GCC:=$(CROSS_COMPILER)gcc

EXE_NAME:=./auto_translator

SRC_DIR:=./src

#tinyxml
TINY_XML_DIR:=$(SRC_DIR)/tinyxml
INCLUDE_DIR+=-I$(TINY_XML_DIR)

CFLAGS+=$(INCLUDE_DIR)

objects := main.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o ini_file.o ini_parser.o xml_parser.o

$(EXE_NAME) : $(objects)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(objects) 	$(LDFLAGS)
	@rm -f *.o
main.o : $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp  $(LDFLAGS)

ini_file.o : $(SRC_DIR)/ini_file.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ini_file.cpp $(LDFLAGS)
xml_parser.o : $(SRC_DIR)/xml_parser.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/xml_parser.cpp $(LDFLAGS)
ini_parser.o : $(SRC_DIR)/ini_parser.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ini_parser.cpp $(LDFLAGS)
tinystr.o : $(TINY_XML_DIR)/tinystr.cpp
	$(CC) $(CFLAGS) -c $(TINY_XML_DIR)/tinystr.cpp $(LDFLAGS)
tinyxmlerror.o : $(TINY_XML_DIR)/tinyxmlerror.cpp
	$(CC) $(CFLAGS) -c $(TINY_XML_DIR)/tinyxmlerror.cpp $(LDFLAGS)
tinyxmlparser.o : $(TINY_XML_DIR)/tinyxmlparser.cpp
	$(CC) $(CFLAGS) -c $(TINY_XML_DIR)/tinyxmlparser.cpp $(LDFLAGS)
tinyxml.o : $(TINY_XML_DIR)/tinyxml.cpp
	$(CC) $(CFLAGS) -c $(TINY_XML_DIR)/tinyxml.cpp $(LDFLAGS)
clean:
	rm -f $(EXE_NAME)
