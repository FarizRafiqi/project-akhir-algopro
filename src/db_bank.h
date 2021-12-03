#include <stdio.h>
#include <stdlib.h>

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
void dataNasabah(nasabah *dn)
{
    nasabah n1 = {"11001", "000007", "Affandra Fahrezi", "aktif", 6500000};
    nasabah n2 = {"11003", "070103", "Aulia El Ihza Fariz Rafiqi", "aktif", 7000000};
    nasabah n3 = {"11017", "123456", "M. Arif Fadhilah", "aktif", 7500000};

    dn[0] = n1;
    dn[1] = n2;
    dn[2] = n3;
}

void getAllDataNasabah(nasabah *dn)
{
    FILE *fp = fopen("src/data/data_nasabah.csv", "r");

    if (!fp)
        printf("Can't open file\n");

    else
    {
        // Here we have taken size of
        // array 1024 you can modify it
        char buffer[1024];

        int row = 1;
        int column = 0;

        while (fgets(buffer, 1024, fp))
        {
            column = 0;
            row++;

            // To avoid printing of column
            // names in file can be changed
            // according to need
            if (row == 1)
                continue;

            // Splitting the data
            char *value = strtok(buffer, ",");

            while (value)
            {
                // Column 1
                if (column == 0)
                {
                    dn[row].norek = value;
                }

                // Column 2
                if (column == 1)
                {
                    dn[row].pin = value;
                }

                // Column 3
                if (column == 2)
                {
                    dn[row].nama = value;
                }

                // Column 4
                if (column == 3)
                {
                    dn[row].status = value;
                }

                // Column 5
                if (column == 4)
                {
                    dn[row].saldo = atoi(value);
                }

                value = strtok(NULL, ",");
                column++;
            }
        }
        fclose(fp);
    }
}