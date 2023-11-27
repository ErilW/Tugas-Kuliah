#include <iostream>
#include <sstream>
#include <unistd.h>
#include <conio.h>
#define WIDTH 100
int intSpace = 75;
#define ID eril
#define PASS password
using namespace std;
// Variable global
// char * judulHome = "MONEY TRACKER";

string k [WIDTH] = {
"Makanan dan Minuman", // 0
"Transportasi", // 1
"Pakaian", // 2
"Pendidikan" ,// 3
 "Kebutuhan Rumah",// 4
  "Kesehatan",// 5
   "Hiburan", // 6
   "Biaya Lainnya"};// 7

string IntToString (unsigned int a)
{
    stringstream temp;
    temp<<a;
    return temp.str();
}

// Fungsi garis
string garis(int jumlahGaris) {
    string dataGaris;
    for (int i = 0; i < jumlahGaris; i++) {
        dataGaris += "-";
    }
    return dataGaris;
}

// Fungsi space
string space(int jumlahSpace) {
    string dataSpace;
    for (int i = 0; i < jumlahSpace; i++) {
        dataSpace += " ";
    }
    return dataSpace;
}

string samadengan(int jumlahsamadengan) {
    string datasamadengan;
    for (int i = 0; i < jumlahsamadengan; i++) {
        datasamadengan += "=";
    }
    return datasamadengan;
}


// Fungsi print
void print(string printString , bool a = true) {
    if(a){
	cout << printString << "\n";
	}else {
	cout << printString;
	}
}

void print(int printInt, bool a = true) {
	if (a){
    cout << printInt << "\n";
	}else{
    cout << printInt;
	}
}



// Fungsi printVariabel
void printVariabel(int jumlah, const std::string& nama , const string& panah = "", const string& spaceparam = "") {
	
    int spasiSebelum = (jumlah) / 2.75 - panah.length() - spaceparam.length();
    int spasiSesudah = jumlah - spasiSebelum - nama.length() -2 - panah.length() - spaceparam.length();;
    cout << "|"<< space(spasiSebelum) << panah << spaceparam  << nama   <<space(spasiSesudah) << "|"<< "\n"  ;
}


void printVariabela(int jumlah, const std::string& nama , const string& panah = "", const string& spaceparam = "") {
	
    int spasiSebelum = (jumlah) / 2.75 - panah.length() - spaceparam.length();
    int spasiSesudah = jumlah - spasiSebelum - nama.length() -2 - panah.length() - spaceparam.length();;
    cout << "|"<< space(spasiSebelum)  << nama << panah   <<space(spasiSesudah) << "|"<< "\n"  ;
}


// fungsi checking
void inputAngka(unsigned long long &a, string stringSalah = "") {
    string input;
    while (true) {
        cin >> input;
        istringstream stream(input);
        if (stream >> a) {
            break;
        } else {
            cerr << stringSalah;
        }
    }
}

void inputChar(char &a, const string stringSalah = "") {
    string input;
    while (true) {
        cin >> input;
        
        if (input.length() == 1) {
        a = input[0];
            break;
        } else {
            cerr << stringSalah;
        }
    }
}


void loading(){
	
cout << "\r" << "Loading" << flush;
sleep(1);
cout  << "." << flush;    
sleep(1);
cout  << "." << flush;   
sleep(1);
cout  << "." << flush;     
sleep(1);

}



//-----------------------------------------------------------
string getPassword() {
    char password[WIDTH]; // Menyimpan karakter yang diketikkan oleh pengguna
    int i = 0;

    cout << "Pass : ";

    char ch;
    do {
        ch = _getch(); // Menggunakan _getch() untuk membaca karakter tanpa menampilkan
        if (ch == '\b') { // Karakter Backspace
            if (i > 0) {
                // Hapus karakter sebelumnya dari tampilan dan dari penyimpanan
                cout << "\b \b";
                --i;
            }
        } else if (ch != '\r' && ch != '\n') {
            // Menampilkan karakter "*" dan menyimpan karakter ke dalam array
            password[i] = ch;
            ++i;
            cout << '*';
        }
    } while (ch != '\r' && ch != '\n');

    password[i] = '\0'; // Menambahkan null terminator ke akhir string

    // Mengonversi char array ke string dan mengembalikannya
    return string(password);
}


template <size_t N>
int len(const string (&array)[N]) {
    return N;
}



