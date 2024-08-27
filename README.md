# Brute_force_decrypt
### How it works
Nesse projeto, por força bruta tenta-se quebrar senhas, com tentativa e falha.

As tentativas são descartadas e armazenam-se apenas os sucessos , é possível observar também que para o meio com o qual foram criptografadas as mensagens existem duas maneiras distintas de se obter a senha requerida

São tentados por duas maneiras distintas, por uma maneira se tenta usando as threads e por outra maneira se tenta por meio de processos, para ser possível ver qual método é mais eficiente

![Imagem1](/files/img.png)

Os tempos dos processos foram analisados por meio do comando time do linux tendo os tempos dados por:

![Imagem2](/files/img2.png)

### Compile and Run

Requires [make](https://www.gnu.org/software/make/)

Compile
```console
foo@bar:~$ make
```

Execute with threads
```console
foo@bar:~$ make run_decrypt
```
Execute with process
```console
foo@bar:~$ make run_decrypt_procss
```
Limpe os arquivos desnecessários
```console
foo@bar:~$ make clean
```
Exemplos
```console
foo@bar:~$ make run
```
