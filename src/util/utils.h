#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

/**
 * @brief Untuk mengatur format nomor, sehingga memiliki pemisah perseribuan
 * 
 * @param n nomor yang ingin diformat
 */
void formatNumber(int n)
{
    if (n < 0)
    {
        printf("-");
        formatNumber(-n);
        return;
    }
    if (n < 1000)
    {
        printf("%d", n);
        return;
    }
    formatNumber(n / 1000);
    printf(".%03d", n % 1000);
}

/**
 * @brief Untuk mencari posisi dari sebuah string
 * 
 * @param haystack sumber string
 * @param needle string yang ingin dicari
 * @return int 
 */
int strpos(char *haystack, char *needle)
{
    char *p = strstr(haystack, needle);
    if (p)
        return p - haystack;
    return -1;
}

/**
 * @brief Untuk mengambil sebagian dari string diantara index awal dan akhir string
 * 
 * @param s sumber string
 * @param sub string yang ingin diambil
 * @param p index awal string
 * @param l index akhir string
 */
void substr(char s[], char sub[], int p, int l)
{
    int c = 0;

    while (c < l)
    {
        sub[c] = s[p + c - 1];
        c++;
    }
    sub[c] = '\0';
}

/**
 * @brief Untuk membaca file
 * 
 * @param filename nama file
 */
void readFile(char *filename)
{
    char s[100];
    FILE *f;

    char fullpath[MAX_PATH];
    strcpy(fullpath, _fullpath(fullpath, filename, MAX_PATH));
    int base_path_pos = strpos(fullpath, "algopro") + 8;
    char base_path[MAX_PATH];
    substr(fullpath, base_path, 1, base_path_pos);
    // printf("%s", base_path);

    f = fopen(strcat(base_path, filename), "r");
    if (f == NULL)
    {
        printf("File tidak bisa dibuka\n");
    }
    else
    {
        while (!feof(f))
        {
            fgets(s, sizeof(s), f);
            printf("%s", s);
        }
        fclose(f);
    }
}