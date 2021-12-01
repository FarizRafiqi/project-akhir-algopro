#include <stdio.h>
#include <string.h>

#define TOTAL_DATA_DB 3

typedef struct
{
    char *norek, *pin, *nama, *status;
    int saldo;
} nasabah;

nasabah data_nasabah[TOTAL_DATA_DB];
/**
 * Data Nasabah Bank
 **/
void *dataNasabah(nasabah *dn)
{
    nasabah n1 = {"11001", "000007", "Affandra Fahrezi", "aktif", 6500000};
    nasabah n2 = {"11003", "070103", "Aulia El Ihza Fariz Rafiqi", "aktif", 7000000};
    nasabah n3 = {"11017", "12345", "M. Arif Fadhilah", "aktif", 7500000};

    dn[0] = n1;
    dn[1] = n2;
    dn[2] = n3;
}
