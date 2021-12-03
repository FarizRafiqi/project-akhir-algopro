#include <stdio.h>
#include <string.h>
#include "db_bank.h"
#include "fungsi.h"
// #include <windows.h>
#include "lib_chalk.h"
#include "chalk.h"

int percobaan = 0;
char pin[6];
char norek[5];
int pilihan = 0;

nasabah loggin_user;

int menu();
int autentikasiNorek(char *in_norek);
int autentikasiPIN(char *in_pin);
void centerString(char *s);
int inputPIN();
int isItBlocked();
void printLine();

int main()
{
    dataNasabah(data_nasabah);
    printf("Masukkan No. Rekening Anda: ");
    scanf("%s", norek);

    if (autentikasiNorek(norek))
    {
        welcome();
        return inputPIN();
    }

    if (!isItBlocked())
    {
        main();
    }

    return 0;
}

int inputPIN()
{
    printf("Masukkan PIN ATM Anda: ");
    scanf("%s", &pin);
    if (autentikasiPIN(pin))
    {
        return menu();
    }

    if (isItBlocked())
    {
        printc("\nKartu ATM Anda Terblokir!\n", FOREGROUND_RED);
        printc("Hubungi Call Center Bank Millenial: 08787878!", FOREGROUND_RED);
        return 0;
    }

    welcome();
    return inputPIN();
}

int isItBlocked()
{
    return percobaan > 3 ? 1 : 0;
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
        printc("No. Rekening Tidak Terdaftar!\n", FOREGROUND_RED);
        return 0;
    }
    else
    {
        printc("No. Rekening Anda Tidak Valid!\n", FOREGROUND_RED);
        printc("Pastikan No. Rekening Anda Benar dan Terdiri dari 5 Digit\n", FOREGROUND_RED);
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
            printc("PIN Anda Salah!\n", FOREGROUND_RED);
            printc("Pastikan PIN Anda Benar dan Terdiri dari 6 Digit\n", FOREGROUND_RED);
        }
        return 0;
    }
    else
    {
        printc("PIN Anda Tidak Valid!\n", FOREGROUND_RED);
        printc("Pastikan PIN Anda Benar dan Terdiri dari 6 Digit\n", FOREGROUND_RED);
        return 0;
    }
}

int menu()
{
    //Untuk menampilkan pesan error jika pilihan user tidak tersedia
    if (pilihan <= 5)
    {
        system("cls");
    }

    printLine();
    centerString("MENU UTAMA");
    printf("\n");
    printLine();

    printf("1 - Tarik Tunai\n");
    printf("2 - Cek Saldo\n");
    printf("3 - Transfer\n");
    printf("4 - Ganti PIN\n");
    printf("5 - Keluar\n");

    printf("\n\nPILIHAN : ");
    scanf("%d", &pilihan);
    switch (pilihan)
    {
    case 1:
        if (penarikan(loggin_user) == 1)
        {
            menu();
        }
        readFile("thanks.txt");
        break;
    case 2:
        if (cekSaldo(loggin_user) == 1)
        {
            menu();
        }
        readFile("thanks.txt");
        break;
    case 3:
        if (transfer(loggin_user) == 1)
        {
            menu();
        }
        readFile("thanks.txt");
        break;
    case 4:
        printf("Ganti PIN");
        // gantiPin();
        break;
    case 5:
        readFile("thanks.txt");
        break;
    default:
        printf(CHALK_BG_RED("Pilihan Menu Tidak Tersedia!\n"));
        menu();
        break;
    }
    return 0;
}

void centerString(char *s)
{
    int l = strlen(s);
    int pos = (int)((60 - l) / 2);
    for (int i = 0; i < pos; i++)
        printf(" ");

    printf("%s", s);
}

void printLine()
{
    for (int i = 0; i < 59; i++)
        printf("=");
    printf("\n");
}