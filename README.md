## Como compilar e verificar hash SHA-512

### Compilar o arquivo em C

```bash
gcc -o sha512 sha512.c
```

### Gerar o hash SHA-512 de um arquivo

```bash
sha512sum nome_do_arquivo
```

### Gerar o hash e salvar em um arquivo (sobrescreve se já existir)

```bash
sha512sum texto.txt > hash.txt
```

### Verificar se o hash do arquivo está correto

```bash
sha512sum -c hash.txt
```