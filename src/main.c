#include <stdio.h>
#include <string.h>
#include "db_bank.h"
#include "fungsi.h"
#include <windows.h>
#include <signal.h>

#define SCREEN_WIDTH 60

int percobaan = 0;
char pin[6];
char norek[5];
int pilihan = 0;

nasabah logged_user;

int menu();
int autentikasiNorek(char *norek);
int autentikasiPIN(char *pin);
int inputPIN();
int isItBlocked();
void printLine();
void showBlockedCardMessage();

int main()
{
    /**
     * Ketika user memasukkan norek, maka jalankan kode di bawah ini 
     * (ditandai dengan scanf() mengembalikan integer lebih dari 0). 
     * Jika tidak, maka jangan jalankan kode di bawah ini. 
     * Hal ini guna menghindari duplikasi output.
     */
    //inisialisasi data nasabah
    dataNasabah(data_nasabah);
    printf("MASUKKAN NO. REKENING ANDA: ");
    if (scanf("%s", norek) == 1)
    {
        if (autentikasiNorek(norek))
        {
            if (!is_error)
            {
                system("cls");
            }
            welcome();
            return inputPIN();
        }
        main();
    }
    
    return 0;
}

int inputPIN()
{
    printf("MASUKKAN PIN ATM ANDA: ");
    scanf("%s", &pin);
    if (autentikasiPIN(pin))
    {
        return menu();
    }

    welcome();
    return inputPIN();
}

int isItBlocked()
{
    return percobaan > 3 ? 1 : 0;
}

int autentikasiNorek(char *norek)
{
    if (strlen(norek) > 0 && strlen(norek) == 5)
    {
        for (int i = 0; i < TOTAL_DATA_DB; i++)
        {
            if (strcmp(norek, data_nasabah[i].norek) == 0)
            {
                logged_user = data_nasabah[i];
                return 1;
            }
        }
        /**
         * jika program menjalankan kode di bawah ini,
         * tandanya ketika mencocokkan no rekening yg diinput
         * dengan yang ada di mini database, itu tidak ada yg
         * cocok sama sekali. Artinya No. Rekening tidak terdaftar.
         */
        printc("\nNO. REKENING ANDA TIDAK TERDAFTAR!\n", FOREGROUND_RED);
        Sleep(2000);
        system("cls");
        return 0;
    }
    else
    {
        printc("\nNO. REKENING ANDA TIDAK VALID!\n", FOREGROUND_RED);
        printc("PASTIKAN NO. REKENING ANDA BENAR DAN TERDIRI DARI 5 DIGIT\n", FOREGROUND_RED);
        Sleep(2000);
        system("cls");
        return 0;
    }
}

int autentikasiPIN(char *pin)
{
    if (strlen(pin) > 0 && strlen(pin) == 6)
    {
        if (strcmp(pin, logged_user.pin) == 0)
        {
            return 1;
        }

        percobaan++;
        system("cls");
        showBlockedCardMessage();

        printc("\nPIN ANDA SALAH!\n", FOREGROUND_RED);
        printc("PASTIKAN PIN ANDA BENAR\n", FOREGROUND_RED);
        Sleep(2000);
        system("cls");
        return 0;
    }
    else
    {
        percobaan++;
        showBlockedCardMessage();
        system("cls");
        printc("\nPIN ANDA TIDAK VALID!\n", FOREGROUND_RED);
        printc("PASTIKAN PIN ANDA BENAR DAN TERDIRI DARI 6 DIGIT\n", FOREGROUND_RED);
        Sleep(2000);
        system("cls");
        return 0;
    }
}

void showBlockedCardMessage()
{
    if (isItBlocked())
    {
        system("cls");
        printc("\nKARTU ATM ANDA TERBLOKIR!\n", FOREGROUND_RED);
        printc("HUBUNGI CALL CENTER BANK MILLENIAL: 08787878!", FOREGROUND_RED);
        logged_user.status = "terblokir";
        exit(1);
    }
}

/**
 * @brief Untuk menampilkan menu utama ATM
 * 
 * @return int 1 jika pilihan benar, 0 jika pilihan salah
 * @todo 
 * - belum ada error handling ketika user memasukkan karakter, 
 *   sehingga langsung keluar dari console ketika input bukan angka
 */
int menu()
{

    //Untuk menghindari layar diclear ketika ada pesan error
    if (pilihan <= 5)
    {
        system("cls");
    }

    printLine("=", SCREEN_WIDTH);
    centerString("MENU UTAMA", SCREEN_WIDTH);
    printf("\n");
    printLine("=", SCREEN_WIDTH);

    printf("1 - TARIK TUNAI\n");
    printf("2 - CEK SALDO\n");
    printf("3 - TRANSFER\n");
    printf("4 - GANTI PIN\n");
    printf("5 - KELUAR\n");

    printf("\n\nPILIHAN : ");
    if (scanf("%d", &pilihan) < 1)
    {
        return 0;
    }

    switch (pilihan)
    {
    case 1:
        if (penarikan(&logged_user) == 1)
        {
            return menu();
        }
        readFile("src/util/thanks.txt");
        break;
    case 2:
        if (cekSaldo(&logged_user) == 1)
        {
            return menu();
        }
        readFile("src/util/thanks.txt");
        break;
    case 3:
        if (transfer(&logged_user) == 1)
        {
            return menu();
        }
        readFile("src/util/thanks.txt");
        break;
    case 4:
        if (gantiPin(&logged_user, pin, &percobaan) == 1)
        {
            return menu();
        }
        readFile("src/util/thanks.txt");
        break;
    case 5:
        readFile("src/util/thanks.txt");
        break;
    default:
        printc("\nPILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
        Sleep(1000);
        system("cls");
        menu();
        break;
    }
    return 0;
}