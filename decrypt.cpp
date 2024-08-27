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

    std::string str_result(TAMANHO_SENHA, '\0');
    for (int i = 0; i < TAMANHO_SENHA; i++) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') {
            int str_idx = c - ASCII_A;
            int chave_idx = c - ASCII_A;
            str_result[i] = ((str_idx + chave_idx) % NUM_LETRAS) + ASCII_A;
        } else {
            std::cout << "Erro: String contém caracteres inválidos.\n";
            exit(EXIT_FAILURE);
        }
    }
    return str_result;
}

// * Estrutura para opder usar a string
struct Struct_palavra {
    std::string nome_arquivo;
    std::vector<std::string> vetor_word;
};

// * Funcao para tentar todas as combinacoes possiveis de caracteres (ate 4)
void* thread_function(void* ptr) {

    Struct_palavra* data = (Struct_palavra*)ptr;

    std::vector<std::string> palavras = data->vetor_word;
    std::string arquivo = data->nome_arquivo;

    std::ofstream file;
    file.open (arquivo);

    delete data; // * Limpa a memoria alocada para o struct

    for (int s = 0; s < 10; s++)
    {
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

                        if (encrypted == palavras[s]) {

                            found2++;
                            if (found2 == 1)
                            {
                                file << "'"<<palavras[s] << "' - " << word_aux <<", ";
                            }
                            if (found2 == 2)
                            {
                                file << word_aux <<"\n";
                                found = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    file.close();
    return nullptr;
}

int main() {
    pthread_t threads[10];

    for (int b = 0; b < 10; b++)
    {

        std::string nome_arquivo = "files/passwd_" + std::to_string(b) + ".txt"; // * Para poder abrir cada arquivo de forma separada
        std::string nome_arquivo_saida = "saida/dec_passwd_" + std::to_string(b) + ".txt";
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

        Struct_palavra* data = new Struct_palavra{nome_arquivo_saida, vetor_word}; // * Usando o new ele ja aloca o espaco para a string tambem
        
        if (pthread_create(&threads[b], nullptr, thread_function, data) != 0){
            std::cout << "Erro ao criar a thread.\n";
        }    
    }
    for (pthread_t &thread : threads) {
        if (pthread_join(thread, nullptr) != 0)
            std::cout << "Erro na thread.\n";
    }
    return 0;
}