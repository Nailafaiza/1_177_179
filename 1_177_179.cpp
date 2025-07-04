#include <iostream>
#include <string>
using namespace std;

//struct untuk menyimpan data pesanan
struct Pesanan{
  int id;
  string namaPelanggan;
  string makanan;
  int jumlah;
  int estimasiWaktu;
  Pesanan *next;
};

//pointer untuk menyimpan head dan tail dari linked list kepala ekor
Pesanan *head = nullptr;
Pesanan *tail = nullptr;

//variabel untuk menyimpan id pesanan selanjutnya
int nextId = 1;

//Fungsi untuk menyimpan data pesanan ke file data_pesanan.txt
void simpanKeFile(Pesanan *head){
  FILE *f = fopen("data_pesanan.txt", "w");
  if (f==NULL){
      cout << "Gagal membuka file untuk menyimpan data" << endl;
      return;
  }

  Pesanan *bantu = head;
    while (bantu != nullptr){
        fprintf(f, "%d,%s,%s,%d,%d\n", bantu->id, bantu->namaPelanggan.c_str(), bantu->makanan.c_str(), bantu->jumlah, bantu->estimasiWaktu);
        bantu = bantu->next;
    }
    fclose(f);
}

//Fungsi untuk menyimpan data pesanan yang sudah diproses ke file riwayat_pesanan.txt
void simpanKeRiwayat(Pesanan *pesanan, const string &hasilReview){
    FILE *f = fopen("riwayat_pesanan.txt", "a");
    if (f == NULL){
        cout << "Gagal membuka file untuk menyimpan riwayat pesanan"<<endl;
        return;
    }
    fprintf(f, "%d,%s,%s,%d,%d,%s\n", pesanan->id, pesanan->namaPelanggan.c_str(), 
    pesanan->makanan.c_str(), pesanan->jumlah, pesanan->estimasiWaktu, hasilReview.c_str());
    fclose(f);   
}

//Fungsi untuk menampilkan riwayat pesanan
void tampilkanRiwayat(){
    FILE *f = fopen("riwayat_pesanan.txt", "r");
    if (f == NULL){
        cout << "Gagal membuka file riwayat pesanan"<<endl;
        return;
    }
    cout << "\n=== Riwayat Pesanan yang Telah Diproses ===\n";
    char line[256];
    while (fgets(line, sizeof(line), f) != NULL){
        int id, jumlah, estimasi;
        char nama[50], makanan[50], review[100];
        // Tambahkan review di parsing
        sscanf(line, "%d,%49[^,],%49[^,],%d,%d,%99[^\n]", &id, nama, makanan, &jumlah, &estimasi, review);

        cout << "ID: " << id << " | Nama: " << nama << " | Menu: " << makanan
             << " | Jumlah: " << jumlah << " | Estimasi: " << estimasi << " menit"
             << " | Review: " << review << endl;
    }
    fclose(f);
}

// Fungsi untuk menambahkan pesanan baru (linked list)
void tambahPesanan(){
    cout<<endl;
    cout << "=== Tambah Pesanan Baru ==="<<endl;
    Pesanan *baru = new Pesanan;
    baru->id = nextId++;
    cout << "Nama Pelanggan   : ";
    cin.ignore();
    getline(cin, baru->namaPelanggan);
    cout << "Menu Makanan     : ";
    getline(cin, baru->makanan);
    cout << "Jumlah Pesanan   : ";
    cin >> baru->jumlah;
    cout << "Estimasi Waktu (menit): ";
    cin >> baru->estimasiWaktu;
    baru->next = nullptr;

    if (head == nullptr){
        head = tail = baru;
    }
    else{
        tail->next = baru;
        tail = baru;
    }
    cout << "\nPesanan berhasil ditambahkan ke antrian!\n";
    
    // Simpan ke file setelah menambah pesanan
    simpanKeFile(head); 
}

// Fungsi review pelanggan (dipanggil setelah proses pesanan)
string reviewPelanggan(){
    int nilai1, nilai2, nilai3;
    cout << "\nReview Kepuasan Pelanggan:\n";
    cout << "1. Makanan tiba tepat waktu (1-5): ";
    cin >> nilai1;
    cout << "2. Kualitas makanan (1-5): ";
    cin >> nilai2;
    cout << "3. Kemungkinan memesan lagi (1-5): ";
    cin >> nilai3;
    float rata2 = (nilai1 + nilai2 + nilai3) / 3.0;

//menggabungkan hasil review pelanggan dalam satu string
    char hasil[100];
    sprintf(hasil, "%d,%d,%d,%.2f", nilai1, nilai2, nilai3, rata2);
    return string(hasil);
     
    // Menyimpan review ke file review.txt
    FILE *f = fopen("review.txt", "a");
    if (f == NULL){
        return "Gagal membuka file!\n";
    }
    fprintf(f, "%d,%d,%d,Rata-rata: %.2f\n", nilai1, nilai2, nilai3, rata2);
    fclose(f);
}

// Fungsi untuk memproses pesanan
void prosesPesanan(){
    if (head == nullptr){
        cout << "\nAntrian kosong. Tidak ada pesanan untuk diproses."<<endl;
        return;
    }else{
    Pesanan *hapus = head;
    cout << "\nMemproses pesanan ID " << hapus->id << " - " << hapus->namaPelanggan << endl;
    head = head->next;

    string hasilReview = reviewPelanggan(); // Simpan hasil review pelanggan
    simpanKeRiwayat(hapus, hasilReview); // Simpan pesanan yang sudah dilayani dan hasil review ke riwayat.txt sebelum dihapus

    delete hapus;
    if (head == nullptr)
        tail = nullptr;

    simpanKeFile(head);
    }
}

// Fungsi untuk mengurutkan pesanan berdasarkan nama makanan (bubble sort)
void urutkanPesanan(){
    if (head == nullptr || head->next == nullptr)
        return;

    Pesanan *i, *j;
    for (i = head; i != nullptr; i = i->next){
        for (j = i->next; j != nullptr; j = j->next){
            if (i->makanan > j->makanan) {
                // Tukar data pesanan
                Pesanan temp;
                temp.id = i->id;
                temp.namaPelanggan = i->namaPelanggan;
                temp.makanan = i->makanan;
                temp.jumlah = i->jumlah;
                temp.estimasiWaktu = i->estimasiWaktu;

                 i->id = j->id;
                i->namaPelanggan = j->namaPelanggan;
                i->makanan = j->makanan;
                i->jumlah = j->jumlah;
                i->estimasiWaktu = j->estimasiWaktu;

                j->id = temp.id;
                j->namaPelanggan = temp.namaPelanggan;
                j->makanan = temp.makanan;
                j->jumlah = temp.jumlah;
                j->estimasiWaktu = temp.estimasiWaktu;
            }
        }
    }
}

// Fungsi untuk menampilkan antrian pesanan
void tampilkanAntrian()
{
    if (head == nullptr)
    {
        cout << "\nAntrian kosong.\n";
        return;
    }
    Pesanan *bantu = head;
    cout << "\nDaftar Antrian:\n";
    while (bantu != nullptr)
    {
        cout << "ID: " << bantu->id
             << " | Nama: " << bantu->namaPelanggan
             << " | Menu: " << bantu->makanan
             << " | Jumlah: " << bantu->jumlah
             << " | Estimasi: " << bantu->estimasiWaktu << " menit\n";
        bantu = bantu->next;
    }
}

// Fungsi untuk mencari pesanan berdasarkan nama pelanggan (sequential search)
void cariPesananNama()
{
    string cari; // menyimpan input dari user saat user ingin mencari pesanan berdasarkan nama pelanggan.
    cout << "\nMasukkan nama pelanggan yang dicari: ";
    cin.ignore();
    getline(cin, cari);
    Pesanan *bantu = head;
    bool ditemukan = false;
    while (bantu != nullptr)
    {
        if (bantu->namaPelanggan == cari)
        {
            cout << "Ditemukan pesanan ID: " << bantu->id
                 << " | Nama Pelanggan: " << bantu->namaPelanggan
                 << " | Menu: " << bantu->makanan
                 << " | Jumlah: " << bantu->jumlah
                 << " | Estimasi: " << bantu->estimasiWaktu << " menit\n";
            ditemukan = true;
        }
        bantu = bantu->next;
    }    
    if (!ditemukan)
        cout << "Tidak ditemukan pesanan atas nama tersebut.\n";
}

// Fungsi untuk mencari pesanan berdasarkan nama makanan (sequential search)
void cariPesananMakanan()
{
    string cari; // menyimpan input dari user saat ingin mencari pesanan berdasarkan nama makanan
    cout << "\nMasukkan nama makanan yang dicari: ";
    cin.ignore();
    getline(cin, cari);
    Pesanan *bantu = head;
    bool ditemukan = false;
    while (bantu != nullptr)
    {
        if (bantu->makanan == cari)
        {
            cout << "Ditemukan pesanan ID: " << bantu->id
                 << " | Nama Pelanggan: " << bantu->namaPelanggan
                 << " | Menu: " << bantu->makanan
                 << " | Jumlah: " << bantu->jumlah
                 << " | Estimasi: " << bantu->estimasiWaktu << " menit\n";
            ditemukan = true;
        }
        bantu = bantu->next;
    }
    if (!ditemukan)
        cout << "Tidak ditemukan pesanan dengan nama makanan tersebut.\n";
}

int main()
{
    int pilihan;
    do
    {
        cout << " ===============================================" << endl;
        cout << "|    MENU SISTEM ANTRIAN PENGANTARAN MAKANAN    |" << endl;
        cout << " ===============================================" << endl;
        cout << "| [1]. Tambah Pesanan                           |" << endl;
        cout << "| [2]. Proses Pesanan                           |" << endl;
        cout << "| [3]. Tampilkan Antrian                        |" << endl;
        cout << "| [4]. Cari Pesanan (Nama Pelanggan)            |" << endl;
        cout << "| [5]. Cari Pesanan (Nama Makanan)              |" << endl;
        cout << "| [6]. Urutkan Pesanan (Nama Makanan)           |" << endl;
        cout << "| [7]. Tampilkan Riwayat Pesanan                |" << endl;
        cout << "| [0]. Keluar                                   |" << endl;
        cout << "=================================================" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        switch (pilihan)
        {
        case 0:
            cout << "\nTerima kasih telah menggunakan layanan kami.\n";
            break;
        case 1:
            tambahPesanan();
            break;
        case 2:
            prosesPesanan();
            break;
        case 3:
            tampilkanAntrian();
            break;
        case 4:
            cariPesananNama();
            break;
        case 5:
            cariPesananMakanan();
            break;
        case 6:
            urutkanPesanan();
            cout << "\nPesanan berhasil diurutkan berdasarkan nama makanan!\n";
            tampilkanAntrian();
            break;
        case 7:
            tampilkanRiwayat();
            break;
        default:
            cout << "\nPilihan tidak valid.\n";
        }
    } while (pilihan != 0);
    return 0;
}
