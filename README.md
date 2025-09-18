## Como compilar e verificar hash SHA-512

### Compilar o arquivo em C

```bash
gcc sha512ThroughBruteForce.c -o sha512ThroughBruteForce -fopenmp
```

### Gerar o hash SHA-512 de um arquivo

```bash
sha512sum nome_do_arquivo
```

### Gerar o hash e salvar em um arquivo (sobrescreve se já existir)

```bash
sha512sum texto.txt > hash.txt
```

##!!!!!!!!! GERAR HASH E SALVAR (EVITA \n de texto quebrando o hash)
```bash
echo -n "$(cat texto.txt)" | sha512sum > hash.txt
```

### Verificar se o hash do arquivo está correto

```bash
sha512sum -c hash.txt
```

### Rodar em máquina específica:
```bash
srun --exclusive -w atlantica10 -N 1 ./sha512ThroughBruteForce_v2 <num_threads>
```
