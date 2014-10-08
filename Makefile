CPP = clang++
CFLAGS = `sdl2-config --cflags` -w
LDFLAGS = `sdl2-config --libs`

BIN_DIR = build/bin
OBJ_DIR = build/obj

.PHONY: clean ch3

# TODO: Figure out how to parameterize 'ch3'. Until then use the marked bit as a template for future
# chapters, taking not that you'll also need to add the chapter to the PHONY list above.

### BEGIN CHAPTER TEMPLATE ###

ch3: $(BIN_DIR)/ch3
	./$(BIN_DIR)/$@

$(BIN_DIR)/ch3: $(OBJ_DIR)/ch3.o
	$(CPP) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/ch3.o: 
	$(CPP) $(CFLAGS) -c ch3/main.cpp -o $@ 

### END CHAPTER TEMPLATE ###

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

