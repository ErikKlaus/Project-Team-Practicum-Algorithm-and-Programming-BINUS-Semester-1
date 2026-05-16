#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 500

typedef struct {
    char kode[10];
    char nama[100];
    char jenis[50];
    double harga;
} Buku;

typedef struct {
    char kodeBuku[10];
    int jumlahTerjual;
    double totalHarga;
} History;

Buku daftarBuku[MAX_DATA];
History daftarHistory[MAX_DATA];

int jmlBuku = 0;
int jmlHistory = 0;

void bacaString(char pesan[], char hasil[], int ukuran) {
    printf("%s", pesan);
    fgets(hasil, ukuran, stdin);
    hasil[strcspn(hasil, "\n")] = '\0';
}

int bacaInt(char pesan[]) {
    char input[100];
    int angka;

    while (1) {
        printf("%s", pesan);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%d", &angka) == 1) {
            return angka;
        }

        printf("Input harus berupa angka!\n");
    }
}

double bacaDouble(char pesan[]) {
    char input[100];
    double angka;

    while (1) {
        printf("%s", pesan);
        fgets(input, sizeof(input), stdin);

        if (sscanf(input, "%lf", &angka) == 1) {
            return angka;
        }

        printf("Input harus berupa angka!\n");
    }
}

void buatKodeBuku(char kodeBaru[]) {
    sprintf(kodeBaru, "BK%03d", jmlBuku + 1);
}

void loadData() {
    FILE *fBuku = fopen("databuku.txt", "r");

    if (fBuku != NULL) {
        while (jmlBuku < MAX_DATA &&
               fscanf(fBuku, "%9[^#]#%99[^#]#%49[^#]#%lf\n",
                      daftarBuku[jmlBuku].kode,
                      daftarBuku[jmlBuku].nama,
                      daftarBuku[jmlBuku].jenis,
                      &daftarBuku[jmlBuku].harga) == 4) {
            jmlBuku++;
        }

        fclose(fBuku);
    }

    FILE *fHistory = fopen("history.txt", "r");

    if (fHistory != NULL) {
        while (jmlHistory < MAX_DATA &&
               fscanf(fHistory, "%9[^#]#%d#%lf\n",
                      daftarHistory[jmlHistory].kodeBuku,
                      &daftarHistory[jmlHistory].jumlahTerjual,
                      &daftarHistory[jmlHistory].totalHarga) == 3) {
            jmlHistory++;
        }

        fclose(fHistory);
    }
}

void simpanData() {
    FILE *fBuku = fopen("databuku.txt", "w");

    if (fBuku == NULL) {
        printf("Gagal menyimpan databuku.txt!\n");
        return;
    }

    for (int i = 0; i < jmlBuku; i++) {
        fprintf(fBuku, "%s#%s#%s#%.2lf\n",
                daftarBuku[i].kode,
                daftarBuku[i].nama,
                daftarBuku[i].jenis,
                daftarBuku[i].harga);
    }

    fclose(fBuku);

    FILE *fHistory = fopen("history.txt", "w");

    if (fHistory == NULL) {
        printf("Gagal menyimpan history.txt!\n");
        return;
    }

    for (int i = 0; i < jmlHistory; i++) {
        fprintf(fHistory, "%s#%d#%.2lf\n",
                daftarHistory[i].kodeBuku,
                daftarHistory[i].jumlahTerjual,
                daftarHistory[i].totalHarga);
    }

    fclose(fHistory);
}

void inputBuku() {
    if (jmlBuku >= MAX_DATA) {
        printf("Data buku sudah penuh!\n");
        return;
    }

    printf("\n=== Input Data Buku Baru ===\n");

    buatKodeBuku(daftarBuku[jmlBuku].kode);

    printf("Kode Buku Otomatis: %s\n", daftarBuku[jmlBuku].kode);

    bacaString("Nama Buku  : ", daftarBuku[jmlBuku].nama, sizeof(daftarBuku[jmlBuku].nama));
    bacaString("Jenis Buku : ", daftarBuku[jmlBuku].jenis, sizeof(daftarBuku[jmlBuku].jenis));

    do {
        daftarBuku[jmlBuku].harga = bacaDouble("Harga Buku : ");

        if (daftarBuku[jmlBuku].harga <= 0) {
            printf("Harga buku harus lebih dari 0!\n");
        }
    } while (daftarBuku[jmlBuku].harga <= 0);

    jmlBuku++;

    printf("Data Buku Berhasil Ditambahkan!\n");
}

void viewHistory() {
    printf("\n=== Histori Transaksi Penjualan ===\n");

    if (jmlHistory == 0) {
        printf("Belum ada histori transaksi.\n");
        return;
    }

    printf("%-3s | %-10s | %-15s | %-15s\n",
           "No", "Kode Buku", "Terjual", "Total Harga");

    printf("------------------------------------------------------\n");

    for (int i = 0; i < jmlHistory; i++) {
        printf("%-3d | %-10s | %-15d | Rp %.2lf\n",
               i + 1,
               daftarHistory[i].kodeBuku,
               daftarHistory[i].jumlahTerjual,
               daftarHistory[i].totalHarga);
    }
}

void viewBuku() {
    printf("\n=== Daftar Buku ===\n");

    if (jmlBuku == 0) {
        printf("Belum ada data buku.\n");
        return;
    }

    printf("%-3s | %-10s | %-30s | %-20s | %-15s\n",
           "No", "Kode", "Nama Buku", "Jenis Buku", "Harga");

    printf("--------------------------------------------------------------------------------------\n");

    for (int i = 0; i < jmlBuku; i++) {
        printf("%-3d | %-10s | %-30s | %-20s | Rp %.2lf\n",
               i + 1,
               daftarBuku[i].kode,
               daftarBuku[i].nama,
               daftarBuku[i].jenis,
               daftarBuku[i].harga);
    }
}

void deleteHistory() {
    if (jmlHistory == 0) {
        printf("\nBelum ada histori transaksi yang bisa dihapus.\n");
        return;
    }

    viewHistory();

    int index = bacaInt("\nMasukkan index history yang ingin dihapus: ");

    if (index < 1 || index > jmlHistory) {
        printf("Index tidak valid!\n");
        return;
    }

    for (int i = index - 1; i < jmlHistory - 1; i++) {
        daftarHistory[i] = daftarHistory[i + 1];
    }

    jmlHistory--;

    printf("Data Successfully delete..\n");
}

void deleteBuku() {
    if (jmlBuku == 0) {
        printf("\nBelum ada data buku yang bisa dihapus.\n");
        return;
    }

    viewBuku();

    int index = bacaInt("\nMasukkan index buku yang ingin dihapus: ");

    if (index < 1 || index > jmlBuku) {
        printf("Index tidak valid!\n");
        return;
    }

    for (int i = index - 1; i < jmlBuku - 1; i++) {
        daftarBuku[i] = daftarBuku[i + 1];
    }

    jmlBuku--;

    printf("Data Successfully delete..\n");
}

int cariBuku(char kode[]) {
    for (int i = 0; i < jmlBuku; i++) {
        if (strcmp(daftarBuku[i].kode, kode) == 0) {
            return i;
        }
    }

    return -1;
}

void inputTransaksi() {
    if (jmlBuku == 0) {
        printf("\nBelum ada data buku. Tambahkan buku terlebih dahulu.\n");
        return;
    }

    if (jmlHistory >= MAX_DATA) {
        printf("Data history sudah penuh!\n");
        return;
    }

    printf("\n=== Input Transaksi Penjualan ===\n");

    viewBuku();

    char kode[10];
    bacaString("\nMasukkan kode buku: ", kode, sizeof(kode));

    int indexBuku = cariBuku(kode);

    if (indexBuku == -1) {
        printf("Kode buku tidak ditemukan!\n");
        return;
    }

    int jumlah;

    do {
        jumlah = bacaInt("Masukkan jumlah terjual: ");

        if (jumlah <= 0) {
            printf("Jumlah terjual harus lebih dari 0!\n");
        }
    } while (jumlah <= 0);

    strcpy(daftarHistory[jmlHistory].kodeBuku, kode);
    daftarHistory[jmlHistory].jumlahTerjual = jumlah;
    daftarHistory[jmlHistory].totalHarga = jumlah * daftarBuku[indexBuku].harga;

    jmlHistory++;

    simpanData();

    printf("Transaksi berhasil dicatat.\n");
}

void tampilMenu() {
    printf("\n========================================\n");
    printf(" TOKO BUKU LITERASI NUSANTARA JAYA\n");
    printf("========================================\n");
    printf("1. Input Data Buku Baru\n");
    printf("2. View History Transaksi Penjualan\n");
    printf("3. View Buku\n");
    printf("4. Delete History\n");
    printf("5. Delete Buku\n");
    printf("6. Exit\n");
    printf("7. Input Transaksi Penjualan\n");
    printf("========================================\n");
}

int main() {
    int menu;

    loadData();

    do {
        tampilMenu();
        menu = bacaInt("Pilih menu: ");

        switch (menu) {
            case 1:
                inputBuku();
                break;

            case 2:
                viewHistory();
                break;

            case 3:
                viewBuku();
                break;

            case 4:
                deleteHistory();
                break;

            case 5:
                deleteBuku();
                break;

            case 6:
                simpanData();
                printf("Data berhasil disimpan. Program selesai.\n");
                break;

            case 7:
                inputTransaksi();
                break;


            default:
                printf("Menu tidak valid!\n");
                break;
        }

    } while (menu != 6);

    return 0;
}