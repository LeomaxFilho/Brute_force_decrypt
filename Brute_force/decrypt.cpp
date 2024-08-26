#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#define NUM_LETRAS 26
#define ASCII_A 65
#define TAMANHO_SENHA 4

std::string encrypt(const std::string& str) {
    if (str.length() != TAMANHO_SENHA) {
        std::cerr << "Erro: A string deve ter exatamente " << TAMANHO_SENHA << " caracteres.\n";
        exit(EXIT_FAILURE);
    }

    std::string str_result(TAMANHO_SENHA, '\0');
    for (int i = 0; i < TAMANHO_SENHA; i++) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') {
            int str_idx = c - ASCII_A;
            int chave_idx = c - ASCII_A;
            str_result[i] = ((str_idx + chave_idx) % NUM_LETRAS) + ASCII_A;
        } else {
            std::cerr << "Erro: String contém caracteres inválidos.\n";
            exit(EXIT_FAILURE);
        }
    }
    return str_result;
}

// * Estrutura para opder usar a string
struct Struct_palavra {
    std::string palavra;
};

// * Funcao para tentar todas as combinacoes possiveis de caracteres (ate 4)
void* thread_function(void* ptr) {

    Struct_palavra* data = (Struct_palavra*)ptr;

    std::string palavra = data->palavra;
    free(data); // * Limpa a memOria alocada para o struct

    std::string word_aux(TAMANHO_SENHA, 'A');
    bool found = false; // * caso ache ele encerra o Loop imediatamente
    int found2 = 0;
    // Tenta todas as combinações possíveis
    for (int i = 0; i < NUM_LETRAS && !found; ++i) {
        word_aux[0] = ASCII_A + i;
        for (int j = 0; j < NUM_LETRAS && !found; ++j) {
            word_aux[1] = ASCII_A + j;
            for (int k = 0; k < NUM_LETRAS && !found; ++k) {
                word_aux[2] = ASCII_A + k;
                for (int l = 0; l < NUM_LETRAS && !found; ++l) {
                    word_aux[3] = ASCII_A + l;
                    std::string encrypted = encrypt(word_aux);
                    if (encrypted == palavra) {
                        found2++;
                        std::cout << "Palavra ["<< found2 << "] para: '" << palavra << "': " << word_aux << std::endl;
                        if (found2 == 2)
                        {
                            std::cout << std::endl;
                            found = true;
                        }
                    }
                }
            }
        }
    }

    return nullptr;
}

int main() {
    for (int b = 0; b < 10; b++)
    {
        std::string nome_arquivo = "files/passwd_" + std::to_string(b) + ".txt"; // * Para poder abrir cada arquivo de forma separada

        std::ifstream file(nome_arquivo);

        std::vector<std::string> vetor_word;
        std::string palavra;

        if (!file) {
            std::cout << "Erro ao abrir o arquivo.\n";
        }

        while (std::getline(file, palavra)) {

            vetor_word.push_back(palavra);
        }

        file.close();

        pthread_t threads[10];

        for (size_t i = 0; i < vetor_word.size(); ++i) {

            Struct_palavra* data = new Struct_palavra{vetor_word[i]}; // * Usando o new ele ja aloca o espaco para a string tambem

            if (pthread_create(&threads[i], nullptr, thread_function, data) != 0){

                std::cout << "Erro ao criar a thread.\n";
                delete data; // * Libera a memoria se a criacao da thread falhar
            }    
        }

        for (pthread_t &thread : threads) {

            if (pthread_join(thread, nullptr) != 0)
                std::cout << "Erro na thread.\n";
        }
    }
    return EXIT_SUCCESS;
}