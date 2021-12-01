#include <stdio.h>
#include <string.h>
#include "db_bank.h"
#include "fungsi.h"

int percobaan = 0;
char pin[6];
char norek[5];
int pilihan = 0;

nasabah loggin_user;

int menu();
int autentikasiNorek(char *in_norek);
int autentikasiPIN(char *in_pin);
int inputPIN();
int isItBlocked();

int main()
{
    dataNasabah(data_nasabah);
    printf("Masukkan No. Rekening Anda: ");
    scanf("%s", norek);

    if (autentikasiNorek(norek))
    {
        welcome();
        inputPIN();
    }
    else
    {
        isItBlocked();
        main();
        system("clear");
    }

    return 0;
}

int inputPIN()
{
    printf("Masukkan PIN Anda:");
    scanf("%s", &pin);
    if (autentikasiPIN(pin))
    {
        menu();
        return 0;
    }

    if (!isItBlocked())
    {
        welcome();
        inputPIN();
    }
}

int isItBlocked()
{
    if (percobaan > 3)
    {
        printf("Kartu ATM Anda Terblokir!\n");
        printf("Hubungi Call Center Bank Millenial: 08787878!");
        return 1;
    }
    return 0;
}

int autentikasiNorek(char *in_norek)
{
    if (strlen(in_norek) > 0 && strlen(in_norek) == 5)
    {
        for (int i = 0; i < TOTAL_DATA_DB; i++)
        {
            if (strcmp(in_norek, data_nasabah[i].norek) == 0)
            {
                loggin_user = data_nasabah[i];
                return 1;
            }
        }
        printf("No. Rekening Tidak Ditemukan!\n");
        return 0;
    }
    else
    {
        printf("No. Rekening Anda Tidak Valid!\n");
        printf("Pastikan No. Rekening Anda Benar dan Terdiri dari 5 Digit\n");
        return 0;
    }
}

int autentikasiPIN(char *in_pin)
{
    if (strlen(in_pin) > 0 && strlen(in_pin) == 6)
    {
        if (strcmp(in_pin, loggin_user.pin) == 0)
        {
            return 1;
        }
        percobaan++;
        if (!isItBlocked())
        {
            printf("PIN Anda Salah!\n");
            printf("Pastikan PIN Anda Benar dan Terdiri dari 6 Digit\n");
        }
        return 0;
    }
    else
    {
        printf("PIN Anda Tidak Valid!\n");
        printf("Pastikan PIN Anda Benar dan Terdiri dari 6 Digit\n");
        return 0;
    }
}

int menu()
{
    system("clear");
    printf("                              MENU UTAMA\n");
    printf("                    ===============================\n");
    printf("  1 - Tarik Tunai\n");
    printf("  2 - Cek Saldo\n");
    printf("  3 - Transfer\n");
    printf("  4 - Ganti PIN\n");
    printf("  5 - Keluar\n");

    printf("\n\nPILIHAN : ");
    scanf("%d", &pilihan);
    switch (pilihan)
    {
    case 1:
        printf("Tarik Tunai");
        // penarikan();
        break;
    case 2:
        printf("Cek Saldo");
        // cekSaldo();
        break;
    case 3:
        transfer(loggin_user);
        break;
    case 4:
        printf("Ganti PIN");
        // gantiPin();
        break;
    case 5:
        readFile("thanks.txt");
        // gantiPin();
        break;
    default:
        // errorInput();
        menu();
    }
    return 0;
}