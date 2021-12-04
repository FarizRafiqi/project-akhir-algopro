#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "util/chalk.h"
#include "util/utils.h"
#include "constants.c"

#define SCREEN_WIDTH 60

char norek_tujuan[5];
char konfirmasi_pin[6];
int nominal = 0;
int biaya_admin = 6500;
int percobaan = 0;
char pin[6];
char norek[5];
int pilihan = 0;
int is_login = 0;

nasabah logged_user;
nasabah nasabah_tujuan;

void formatNumber(int n);
void readFile(char *filename);
int inputTransferNominal(int *in_nominal);
int transactionQuestion();
int isPINValid(char *pin);
int confirmNewPIN(char *pin);
void simpanStruk(nasabah *logged_user, nasabah nasabah_tujuan, int nominal, int total);
void menu();
void penarikan(nasabah *logged_user);
int cekSaldo(nasabah *logged_user);
void transfer(nasabah *logged_user);
void gantiPin(nasabah *logged_user, char *pin, int *percobaan);

int isItBlocked()
{
	return percobaan > 3 ? 1 : 0;
}

void welcome()
{
	readFile("src\\util\\welcome.txt");
	printf("TEKAN 'ENTER' PADA KEYBOARD JIKA SELESAI MEMASUKKAN PIN\n");
}

void exitProgram()
{
	system("cls");
	readFile("src\\util\\thanks.txt");
	exit(1);
}

void yesNoQuestion(char *text)
{
	centerString(text, SCREEN_WIDTH);
	printf("\n\n1. YA\n2. TIDAK\n");

	printf("MASUKKAN PILIHAN ANDA: ");
	scanf("%d", &pilihan);
	if (pilihan < 1 && pilihan > 2)
	{
		printc("PILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
		Sleep(1000);
		yesNoQuestion(text);
	}

	if (pilihan == 1)
	{
		menu();
	}
	else
	{
		exitProgram();
	}
}

int transactionQuestion()
{
	printf("\n\n1. BENAR\n");
	printf("2. KELUAR\n");
	printf("3. KEMBALI KE MENU\n");
	printf("MASUKKAN PILIHAN ANDA: ");
	scanf("%d", &pilihan);

	if (pilihan < 1 && pilihan > 3)
	{
		printc("PILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
		Sleep(1000);
		return transactionQuestion();
	}

	if (pilihan == 2)
	{
		exitProgram();
	}
	else if (pilihan == 3)
	{
		menu();
	}

	return pilihan;
}

void showBlockedCardMessage()
{
	if (isItBlocked())
	{
		system("cls");
		printc("\nKARTU ATM ANDA TERBLOKIR!\n", FOREGROUND_RED);
		printc("HUBUNGI CALL CENTER BANK MILLENIAL: 08787878!", FOREGROUND_RED);
		logged_user.status = "terblokir";
		exitProgram();
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

/**
 * @brief Untuk menampilkan menu utama ATM
 *
 * @return int 1 jika pilihan benar, 0 jika pilihan salah
 * @todo
 * - belum ada error handling ketika user memasukkan karakter,
 *   sehingga langsung keluar dari console ketika input bukan angka
 */
void menu()
{
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
		exitProgram();
	}

	switch (pilihan)
	{
	case 1:
		penarikan(&logged_user);
		break;
	case 2:
		cekSaldo(&logged_user);
		break;
	case 3:
		transfer(&logged_user);
		break;
	case 4:
		gantiPin(&logged_user, pin, &percobaan);
		break;
	case 5:
		exitProgram();
		break;
	default:
		printc("\nPILIHAN MENU TIDAK TERSEDIA!\n", FOREGROUND_RED);
		Sleep(1000);
		system("cls");
		menu();
	}
}

int autentikasiNorek(char *norek)
{
	if (strlen(norek) > 0 && strlen(norek) == 5)
	{
		for (int i = 0; i < TOTAL_DATA_DB; i++)
		{
			if (strcmp(norek, data_nasabah[i].norek) == 0)
			{
				// untuk mengecek apakah user sudah login atau belum
				if (!is_login)
				{
					is_login = 1;
					logged_user = data_nasabah[i];
				}
				return 1;
			}
		}
		/**
		 * jika program menjalankan kode di bawah ini,
		 * tandanya ketika mencocokkan no rekening yg diinput
		 * dengan yang ada di mini database, itu tidak ada yg
		 * cocok sama sekali. Artinya No. Rekening tidak terdaftar.
		 */
		printc("\nNO. REKENING TIDAK TERDAFTAR!\n", FOREGROUND_RED);
		Sleep(2000);
		system("cls");
		return 0;
	}
	else
	{
		printc("\nNO. REKENING ANDA TIDAK VALID!\n", FOREGROUND_RED);
		printc("PASTIKAN NO. REKENING ANDA BENAR DAN TERDIRI DARI 5 DIGIT\n", FOREGROUND_RED);
		Sleep(2500);
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

int cariNasabahBerdasarkanNorek(char *norek)
{
	if (autentikasiNorek(norek))
	{
		for (int i = 0; i < TOTAL_DATA_DB; i++)
		{
			if (strcmp(norek, data_nasabah[i].norek) == 0)
			{
				nasabah_tujuan = data_nasabah[i];
				return 1;
			}
		}
		printc("NO. REKENING TUJUAN TIDAK TERDAFTAR!\n", FOREGROUND_RED);
		Sleep(1000);
		return 0;
	}
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
}

void penarikan(nasabah *logged_user)
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
		exitProgram();
	}

	if (transactionQuestion())
	{
		if (!(nominal % 50000 == 0))
		{
			printc("Nominal Tidak Valid", FOREGROUND_RED);
			Sleep(1500);
			system("cls");
			return penarikan(logged_user);
		}

		if (!isTheBalanceEnough(logged_user->saldo, nominal))
		{
			yesNoQuestion("TRANSAKSI LAGI?");
		}

		logged_user->saldo -= nominal;
		system("cls");
		cekSaldo(logged_user);
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

void transfer(nasabah *logged_user)
{
	system("cls");

	printLine("=", SCREEN_WIDTH);
	centerString("TRANSFER", SCREEN_WIDTH);
	printf("\n");
	printLine("=", SCREEN_WIDTH);

	printf("MASUKKAN NO. REKENING TUJUAN: ");

	if (scanf("%s", &norek_tujuan) < 1)
	{
		exitProgram();
	}

	if (!autentikasiNorek(norek_tujuan))
	{
		transfer(logged_user);
	}

	if (strcmp(norek_tujuan, logged_user->norek) == 0)
	{
		printc("NO. REKENING TUJUAN HARUS BERBEDA DENGAN NO. REKENING ANDA!\n", FOREGROUND_RED);
		Sleep(2500);
		transfer(logged_user);
	}

	if (!cariNasabahBerdasarkanNorek(norek_tujuan))
	{
		transfer(logged_user);
	}

	system("cls");
	inputTransferNominal(&nominal);

	if (!isTheBalanceEnough(logged_user->saldo, nominal))
	{
		yesNoQuestion("TRANSAKSI LAGI?");
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

	if (transactionQuestion())
	{
		logged_user->saldo -= total;
		nasabah_tujuan.saldo += nominal;

		printc("TRANSAKSI BERHASIL\n", FOREGROUND_GREEN);
		simpanStruk(logged_user, nasabah_tujuan, nominal, total);
		cekSaldo(logged_user);
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

void gantiPin(nasabah *logged_user, char *pin, int *percobaan)
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

	//
	if (!autentikasiPIN(pin))
	{
		gantiPin(logged_user, pin, percobaan);
	}

	printf("\nMASUKKAN PIN BARU: ");
	maskingInput(pin, "*");
	// if (scanf("%s", pin) < 1)
	// {
	// 	return 0;
	// }

	if (!autentikasiPIN(pin))
	{
		gantiPin(logged_user, pin, percobaan);
	}

	if (confirmNewPIN(pin))
	{
		logged_user->pin = pin;

		printc("\n\nPIN BERHASIL DIUBAH SEMENTARA!\n\n", FOREGROUND_GREEN);
		printc("\nPIN AKAN KEMBALI SEPERTI SEMULA JIKA PROGRAM DITUTUP!\n", FOREGROUND_BLUE);
		printf("PIN BARU: %s\n", pin);
		yesNoQuestion("APAKAH ANDA INGIN MELAKUKAN TRANSAKSI LAIN?");
	}
}

int confirmNewPIN(char *pin)
{
	printf("\nKONFIRMASI PIN BARU: ");
	maskingInput(konfirmasi_pin, "*");
	//	if (scanf("%s", konfirmasi_pin) < 1)
	//	{
	//		return 0;
	//	}

	if (!autentikasiPIN(konfirmasi_pin))
	{
		return confirmNewPIN(pin);
	}

	if (strcmp(pin, konfirmasi_pin) == 1)
	{
		return 1;
	}
}
