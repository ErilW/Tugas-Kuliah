#include <conio.h>
#include <time.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <sstream>

#include "data (2).h"
#define MAX_TRANS 150
using namespace std;

void printKategori(void);

enum transactionType { Income, Outcome };

struct Transaksi {
   transactionType type;
   tm tanggal;
   string deskripsi;
   unsigned long long jumlah;
};

struct Kategori {
   string nama;
   unsigned long long jumlahUtamaKategori;
};

class TrackerTransaksi {
  private:
   unsigned long long jumlahUang = 0;
   int currentIndex = 0;
   Transaksi transaksi[MAX_TRANS];
   Kategori kategori[MAX_TRANS];

   void tambahUangPerKategori(string nama /*nama kategori input user*/,
                              unsigned long long jumllah) {
      kategori[currentIndex].nama = nama;
      kategori[currentIndex].jumlahUtamaKategori += jumllah;
   }

   unsigned long long updateDataPerKategori(string namaKategori) {
      unsigned long long totalUtamaPerKategori = 0;

      int i;  // Declare i outside the loop

      for (i = 0; i < currentIndex; i++) {
         if (namaKategori == kategori[i].nama) {
            totalUtamaPerKategori += transaksi[i].jumlah;
         }
      }

      if (i > 0) {
         kategori[i - 1].jumlahUtamaKategori =
             totalUtamaPerKategori;  // Update the total in the kategori array
      }

      return totalUtamaPerKategori;
   }

  public:
   unsigned long long jumlahKategoriPerArrayStruct(string& namaKategori) {
      return updateDataPerKategori(namaKategori);
   }

  	bool checkKategoriOnStruct(int &index,string array[]){
 		for (int i = 0; i <= currentIndex; i ++){
 			if( array[index] == kategori[i].nama){
 				return true;
			 }
		
		 }
		 return false;
	 }

   // Sebagai Saldo Awal data jumlahUatama
   unsigned long long saldoAwal(unsigned long long saldo) {
      tambahUang(saldo);
      return saldo;
   }

   // fungsi pengecekan jumlahUTama transaksi
   unsigned long long cekSaldo() { return jumlahUang; }

   // Tambah transaksi per index sesuai dengan current index dari private
   void tambahTrans(transactionType type, const string deskripsi, unsigned long long jumlah,
                    string namaKategori) {
      time_t rawtime;
      time(&rawtime);
      tm* waktu = localtime(&rawtime);

      transaksi[currentIndex].type = type;
      transaksi[currentIndex].tanggal = *waktu;
      transaksi[currentIndex].deskripsi = deskripsi;
      transaksi[currentIndex].jumlah = jumlah;

      // Check if the category already exists
      bool categoryExists = false;
      int existingCategoryIndex = -1;

      for (int i = 0; i < currentIndex; ++i) {
         if (kategori[i].nama == namaKategori) {
            categoryExists = true;
            existingCategoryIndex = i;
            break;
         }
      }

      // If the category exists, use the existing category
      if (categoryExists) {
         // If enum is Income, add the amount to the main total
         if (type == Income) {
            tambahUang(jumlah);
            currentIndex++;
         } else {
            // Check if there's enough balance for the outcome
            if ((jumlahUang - jumlah) >= 0) {
               // Subtract the amount
               kurangUang(jumlah);
               // Add the amount to the existing category
               tambahUangPerKategori(namaKategori, jumlah);
               updateDataPerKategori(namaKategori);
               currentIndex++;
            } else {
               cout << "Saldo tidak cukup!" << endl;
               cekSaldo();
               return;
            }
         }
      } else {
         // If the category doesn't exist, add a new category
         kategori[currentIndex].nama = namaKategori;

         // If enum is Income, add the amount to the main total
         if (type == Income) {
            tambahUang(jumlah);
            currentIndex++;
         } else {
            // Check if there's enough balance for the outcome
            if ((jumlahUang - jumlah) >= 0) {
               // Subtract the amount
               kurangUang(jumlah);

               // Add the amount to the new category
               tambahUangPerKategori(namaKategori, jumlah);
               updateDataPerKategori(namaKategori);
               currentIndex++;
            } else {
               cout << "Saldo tidak cukup!" << endl;
               cekSaldo();
               return;
            }
         }
      }
   }

   // Jumlah Keseluruhan income yang sudah dimasukkan di fungsi nambahTrans()
   unsigned long long jumlahIncome() {
      // jumlah yang akan direturn
      unsigned long long totalIncome = 0;
      // looping sesuai dengan index sekarang
      for (int i = 0; i < currentIndex; ++i) {
         // pengecekan jika type nya adalah income
         if (transaksi[i].type == Income) {
            // mereturn total income setelah penambahan seusai dengan array nambahTrans()
            totalIncome += transaksi[i].jumlah;
         }
      }
      return totalIncome;
   }
   // Jumlah Keseluruhan OutCome yang sudah dimasukkan di fungsi nambahTrans()
   unsigned long long jumlahOutcome() {
      // jumlah outcome keseluruhan
      unsigned long long totalOutcome = 0;
      // looping
      for (int i = 0; i < currentIndex; ++i) {
         // type nya adalah outcome
         if (transaksi[i].type == Outcome) {
            // mereturn nilai penambahan dari seluruh outcome yang sudah pernah dilakukan
            totalOutcome += transaksi[i].jumlah;
         }
      }
      return totalOutcome;
   }

   void tambahKategori(const string& nama) {
      // Check if the category already exists
      bool categoryExists = false;

      for (int i = 0; i < currentIndex; ++i) {
         if (kategori[i].nama == nama) {
            categoryExists = true;
            break;
         }
      }

      // If the category doesn't exist, add a new category
      if (!categoryExists) {
         kategori[currentIndex].nama = nama;
         currentIndex++;
      } else {
         cout << "Kategori sudah ada!" << endl;
      }
   }

   // fungsi nambah uang

   void tambahUang(unsigned long long saldo) { jumlahUang += saldo; }

   // fungsi kurang uang
   void kurangUang(unsigned long long saldo) {
      // check bahwa saldo yang ingin dikurang tidak akan lebih dari jumlahUtama dan jumlahuang
      // tidak 0
      if (jumlahUang != 0 && jumlahUang >= saldo) {
         jumlahUang -= saldo;
      }
   }
   // fungsi yang menambahkan per kategori, dan hanya diakses oleh tambahTrans()

   void transfer(TrackerTransaksi& to, unsigned long long amount, const string& description) {
      if (cekSaldo() >= amount) {
         tambahTrans(Outcome, description, amount, "transfer");
         to.tambahTrans(Income, description, amount, "");
         cout << "Transfer successful!" << endl;
      } else {
         cout << "Transfer failed. Insufficient funds." << endl;
      }
   }

   //-------------------------------PRINTING------------------------------------------
   // print transaksi yang sudah pernah dilakukan
   void printTransOutcome() {
      cout << "\tOUTCOME\n";
      cout << setw(20) << "Tanggal" << setw(30) << "Deskripsi" << setw(15) << "Kategori" << setw(15)
           << "Nominal" << endl;
      for (int i = 0; i < currentIndex; ++i) {
         // Pengecekan jika kategori tidak kosong dan deskripsi tidak kosong
         if (!kategori[i].nama.empty() && !transaksi[i].deskripsi.empty()) {
            // Outcome
            time_t waktu = mktime(&transaksi[i].tanggal);  // Konversi struct tm ke time_t
            tm* waktuTm = localtime(&waktu);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d, %H:%M:%S", waktuTm);
            if (transaksi[i].type != Income) {
               cout << setw(20) << buffer << setw(30) << transaksi[i].deskripsi << setw(15)
                    << kategori[i].nama << setw(15) << transaksi[i].jumlah << endl;
            }
         }
      }
   }

   void printTransIncome(string judul) {
      print(samadengan(intSpace));

      printVariabel(intSpace, judul, "INCOME ");
      print(samadengan(intSpace));
      cout << setw(20) << "Tanggal" << setw(30) << "Deskripsi" << setw(15) << "Nominal" << endl;
      for (int i = 0; i < currentIndex; ++i) {
         // Pengecekan jika kategori tidak kosong dan deskripsi tidak kosong
         if (!transaksi[i].deskripsi.empty()) {
            // Income
            time_t waktu = mktime(&transaksi[i].tanggal);  // Konversi struct tm ke time_t
            tm* waktuTm = localtime(&waktu);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d, %H:%M:%S", waktuTm);
            if (transaksi[i].type == Income) {
               cout << setw(20) << buffer << setw(30) << transaksi[i].deskripsi << setw(15)
                    << transaksi[i].jumlah << endl;
            }
         }
      }
   }

   void printTheCurrentTrans() {
      if (currentIndex > 0) {
         print(samadengan(WIDTH));
         printVariabel(WIDTH, "Data Transaksi Terakhir");
         print(samadengan(WIDTH));
         cout << "\n";
         cout << setw(20) << "Tanggal" << setw(30) << "Deskripsi" << setw(15) << "Kategori"
              << setw(15) << "Nominal" << setw(20) << "Dana" << endl;

         int i = currentIndex - 1;  // Mengambil data pada index terakhir
         time_t waktu = mktime(&transaksi[i].tanggal);
         tm* waktuTm = localtime(&waktu);
         char buffer[20];
         strftime(buffer, sizeof(buffer), "%d, %H:%M:%S", waktuTm);

         if (!kategori[i].nama.empty() && !transaksi[i].deskripsi.empty()) {
            // Outcome
            if (transaksi[i].type != Income) {
               cout << setw(20) << buffer << setw(30) << transaksi[i].deskripsi << setw(15)
                    << kategori[i].nama << setw(15) << transaksi[i].jumlah;
               cout << setw(20) << cekSaldo() << endl;
            }
         } else {
            // Income
            if (transaksi[i].type == Income) {
               cout << setw(20) << buffer << setw(30) << transaksi[i].deskripsi << setw(15)
                    << transaksi[i].jumlah;
               cout << setw(20) << cekSaldo() << endl;
            }
         }
      } else {
         cout << "Belum ada transaksi terakhir ..." << endl;
      }
   }

//   void printKategoriPerStruct(string judul) {
//      print(samadengan(intSpace));
//      printVariabel(intSpace, judul);
//      print(samadengan(intSpace));
//      cout << endl;
//      cout << setw(20) << "Nama Kategori" << setw(30) << "Nominal" << endl;
//      print(garis(intSpace));
//      for (int i = 0; i < currentIndex; ++i) {
//         if (!kategori[i].nama.empty() && transaksi[i].type != Income) {
//            // Cek apakah nama kategori sudah ada sebelumnya
//            bool found = false;
//            for (int j = 0; j < i; ++j) {
//               if (kategori[j].nama == kategori[i].nama) {
//                  found = true;
//                  break;
//               }
//            }
//            // Jika nama kategori belum pernah ditemukan sebelumnya, cetak nama kategori dan jumlah
//            // utama
//            if (!found) {
//               cout << setw(20) << kategori[i].nama << setw(30)
//                    << updateDataPerKategori(kategori[i].nama) << endl;
//            }
//         }
//      }
//   }

   // Tambahkan fungsi berikut ke dalam kelas TrackerTransaksi
   void pilihanKategoriPerInput(unsigned long long& nominal, string& desk, string kategori) {
      cout << "Silahkan Masukkan Nominal : ";
      inputAngka(nominal, "SILAHKAN MASUKKAN NOMINAL : ");
      cout << "Silahkan masukkan Deskripsi : ";
      cin.ignore();  // Membersihkan buffer
      getline(cin, desk);
      if(desk == ""){
      	desk = "Kosong!";
	  }
      
      if (jumlahUang <= nominal) {
         cout << "Uang Tidak Mencukupi ...";
         getch();
      } else {
         tambahTrans(Outcome, desk, nominal, kategori);
      }
      system("cls");
      printTheCurrentTrans();
      cout << "\n\nTekan apa saja untuk kembali ...";
      getch();
   }

   void printTrans() {
      // Print header
      cout << setw(20) << "Tanggal" << setw(20) << "Deskripsi" << setw(15) << "Jenis" << setw(15)
           << "Transaksi" << endl;
      // Print transactions
      for (int i = 0; i < MAX_TRANS; ++i) {
         if (transaksi[i].jumlah != 0 && transaksi[i].deskripsi != "") {
            string jenis = (transaksi[i].type == Income) ? "Income" : "Outcome";
            time_t waktu = mktime(&transaksi[i].tanggal);  // Konversi struct tm ke time_t
            tm* waktuTm = localtime(&waktu);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d, %H:%M:%S", waktuTm);
            // Convert time_t to readable format using ctime
            cout << setw(20) << buffer << setw(20) << transaksi[i].deskripsi << setw(15) << jenis
                 << setw(15) << transaksi[i].jumlah << endl;
         }
      }
   }
};

//---------------CLASS USER---------------//

class User {
  private:
   unsigned long long jumlahUangPerTrack(string namaKategori) {
      unsigned long long totalSemuaTrack;
      totalSemuaTrack = trackTunai.jumlahKategoriPerArrayStruct(namaKategori) +
                        trackBank.jumlahKategoriPerArrayStruct(namaKategori) +
                        trackEMoney.jumlahKategoriPerArrayStruct(namaKategori);
      return totalSemuaTrack;
   }

  public:
   TrackerTransaksi trackTunai;
   TrackerTransaksi trackEMoney;
   TrackerTransaksi trackBank;

   void transferUang(TrackerTransaksi& from, TrackerTransaksi& to, unsigned long long amount,
                     const string& description) {
      from.transfer(to, amount, description);
   }

   void printSemuaTrans() {
      cout << "Saldo Tunai: " << trackTunai.cekSaldo() << endl;
      cout << "Saldo EMoney: " << trackEMoney.cekSaldo() << endl;
      cout << "Saldo Bank: " << trackBank.cekSaldo() << endl;
      cout << "\nTransaksi Tunai:\n\n";
      trackTunai.printTrans();

      cout << "\nTransaksi E-Money:\n\n";
      trackEMoney.printTrans();

      cout << "\nTransaksi Bank:\n\n";
      trackBank.printTrans();
   }

//   void printKategoriPerTrack() {
//      trackTunai.printKategoriPerStruct("TUNAI");
//      trackEMoney.printKategoriPerStruct("E-MONEY");
//      trackBank.printKategoriPerStruct("BANK");
//   }

   void printAllIncomeTrack() {
      trackTunai.printTransIncome("TUNAI");
      trackEMoney.printTransIncome("E-MONEY");
      trackBank.printTransIncome("BANK");
   }

void printTabelAkunPerKat(string stringArray[]) {
    cout << "| KATEGORI                    | TUNAI                   | E-MONEY                   | BANK                  | TOTAL |\n";
    for (int i = 0; i < len(k); i++) {
        if (k[i] != "" && trackTunai.checkKategoriOnStruct(i, k)) {
            cout << "| " << setw(15) << k[i] << " | " << setw(5) << trackTunai.jumlahKategoriPerArrayStruct(k[i]) << " | "
                 << setw(7) << trackEMoney.jumlahKategoriPerArrayStruct(k[i]) << " | " << setw(5)
                 << trackBank.jumlahKategoriPerArrayStruct(k[i]) << " | " << setw(5) << jumlahUangPerTrack(k[i]) << " |\n";
        }
    }
}
};

//----------------------DILUAR CLASS-----------------------------//

char akun(bool isCls = false, string pesan = "Pilih Metode Akun : ") {
   char pilihan;
   print(garis(intSpace));
   printVariabel(intSpace, "AKUN");
   print(garis(intSpace));
   printVariabel(intSpace, "1. TUNAI");
   printVariabel(intSpace, "2. E-MONEY");
   printVariabel(intSpace, "3. BANK");
   print(garis(intSpace));

   print(pesan, false);
   inputChar(pilihan, "Silahkan Masukkan Ulang : ");

   if (pilihan == '1' || pilihan == '2' || pilihan == '3') {
      return pilihan;
   } else {
      print("Tekan Apa saja Untuk Merestart ...");
      getch();
      if (isCls) {
         system("cls");
      }
      return akun(isCls = false, "Pilih Metode Akun : ");
   }
}

//--------------------------------------------------------
void projectExpenseTracker() {
   print(samadengan(intSpace));
   printVariabel(intSpace, "PROJECT EXPENSE TRACKER");
   print(samadengan(intSpace));
   printVariabel(intSpace, "NAMA KELOMPOK : 1");
   print(garis(intSpace));
   loading();
   system("cls");
}
//-----------------------------------------------------------------
// fungsi sekali pakai, ingat!!!
void printSaldoAwal(User& user, unsigned long long& saldo) {
   print(samadengan(intSpace));
   printVariabel(intSpace, "SALDO AWAL");
   print(samadengan(intSpace));
   switch (akun(true)) {
      // case saat menggunakan cash
      //--------------
      case '1':
         print("Masukkan Nominal : ", false);
         // meninput nilai uang ke dalam dana cash sesuai parameter index saat dipanggil

         inputAngka(saldo, "Masukkan Nominal : ");
         user.trackTunai.saldoAwal(saldo);
         cout << "Saldo Awal Tunai : Rp." << user.trackTunai.cekSaldo();
         sleep(2);
         break;
      //-------------
      case '2':
         print("Masukkan Nominal : ", false);
         inputAngka(saldo, "Masukkan Nominal : ");
         user.trackEMoney.saldoAwal(saldo);
         cout << "Saldo Awal E-Money : Rp." << user.trackEMoney.cekSaldo();
         sleep(2);
         break;

      //------------------
      case '3':
         print("Masukkan Nominal : ", false);
         inputAngka(saldo, "Masukkan Nominal : ");
         user.trackBank.saldoAwal(saldo);
         cout << "Saldo Awal Bank : Rp." << user.trackBank.cekSaldo();
         sleep(2);
         break;
   }
}
//----------------------------------------------------------------

void printKategori(string judul) {
   print(samadengan(intSpace));
   printVariabel(intSpace, "KATEGORI", judul, " ");
   print(samadengan(intSpace));

   for (int i = 0; i < len(k); i++) {
      if (k[i] != "") {
         printVariabel(intSpace, k[i], IntToString(i + 1), " ");
      }
   }
   printVariabel(intSpace, "Tambah Kategori", "+ ");
   print(garis(intSpace));
}
//----------------------------------------------------------

//----------------------------------------------------------
void expenseInput(TrackerTransaksi& track, string judul) {
   int currentIndex = 0;  // Initialize currentIndex to zero

   // Find the first empty slot in the array
   for (int i = 0; i < len(k); i++) {
      if (k[i] == "") {
         currentIndex = len(k) - (len(k) - i);
         break;  // Stop searching once an empty slot is found
      }
   }  // 8

   char pilih;
   unsigned long long nominal;
   string desk;

   system("cls");
   printKategori(judul);
   string data = "Pilih Kategori (1 - " + IntToString(currentIndex) + ") : ";
   cout << "Pilih Kategori (1 - " << IntToString(currentIndex) << ") : ";
   inputChar(pilih, data);

   int index = pilih - '0';

   if (index >= 1 && index < currentIndex) {
      track.pilihanKategoriPerInput(nominal, desk, k[index - 1]);
   } else if (pilih == '+') {
      print("Silahkan menambahkan kategori : ", false);
      string newString;
      cin.ignore();
      getline(cin, newString);
      k[currentIndex] = newString;
      ++currentIndex;
      system("cls");
      expenseInput(track, judul);
   } else {
      cout << "Input tidak valid.";
      getch();
   }
}

//----------------------------------------------------------

void expense(User& user) {
   char akunChoice = akun(false, "Pilih Akun Pengeluaran : ");

   switch (akunChoice) {
      case '1':
         expenseInput(user.trackTunai, "TUNAI");
         break;
      case '2':
         expenseInput(user.trackEMoney, "E-MONEY");
         break;
      case '3':
         expenseInput(user.trackBank, "BANK");
         break;
      default:
         cout << "Tekan apa saja untuk Restart ... ";
         getch();
         system("cls");
         expense(user);
   }
}

//-------------------CHOOSE 2
void printIncomeInterface(string akun, unsigned long long saldoPerAkun, unsigned long long saldo = 0, string desk = "-") {
   print(samadengan(intSpace));
   printVariabel(intSpace, "INCOME");
   print(samadengan(intSpace));
   cout << "AKUN : " << akun << " (" << saldoPerAkun << ")\n";
   print(garis(intSpace));
   cout << "JUMLAH :\t" << saldo << endl;
   print(garis(intSpace));
   cout << "DESKRIPSI :\t" << desk << endl;
   print(garis(intSpace));
   cout << "\n\n";
}

void income(User& user) {
   unsigned long long amount;
   string desk;
   switch (akun("Pilih Akun Pemasukan : ")) {
      case '1':
         system("cls");
         printIncomeInterface("TUNAI", user.trackTunai.cekSaldo());
         print("Masukkan Jumlah : ", false);
         inputAngka(amount, "MASUKKAN JUMLAH : ");
         system("cls");
         printIncomeInterface("TUNAI", user.trackTunai.cekSaldo(), amount);
         cout << "Masukkan Deskripsi : ";
         cin.ignore();  // Membersihkan buffer
         getline(cin, desk);
         system("cls");
         user.trackTunai.tambahTrans(Income, desk, amount, "");
         printIncomeInterface("TUNAI", user.trackTunai.cekSaldo(), amount, desk);
         cout << "Tekan apa saja untuk kembali ...";
         getch();
         break;
      case '2':
         system("cls");
         printIncomeInterface("E-MONEY", user.trackEMoney.cekSaldo());
         print("Masukkan Jumlah : ", false);
         inputAngka(amount, "MASUKKAN JUMLAH : ");
         system("cls");
         printIncomeInterface("E-MONEY", user.trackEMoney.cekSaldo(), amount);
         cout << "Masukkan Deskripsi : ";
         cin.ignore();  // Membersihkan buffer
         getline(cin, desk);
         system("cls");
         user.trackEMoney.tambahTrans(Income, desk, amount, "");
         printIncomeInterface("E-MONEY", user.trackEMoney.cekSaldo(), amount, desk);
         cout << "Tekan apa saja untuk kembali ...";
         getch();
         break;
      case '3':
         system("cls");
         printIncomeInterface("BANK", user.trackBank.cekSaldo());
         print("Masukkan Jumlah : ", false);
         inputAngka(amount, "MASUKKAN JUMLAH : ");
         system("cls");
         printIncomeInterface("BANK", user.trackBank.cekSaldo(), amount);
         cout << "Masukkan Deskripsi : ";
         cin.ignore();  // Membersihkan buffer
         getline(cin, desk);
         system("cls");
         user.trackBank.tambahTrans(Income, desk, amount, "");
         printIncomeInterface("BANK", user.trackBank.cekSaldo(), amount, desk);
         cout << "Tekan apa saja untuk kembali ...";
         getch();
         break;
      default:
         akun(true, "Pilih Akun Pemasukan");
   }
}
//-----------------------------------

//--------------------------

// mengupdate interface sebanyak tiga kali sesuai yang diinginkan
void printTransferInterface(string fromAkun, unsigned long long jumlahSaldoFrom, string toAkun,
                            unsigned long long jumlahSaldoTo, unsigned long long jumlahTrans,
                            string desk = "-") {
   print(samadengan(intSpace));
   printVariabel(intSpace, "TRANSFER");
   print(samadengan(intSpace));
   cout << "FROM : " << fromAkun << " (" << jumlahSaldoFrom << ")"
        << "\t| TO : " << toAkun << " (" << jumlahSaldoTo << ")\n";
   print(garis(intSpace));
   cout << "Amount : " << jumlahTrans << "\n";
   print(garis(intSpace));
   cout << "DESKRIPSI : " << desk << "\n";
   print(garis(intSpace));
}
//--------------------CHOOSE 3

void transfer(User& user) {
   unsigned long long amount;
   string desk;
   printTransferInterface(space(10), 0, space(10), 0, 0);
   char akunChoice = akun(false, "Account From : ");
   switch (akunChoice) {
      case '1':
         system("cls");
         printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), space(10), 0, 0);
         print(garis(intSpace));
         printVariabel(intSpace, "AKUN");
         print(garis(intSpace));
         printVariabel(intSpace, "1. E-MONEY");
         printVariabel(intSpace, "2. BANK");
         print(garis(intSpace));

         cout << "Account To : ";
         char a;
         inputChar(a, "ACCOUNT TO (1/2) : ");
         if (a == '1') {
            system("cls");
            printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), "E-MONEY",
                                   user.trackEMoney.cekSaldo(), 0);
            cout << "\nJumlah Transfer : ";
            inputAngka(amount, "JUMLAH TRANSFER (ANGKA) : ");
            system("cls");
            printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), "E-MONEY",
                                   user.trackBank.cekSaldo(), amount);
            cout << "\nDeskripsi : ";
            cin.ignore();  // Membersihkan buffer
            getline(cin, desk);
            system("cls");
            user.transferUang(user.trackTunai, user.trackEMoney, amount, desk);
            printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), "E-MONEY",
                                   user.trackEMoney.cekSaldo(), amount, desk);
            cout << " Tekan Apa saja untuk keluar ...";
            getch();
         } else if (a == '2') {
            system("cls");
            printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), "BANK",
                                   user.trackBank.cekSaldo(), 0);
            cout << "\nJumlah Transfer : ";
            inputAngka(amount, "JUMLAH TRANSFER (ANGKA) : ");
            system("cls");
            printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), "BANK",
                                   user.trackBank.cekSaldo(), amount);
            cout << "\nDeskripsi : ";
            cin.ignore();  // Membersihkan buffer
            getline(cin, desk);
            system("cls");
            user.transferUang(user.trackTunai, user.trackBank, amount, desk);
            printTransferInterface("TUNAI", user.trackTunai.cekSaldo(), "BANK",
                                   user.trackBank.cekSaldo(), amount, desk);
            cout << "Tekan Apa saja untuk keluar ...";
            getch();
         } else {
            cout << "Invalid input!";
            system("cls");
            transfer(user);
         }
         break;
      case '2':
         system("cls");
         printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), space(10), 0, 0);
         print(garis(intSpace));
         printVariabel(intSpace, "AKUN");
         print(garis(intSpace));
         printVariabel(intSpace, "1. TUNAI");
         printVariabel(intSpace, "2. BANK");
         print(garis(intSpace));

         cout << "Account To : ";
         char b;
         inputChar(b, "ACCOUNT TO : ");

         if (b == '1') {
            system("cls");
            printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), "TUNAI",
                                   user.trackTunai.cekSaldo(), 0);
            cout << "\nJumlah Transfer : ";
            inputAngka(amount, "JUMLAH TRANSFER (ANGKA) : ");
            system("cls");
            printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), "TUNAI",
                                   user.trackTunai.cekSaldo(), amount);
            cout << "\nDeskripsi : ";
            cin.ignore();  // Membersihkan buffer
            getline(cin, desk);
            system("cls");
            user.transferUang(user.trackEMoney, user.trackTunai, amount, desk);
            printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), "TUNAI",
                                   user.trackTunai.cekSaldo(), amount, desk);

         } else if (b == '2') {
            system("cls");
            printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), "BANK",
                                   user.trackBank.cekSaldo(), 0);
            cout << "\nJumlah Transfer : ";
            inputAngka(amount, "JUMLAH TRANSFER (ANGKA) : ");
            system("cls");
            printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), "BANK",
                                   user.trackBank.cekSaldo(), amount);
            cout << "\nDeskripsi : ";
            cin.ignore();  // Membersihkan buffer
            getline(cin, desk);
            system("cls");
            user.transferUang(user.trackEMoney, user.trackBank, amount, desk);
            printTransferInterface("E-MONEY", user.trackEMoney.cekSaldo(), "BANK",
                                   user.trackBank.cekSaldo(), amount, desk);

         } else {
            cout << "Invalid input!";
            system("cls");
            transfer(user);
         }

         cout << "Tekan Apa saja untuk keluar ...";
         getch();
         break;

      case '3':
         system("cls");
         printTransferInterface("BANK", user.trackBank.cekSaldo(), space(10), 0, 0);
         print(garis(intSpace));
         printVariabel(intSpace, "AKUN");
         print(garis(intSpace));
         printVariabel(intSpace, "1. TUNAI");
         printVariabel(intSpace, "2. E-MONEY");
         print(garis(intSpace));

         cout << "Account To : ";
         char c;
         inputChar(c, "ACCOUNT TO : ");

         if (c == '1') {
            system("cls");
            printTransferInterface("BANK", user.trackBank.cekSaldo(), "TUNAI",
                                   user.trackTunai.cekSaldo(), 0);
            cout << "\nJumlah Transfer : ";
            inputAngka(amount, "JUMLAH TRANSFER (ANGKA) : ");
            system("cls");
            printTransferInterface("BANK", user.trackBank.cekSaldo(), "TUNAI",
                                   user.trackTunai.cekSaldo(), amount);
            cout << "\nDeskripsi : ";
            cin.ignore();  // Membersihkan buffer
            getline(cin, desk);
            system("cls");
            user.transferUang(user.trackBank, user.trackTunai, amount, desk);
            printTransferInterface("BANK", user.trackBank.cekSaldo(), "TUNAI",
                                   user.trackTunai.cekSaldo(), amount, desk);
         } else if (c == '2') {
            system("cls");
            printTransferInterface("BANK", user.trackBank.cekSaldo(), "E-MONEY",
                                   user.trackEMoney.cekSaldo(), 0);
            cout << "\nJumlah Transfer : ";
            inputAngka(amount, "JUMLAH TRANSFER (ANGKA) : ");
            system("cls");
            printTransferInterface("BANK", user.trackBank.cekSaldo(), "E-MONEY",
                                   user.trackEMoney.cekSaldo(), amount);
            cout << "\nDeskripsi : ";
            cin.ignore();  // Membersihkan buffer
            getline(cin, desk);
            system("cls");
            user.transferUang(user.trackBank, user.trackEMoney, amount, desk);
            printTransferInterface("BANK", user.trackBank.cekSaldo(), "E-MONEY",
                                   user.trackEMoney.cekSaldo(), amount, desk);

         } else {
            cout << "Invalid input!";
            system("cls");
            transfer(user);
         }

         cout << "Tekan Apa saja untuk keluar ...";
         getch();

         break;

      default:
         cout << "Error!";
         getch();
   }
}

//---------------------CHOOSE 4
void analitik(User& user) {
   char pilihan;
   print(samadengan(intSpace));
   printVariabel(intSpace, "ANALYTICAL");
   print(samadengan(intSpace));
   printVariabel(intSpace, "1. HISTORY");
   printVariabel(intSpace, "2. PEMASUKAN");
   printVariabel(intSpace, "3. PENGELUARAN");
   printVariabel(intSpace, "0. KEMBALI");
   print(garis(intSpace));

   print("Pilih : ", false);
   inputChar(pilihan, "PILIH (1-4) : ");
   if (pilihan == '0') {
      return;
   } else if (pilihan == '1') {
      system("cls");
      user.printSemuaTrans();
      print("\n\nTekan Apa saja Untuk Kembali ...");
      getch();
      system("cls");
      analitik(user);
   } else if (pilihan == '2') {
      system("cls");
      user.printAllIncomeTrack();
      print("\n\nTekan Apa saja Untuk Kembali ...");
      getch();
      system("cls");
      analitik(user);
   } else if (pilihan == '3') {
      system("cls");
      user.printTabelAkunPerKat(k);
      print("\n\nTekan Apa saja Untuk Kembali ...");
      getch();
      system("cls");
      analitik(user);
   }
}

//---------------------------------------------------------------------
void printTampilanMainMenu() {
   print(samadengan(intSpace));
   printVariabel(intSpace, "MONEY TRACKER");
   print(samadengan(intSpace));
   printVariabel(intSpace, "1. PENGELUARAN");
   printVariabel(intSpace, "2. PEMASUKAN");
   printVariabel(intSpace, "3. TRANSFER");
   printVariabel(intSpace, "4. ANALISIS");
   printVariabel(intSpace, "0. EXIT");
   print(garis(intSpace));
}

void printMainMenu(User& user) {
backToMenu:
   char pilih;
   printTampilanMainMenu();
   cout << "\nPilih : ";
   inputChar(pilih, "Pilih (1-4) : ");

   switch (pilih) {
      case '0':
         system("cls");
         loading();
         break;
      case '1':

         // OUTCOME
         expense(user);
         system("cls");
         printMainMenu(user);
         break;
      case '2':
         // PEMASUKAN
         income(user);
         system("cls");
         printMainMenu(user);
         break;
      case '3':
         system("cls");
         transfer(user);
         system("cls");
         printMainMenu(user);
         break;
      case '4':
         system("cls");
         analitik(user);
         system("cls");
         printMainMenu(user);
         break;
      default:
         print("Tekan Apa saja Untuk Merestart ...");
         getch();
         system("cls");
         printMainMenu(user);
   }
}

//------------------fungsi utama-------------------
int main() {
atas:
   system("cls");
   cout << "Nama : ";
   string id;
   cin >> id;
   string pass = getPassword();
   cout << "\n\n";
   //
   //	if(pass == "sanjaya"){
   //		print(pass);
   //		sleep(3);
   //	}

   //	system("cls");

   unsigned long long saldo;
   User eril;

   loading();
   system("cls");
   cout << "Selamat Datang , " << id << "\n\nSilahkan Masukkan Saldo Awal Kamu :) \n";
   printSaldoAwal(eril, saldo);
   system("cls");
   printMainMenu(eril);
   goto atas;
   return 0;
}
