//compilar com: gcc -o sha512_forcaBruta sha512_forcaBruta.c -std=c17

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Circular right shift
#define ROTR(x, n) ((x >> n) | (x << (64 - n)))

// SHA-512 functions
#define Ch(x, y, z) ((x & y) ^ ((~x) & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define Sigma0(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define Sigma1(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define sigma0(x) (ROTR(x, 1) ^ ROTR(x, 8) ^ (x >> 7))
#define sigma1(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ (x >> 6))

// Tabela de constantes
uint64_t k[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

// Valores iniciais
uint64_t H0[8] = {
    0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

uint64_t W[80];

void sha512_compress(uint64_t *state, const uint8_t *block) {
    int t;
    uint64_t temp1, temp2;

    for (t = 0; t < 16; t++) {
        W[t] = ((uint64_t)block[t * 8] << 56) | ((uint64_t)block[t * 8 + 1] << 48) |
               ((uint64_t)block[t * 8 + 2] << 40) | ((uint64_t)block[t * 8 + 3] << 32) |
               ((uint64_t)block[t * 8 + 4] << 24) | ((uint64_t)block[t * 8 + 5] << 16) |
               ((uint64_t)block[t * 8 + 6] << 8) | ((uint64_t)block[t * 8 + 7]);
    }
    for (t = 16; t < 80; t++)
        W[t] = sigma1(W[t - 2]) + W[t - 7] + sigma0(W[t - 15]) + W[t - 16];

    uint64_t a = state[0];
    uint64_t b = state[1];
    uint64_t c = state[2];
    uint64_t d = state[3];
    uint64_t e = state[4];
    uint64_t f = state[5];
    uint64_t g = state[6];
    uint64_t h = state[7];

    for (t = 0; t < 80; t++) {
        temp1 = h + Sigma1(e) + Ch(e, f, g) + k[t] + W[t];
        temp2 = Sigma0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

void sha512_hash(const char *message, uint64_t output[8]) {
    uint8_t block[128];
    uint64_t bitlen = strlen(message) * 8;
    uint64_t state[8];
    int i, len, offset;

    for (i = 0; i < 8; i++)
        state[i] = H0[i];

    len = strlen(message);
    offset = 0;

    while (offset < len) {
        int block_size = (len - offset < 128) ? len - offset : 128;

        memset(block, 0, 128);
        memcpy(block, message + offset, block_size);

        if (block_size < 128) {
            block[block_size] = 0x80;
            if (block_size < 112) {
                for (i = 0; i < 8; i++)
                    block[112 + i] = (bitlen >> ((7 - i) * 8)) & 0xFF;
            }
        }
        sha512_compress(state, block);
        offset += block_size;
    }

    for (i = 0; i < 8; i++)
        output[i] = state[i];
}

int compare_hash(uint64_t a[8], uint64_t b[8]) {
    for (int i = 0; i < 8; i++)
        if (a[i] != b[i]) return 0;
    return 1;
}

// brute force recursivo
void brute_rec(const char *charset, int charset_len, char *buffer, int pos, int len, uint64_t target[8]) {
    if (pos == len) {
        buffer[pos] = '\0';
        uint64_t hash[8];
        sha512_hash(buffer, hash);
        if (compare_hash(hash, target)) {
            printf("Encontrado: %s\n", buffer);
            exit(0);
        }
        return;
    }
    for (int i = 0; i < charset_len; i++) {
        buffer[pos] = charset[i];
        brute_rec(charset, charset_len, buffer, pos + 1, len, target);
    }
}

void brute_force_iter(const char *charset, int max_len, uint64_t target[8]) {
    int charset_len = strlen(charset);
    char buffer[64];

    // para cada tamanho de senha
    for (int len = 1; len <= max_len; len++) {
        // vetor de índices, começa tudo zero
        int idx[64] = {0};

        while (1) {
            // monta a string atual
            for (int i = 0; i < len; i++)
                buffer[i] = charset[idx[i]];
            buffer[len] = '\0';

            // calcula hash
            uint64_t hash[8];
            sha512_hash(buffer, hash);
            if (compare_hash(hash, target)) {
                printf("Encontrado: %s\n", buffer);
                exit(0);
            }

            // incrementa como contador em base charset_len
            int pos = len - 1;
            while (pos >= 0) {
                idx[pos]++;
                if (idx[pos] < charset_len) break;
                idx[pos] = 0;
                pos--;
            }
            if (pos < 0) break; // terminou todas combinações para esse len
        }
    }
}


void brute_force(const char *charset, int max_len, uint64_t target[8]) {
    int charset_len = strlen(charset);
    char buffer[64];
    for (int len = 1; len <= max_len; len++) {
        brute_rec(charset, charset_len, buffer, 0, len, target);
    }
}

int main() {
    // sentença que queremos gerar e depois quebrar
    const char *sentenca = "abcdef";

    // gerar hash da sentença
    uint64_t hash_da_sentenca[8];
    sha512_hash(sentenca, hash_da_sentenca);

    printf("Hash gerado para \"%s\":\n", sentenca);
    for (int i = 0; i < 8; i++)
        printf("%016llx ", (unsigned long long)hash_da_sentenca[i]);
    printf("\n");

    // defina aqui os caracteres permitidos
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    // agora usar força bruta para tentar descobrir a sentença original
    printf("Iniciando brute force...\n");
    brute_force_iter(charset, 5, hash_da_sentenca);


    printf("Não encontrado.\n");
    return 0;
}

