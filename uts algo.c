/*
 * Sistem Monitoring Stok dan Penjualan Kantin Kampus
 * Aplikasi console berbahasa C
 *
 * Struktur data utama (semua berbasis array):
 *   namaProduk[MAKS_PRODUK][PANJANG_NAMA] -> array karakter 2 dimensi (nama)
 *   hargaProduk[MAKS_PRODUK]              -> array 1 dimensi (harga)
 *   stokProduk[MAKS_PRODUK]               -> array 1 dimensi (stok)
 *   terjualProduk[MAKS_PRODUK]            -> array 1 dimensi (jumlah terjual)
 *   jumlahProduk                          -> jumlah data aktif (indeks berikutnya)
 *
 * Indeks yang sama di semua array menunjuk ke produk yang sama.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAKS_PRODUK   50   /* kapasitas maksimal produk */
#define PANJANG_NAMA  30   /* panjang maksimal nama (termasuk '\0') */
#define BATAS_MENIPIS 5    /* stok di bawah angka ini dianggap menipis */

/* ---------- Data utama (array) ---------- */
char namaProduk[MAKS_PRODUK][PANJANG_NAMA];
int  hargaProduk[MAKS_PRODUK];
int  stokProduk[MAKS_PRODUK];
int  terjualProduk[MAKS_PRODUK];
int  jumlahProduk = 0;

/* ---------- Deklarasi fungsi ---------- */
void isiDataAwal(void);
void tampilMenu(void);
int  bacaInteger(const char *pesan, int *hasil);
void bacaTeks(const char *pesan, char *tujuan, int ukuran);
int  pilihNomorProduk(const char *pesan);
void garis(void);

void tampilDaftarProduk(void);
void tambahProduk(void);
void ubahProduk(void);
void transaksiPenjualan(void);
void tambahStok(void);
void cariProduk(void);
void urutkanProduk(void);
void laporanPenjualan(void);
void cekStokMenipis(void);

/* ================================================================== */
/*                        FUNGSI BANTU / INPUT                         */
/* ================================================================== */

void garis(void)
{
    printf("------------------------------------------------------------\n");
}

/*
 * Membaca satu bilangan bulat dari pengguna dengan aman.
 * Mengembalikan 1 jika berhasil, 0 jika input bukan angka.
 */
int bacaInteger(const char *pesan, int *hasil)
{
    char buffer[64];
    char *akhir;
    long nilai;

    printf("%s", pesan);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\nInput berakhir. Program ditutup.\n");
        exit(0);
    }

    nilai = strtol(buffer, &akhir, 10);

    /* tidak ada angka yang terbaca */
    if (akhir == buffer) {
        printf("[!] Input tidak valid! Masukkan angka bulat.\n");
        return 0;
    }
    /* lewati spasi setelah angka; jika masih ada karakter lain -> tidak valid */
    while (*akhir == ' ' || *akhir == '\t') {
        akhir++;
    }
    if (*akhir != '\n' && *akhir != '\0') {
        printf("[!] Input tidak valid! Masukkan angka bulat.\n");
        return 0;
    }
    if (nilai > 1000000000L || nilai < -1000000000L) {
        printf("[!] Angka terlalu besar!\n");
        return 0;
    }

    *hasil = (int)nilai;
    return 1;
}

/* Membaca teks (boleh mengandung spasi) dan membuang karakter newline. */
void bacaTeks(const char *pesan, char *tujuan, int ukuran)
{
    printf("%s", pesan);
    if (fgets(tujuan, ukuran, stdin) == NULL) {
        printf("\nInput berakhir. Program ditutup.\n");
        exit(0);
    }
    tujuan[strcspn(tujuan, "\n")] = '\0';

    /* jika input lebih panjang dari buffer, buang sisa karakternya */
    if (strlen(tujuan) == (size_t)(ukuran - 1)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* buang */
        }
    }
}

/*
 * Meminta nomor produk (1..jumlahProduk).
 * Mengembalikan indeks array (0..jumlahProduk-1), atau -1 jika tidak valid.
 */
int pilihNomorProduk(const char *pesan)
{
    int nomor;

    if (!bacaInteger(pesan, &nomor)) {
        return -1;
    }
    if (nomor < 1 || nomor > jumlahProduk) {
        printf("[!] Nomor produk %d tidak ditemukan!\n", nomor);
        return -1;
    }
    return nomor - 1;
}

/* ================================================================== */
/*                              DATA AWAL                              */
/* ================================================================== */

/* Mengisi 6 produk awal (minimal 5 sesuai ketentuan). */
void isiDataAwal(void)
{
    const char *nama[]  = {"Seblak Jeletot", "Batagor Kuah", "Es Cincau Hijau",
                           "Roti Bakar Coklat", "Kopi Susu Gula Aren",
                           "Cireng Isi Ayam"};
    int harga[] = {13000, 11000, 5000, 8000, 9000, 2500};
    int stok[]  = {18, 12, 25, 3, 22, 40};
    int i;

    for (i = 0; i < 6; i++) {
        strcpy(namaProduk[i], nama[i]);
        hargaProduk[i]   = harga[i];
        stokProduk[i]    = stok[i];
        terjualProduk[i] = 0;
    }
    jumlahProduk = 6;
}

void tampilMenu(void)
{
    printf("\n============================================================\n");
    printf("   SISTEM MONITORING STOK & PENJUALAN KANTIN KAMPUS\n");
    printf("============================================================\n");
    printf(" 1. Lihat Daftar Produk      6. Cari Produk\n");
    printf(" 2. Tambah Produk            7. Urutkan Produk\n");
    printf(" 3. Ubah Produk              8. Laporan Penjualan\n");
    printf(" 4. Transaksi Penjualan      9. Cek Stok Menipis\n");
    printf(" 5. Tambah Stok              0. Keluar\n");
    printf("============================================================\n");
}

/* ================================================================== */
/*                           FITUR 1: DAFTAR                           */
/* ================================================================== */

void tampilDaftarProduk(void)
{
    int i;

    printf("\n=== DAFTAR PRODUK ===\n");
    if (jumlahProduk == 0) {
        printf("Belum ada produk.\n");
        return;
    }
    garis();
    printf("%-4s %-25s %10s %8s\n", "No", "Nama Produk", "Harga(Rp)", "Stok");
    garis();
    for (i = 0; i < jumlahProduk; i++) {
        printf("%-4d %-25s %10d %8d\n", i + 1, namaProduk[i],
               hargaProduk[i], stokProduk[i]);
    }
    garis();
    printf("Jumlah produk: %d dari %d\n", jumlahProduk, MAKS_PRODUK);
}

/* ================================================================== */
/*                        FITUR 2: TAMBAH PRODUK                       */
/* ================================================================== */

void tambahProduk(void)
{
    char nama[PANJANG_NAMA];
    int harga, stok;

    printf("\n=== TAMBAH PRODUK ===\n");

    if (jumlahProduk >= MAKS_PRODUK) {
        printf("[!] Kapasitas penuh! Maksimal %d produk.\n", MAKS_PRODUK);
        return;
    }

    bacaTeks("Nama produk : ", nama, PANJANG_NAMA);
    if (strlen(nama) == 0) {
        printf("[!] Nama produk tidak boleh kosong!\n");
        return;
    }

    if (!bacaInteger("Harga (Rp) : ", &harga)) return;
    if (harga < 0) {
        printf("[!] Harga tidak boleh negatif!\n");
        return;
    }

    if (!bacaInteger("Stok awal   : ", &stok)) return;
    if (stok < 0) {
        printf("[!] Stok tidak boleh negatif!\n");
        return;
    }

    /* simpan ke array pada indeks jumlahProduk, lalu naikkan penghitung */
    strcpy(namaProduk[jumlahProduk], nama);
    hargaProduk[jumlahProduk]   = harga;
    stokProduk[jumlahProduk]    = stok;
    terjualProduk[jumlahProduk] = 0;
    jumlahProduk++;

    printf("Produk \"%s\" berhasil ditambahkan.\n", nama);
}

/* ================================================================== */
/*                         FITUR 3: UBAH PRODUK                        */
/* ================================================================== */

void ubahProduk(void)
{
    int idx, harga, stok;

    printf("\n=== UBAH PRODUK ===\n");
    tampilDaftarProduk();
    if (jumlahProduk == 0) return;

    idx = pilihNomorProduk("Nomor produk yang diubah: ");
    if (idx < 0) return;

    printf("Produk dipilih: %s (harga Rp%d, stok %d)\n",
           namaProduk[idx], hargaProduk[idx], stokProduk[idx]);

    if (!bacaInteger("Harga baru (Rp): ", &harga)) return;
    if (harga < 0) {
        printf("[!] Harga tidak boleh negatif!\n");
        return;
    }

    if (!bacaInteger("Stok baru     : ", &stok)) return;
    if (stok < 0) {
        printf("[!] Stok tidak boleh negatif!\n");
        return;
    }

    hargaProduk[idx] = harga;
    stokProduk[idx]  = stok;
    printf("Data produk \"%s\" berhasil diubah.\n", namaProduk[idx]);
}

/* ================================================================== */
/*                     FITUR 4: TRANSAKSI PENJUALAN                    */
/* ================================================================== */

void transaksiPenjualan(void)
{
    int keranjang[MAKS_PRODUK] = {0}; /* jumlah beli per produk (sementara) */
    int subtotal, total = 0, adaItem = 0;
    int idx, jumlah, i, lagi;

    printf("\n=== TRANSAKSI PENJUALAN ===\n");
    if (jumlahProduk == 0) {
        printf("Belum ada produk untuk dijual.\n");
        return;
    }
    tampilDaftarProduk();

    do {
        lagi = 0;

        idx = pilihNomorProduk("\nNomor produk yang dibeli: ");
        if (idx >= 0) {
            if (bacaInteger("Jumlah beli: ", &jumlah)) {
                if (jumlah <= 0) {
                    printf("[!] Jumlah pembelian harus lebih dari nol!\n");
                } else if (jumlah + keranjang[idx] > stokProduk[idx]) {
                    /* periksa stok (termasuk yang sudah ada di keranjang) */
                    printf("[!] Stok tidak cukup! Stok %s tersedia: %d, "
                           "sudah di keranjang: %d.\n",
                           namaProduk[idx], stokProduk[idx], keranjang[idx]);
                    printf("    Pembelian ditolak, stok tidak berubah.\n");
                } else {
                    keranjang[idx] += jumlah;
                    adaItem = 1;
                    printf("Ditambahkan: %s x %d = Rp%d\n", namaProduk[idx],
                           jumlah, hargaProduk[idx] * jumlah);
                }
            }
        }

        /* tanya apakah ingin menambah produk lain */
        while (1) {
            if (bacaInteger("Tambah produk lain? (1=Ya, 0=Selesai): ", &lagi)) {
                if (lagi == 0 || lagi == 1) break;
                printf("[!] Pilih 1 atau 0!\n");
            }
        }
    } while (lagi == 1);

    if (!adaItem) {
        printf("Tidak ada produk yang dibeli. Transaksi dibatalkan.\n");
        return;
    }

    /* cetak struk, hitung total, lalu kurangi stok & tambah terjual */
    printf("\n=============== STRUK PEMBELIAN ===============\n");
    printf("%-22s %5s %10s %10s\n", "Produk", "Qty", "Harga", "Subtotal");
    garis();
    for (i = 0; i < jumlahProduk; i++) {
        if (keranjang[i] > 0) {
            subtotal = hargaProduk[i] * keranjang[i];
            total += subtotal;
            printf("%-22s %5d %10d %10d\n", namaProduk[i], keranjang[i],
                   hargaProduk[i], subtotal);

            stokProduk[i]    -= keranjang[i];
            terjualProduk[i] += keranjang[i];
        }
    }
    garis();
    printf("%-22s %27d\n", "TOTAL BAYAR (Rp)", total);
    printf("Transaksi berhasil. Stok telah diperbarui.\n");
}

/* ================================================================== */
/*                        FITUR 5: TAMBAH STOK                         */
/* ================================================================== */

void tambahStok(void)
{
    int idx, tambah;

    printf("\n=== TAMBAH STOK ===\n");
    tampilDaftarProduk();
    if (jumlahProduk == 0) return;

    idx = pilihNomorProduk("Nomor produk: ");
    if (idx < 0) return;

    if (!bacaInteger("Jumlah stok yang ditambahkan: ", &tambah)) return;
    if (tambah <= 0) {
        printf("[!] Jumlah tambahan harus lebih dari nol!\n");
        return;
    }

    stokProduk[idx] += tambah;
    printf("Stok %s bertambah %d, sekarang: %d\n", namaProduk[idx], tambah,
           stokProduk[idx]);
}

/* ================================================================== */
/*                          FITUR 6: CARI PRODUK                       */
/* ================================================================== */

/* Mengubah string ke huruf kecil (hasil disimpan di 'tujuan'). */
static void keHurufKecil(const char *sumber, char *tujuan)
{
    int i;
    for (i = 0; sumber[i] != '\0'; i++) {
        tujuan[i] = (char)tolower((unsigned char)sumber[i]);
    }
    tujuan[i] = '\0';
}

/* Pencarian linear: cocok jika kata kunci menjadi bagian dari nama produk. */
void cariProduk(void)
{
    char kunci[PANJANG_NAMA], kunciKecil[PANJANG_NAMA], namaKecil[PANJANG_NAMA];
    int i, ketemu = 0;

    printf("\n=== CARI PRODUK ===\n");
    bacaTeks("Masukkan nama produk: ", kunci, PANJANG_NAMA);
    if (strlen(kunci) == 0) {
        printf("[!] Kata kunci tidak boleh kosong!\n");
        return;
    }
    keHurufKecil(kunci, kunciKecil);

    for (i = 0; i < jumlahProduk; i++) {
        keHurufKecil(namaProduk[i], namaKecil);
        if (strstr(namaKecil, kunciKecil) != NULL) {
            if (!ketemu) {
                garis();
                printf("%-4s %-25s %10s %8s\n", "No", "Nama Produk",
                       "Harga(Rp)", "Stok");
                garis();
            }
            printf("%-4d %-25s %10d %8d\n", i + 1, namaProduk[i],
                   hargaProduk[i], stokProduk[i]);
            ketemu++;
        }
    }

    if (ketemu) {
        garis();
        printf("Ditemukan %d produk.\n", ketemu);
    } else {
        printf("Produk dengan kata kunci \"%s\" tidak ditemukan.\n", kunci);
    }
}

/* ================================================================== */
/*                        FITUR 7: URUTKAN PRODUK                      */
/* ================================================================== */

/*
 * Pengurutan memakai selection sort pada array indeks 'urutan'.
 * Data asli tidak diubah, sehingga nomor produk tetap konsisten.
 * mode 1 = harga termurah -> termahal
 * mode 2 = jumlah terjual terbanyak -> tersedikit
 */
void urutkanProduk(void)
{
    int urutan[MAKS_PRODUK];
    int pilihan, i, j, posisi, temp, tukar;

    printf("\n=== URUTKAN PRODUK ===\n");
    if (jumlahProduk == 0) {
        printf("Belum ada produk.\n");
        return;
    }
    printf("1. Berdasarkan harga (termurah ke termahal)\n");
    printf("2. Berdasarkan jumlah terjual (terbanyak ke tersedikit)\n");

    if (!bacaInteger("Pilihan: ", &pilihan)) return;
    if (pilihan != 1 && pilihan != 2) {
        printf("[!] Pilihan urutan tidak tersedia!\n");
        return;
    }

    for (i = 0; i < jumlahProduk; i++) {
        urutan[i] = i;
    }

    for (i = 0; i < jumlahProduk - 1; i++) {
        posisi = i;
        for (j = i + 1; j < jumlahProduk; j++) {
            if (pilihan == 1) {
                tukar = hargaProduk[urutan[j]] < hargaProduk[urutan[posisi]];
            } else {
                tukar = terjualProduk[urutan[j]] > terjualProduk[urutan[posisi]];
            }
            if (tukar) posisi = j;
        }
        temp = urutan[i];
        urutan[i] = urutan[posisi];
        urutan[posisi] = temp;
    }

    printf("\nHasil pengurutan %s:\n",
           pilihan == 1 ? "harga (termurah)" : "jumlah terjual (terbanyak)");
    garis();
    printf("%-4s %-25s %10s %8s %8s\n", "No", "Nama Produk", "Harga(Rp)",
           "Stok", "Terjual");
    garis();
    for (i = 0; i < jumlahProduk; i++) {
        printf("%-4d %-25s %10d %8d %8d\n", i + 1, namaProduk[urutan[i]],
               hargaProduk[urutan[i]], stokProduk[urutan[i]],
               terjualProduk[urutan[i]]);
    }
    garis();
}

/* ================================================================== */
/*                       FITUR 8: LAPORAN PENJUALAN                    */
/* ================================================================== */

void laporanPenjualan(void)
{
    int i, totalUnit = 0, totalPendapatan = 0, terlaris = 0;

    printf("\n=== LAPORAN PENJUALAN ===\n");
    if (jumlahProduk == 0) {
        printf("Belum ada produk.\n");
        return;
    }

    garis();
    printf("%-4s %-25s %8s %14s\n", "No", "Nama Produk", "Terjual",
           "Pendapatan(Rp)");
    garis();
    for (i = 0; i < jumlahProduk; i++) {
        int pendapatan = terjualProduk[i] * hargaProduk[i];
        printf("%-4d %-25s %8d %14d\n", i + 1, namaProduk[i],
               terjualProduk[i], pendapatan);

        totalUnit       += terjualProduk[i];
        totalPendapatan += pendapatan;

        if (terjualProduk[i] > terjualProduk[terlaris]) {
            terlaris = i;
        }
    }
    garis();
    printf("Total unit terjual : %d\n", totalUnit);
    printf("Total pendapatan   : Rp%d\n", totalPendapatan);

    if (totalUnit == 0) {
        printf("Produk terlaris    : (belum ada penjualan)\n");
    } else {
        printf("Produk terlaris    : %s (%d unit)\n", namaProduk[terlaris],
               terjualProduk[terlaris]);
    }
}

/* ================================================================== */
/*                      FITUR 9: CEK STOK MENIPIS                      */
/* ================================================================== */

void cekStokMenipis(void)
{
    int i, ada = 0;

    printf("\n=== CEK STOK MENIPIS (stok < %d) ===\n", BATAS_MENIPIS);
    for (i = 0; i < jumlahProduk; i++) {
        if (stokProduk[i] < BATAS_MENIPIS) {
            if (!ada) {
                garis();
                printf("%-4s %-25s %8s\n", "No", "Nama Produk", "Stok");
                garis();
            }
            printf("%-4d %-25s %8d  <-- PERLU RESTOK\n", i + 1, namaProduk[i],
                   stokProduk[i]);
            ada++;
        }
    }
    if (ada) {
        garis();
        printf("Peringatan: %d produk stoknya menipis!\n", ada);
    } else {
        printf("Semua produk stoknya aman.\n");
    }
}

/* ================================================================== */
/*                                MAIN                                 */
/* ================================================================== */

int main(void)
{
    int pilihan = -1;

    isiDataAwal();

    do {
        tampilMenu();
        if (!bacaInteger("Pilih menu (0-9): ", &pilihan)) {
            pilihan = -1;
            continue; /* input bukan angka: ulangi menu */
        }

        switch (pilihan) {
            case 1: tampilDaftarProduk();  break;
            case 2: tambahProduk();        break;
            case 3: ubahProduk();          break;
            case 4: transaksiPenjualan();  break;
            case 5: tambahStok();          break;
            case 6: cariProduk();          break;
            case 7: urutkanProduk();       break;
            case 8: laporanPenjualan();    break;
            case 9: cekStokMenipis();      break;
            case 0: printf("\nTerima kasih. Program selesai.\n"); break;
            default:
                printf("[!] Pilihan menu %d tidak tersedia! Pilih 0-9.\n",
                       pilihan);
        }
    } while (pilihan != 0);

    return 0;
}
