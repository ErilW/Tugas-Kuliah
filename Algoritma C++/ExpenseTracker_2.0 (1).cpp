#include <conio.h>
#include <time.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <sstream>

#include "data (2).h"
#define MAX_TRANS 150
using namespace std;


// menggunakan print kategori ke paling atas
void printKategori(void);

//tipe transaksi
enum transactionType { Income, Outcome };
//data transaksi
struct Transaksi {
   transactionType type;
   tm tanggal;
   string deskripsi;
   unsigned long long jumlah;
};
//data kategori
struct Kategori {
   string nama;
   unsigned long long jumlahUtamaKategori;
};
//pengolahan transaksi
class TrackerTransaksi {
	//data private yang akan diakses oleh methods public
  private:
   unsigned long long jumlahUang = 0;
   int currentIndex = 0;
   //Jumlah trans = 100
   Transaksi transaksi[MAX_TRANS];
   Kategori kategori[MAX_TRANS];
   
   // fungsi menambahkan kategori dipakai tambah trans
   void tambahUangPerKategori(string nama, unsigned long long jumllah) {
      kategori[currentIndex].nama = nama;
      kategori[currentIndex].jumlahUtamaKategori += jumllah;
   }
   
   //mengupdate data dengan kategori yang sama
   unsigned long long updateDataPerKategori(string namaKategori) {
      unsigned long long totalUtamaPerKategori = 0;

      int i;
      for (i = 0; i < currentIndex; i++) {
         if (namaKategori == kategori[i].nama) {
            totalUtamaPerKategori += transaksi[i].jumlah;
         }
      }
      //double check
      if (i > 0) {
         kategori[i - 1].jumlahUtamaKategori =
             totalUtamaPerKategori;  // Update the total in the kategori array
      }

      return totalUtamaPerKategori;
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

	//data yang bisa dipakai semua oleh pemakai class
  public:
  	//kembalikan hasil penghitungan jumlah uang dalam param kategori
   unsigned long long jumlahKategoriPerArrayStruct(string& namaKategori) {
      return updateDataPerKategori(namaKategori);
   }
   
   //mengecek nama kategori dalam array string didalam data kategori, apakah ada atau tidaknya
  	bool checkKategoriOnStruct(int &index,string array[]){
 		for (int i = 0; i <= currentIndex; i ++){
 			if( array[index] == kategori[i].nama){
 				return true;
			 }
		
		 }
		 return false;
	 }

   // Sebagai penambahan Saldo Awal data jumlahUatama
   unsigned long long saldoAwal(unsigned long long saldo) {
      tambahUang(saldo);
      return saldo;
   }

   // fungsi pengecekan jumlahUTama transaksi
   unsigned long long cekSaldo() { return jumlahUang; }
   unsigned int cekSaldoINT() { 
  	 unsigned int a = jumlahUang;
 		  return a; 
		   }
		   
   // Tambah transaksi per index sesuai dengan current index dari private
   void tambahTrans(transactionType type, const string deskripsi, unsigned long long jumlah, string namaKategori) {
   	//format waktu
      time_t rawtime;
      time(&rawtime);
      tm* waktu = localtime(&rawtime);
      //simpan data struct kedalam parameter
      transaksi[currentIndex].type = type;
      transaksi[currentIndex].tanggal = *waktu;
      transaksi[currentIndex].deskripsi = deskripsi;
      transaksi[currentIndex].jumlah = jumlah;

      // Check jika kategori udah ada atau belum
      bool categoryExists = false;
      int existingCategoryIndex = -1;
      for (int i = 0; i < currentIndex; ++i) {
         if (kategori[i].nama == namaKategori) {
            categoryExists = true;
            existingCategoryIndex = i;
            break;
         }
      }
      
      // jika ya, maka akan menggunakan kategori yang sudah ada
      if (categoryExists) {
         // jika pilihan transaksi nya adalh income
         if (type == Income) {
            tambahUang(jumlah);
            currentIndex++;
            //jika bukan
         } else {
            // check, apaka bisa jumlah uang utama mengurangi?
            if ((jumlahUang - jumlah) >= 0) {
               //kurang uang utama transaksi jika emang bisa dikurangi
               kurangUang(jumlah);
               //menambah kan uang ke expense perkategori
               tambahUangPerKategori(namaKategori, jumlah);
               updateDataPerKategori(namaKategori);
               //index berubah ke transaksi selanjutnya
               currentIndex++;
            } else {
            	//jika gak cukup, cuman kasih tunjuk uang nya
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
   
   //fungsi untuk menambah kan kategori baru
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


   // fungsi transfer dari akun lain
   void transfer(TrackerTransaksi& to, unsigned long long amount, const string& description) {
   	  // jika uang utama dalam akun tersebut kosong atau lebih kecil dari pengiriman
      if (cekSaldo() >= amount) {
      	//tambah trans untuk mengeluarkan uang dari track ini ke track lainnya
         tambahTrans(Outcome, description, amount, "transfer");
         to.tambahTrans(Income, description, amount, "");
         cout << "Transfer successful!" << endl;
      } else {
         cout << "Transfer failed. Insufficient funds." << endl;
      }
   }

   //-------------------------------PRINTING------------------------------------------
   

	//print income keseluruhan
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
   
   //fungsi untuk mengeprint transaksi saat ini
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


 
 //print keseluruhan transaksi
   void printTrans() {
      // Print header
      cout << setw(20) << "Tanggal" << setw(20) << "Deskripsi" << setw(15) << "Jenis" << setw(15)
           << "Transaksi" << endl;
      // Print transactions
      for (int i = currentIndex; i > 0; --i) {
      	
      	if(transaksi[i].deskripsi == ""){
      		transaksi[i].deskripsi = "Kosong !";
		  }
      	
         if (transaksi[i].tanggal.tm_hour != 0 && transaksi[i].tanggal.tm_min != 0 && transaksi[i].tanggal.tm_sec != 0 && transaksi[i].deskripsi != "") {
            string jenis = (transaksi[i].type == Income) ? "Income" : "Outcome";
            time_t waktu = mktime(&transaksi[i].tanggal);  // Konversi struct tm ke time_t
            tm* waktuTm = localtime(&waktu);
            char buffer[80];
			strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &transaksi[i].tanggal);
            // Convert time_t to readable format using ctime
            cout << setw(20) << buffer << setw(20) << transaksi[i].deskripsi << setw(15) << jenis
                 << setw(15) << transaksi[i].jumlah << endl;
         }
      }
   }
   
   
   //----------FUNSI KHUSUS DIPAKAI DILUAR---------------------
    
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
};

//--------------------------------------CLASS USER----------------------------------//

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
  	//methode akun
   TrackerTransaksi trackTunai;
   TrackerTransaksi trackEMoney;
   TrackerTransaksi trackBank;

   void transferUang(TrackerTransaksi& from, TrackerTransaksi& to, unsigned long long amount, const string& description) {
      from.transfer(to, amount, description);
   }
   
   //print keseluruhan transaksi dari setiap akun
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
   
   //print keseluruhan transaksi income per track
   void printAllIncomeTrack() {
      trackTunai.printTransIncome("TUNAI");
      trackEMoney.printTransIncome("E-MONEY");
      trackBank.printTransIncome("BANK");
   }

 	
 	
//Print tabel outcome per kategori
   void printTabelAkunPerKat(string stringArray[]) {
    	int kategoriWidth = 20;
		int tunaiWidth = 15;
  		int emoneyWidth = 15;
  		int bankWidth = 10;
  		int totalWidth = 15;

    cout << "| " << setw(kategoriWidth) << "KATEGORI" << " | "
         << setw(tunaiWidth) << "TUNAI" << " | "
         << setw(emoneyWidth) << "E-MONEY" << " | "
         << setw(bankWidth) << "BANK" << " | "
         << setw(totalWidth) << "TOTAL" << " |\n";

    for (int i = 0; i < len(k); i++) {
        if (k[i] != "" && trackTunai.checkKategoriOnStruct(i, k)) {
            cout << "| " << setw(kategoriWidth) << k[i] << " | "
                 << setw(tunaiWidth) << trackTunai.jumlahKategoriPerArrayStruct(k[i]) << " | "
                 << setw(emoneyWidth) << trackEMoney.jumlahKategoriPerArrayStruct(k[i]) << " | "
                 << setw(bankWidth) << trackBank.jumlahKategoriPerArrayStruct(k[i]) << " | "
                 << setw(totalWidth) << jumlahUangPerTrack(k[i]) << " |\n";
        }
    }
}
};

//----------------------DILUAR CLASS-----------------------------//



//fungsi memilih jenis akun
char akun(User &user, bool isCls = false, string pesan = "Pilih Metode Akun : ") {
   char pilihan;
   print(garis(intSpace));
   printVariabel(intSpace, "AKUN");
   print(garis(intSpace));
//   s;
 printVariabela(intSpace,  "1. TUNAI --> ", IntToString(user.trackTunai.cekSaldoINT()) );
   printVariabela(intSpace, "2. E-MONEY --> ", IntToString(user.trackEMoney.cekSaldoINT()));
   printVariabela(intSpace, "3. BANK --> ",IntToString(user.trackBank.cekSaldoINT()));
   print(garis(intSpace));

   print(pesan, false);
   inputChar(pilihan, "PILIH METODE AKUN (1-3) : ");

   if (pilihan == '1' || pilihan == '2' || pilihan == '3') {
      return pilihan;
   } else {
      print("Tekan Apa saja Untuk Merestart ...");
      getch();
      if (isCls) {
         system("cls");
      }
      return akun(user,isCls = false, "Pilih Metode Akun : ");
   }
}

//--------------------------------------------------------

//fungsi menampilkan nama project
void projectExpenseTracker() {
	int intSpace = 53;
   print(samadengan(intSpace));
   cout <<  "|   PROJECT    EXPENSE   TRACKER                    |\n";
   print(samadengan(intSpace));
   cout<<   "|   NAMA KELOMPOK     :    SOLAR (1)                |\n";
   print(garis(intSpace));
   cout<<   "|   DOSEN             :    Pak Yodi                 |\n";
   print(garis(intSpace));
    cout << "|   1.  Eril Sanjaya         --> 2023131009         |\n";
    cout << "|   2.  Lustini              --> 2023131001         |\n";
    cout << "|   3.  James Lim            --> 2023131005         |\n";
    cout << "|   4.  Muhammad Naufal      --> 2023131017         |\n";
    cout << "|   5.  Peter Pangaribuan    --> 2023133001         |\n";
    cout << "|   6.  Victor               --> 2023131013         |\n";
    cout << "|   7.  Sidarta David Setia  --> 2023133005         |\n";

   print(garis(intSpace));
//   printVariabel(intSpace, "--> KETUA");
   
  
}
//-----------------------------------------------------------------
// fungsi sekali pakai, ingat!!!
void printSaldoAwal(User& user, unsigned long long& saldo) {
   print(samadengan(intSpace));
   printVariabel(intSpace, "SALDO AWAL");
   print(samadengan(intSpace));
   switch (akun(user,true)) {
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

void printKategori(string judul,  int check) {
   print(samadengan(intSpace));
   printVariabel(intSpace, "KATEGORI", judul, " ");
   print(samadengan(intSpace));

   for (int i = 0; i < len(k); i++) {
      if (k[i] != "") {
         printVariabel(intSpace, k[i], IntToString(i + 1), " ");
      }
   }
   printVariabel(intSpace, "Tambah Kategori", IntToString(check), " ");
   print(garis(intSpace));
}
//----------------------------------------------------------

//----------------------------------------------------------
void expenseInput(TrackerTransaksi& track, string judul) {
   int currentIndex = 0;

   atasExpense:

   for (int i = 0; i < len(k); i++) {
      if (k[i] == "") {
         currentIndex = len(k) - (len(k) - i);
         break;
      }
      
   }

  unsigned long long pilih;
unsigned long long nominal;
string desk;

system("cls");
printKategori(judul, currentIndex + 1);

string data = "Pilih Kategori (1 - " + IntToString(currentIndex) + ") : ";
cout << "Pilih Kategori (1 - " << IntToString(currentIndex) << ") : ";
inputAngka(pilih, "PILIH KATEGORI : ");

if (pilih > 0 && pilih <= currentIndex) {
    track.pilihanKategoriPerInput(nominal, desk, k[pilih - 1]);
} else if (pilih == currentIndex + 1) {
    print("Silahkan menambahkan kategori : ", false);
    string newString;
    cin.ignore();
    getline(cin, newString);
    
   for (int j = 0; j < currentIndex; j++) {
    if (k[j] == newString) {
        cout << "Sudah ada Kategori yang sama !!!";
        // If a matching category is found, inform the user and exit the function
        getch();
        return;
    }
}

// If no matching category is found, add the new category
k[currentIndex] = newString;
++currentIndex;

// Clear the screen and go back to the beginning
system("cls");
goto atasExpense;
 }else{
 	cout<<"TEKAN APA SAJA UNTUK KELUAR !!!";
 	getch();
 }
}


//----------------------------------------------------------

void expense(User& user) {
   char akunChoice = akun(user,false, "Pilih Akun Pengeluaran : ");

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

//-------------------CHOOSE 2---------
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
   switch (akun(user,"Pilih Akun Pemasukan : ")) {
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
         akun(user,true, "Pilih Akun Pemasukan");
   }
}

//--------------------CHOOSE 3

// mengupdate interface sebanyak tiga kali sesuai yang diinginkan
void printTransferInterface(string fromAkun, unsigned long long jumlahSaldoFrom, 
							string toAkun, unsigned long long jumlahSaldoTo, unsigned long long jumlahTrans,
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

void transfer(User& user) {
   unsigned long long amount;
   string desk;
   printTransferInterface(space(10), 0, space(10), 0, 0);
   char akunChoice = akun(user,false, "Account From : ");
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
   printVariabel(intSpace, "5. KREDIT");
   printVariabel(intSpace, "0. EXIT");
   print(garis(intSpace));
}

void printMainMenu(User& user) {
backToMenu:
   char pilih;
   printTampilanMainMenu();
   cout << "\nPilih : ";
   inputChar(pilih, "Pilih (1-5) : ");

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
      	//system transfer
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
      case '5':
         system("cls");
         projectExpenseTracker();
         cout<<"\n\nTekan Apa saja untuk kembali ...";
         getch();
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
	projectExpenseTracker();
	loading();
   system("cls");
   cout << "Nama : ";
   string id;
   cin >> id;
   string pass = getPassword();
   cout << "\n\n";
   
   
   	if(pass == "admin"){
   loading();
   	system("cls");
   unsigned long long saldo;	
   		User admin;
   		admin.trackTunai.saldoAwal(5000000);
   		admin.transferUang(admin.trackTunai, admin.trackEMoney, 2000000, "Transfer");
   		admin.trackTunai.tambahTrans(Outcome, "" ,20000, k[0]);
   		admin.trackEMoney.tambahTrans(Outcome, "" ,15000, k[1]);
   		admin.trackTunai.tambahTrans(Outcome, "" ,100000, k[2]);
   		admin.trackTunai.tambahTrans(Outcome, "" ,150000, k[4]);
   		admin.trackEMoney.tambahTrans(Outcome, "" ,100000, k[6]);
   		admin.trackTunai.tambahTrans(Outcome, "" ,25000 , k[0]);
   		admin.trackTunai.tambahTrans(Outcome, "" , 120000 , k[5]);
   		admin.trackEMoney.tambahTrans(Outcome, "" ,  20000 , k[1]);
   		admin.trackTunai.tambahTrans(Outcome, "Service Hp" ,  500000 , k[7]);
   		admin.trackEMoney.tambahTrans(Outcome, "" ,  50000 , k[6]);
   		admin.trackBank.tambahTrans(Income, "gaji bulan agustus" ,  4500000 ,"");
   		admin.trackBank.tambahTrans(Outcome, "spp bulan agustus" ,  1500000 , k[3]);
   		admin.trackTunai.tambahTrans(Outcome, "" ,50000, k[0]);	
   		admin.trackEMoney.tambahTrans(Outcome, "" ,20000, k[1]);	
   		admin.trackBank.tambahTrans(Outcome, "" ,80000, k[6]);	
		admin.trackTunai.tambahTrans(Outcome, "" ,20000, k[0]);
		admin.trackEMoney.tambahTrans(Outcome, "" ,20000, k[1]);
		admin.trackTunai.tambahTrans(Outcome, "" ,50000, k[4]);
		admin.trackTunai.tambahTrans(Outcome, "" ,250000, k[2]);
		admin.trackTunai.tambahTrans(Outcome, "" ,25000, k[0]);
		admin.trackBank.tambahTrans(Outcome, "" ,150000, k[4]);
		admin.trackTunai.tambahTrans(Outcome, "" ,80000, k[6]);
		admin.trackTunai.tambahTrans(Outcome, "" ,25000, k[0]);
		admin.trackEMoney.tambahTrans(Outcome, "" ,15000, k[1]);
		admin.trackTunai.tambahTrans(Outcome, "Service Laptop" , 800000, k[7]);
		admin.transferUang(admin.trackBank, admin.trackTunai, 1000000, "");
		admin.trackTunai.tambahTrans(Outcome, "Service Kendaraan" , 200000, k[2]);
		admin.trackTunai.tambahTrans(Outcome, "" , 30000, k[0]);
		admin.trackBank.tambahTrans(Outcome, "" ,40000, k[6]);
		admin.trackEMoney.tambahTrans(Outcome, "" ,30000, k[1]);
		

   		 system("cls");
   		 cout << "Selamat Datang kembali , " << id << "\n";
       printMainMenu(admin);
		goto atas;
return 0;
   		
   	}else{
   system("cls");
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
}
