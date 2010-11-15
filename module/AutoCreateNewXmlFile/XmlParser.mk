all:
	g++ -o xml_parser_test ./src/xml_parser_test.cpp ./src/xml_parser.cpp ./src/tinyxml/tinystr.cpp ./src/tinyxml/tinyxmlparser.cpp ./src/tinyxml/tinyxml.cpp ./src/tinyxml/tinyxmlerror.cpp -I./src/tinyxml
	@./xml_parser_test
clean:
	rm -rf xml_parser_test
