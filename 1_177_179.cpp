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
