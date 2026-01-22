#include <winkey.h>

void to_hex(const unsigned char *in, size_t len, char *out)
{
    for (size_t i = 0; i < len; i++)
        sprintf_s(out + i * 2, 3, "%02X", in[i]);
    out[len * 2] = '\0';
}

unsigned char from_hex(char c)
{
    if ('0' <= c && c <= '9')
        return (c - '0');
    if ('A' <= c && c <= 'F')
        return (c - 'A' + 10);
    if ('a' <= c && c <= 'f')
        return (c - 'a' + 10);
    return (0);
}

unsigned char *hex_to_bytes(char *hex, size_t *outlen)
{
    size_t len = strlen(hex);
    if (len % 2 != 0)
        return (NULL);

    *outlen = len / 2;
    unsigned char *out = malloc((*outlen));

    for (size_t i = 0; i < *outlen; i++)
        out[i] = (from_hex(hex[i * 2]) << 4) | from_hex(hex[i * 2 + 1]);

    return (out);
}

unsigned char *XorStrings(char *enc)
{
    unsigned char   key[] = {55, 12, 32, 50, 60};
    size_t          keylen = sizeof(key);
    size_t          binlen;
    
    unsigned char   *bin = hex_to_bytes(enc, &binlen);
    if (!bin)
        return (NULL);
    for (size_t i = 0; i < binlen; i++)
        bin[i] ^= key[i % keylen];

    return (bin);
}