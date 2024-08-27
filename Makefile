CXX = g++

CXXFLAGS = -Wall -O2

SRC1 = decrypt.cpp
SRC2 = decrypt_procss.cpp

OUT1 = decrypt
OUT2 = decrypt_procss


all: $(OUT1) $(OUT2)

$(OUT1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $(OUT1) $(SRC1)

$(OUT2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $(OUT2) $(SRC2)

# Regras de execução

run_decrypt: $(OUT1)
	./$(OUT1)

run_decrypt_procss: $(OUT2)
	./$(OUT2)

# Limpeza dos binários
clean:
	rm -f $(OUT1) $(OUT2)

.PHONY: all clean run_decrypt run_decrypt_procss