#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "util/chalk.h"
#include "util/utils.h"

char *ptr;
char norek_tujuan[5];
int pilihan;
int nominal = 0;
int biaya_admin = 6500;
nasabah nasabah_tujuan;

int cariNasabahBerdasarkanNorek(char *in_norek);
void formatNumber(int n);
void readFile(char *filename);
int inputTransferNominal(int *in_nominal);

void welcome()
{
	readFile("src/util/welcome.txt");
	printf("TEKAN 'ENTER' PADA KEYBOARD JIKA SELESAI MEMASUKKAN PIN\n");
}

void yesNoQuestion(char *text)
{
	printf("\n%s\n", text);
	printf("1. YA\n2. TIDAK\n");

	printf("Masukkan pilihan Anda: ");
	scanf("%d", &pilihan);
	if (pilihan < 0 && pilihan > 2)
	{
		printf("Pilihan Menu Tidak Tersedia!");
		yesNoQuestion(text);
	}
}

int cekSaldo(nasabah loggin_user)
{
	system("cls");
	printf("\n\n                              SISA SALDO ANDA\n\n");
	formatNumber(loggin_user.saldo);
	yesNoQuestion("\n\n\n                APAKAH ANDA INGIN MELAKUKAN TRANSAKSI LAIN?\n\n");	
	if (pilihan == 1)
		return 1;
	else
		return 0;	
}

int penarikan(nasabah loggin_user)
{
	int nominal;
	system("cls");
    printf("\n\n                   MASUKKAN NOMINAL KELIPATAN RP 50000\n\n");
    printf("                           Rp ");
    scanf("%d", &nominal);
    printf("\n\n                  1. BENAR\n\n                  2.KELUAR\n\n                  3.KEMBALI KE MENU");
    printf("\n\n\tPILIHAN : ");
    scanf("%d", &pilihan);

    switch (pilihan)
    {
    case 1:
        if ((nominal % 50000 == 0) && loggin_user.saldo > nominal)
        {
            loggin_user.saldo -= nominal;
            formatNumber(loggin_user.saldo);
			break;            
        }
        else
        {
            printf("\t\t                    Nominal Tidak Valid");
            formatNumber(loggin_user.saldo);
            break;
        }
		
    case 2:
        return 0;
		break;
    default:
        return 1;
        break;
    }    
}
/**
 * @todo Belum dilakukan pengecekan apakah inputnya karakter atau angka 
 */
int inputTransferNominal(int *in_nominal)
{
	printf("\nMasukkan Nominal: ");
	scanf("%d", in_nominal);
	if (*in_nominal < 50000)
	{
		printf("Minimal Transfer Adalah Rp 50.000");
		return inputTransferNominal(in_nominal);
	}
	else if (*in_nominal > 100000000)
	{
		printf("Maksimal Transfer per Hari Adalah Rp 100.000.000");
		return inputTransferNominal(in_nominal);
	}
	return 1;
}

int transfer(nasabah logged_user)
{
	printf("Masukkan No. Rekening Tujuan: ");
	scanf("%s", norek_tujuan);

	if (!strcmp(norek_tujuan, logged_user.norek))
	{
		printf("No. Rekening Tujuan Harus Berbeda dengan No. Rekening Anda!\n");
		transfer(logged_user);
	}

	if (cariNasabahBerdasarkanNorek(norek_tujuan))
	{
		system("cls");
		inputTransferNominal(&nominal);

		if (logged_user.saldo < nominal)
		{
			printc("Transaksi Gagal\n", FOREGROUND_RED);
			printc("Saldo Tidak Mencukupi!", FOREGROUND_RED);
			yesNoQuestion("Transaksi lagi?");
			return pilihan;
		}

		system("cls");

		printf("\nDari\t\t: %s\n", logged_user.nama);
		printf("Tujuan\t\t: %s\n", nasabah_tujuan.nama);
		printf("Jumlah\t\t: Rp ");
		formatNumber(nominal);
		printf("\nBiaya Admin\t: Rp ");
		formatNumber(biaya_admin);

		int total = nominal + biaya_admin;
		printf("\nTotal\t\t: Rp ");
		formatNumber(total);

		printf("\n\n1. Benar\n");
		printf("2. Keluar\n");
		printf("3. Kembali ke Menu\n");
		printf("Masukkan Pilihan Anda: ");
		scanf("%d", &pilihan);

		system("cls");

		switch (pilihan)
		{
		case 1:
			logged_user.saldo -= total;
			nasabah_tujuan.saldo += nominal;
			printf("Transaksi Berhasil Sisa Saldo Anda: Rp ");
			formatNumber(logged_user.saldo);
			yesNoQuestion("Apakah Anda Ingin Melakukan Transaksi Lain?");
			return pilihan;
		case 2:
			return 0;
		case 3:
			return 1;
		default:
			printf("Pilihan Menu Tidak Tersedia");
			return 0;
		}
	}
	transfer(logged_user);
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
		printf("No. Rekening Tujuan Tidak Terdaftar!\n");
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
//         system("cls");
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