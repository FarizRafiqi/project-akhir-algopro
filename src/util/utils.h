#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include <assert.h>

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

void basePath(char *fullpath, char *folder)
{
    strcpy(fullpath, _fullpath(fullpath, folder, MAX_PATH));
    int base_path_pos = strpos(fullpath, "algopro") + 8;
    char base_path[MAX_PATH];
    substr(fullpath, base_path, 1, base_path_pos);
    strcat(fullpath, "\\");
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

/**
 * @brief Untuk memposisikan string agar berada pas ditengah
 * 
 * @param s string yang ingin ditengahkan posisinya
 * @param w lebar string
 */
void centerString(char *s, int w)
{
    int l = strlen(s);
    int pos = (int)((w - l) / 2);
    for (int i = 0; i < pos; i++)
        printf(" ");

    printf("%s", s);
}

char *getCenteredString(char *s, int w)
{
    int l = strlen(s);
    int pos = (int)((w - l) / 2);
    for (int i = 0; i < pos; i++)
        printf(" ");

    sprintf(s, "%s");
    return s;
}
/**
 * @brief Untuk membuat garis dari karakter yang ditentukan
 * 
 * @param c karakter yang ingin dicetak
 * @param n banyak karakter yang ingin dicetak
 */
void printLine(char *c, int n)
{
    for (int i = 0; i < n; i++)
        printf(c);
    printf("\n");
}

void getDate(char *date)
{
    // variables to store the date and time components
    int hours, minutes, seconds, day, month, year;

    // `time_t` is an arithmetic time type
    time_t now;

    // Obtain current time
    // `time()` returns the current time of the system as a `time_t` value
    time(&now);

    // Convert to local time format and print to stdout
    // printf("Today is %s", ctime(&now));

    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    hours = local->tm_hour;  // get hours since midnight (0-23)
    minutes = local->tm_min; // get minutes passed after the hour (0-59)
    seconds = local->tm_sec; // get seconds passed after a minute (0-59)

    day = local->tm_mday;         // get day of month (1 to 31)
    month = local->tm_mon + 1;    // get month of year (0 to 11)
    year = local->tm_year + 1900; // get year since 1900

    // // print local time
    if (hours < 12)
    {
        //     // before midday
        sprintf(date, "%02d:%02d:%02d am\n", hours, minutes, seconds);
    }
    else
    {
        // after midday
        sprintf(date, "%02d:%02d:%02d pm\n", hours, minutes, seconds);
    }

    // // print the current date
    sprintf(date, "%02d/%02d/%d\n", day, month, year);
}

char *strrep(const char *s1, const char *s2, const char *s3)
{
    if (!s1 || !s2 || !s3)
        return 0;
    size_t s1_len = strlen(s1);
    if (!s1_len)
        return (char *)s1;
    size_t s2_len = strlen(s2);
    if (!s2_len)
        return (char *)s1;

    /*
     * Two-pass approach: figure out how much space to allocate for
     * the new string, pre-allocate it, then perform replacement(s).
     */

    size_t count = 0;
    const char *p = s1;
    assert(s2_len); /* otherwise, strstr(s1,s2) will return s1. */
    do
    {
        p = strstr(p, s2);
        if (p)
        {
            p += s2_len;
            ++count;
        }
    } while (p);

    if (!count)
        return (char *)s1;

    /*
     * The following size arithmetic is extremely cautious, to guard
     * against size_t overflows.
     */
    assert(s1_len >= count * s2_len);
    assert(count);
    size_t s1_without_s2_len = s1_len - count * s2_len;
    size_t s3_len = strlen(s3);
    size_t s1_with_s3_len = s1_without_s2_len + count * s3_len;
    if (s3_len &&
        ((s1_with_s3_len <= s1_without_s2_len) || (s1_with_s3_len + 1 == 0)))
        /* Overflow. */
        return 0;

    char *s1_with_s3 = (char *)malloc(s1_with_s3_len + 1); /* w/ terminator */
    if (!s1_with_s3)
        /* ENOMEM, but no good way to signal it. */
        return 0;

    char *dst = s1_with_s3;
    const char *start_substr = s1;
    size_t i;
    for (i = 0; i != count; ++i)
    {
        const char *end_substr = strstr(start_substr, s2);
        assert(end_substr);
        size_t substr_len = end_substr - start_substr;
        memcpy(dst, start_substr, substr_len);
        dst += substr_len;
        memcpy(dst, s3, s3_len);
        dst += s3_len;
        start_substr = end_substr + s2_len;
    }

    /* copy remainder of s1, including trailing '\0' */
    size_t remains = s1_len - (start_substr - s1) + 1;
    assert(dst + remains == s1_with_s3 + s1_with_s3_len + 1);
    memcpy(dst, start_substr, remains);
    assert(strlen(s1_with_s3) == s1_with_s3_len);
    return s1_with_s3;
}