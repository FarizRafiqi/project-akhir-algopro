#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

char norek_tujuan[5];
int pilihan;
int nominal = 0;
int biaya_admin = 6500;
nasabah nasabah_tujuan;

int cariNasabahBerdasarkanNorek(char *in_norek);
void formatNumber(int n);
void readFile(char *filename);

void welcome()
{
	// system("clear");
	readFile("welcome.txt");
}

// void welcomeError()
// {
//     system("clear");
//     printf("                          SELAMAT DATANG\n");
//     printf("                                DI\n");
//     printf("                           BANK BERSAMA\n\n");
//     printf("                            PIN SALAH\n\n");
//     printf("                      SILAHKAN MASUKKAN PIN");
//     printf("\n\n\t\t\t      ");
//     scanf("%d", &pin);
// }

void errorInput()
{
	system("clear");
	printf("\n\n\n                   Maaf Pilihan Tidak Tersedia\n\n");
}

int cekSaldo()
{
	system("clear");
	printf("\n\n                              SISA SALDO ANDA\n\n");
	// printf("\nRp %d", saldo);
	printf("\n\n\n                APAKAH ANDA INGIN MELAKUKAN TRANSAKSI LAIN?\n\n");
	printf("                          1. YA\n\n                          2. TIDAK\n\n");
	printf("Pilihan : ");
	// scanf("%d", &pilihan);
	// switch (pilihan)
	// {
	// case 1:
	//     menu();
	//     break;
	// case 2:
	//     keluar();
	//     break;
	// default:
	//     errorInput();
	//     cekSaldo();
	// }
	return 0;
}

// int penarikan(norek, pin)
// {
//     int nominal;

//     system("clear");
//     printf("\n\n                   MASUKKAN NOMINAL KELIPATAN RP 50000\n\n");
//     printf("                           Rp ");
//     scanf("%d", &nominal);
//     printf("\n\n                  1. BENAR\n\n                  2.KELUAR\n\n                  3.KEMBALI KE MENU");
//     printf("\n\n\tPILIHAN : ");
//     scanf("%d", &pilihan);

//     switch (pilihan)
//     {
//     case 1:

//         // if ((nominal % 50000 == 0) && data_nasabah[counter]->saldo > nominal)
//         // {
//         //     saldo -= nominal;
//         //     cekSaldo();
//         //     break;
//         // }
//         // else
//         // {
//         //     printf("\t\t                    Nominal Tidak Valid");
//         //     cekSaldo();
//         //     break;
//         // }

//     case 2:
//         keluar();
//     default:
//         menu();
//         break;
//     }
//     return 0;

int transfer(nasabah loggin_user)
{
	setlocale(LC_NUMERIC, "");
	printf("Masukkan No. Rekening Tujuan: ");
	scanf("%s", norek_tujuan);

	if (cariNasabahBerdasarkanNorek(norek_tujuan))
	{
		printf("\nMasukkan Nominal: ");
		scanf("%d", &nominal);
		if (nominal >= 50000)
		{
			if (nasabah_tujuan.saldo >= nominal)
			{
				system("clear");
				printf("\nDari   : %s\n", loggin_user.nama);
				printf("Tujuan : %s\n", nasabah_tujuan.nama);
				printf("Jumlah : ");
				formatNumber(nominal);
				printf("\nBiaya Admin : ");
				formatNumber(biaya_admin);

				printf("\n\n1. Benar\n");
				printf("2. Keluar\n");
				printf("3. Kembali ke Menu\n");
				printf("Masukkan Pilihan Anda: ");
				scanf("%d", &pilihan);
				system("clear");

				switch (pilihan)
				{
				case 1:
					loggin_user.saldo -= nominal;
					nasabah_tujuan.saldo += nominal;
					printf("Transaksi berhasil sisa saldo Anda: Rp. %d\n", loggin_user.saldo);

					printf("Apakah Anda ingin melakukan Transaksi lain? \n");
					printf("1. YA\n2. TIDAK");

					printf("Masukkan Pilihan anda: ");
					scanf("%d", &pilihan);
					return pilihan == 1 ? 0 : 1;
				case 2:
					break;
				case 3:
					return 0;
				default:
					printf("Pilihan Menu Tidak Tersedia");
					return 0;
				}
			}
			else
			{
				printf("Saldo tidak mencukupi!");
			}
		}
		else
		{
			printf("Minimal Transfer Adalah Rp. 50.000");
		}
	}
}

int cariNasabahBerdasarkanNorek(char *in_norek)
{
	if (strlen(in_norek) > 0 && strlen(in_norek) == 5)
	{
		for (int i = 0; i < TOTAL_DATA_DB; i++)
		{
			if (strcmp(in_norek, data_nasabah[i].norek) == 0)
			{
				nasabah_tujuan = data_nasabah[i];
				return 1;
			}
		}
		printf("No. Rekening Tujuan Tidak Ditemukan!\n");
		return 0;
	}
	else
	{
		printf("No. Rekening Tujuan Tidak Valid!\n");
		printf("Pastikan No. Rekening Tujuan Benar dan Terdiri dari 5 Digit\n");
		return 0;
	}
}

// void gantiPin()
// {
//     int in_pin, in_menu;
//     int pin_db = 111000;

//     printf("Masukkan PIN lama:\n");
//     scanf("%d", &in_pin);
//     if (in_pin == pin_db)
//     {
//         printf("Masukkan PIN baru:\n");
//         scanf("%d", &in_pin);
//         pin_db = in_pin;
//         printf("PIN berhasil diubah!");
//     }
//     else
//     {
//         system("clear");
//         printf("PIN lama salah, harap periksa kembali PIN lama anda!\n");
//         printf("1. Lanjutkan\n");
//         printf("2. Keluar\n");
//         printf("3. Kembali ke Menu\n");
//         printf("Masukkan Pilihan anda: ");
//         scanf("%d", &pilihan);
//     }
//     switch (pilihan)
//     {
//     case 1:
//         if (pilihan == 1)
//         {
//             gantiPin(); //lanjut masukkan PIN kembali
//             break;
//         }
//     case 2:
//         if (pilihan == 2)
//         {
//             break; //Keluar
//         }

//     case 3:
//         if (pilihan == 3)
//         {
//             //menu_utama(); //Kembali ke menu utama
//             break;
//         }
//         else
//         {
//             printf("Pilihan menu tidak tersedia!");
//             //kembali ke menu utama
//             break;
//         }
//     }
// }

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

void readFile(char *filename)
{
    char s[100];
    FILE *f;
    f = fopen(filename, "r");
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