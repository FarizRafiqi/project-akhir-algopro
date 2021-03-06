#include <stdio.h>
#include "db_bank.h"
#include "fungsi.h"

void inputPIN();

int main()
{
    /**
     * Ketika user memasukkan norek, maka jalankan kode di bawah ini
     * (ditandai dengan scanf() mengembalikan integer lebih dari 0).
     * Jika tidak, maka jangan jalankan kode di bawah ini.
     * Hal ini guna menghindari duplikasi output.
     */

    // inisialisasi data nasabah
    dataNasabah(data_nasabah);
    // logged_user = data_nasabah[0];
    // is_login = 1;
    // cekSaldo(&logged_user);
    printf("MASUKKAN NO. REKENING ANDA: ");
    if (scanf("%s", norek) == 1)
    {
        if (autentikasiNorek(norek))
        {
            system("cls");
            welcome();
            inputPIN();
        }
        main();
    }

    return 0;
}

void inputPIN()
{
    printf("MASUKKAN PIN ATM ANDA: ");
    maskingInput(pin, "*");
    // scanf("%s", &pin);
    if (autentikasiPIN(pin))
    {
        system("cls");
        menu();
    }
    printHeader("ATM MILLENIAL");
    inputPIN();
}
