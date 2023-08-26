# project: λδ(lambdadelta)
# license: GPL3
# author: 0kalekale
 
TARGET = target
SRC = src
CFLAGS = -std=gnu99  
LDFLAGS = -lfl -ly -lm
BIN = lambdadelta

build:	prepare parser_lexer compile link

prepare:
	@mkdir -p $(TARGET)
	@mkdir -p $(TARGET)/src 

parser_lexer:
	@echo "YACC 	$(SRC)/grammar.y" 
	@yacc --defines=$(SRC)/grammar.h -o $(SRC)/grammar.c $(SRC)/grammar.y
	@echo "LEX 	$(SRC)/scanner.l"
	@flex --outfile=$(SRC)/scanner.c $(SRC)/scanner.l

compile:
	@for i in $(SRC)/*.c; do echo "CC	 $$i"; done 
	@cd $(TARGET) && $(CC) $(CFLAGS) -I../src -c ../$(SRC)/*.c    

link:
	@echo "BIN	" $(TARGET)/$(BIN) 
	@cd $(TARGET) && $(CC) *.o $(LDFLAGS) -o $(BIN)
test:
	@cd tests && ./berntestel *.λδ

clean:
	@rm $(SRC)/grammar.c $(SRC)/scanner.c src/grammar.h
	@rm -rf $(TARGET)
