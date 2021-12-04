#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "util/chalk.h"
#include "util/utils.h"
#include "constants.c"

int is_error = 0;
char norek_tujuan[5];
char konfirmasi_pin[6];
int pilihan;
int nominal = 0;
int biaya_admin = 6500;
nasabah nasabah_tujuan;

#define SCREEN_WIDTH 60

int cariNasabahBerdasarkanNorek(char *in_norek);
void formatNumber(int n);
void readFile(char *filename);
int inputTransferNominal(int *in_nominal);
void transactionQuestion();
int isPINValid(char *pin);
int confirmNewPIN(char *pin);
int isTheBalanceEnough(int saldo, int nominal);
void simpanStruk(nasabah *logged_user, nasabah nasabah_tujuan, int nominal, int total);

void welcome()
{
	readFile("src\\util\\welcome.txt");
	printf("TEKAN 'ENTER' PADA KEYBOARD JIKA SELESAI MEMASUKKAN PIN\n");
}

void yesNoQuestion(char *text)
{
	centerString(text, SCREEN_WIDTH);
	printf("\n\n1. YA\n2. TIDAK\n");

	printf("MASUKKAN PILIHAN ANDA: ");
	scanf("%d", &pilihan);
	if (pilihan < 0 && pilihan > 2)
	{
		printc("PILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
		yesNoQuestion(text);
	}
}

void transactionQuestion()
{
	printf("\n\n1. BENAR\n");
	printf("2. KELUAR\n");
	printf("3. KEMBALI KE MENU\n");
	printf("MASUKKAN PILIHAN ANDA: ");
	scanf("%d", &pilihan);
}

int cekSaldo(nasabah *logged_user)
{
	system("cls");
	centerString("SISA SALDO ANDA", SCREEN_WIDTH);
	printf("\n\n");
	centerString("Rp ", SCREEN_WIDTH - 10);
	formatNumber(logged_user->saldo);
	printf("\n\n");
	yesNoQuestion("APAKAH ANDA INGIN MELAKUKAN TRANSAKSI LAIN?");

	if (pilihan == 1)
		return 1;
	else if (pilihan == 2)
		return 0;
	else
	{
		system("cls");
		printc("PILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
		Sleep(1000);
		yesNoQuestion("APAKAH ANDA INGIN MELAKUKAN TRANSAKSI LAIN?");
	}
}

int penarikan(nasabah *logged_user)
{
	int nominal;
	system("cls");
	printLine("=", SCREEN_WIDTH);
	centerString("PENARIKAN", SCREEN_WIDTH);
	printf("\n");
	printLine("=", SCREEN_WIDTH);

	centerString("MASUKKAN NOMINAL KELIPATAN RP 50000\n\n", SCREEN_WIDTH);
	centerString("Rp ", SCREEN_WIDTH - 10);

	if (scanf("%d", &nominal) < 1)
	{
		return 0;
	}

	transactionQuestion();
	switch (pilihan)
	{
	case 1:
		if ((nominal % 50000 == 0))
		{
			if (!isTheBalanceEnough(logged_user->saldo, nominal))
			{
				yesNoQuestion("TRANSAKSI LAGI?");
				return pilihan == 1 ? pilihan : 0;
			}

			logged_user->saldo -= nominal;
			system("cls");
			return cekSaldo(logged_user);
			break;
		}
		else
		{
			printc("Nominal Tidak Valid", FOREGROUND_RED);
			Sleep(1500);
			system("cls");
			return penarikan(logged_user);
			break;
		}
		break;
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
	system("cls");
	printLine("=", SCREEN_WIDTH);
	centerString("TRANSFER", SCREEN_WIDTH);
	printf("\n");
	printLine("=", SCREEN_WIDTH);
	centerString("MINIMAL TRANSFER ADALAH RP 50.000", SCREEN_WIDTH);

	printf("\n\nMASUKKAN NOMINAL: ");
	if (scanf("%d", in_nominal) < 1)
	{
		return 0;
	}

	if (*in_nominal < 50000)
	{
		printc("MINIMAL TRANSFER ADALAH Rp 50.000", FOREGROUND_RED);
		Sleep(1000);
		system("cls");
		return inputTransferNominal(in_nominal);
	}
	else if (*in_nominal > 100000000)
	{
		printc("MAKSIMAL TRANSFER PER HARI ADALAH Rp 100.000.000", FOREGROUND_RED);
		Sleep(1000);
		system("cls");
		return inputTransferNominal(in_nominal);
	}
	return 1;
}

int transfer(nasabah *logged_user)
{
	system("cls");

	printLine("=", SCREEN_WIDTH);
	centerString("TRANSFER", SCREEN_WIDTH);
	printf("\n");
	printLine("=", SCREEN_WIDTH);

	printf("MASUKKAN NO. REKENING TUJUAN: ");
	if (scanf("%s", norek_tujuan) < 1)
	{
		return 0;
	}

	if (!strcmp(norek_tujuan, logged_user->norek))
	{
		printc("NO. REKENING TUJUAN HARUS BERBEDA DENGAN NO. REKENING ANDA!\n", FOREGROUND_RED);
		Sleep(2500);
		transfer(logged_user);
	}

	if (!cariNasabahBerdasarkanNorek(norek_tujuan))
	{

		return transfer(logged_user);
	}

	system("cls");
	inputTransferNominal(&nominal);

	if (!isTheBalanceEnough(logged_user->saldo, nominal))
	{
		yesNoQuestion("TRANSAKSI LAGI?");
		return pilihan == 1 ? pilihan : 0;
	}

	system("cls");

	printLine("=", SCREEN_WIDTH);
	centerString("KONFIRMASI TRANSFER ATM", SCREEN_WIDTH);
	printf("\n");
	printLine("=", SCREEN_WIDTH);

	printf("\nDARI\t\t: %s (%s)\n", logged_user->nama, logged_user->norek);
	printf("TUJUAN\t\t: %s (%s)\n", nasabah_tujuan.nama, nasabah_tujuan.norek);
	printf("JUMLAH\t\t: Rp ");
	formatNumber(nominal);
	printf("\nBIAYA ADMIN\t: Rp ");
	formatNumber(biaya_admin);

	int total = nominal + biaya_admin;
	printf("\nTOTAL\t\t: Rp ");
	formatNumber(total);

	transactionQuestion();

	system("cls");

	switch (pilihan)
	{
	case 1:
		logged_user->saldo -= total;
		nasabah_tujuan.saldo += nominal;
		printf("TRANSAKSI BERHASIL\n");

		simpanStruk(logged_user, nasabah_tujuan, nominal, total);

		cekSaldo(logged_user);
		system("cls");
		return pilihan;
	case 2:
		return 0;
	case 3:
		return 1;
	default:
		printc("PILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
		transfer(logged_user);
		return 0;
	}
}

int isTheBalanceEnough(int saldo, int nominal)
{
	if (saldo < nominal)
	{
		system("cls");
		printc("TRANSAKSI GAGAL\n", FOREGROUND_RED);
		printc("SALDO TIDAK MENCUKUPI!\n", FOREGROUND_RED);
		return 0;
	}
	return 1;
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
		printc("NO. REKENING TUJUAN TIDAK TERDAFTAR!\n", FOREGROUND_RED);
		Sleep(1000);
		return 0;
	}
	else
	{
		printc("NO. REKENING TUJUAN TIDAK VALID!\n", FOREGROUND_RED);
		printc("PASTIKAN NO. REKENING TUJUAN BENAR DAN TERDIRI DARI 5 DIGIT\n", FOREGROUND_RED);
		Sleep(1000);
		return 0;
	}
}

void simpanStruk(nasabah *logged_user, nasabah nasabah_tujuan, int nominal, int total)
{
	FILE *file_struk;
	char tanggal[50] = "";
	getDate(tanggal);

	char nama_file[100] = "struk_";
	strcat(nama_file, logged_user->norek);
	strcat(nama_file, "_");

	char *result = strrep(tanggal, "/", "_");
	strcat(nama_file, result);

	char fullpath[MAX_PATH];
	basePath(fullpath, "src\\struk_nasabah");
	// sprintf(fullpath, "%s%s%s", "src/struk_nasabah/", nama_file, ".txt");
	strcat(fullpath, nama_file);
	strcat(fullpath, ".txt");
	const char *absolute_fullpath = fullpath;
	// printf("%s", absolute_fullpath);exit(1);
	file_struk = fopen("src/struk_nasabah/struk.txt", "w");
	if (file_struk == NULL)
	{
		printc("FILE TIDAK BISA DIBUKA!", FOREGROUND_RED);
	}
	else
	{
		char *nama_bank = APP_NAME;
		strcat(tanggal, "\n");
		char lokasi[50] = "UPN Veteran Jakarta\n";
		// char *title = getCenteredString("TRANSFER ATM\n", SCREEN_WIDTH);
		char kolom_bank_asal[50] = "BANK\t\t:";
		strcat(kolom_bank_asal, nama_bank);
		strcat(kolom_bank_asal, "\n");
		// printf("%s", kolom_bank_asal);
		// exit(1);
		char kolom_nama_pengirim[100] = "NAMA\t\t:";
		strcat(kolom_nama_pengirim, logged_user->nama);
		strcat(kolom_nama_pengirim, "\n");

		char kolom_bank_tujuan[50] = "BANK\t\t:";
		strcat(kolom_bank_tujuan, APP_NAME);
		strcat(kolom_bank_tujuan, "\n");

		char kolom_nama_penerima[100] = "NAMA\t\t:";
		strcat(kolom_nama_penerima, nasabah_tujuan.nama);
		strcat(kolom_nama_penerima, "\n");

		char kolom_rekening[50] = "REKENING\t:";
		strcat(kolom_rekening, nasabah_tujuan.norek);
		strcat(kolom_rekening, "\n\n");

		char kolom_jumlah[100] = "JUMLAH\t\t:";
		char jumlah[50] = "";
		itoa(nominal, jumlah, 10);
		strcat(kolom_jumlah, jumlah);
		strcat(kolom_jumlah, "\n");

		char kolom_biaya_admin[20] = "BIAYA ADMIN\t:";
		char admin_fee[10];
		itoa(biaya_admin, admin_fee, 10);
		strcat(kolom_biaya_admin, admin_fee);
		strcat(kolom_biaya_admin, "\n");

		char kolom_total[20] = "TOTAL\t\t:";
		char total_val[10];
		itoa(total, total_val, 10);
		strcat(kolom_total, total_val);
		strcat(kolom_total, "\n");
		// fputs(getCenteredString(nama_bank, SCREEN_WIDTH), file_struk);
		fputs(tanggal, file_struk);
		fputs(lokasi, file_struk);
		// fputs(title, file_struk);
		fputs("DARI\n", file_struk);
		fputs(kolom_bank_asal, file_struk);
		fputs(kolom_nama_pengirim, file_struk);
		fputs("KE\n", file_struk);
		fputs(kolom_bank_tujuan, file_struk);
		fputs(kolom_nama_penerima, file_struk);
		fputs(kolom_rekening, file_struk);
		fputs(kolom_jumlah, file_struk);
		fputs(kolom_biaya_admin, file_struk);
		fputs(kolom_total, file_struk);
	}
	fclose(file_struk);
}

int gantiPin(nasabah *logged_user, char *pin, int *percobaan)
{
	system("cls");

	printLine("=", SCREEN_WIDTH);
	centerString("GANTI PIN", SCREEN_WIDTH);
	printf("\n");
	printLine("=", SCREEN_WIDTH);

	centerString("GANTI PIN SECARA BERKALA DEMI KEAMANAN REKENING ANDA\n\n\n", SCREEN_WIDTH);

	printf("MASUKKAN PIN LAMA: ");
	maskingInput(pin, "*");
	//	if (scanf("%s", pin) < 1)
	//	{
	//		return 0;
	//	}

	if (!isPINValid(pin))
	{
		percobaan++;
		gantiPin(logged_user, pin, percobaan);
	}

	if (strcmp(pin, logged_user->pin) == 0)
	{
		printf("\nMASUKKAN PIN BARU: ");
		maskingInput(pin, "*");
		//		if (scanf("%s", pin) < 1)
		//		{
		//			return 0;
		//		}

		if (!isPINValid(pin))
		{
			percobaan++;
			gantiPin(logged_user, pin, percobaan);
		}

		confirmNewPIN(pin);
		logged_user->pin = pin;

		printc("\n\nPIN BERHASIL DIUBAH!\n\n", FOREGROUND_GREEN);
		yesNoQuestion("APAKAH ANDA INGIN MELAKUKAN TRANSAKSI LAIN?");

		switch (pilihan)
		{
		case 1:
			return 1;
		case 2:
			return 0;
		default:
			break;
		}
	}
	else
	{
		percobaan++;
		printc("\nPIN ANDA SALAH!\n", FOREGROUND_RED);
		printc("PASTIKAN PIN ANDA BENAR!\n", FOREGROUND_RED);
		Sleep(2000);
		system("cls");
		gantiPin(logged_user, pin, percobaan);
	}
}

int isPINValid(char *pin)
{
	if (strlen(pin) > 0 && strlen(pin) == 6)
		return 1;

	printc("\nPIN ANDA TIDAK VALID!\n", FOREGROUND_RED);
	printc("PASTIKAN PIN ANDA BENAR DAN TERDIRI DARI 6 DIGIT!\n", FOREGROUND_RED);
	Sleep(2000);
	system("cls");
	return 0;
}

int confirmNewPIN(char *pin)
{
	printf("\nKONFIRMASI PIN BARU: ");
	maskingInput(konfirmasi_pin, "*");
	//	if (scanf("%s", konfirmasi_pin) < 1)
	//	{
	//		return 0;
	//	}

	if (!isPINValid(konfirmasi_pin))
	{
		return confirmNewPIN(pin);
	}

	if (strcmp(pin, konfirmasi_pin) == 1)
	{
		return 1;
	}

	return 0;
}
