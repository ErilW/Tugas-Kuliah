	#include <iostream>
	#include <time.h>
	#include <iomanip>
	#include <unistd.h>
	#include "data (2).h"
	#include <conio.h>
	#define MAX_TRANS 100
	using namespace std;
	
	enum transactionType {
	    Income,
	    Outcome
	};
	
	struct Transaksi {
	    transactionType type;
	    tm tanggal;
	    string deskripsi;
	    unsigned int jumlah;
	    
	};
	
	struct Kategori {
	    string nama;
	    unsigned int jumlahUtamaKategori;
	};
	
	class TrackerTransaksi {
	private:
	    unsigned int jumlahUang = 0;
	    int currentIndex = 0;
	    Transaksi transaksi[MAX_TRANS];
	    Kategori kategori[MAX_TRANS];
	    
	       void tambahUangPerKategori(string nama /*nama kategori input user*/, unsigned int jumllah){
	    	kategori[currentIndex].nama = nama;
	    	kategori[currentIndex].jumlahUtamaKategori += jumllah; 
		}
	    
	public:
		
		
	     //Sebagai Saldo Awal data jumlahUatama
	    unsigned int saldoAwal(unsigned int saldo) {
	        tambahUang(saldo);
	        return saldo;
	    }
	    
	    // fungsi pengecekan jumlahUTama transaksi
	    unsigned int cekSaldo() {
	        return jumlahUang;
	    }
	
	  
	    //Tambah transaksi per index sesuai dengan current index dari private
 void tambahTrans(transactionType type, const string deskripsi, unsigned int jumlah, string namaKategori = "") {
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
	    unsigned int jumlahIncome() {
	    	//jumlah yang akan direturn
	        unsigned int totalIncome = 0;
	        //looping sesuai dengan index sekarang 
	        for (int i = 0; i < currentIndex; ++i) {
	        	//pengecekan jika type nya adalah income
	            if (transaksi[i].type == Income) {
	            	//mereturn total income setelah penambahan seusai dengan array nambahTrans()
	                totalIncome += transaksi[i].jumlah;
	            }
	        }
	        return totalIncome;
	    }
	// Jumlah Keseluruhan OutCome yang sudah dimasukkan di fungsi nambahTrans()
	    unsigned int jumlahOutcome() {
	    	//jumlah outcome keseluruhan
	        unsigned int totalOutcome = 0;
	        //looping 
	        for (int i = 0; i < currentIndex; ++i) {
	        	//type nya adalah outcome
	            if (transaksi[i].type == Outcome) {
	            	//mereturn nilai penambahan dari seluruh outcome yang sudah pernah dilakukan
	                totalOutcome += transaksi[i].jumlah;
	            }
	        }
	        return totalOutcome;
	    }
	
		// print transaksi yang sudah pernah dilakukan
	void printTransOutcome() {
		cout<<"\tOUTCOME\n";
    cout << setw(20) << "Tanggal" << setw(30) << "Deskripsi" << setw(15) << "Kategori" << setw(15) << "Nominal" << endl;
    for (int i = 0; i < currentIndex; ++i) {
        // Pengecekan jika kategori tidak kosong dan deskripsi tidak kosong
         if (!kategori[i].nama.empty() && !transaksi[i].deskripsi.empty()) {
            // Outcome
            time_t waktu = mktime(&transaksi[i].tanggal);  // Konversi struct tm ke time_t
            tm* waktuTm = localtime(&waktu);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d, %H:%M:%S", waktuTm);
            if (transaksi[i].type != Income) {
                cout << setw(20) << buffer << setw(30) << transaksi[i].deskripsi << setw(15) << kategori[i].nama << setw(15) << transaksi[i].jumlah << endl;
            } 
        }  
    }
}

void printTransIncome() {
    cout << "\tINCOME\n";
    cout << setw(20) << "Tanggal" << setw(30) << "Deskripsi"  << setw(15) << "Nominal" << endl;
    for (int i = 0; i < currentIndex; ++i) {
        // Pengecekan jika kategori tidak kosong dan deskripsi tidak kosong
        if (!transaksi[i].deskripsi.empty()) {
            // Income
            time_t waktu = mktime(&transaksi[i].tanggal);  // Konversi struct tm ke time_t
            tm* waktuTm = localtime(&waktu);
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%d, %H:%M:%S", waktuTm);
            if (transaksi[i].type == Income) {
                cout << setw(20) << buffer << setw(30) << transaksi[i].deskripsi  << setw(15) << transaksi[i].jumlah << endl;
            }
        }
    }
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
		   
		   
		    void tambahUang(unsigned int saldo) {
	        jumlahUang += saldo;
		    }
		    
		    
	   // fungsi kurang uang
	    void kurangUang(unsigned int saldo) {
	    	//check bahwa saldo yang ingin dikurang tidak akan lebih dari jumlahUtama dan jumlahuang tidak 0
	        if (jumlahUang != 0 && jumlahUang >= saldo) {
	            jumlahUang -= saldo;
	        }
	    }
	    //fungsi yang menambahkan per kategori, dan hanya diakses oleh tambahTrans()
	 
	
	    void transfer(TrackerTransaksi& to, unsigned int amount, const string& description) {
	        if (cekSaldo() >= amount) {
	            tambahTrans(Outcome, description, amount, "transfer");
	            to.tambahTrans(Income, description, amount, "");
	            cout << "Transfer successful!" << endl;
	        }else {
	            cout << "Transfer failed. Insufficient funds." << endl;
	        }
	    }
	    
	//melakukan pengecekan disetiap nama, jika nama yang diinput user(parameter) sama dengan nama di kategori saat itu
	//pengisian parameter string untuk melakukan pengecekan bahwa parameter tersebut sudah pernah dipakai sebelumnya
	
	unsigned int updateDataPerKategori (string namaKategori){
	 unsigned int totalUtamaPerKategori = 0;
	  for (int i=0; i < currentIndex; i++){
	    if(namaKategori == kategori[i].nama){ //  dicek 1 - 100
	    // jika ya maka akan dilakukan penambahan disetiap inputannya
	    //hanya menambahkan ketika benar benar sesuai dengan kategori nya sendiri
	  	totalUtamaPerKategori += kategori[i].jumlahUtamaKategori ;
	    }
	  }
	  return totalUtamaPerKategori;
	}
	
	





	void printKategoriPerStruct() {
    cout << setw(20) << "Nama Kategori" << setw(30) << "Nominal" << endl;
    for (int i = 0; i < currentIndex; ++i) {
        if (!kategori[i].nama.empty()) {
            // Cek apakah nama kategori sudah ada sebelumnya
            bool found = false;
            for (int j = 0; j < i; ++j) {
                if (kategori[j].nama == kategori[i].nama) {
                    found = true;
                    break;
                }
            }
            // Jika nama kategori belum pernah ditemukan sebelumnya, cetak nama kategori dan jumlah utama
            if (!found) {
                cout << setw(20) << kategori[i].nama << setw(30) << updateDataPerKategori(kategori[i].nama) << endl;
            }
   	     }
   	 }
		}

	
};
	
	class User {
	public:
	    
	    TrackerTransaksi trackTunai;
	    TrackerTransaksi trackEMoney;
	    TrackerTransaksi trackBank;
	
	    void transfer(TrackerTransaksi& from, TrackerTransaksi& to, unsigned int amount, const string& description) {
	        from.transfer(to, amount, description);
	    }
	
	    void printSemuaTrans() {
	        cout << "Saldo Tunai: " << trackTunai.cekSaldo() << endl;
	        cout << "Saldo Bank: " << trackBank.cekSaldo() << endl;
	        cout << "Saldo EMoney: " << trackEMoney.cekSaldo() << endl;
	        cout << "\nTransaksi Tunai:\n";
	        trackTunai.printTransOutcome();
	
//	        cout << "\nTransaksi Bank:\n";
//	        trackBank.printTrans(true);
	
//	        cout << "\nTransaksi EMoney:\n";
//	        trackEMoney.printTrans(true);
	    }
	};
	
	
	


char akun(bool isCls = false) {
    char pilihan;
    print(garis(intSpace));
    printVariabel(intSpace, "AKUN");
    print(garis(intSpace));
    printVariabel(intSpace, "1. TUNAI");
    printVariabel(intSpace, "2. E-MONEY");
    printVariabel(intSpace, "3. BANK");
    print(garis(intSpace));
    
    print("Pilih Metode Akun : ", false);
    inputChar(pilihan, "Silahkan Masukkan Ulang : ");

    if (pilihan >= '1' && pilihan <= '3') {
        return pilihan;
    }else{
	
 	   print("Tekan Apa saja Untuk Merestart ...");
 	   getch();
 	   if(isCls){
		system("cls");
		}
  	        return akun();
	}
}

		

		
		
	//--------------------------------------------------------
		void projectExpenseTracker(){
	print(samadengan(intSpace));
	printVariabel(intSpace, "PROJECT EXPENSE TRACKER");
	print (samadengan(intSpace));
	printVariabel(intSpace, "NAMA KELOMPOK : 1");
	print(garis(intSpace));
	loading();
	system("cls");
} 
//-----------------------------------------------------------------
//fungsi sekali pakai, ingat!!!
void printSaldoAwal(User user, unsigned int & saldo){
	print(samadengan(intSpace));
		printVariabel(intSpace,"SALDO UTAMA");
	print(samadengan(intSpace));
	switch (akun(true)){
		// case saat menggunakan cash
		//--------------
		case '1' : 
     	print("Masukkan Nominal : ", false);
     	// meninput nilai uang ke dalam dana cash sesuai parameter index saat dipanggil
     	
        inputAngka(saldo, "Masukkan Nominal : ");
        user.trackTunai.saldoAwal(saldo);
        cout<< "Saldo Awal Tunai : Rp."<<user.trackTunai.cekSaldo();
    	sleep(2);
		 break;
        //-------------
        case '2' : 
        print("Masukkan Nominal : ", false);
        inputAngka(saldo, "Masukkan Nominal : ");
        user.trackEMoney.saldoAwal(saldo);
        cout<< "Saldo Awal E-Money : Rp."<<user.trackEMoney.cekSaldo();
    	sleep(2);
		 break;
		 
		 //------------------
		 case '3' : 
        print("Masukkan Nominal : ", false);
        inputAngka(saldo, "Masukkan Nominal : ");
        user.trackBank.saldoAwal(saldo);
        cout<< "Saldo Awal Bank : Rp."<<user.trackBank.cekSaldo();
    	sleep(2);
		 break;
		}
		
}
//----------------------------------------------------------------
//-----------------------------------------------------------------------
	
	//------------------CHOOSE 1
	void expense(User &user){
		switch(akun()){
			case '1' :
				char pilih;
				unsigned int nominal;
				string desk;
				system("cls");
			print(samadengan(intSpace));
			printVariabel(intSpace,"KATEGORI");
			print(samadengan(intSpace));
			printVariabel(intSpace,"Harian", "1. ");
			printVariabel(intSpace,"Sosial", "2. ");
			printVariabel(intSpace,"Pendidikan", "3. ");
			printVariabel(intSpace,"LifeStyle", "4. ");
			print(garis(intSpace));
			print("Pilih Kategori : ", false);
			
			inputChar( pilih ,"Pilih Kategori (1-4) : ");
			  switch(pilih){
			  	case '1' : 
			  	
				  print("Silahkan Masukkan Nominal : ", false);
				  inputAngka(nominal, "Silahkan masukkan angka : ");
				  print("Silahkan masukkan Deskripsi : ", false);
				  cin>>desk;
				  
				   user.trackTunai.tambahTrans(Outcome, desk, nominal, "Harian");
				   user.trackTunai.printTransOutcome();
				   	print("Tekan Apa saja Untuk Merestart ...");
 	 			   getch();
				  break;
			  }
//			 user.trackTunai.tambahTrans(Outcome, )
			break;
		}
	}
	
	
	//-------------------CHOOSE 2
	void income(User &user){
	}
	
	//--------------------CHOOSE 3
	void transfer(User &user){
	}
	
	//---------------------CHOOSE 4
	void analitik(User &user){
	  	print(samadengan(intSpace));
		printVariabel(intSpace,"ANALYTICAL");
		print(samadengan(intSpace));
		printVariabel(intSpace, "1. HISTORY");
		printVariabel(intSpace, "2. INCOME");
		printVariabel(intSpace, "3. EXPENSE");
		printVariabel(intSpace, "0. KEMBALI");
		print(garis(intSpace));
		
	}
	
//---------------------------------------------------------------------

void printMainMenu(User &user){
	
	char pilih;
	 print(samadengan(intSpace));
    printVariabel(intSpace, "MONEY TRACKER");
    print(samadengan(intSpace));
   printVariabel(intSpace, "1. EXPENSE" );   
   printVariabel(intSpace, "2. INCOME" );   
   printVariabel(intSpace, "3. TRANSFER" );   
   printVariabel(intSpace, "4. ANALYTICAL" );   
   print(garis(intSpace));
   
   
   cout<<"\nPilih : ";
   inputChar(pilih, "Pilih (1-4) : ");
   
   switch (pilih){
   	case '1' : 
   	
   	//OUTCOME
   	  expense(user);
   	  break;
   	case '2': 
   	//PEMASUKAN
   	   income(user);
		  break;
	case '3': 
		transfer(user);	
		break;
	case '4' : 
		analitik(user);
		break;
	default :    
	print("Tekan Apa saja Untuk Merestart ...");
 	   getch();
		system("cls");
	  printMainMenu(user); 
   }
}
	
	
	
	
	
int main() {
	unsigned int saldo;
	User eril;
//	    user.trackTunai.saldoAwal(1000000);
//	    user.trackEMoney.saldoAwal(1000);
//	    user.trackBank.saldoAwal(1000);
//		
//	    user.trackTunai.tambahTrans(Outcome, "tf ke mama", 2000, "testing");
////		sleep(5);
//		user.trackTunai.tambahTrans(Outcome, "dapat uang dijalan", 10000, "testing");
////		sleep(5);
//		user.trackTunai.tambahTrans(Outcome, "makan bareng ayang bro", 10000, "mamam");
//		user.trackTunai.tambahTrans(Income, "di tf ayang bro", 10000,"");
//	    user.transfer(user.trackTunai, user.trackEMoney, 1000, "Penambahan Uang");
//	    user.transfer(user.trackTunai, user.trackEMoney, 2000, " Uang");
//	    user.transfer(user.trackTunai, user.trackEMoney, 5000, " Uang");
//	    user.trackTunai.updateDataPerKategori("testing");
//	    user.trackTunai.printKategoriPerStruct();
//	
//	    user.trackTunai.tambahTrans(Income, "damn bro", 10000, "Salary");
//	    user.trackTunai.tambahTrans(Outcome, "beli wanita", 2000, "Shopping");
//	
//	    user.printSemuaTrans();
//	    cout<<"\n\n\n";
//	    user.trackTunai.printTransIncome();
   
//    printSaldoAwal(eril,saldo);
//    system("cls");
//    printMainMenu(eril);
expense(eril);

	    return 0;
}
	
