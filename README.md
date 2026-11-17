# Proyek-Array-C-C2583207043
NAMA : BINTANG ARIFA LUDIANI
NIM : C2583207043 
KELAS : PTI 3B
MATA KULIAH : ALGORITMA DAN PEMOGRAMAN LANJUT

## Deskripsi Singkat

Program ini adalah aplikasi berbasis terminal yang dibuat dengan bahasa C untuk membantu mengelola stok dan penjualan di kantin kampus. Dengan program ini, pengelola bisa melihat daftar produk, menambah atau mengubah data, mencatat pembelian, mencari dan mengurutkan produk, serta melihat laporan penjualan tanpa pencatatan manual. Semua data disimpan di dalam array selama program berjalan, tanpa database dan tanpa file eksternal.

## Daftar Fitur

| No | Menu | Fungsi |
|----|------|--------|
| 1 | Lihat Daftar Produk | Menampilkan nama, harga, dan stok semua produk |
| 2 | Tambah Produk | Menambah produk baru, maksimal 50 produk |
| 3 | Ubah Produk | Mengubah harga dan stok produk yang dipilih |
| 4 | Transaksi Penjualan | Memproses pembelian beberapa produk sekaligus, menghitung subtotal dan total bayar, lalu mengurangi stok |
| 5 | Tambah Stok | Menambah stok produk yang sudah ada |
| 6 | Cari Produk | Mencari produk berdasarkan nama |
| 7 | Urutkan Produk | Mengurutkan produk dari harga termurah atau dari jumlah terjual terbanyak |
| 8 | Laporan Penjualan | Menampilkan unit terjual per produk, total unit, total pendapatan, dan produk terlaris |
| 9 | Cek Stok Menipis | Menampilkan produk yang stoknya kurang dari 5 |
| 0 | Keluar | Menutup program |

## Penjelasan Penggunaan Array

Data produk disimpan dalam beberapa array yang berukuran 50, sesuai batas maksimal produk. Elemen dengan indeks yang sama di semua array merupakan data untuk satu produk yang sama.

| Variabel | Jenis | Kegunaan |
|----------|-------|----------|
| `namaProduk[50][30]` | Array karakter 2 dimensi | Menyimpan nama produk. Setiap baris untuk satu produk, dan setiap nama berupa deretan karakter |
| `hargaProduk[50]` | Array 1 dimensi (int) | Menyimpan harga setiap produk |
| `stokProduk[50]` | Array 1 dimensi (int) | Menyimpan stok setiap produk |
| `terjualProduk[50]` | Array 1 dimensi (int) | Menyimpan jumlah unit yang sudah terjual |
| `jumlahProduk` | Variabel int | Mencatat jumlah produk aktif, sekaligus menjadi indeks untuk produk baru berikutnya |

Pengurutan dilakukan dengan algoritma selection sort pada array indeks bantu, sehingga urutan data asli tidak berubah. Pencarian produk dilakukan secara linear dari indeks pertama sampai terakhir.

## Cara Kompilasi dan Menjalankan

### Menggunakan Code::Blocks
1. Buka Code::Blocks, pilih **File > New > Project > Console application**, lalu pilih bahasa **C**.
2. Beri nama project, lalu klik Finish.
3. Buka file `main.c` pada project, hapus isinya, lalu salin seluruh isi `kantin.c` ke dalamnya.
4. Klik **Build and Run** (atau tekan F9). Program akan terbuka di jendela console.

### Menggunakan terminal (GCC)
```
gcc kantin.c -o kantin
./kantin
```
Di Windows, jalankan dengan `kantin.exe`.

<img width="960" height="540" alt="menu 1,2" src="https://github.com/user-attachments/assets/ea9a6c5c-251d-44bd-92de-c636418bb023" />

<img width="960" height="540" alt="menu 1,2 2" src="https://github.com/user-attachments/assets/555f7c20-86e1-4c4b-9eab-42234819053a" />

<img width="960" height="540" alt="menu 1 3" src="https://github.com/user-attachments/assets/5a05f67b-d21e-4dda-a138-e3c20cad71e6" />

<img width="960" height="540" alt="menu 2 error kapasitas" src="https://github.com/user-attachments/assets/15e41c5e-2a11-43eb-b60e-a04f77cf48d4" />

<img width="960" height="540" alt="menu 2 error" src="https://github.com/user-attachments/assets/eacb1598-3fda-46e9-838d-5e54f886fab1" />

<img width="960" height="540" alt="menu 3" src="https://github.com/user-attachments/assets/5fff827b-4ec4-4a53-abbf-6c95c79f1bbf" />

<img width="960" height="540" alt="menu 4" src="https://github.com/user-attachments/assets/ea5c3752-8a08-4f15-a1bd-72579b8ff3da" />

<img width="960" height="540" alt="menu 4,1" src="https://github.com/user-attachments/assets/6c7855d3-8940-4be4-bebe-df6ffbeb96e4" />

<img width="960" height="540" alt="menu 5" src="https://github.com/user-attachments/assets/9567a2f3-5a46-4cd4-89d8-b0ff6caf29da" />

<img width="960" height="540" alt="menu 6 dan 7" src="https://github.com/user-attachments/assets/01d2b033-a6d1-46ca-ae31-f3cde67aca55" />

<img width="960" height="540" alt="menu 7,2 dan 8" src="https://github.com/user-attachments/assets/fb2a5787-8475-486e-b16a-53bd29f6e198" />

<img width="960" height="540" alt="menu 8" src="https://github.com/user-attachments/assets/14e4827a-d218-420a-ae87-87c3e3e02f2d" />
