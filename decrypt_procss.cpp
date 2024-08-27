#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
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
    std::string nome_arquivo;
    std::string palavra;
};

// * Funcao para tentar todas as combinacoes possiveis de caracteres (ate 4)
void* process_function(void* ptr) {

    Struct_palavra* data = (Struct_palavra*)ptr;

    std::string palavra = data->palavra;

    std::string arquivo = data->nome_arquivo;
    std::ofstream file;
    file.open(arquivo, std::ios::out | std::ios::app);

    delete data; // * Limpa a memoria alocada para o struct

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
                        if (found2 == 1)
                        {
                            file << "'"<< palavra << "' - " << word_aux <<", ";
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

    file.close();
    return nullptr;
}

int main() {
    int main_process = getpid();
    int child_pids[10];

    for (int i = 0; i < 10; i++)
    {
        if (main_process == getpid())
        {
            std::string nome_arquivo = "files/passwd_" + std::to_string(i) + ".txt"; // * Para poder abrir cada arquivo de forma separada 
            std::string nome_arquivo_saida = "saida/dec_passwd_" + std::to_string(i) + ".txt";

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

            child_pids[i] = fork();

            if (child_pids[i] == 0)
            {
                for (int j = 0; j < 10; j++)
                {
                    Struct_palavra* data = new Struct_palavra{nome_arquivo_saida,vetor_word[j]}; // * Usando o new ele ja aloca o espaco para a string tambem
                    process_function(data);
                }
                exit(EXIT_SUCCESS);
            }
            
        }
    }

    for (int i = 0; i < 10; i++) {
        waitpid(child_pids[i], NULL, 0); // Espera todos os processos filhos
    }
    return 0;
}