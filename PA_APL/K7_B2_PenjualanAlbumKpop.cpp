#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string password;
    bool isAdmin;
};

struct Album {
    int id;
    string name;
    string artist;
    string releaseDate;
    string mainSong;
    int sales;
};

vector<User> users;
vector<Album> albumsOriginal; // vektor untuk menyimpan album sebelum sorting
vector<Album> albumsSortedAscending;   // vektor untuk menyimpan album setelah sorting secara ascending
vector<Album> albumsSortedDescending;   // vektor untuk menyimpan album setelah sorting secara descending
int nextAlbumId = 1;

void saveUserToFile(const User& user) {
    ofstream file("users.csv", ios::app);
    if (file.is_open()) {
        file << user.username << "," << user.password << "," << user.isAdmin << "\n";
        file.close();
    } else {
        cerr << "Gagal membuka file untuk penyimpanan.\n";
    }
}

bool isUsernameTaken(const string& username) {
    for (const auto& user : users) {
        if (user.username == username) {
            return true;
        }
    }
    return false;
}

void registerUser() {
    User newUser;
    cout << "================= R E G I S T R A S I ================" << endl;
    cout << "Masukkan Nama Pengguna: ";
    cin >> newUser.username;

    if (isUsernameTaken(newUser.username)) {
        cout << "Nama pengguna sudah digunakan. Mohon pilih nama pengguna lain.\n";
        cout << "======================================================" << endl;
        return;
    }

    cout << "Masukkan password: ";
    cin >> newUser.password;
    newUser.isAdmin = false;
    users.push_back(newUser);
    saveUserToFile(newUser);
    cout << "User registered successfully.\n";
    cout << "======================================================" << endl;
}

bool loginUser(User &loggedInUser) {
    string username, password;
    int attempt = 0;

    while (attempt < 3) {
        cout << "================= L O G I N ================" << endl;
        cout << "Masukkan Nama Pengguna: ";
        cin >> username;
        cout << "Masukkan password: ";
        cin >> password;

        for (const auto& user : users) {
            if (user.username == username && user.password == password) {
                cout << "Login successful.\n";
                loggedInUser = user;
                cout << "============================================" << endl;
                return true;
            }
        }

        if (username == "Avin" && password == "087") {
            cout << "Login successful as admin.\n";
            loggedInUser.username = username;
            loggedInUser.password = password;
            loggedInUser.isAdmin = true;
            return true;
        }

        attempt++;
        if (attempt < 3) {
            cout << "Nama Pengguna atau Password anda salah. Tersisa " << (3 - attempt) << " kali percobaan lagi.\n";
        }
    }

    cout << "Login gagal, anda mencapai batas login.\n";
    return false;
}

void addAlbum() {
    Album* newAlbum = new Album; // Using pointer
    newAlbum->id = nextAlbumId++;
    cout << "Masukkan Nama Album: "; 
    cin.ignore();
    getline(cin, newAlbum->name);
    cout << "Masukkan Nama Artis: ";
    getline(cin, newAlbum->artist);
    cout << "Masukkan Tanggal Rilis (YYYY-MM-DD): ";
    getline(cin, newAlbum->releaseDate);
    cout << "Masukkan Lagu Utama: ";
    getline(cin, newAlbum->mainSong);
    cout << "Masukkan Harga: ";
    while (!(cin >> newAlbum->sales)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan harga yang valid: ";
    }
    albumsOriginal.push_back(*newAlbum); // Dereference and push back
    delete newAlbum; // Free the allocated memory
    cout << "Album berhasil ditambahkan.\n";
}

void viewAlbums(const vector<Album>& albums) {
    cout << "================= D A F T A R  A L B U M ================" << endl;
    for (const auto& album : albums) {
        cout << "ID: " << album.id << endl;
        cout << "Nama Album: " << album.name << endl;
        cout << "Nama Artis: " << album.artist << endl;
        cout << "Tanggal Rilis: " << album.releaseDate << endl;
        cout << "Lagu Utama: " << album.mainSong << endl;
        cout << "Harga: " << album.sales << endl;
        cout << "--------------------------------------------------------" << endl;
    }
    if (albums.empty()) {
        cout << "Belum ada album yang ditambahkan.\n";
    }
}

void sortByPriceAscending(vector<Album>& albums) {
    sort(albums.begin(), albums.end(), [](const Album& a, const Album& b) {
        return a.sales < b.sales;
    });
}

void sortByPriceDescending(vector<Album>& albums) {
    sort(albums.begin(), albums.end(), [](const Album& a, const Album& b) {
        return a.sales > b.sales;
    });
}

void initializeSortedAlbumsAscending() {
    albumsSortedAscending = albumsOriginal;
}

void initializeSortedAlbumsDescending() {
    albumsSortedDescending = albumsOriginal;
}

void viewAlbumsSortedBySales(bool ascending) {
    if (ascending) {
        initializeSortedAlbumsAscending();
        sortByPriceAscending(albumsSortedAscending);
        viewAlbums(albumsSortedAscending);
    } else {
        initializeSortedAlbumsDescending();
        sortByPriceDescending(albumsSortedDescending);
        viewAlbums(albumsSortedDescending);
    }
}

void updateAlbum() {
    int id;
    cout << "Masukkan ID album yang ingin diperbarui: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan ID yang valid: ";
    }
    for (auto& album : albumsOriginal) {
        if (album.id == id) {
            Album* updatePtr = &album; // Using pointer
            cout << "Masukkan Nama Album baru: ";
            cin.ignore();
            getline(cin, updatePtr->name);
            cout << "Masukkan Nama Artis baru: ";
            getline(cin, updatePtr->artist);
            cout << "Masukkan Tanggal Rilis baru (YYYY-MM-DD): ";
            getline(cin, updatePtr->releaseDate);
            cout << "Masukkan Lagu Utama baru: ";
            getline(cin, updatePtr->mainSong);
            cout << "Masukkan Harga baru: ";
            while (!(cin >> updatePtr->sales)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Input tidak valid. Masukkan harga yang valid: ";
            }
            cout << "Album berhasil diperbarui.\n";
            return;
        }
    }
    cout << "Album dengan ID tersebut tidak ditemukan.\n";
}


void deleteAlbum() {
    int id;
    cout << "Masukkan ID album yang ingin dihapus: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan ID yang valid: ";
    }
    for (auto it = albumsOriginal.begin(); it != albumsOriginal.end(); ++it) {
        if (it->id == id) {
            Album* deletePtr = &(*it); // Using pointer
            albumsOriginal.erase(it);
            cout << "Album berhasil dihapus.\n";
            return;
        }
    }
    cout << "Album dengan ID tersebut tidak ditemukan.\n";
}


void buyAlbum() {
    int id;
    cout << "================= D A F T A R  A L B U M ================" << endl;
    for (const auto& album : albumsOriginal) {
        cout << "ID: " << album.id << endl;
        cout << "Nama Album: " << album.name << endl;
        cout << "Nama Artis: " << album.artist << endl;
        cout << "Tanggal Rilis: " << album.releaseDate << endl;
        cout << "Lagu Utama: " << album.mainSong << endl;
        cout << "Harga: " << album.sales << endl;
        cout << "--------------------------------------------------------" << endl;
    }
    cout << "Masukkan ID album yang ingin dibeli: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan ID yang valid: ";
    }
    for (auto& album : albumsOriginal) {
        if (album.id == id) {
            Album* buyPtr = &album; // Using pointer
            buyPtr->sales++;
            cout << "Anda telah membeli album " << buyPtr->name << " oleh " << buyPtr->artist << ".\n";
            return;
        }
    }
    cout << "Album dengan ID tersebut tidak ditemukan.\n";
}


int binarySearchAlbumRec(const vector<Album>& albums, int left, int right, int id) {
    if (left > right) {
        return -1; // Basis: Album tidak ditemukan
    }

    int mid = left + (right - left) / 2;

    if (albums[mid].id == id) {
        return mid; // Basis: Album ditemukan
    } else if (albums[mid].id < id) {
        return binarySearchAlbumRec(albums, mid + 1, right, id); // Rekurens: Pencarian di bagian kanan
    } else {
        return binarySearchAlbumRec(albums, left, mid - 1, id); // Rekurens: Pencarian di bagian kiri
    }
}


void searchAlbum() {
    int id;
    cout << "Masukkan ID album yang ingin dicari: ";
    while (!(cin >> id)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input tidak valid. Masukkan ID yang valid: ";
    }

    // Pastikan albumsOriginal diurutkan sebelum mencari
    sort(albumsOriginal.begin(), albumsOriginal.end(), [](const Album& a, const Album& b) {
        return a.id < b.id;
    });

    int index = binarySearchAlbumRec(albumsOriginal, 0, albumsOriginal.size() - 1, id);
    if (index != -1) {
        const auto& album = albumsOriginal[index];
        cout << "ID: " << album.id << endl;
        cout << "Nama Album: " << album.name << endl;
        cout << "Nama Artis: " << album.artist << endl;
        cout << "Tanggal Rilis: " << album.releaseDate << endl;
        cout << "Lagu Utama: " << album.mainSong << endl;
        cout << "Harga: " << album.sales << endl;
    } else {
        cout << "Album dengan ID tersebut tidak ditemukan.\n";
    }
}

// Panggil fungsi untuk menyortir albumsOriginal sebelum memulai binary search
void initializeSortedAlbumsOriginal() {
    sort(albumsOriginal.begin(), albumsOriginal.end(), [](const Album& a, const Album& b) {
        return a.id < b.id;
    });
}

// Pastikan memanggil fungsi initializeSortedAlbumsOriginal() di bagian yang relevan

void adminMenu() {
    int choice;
    while (true) {
        cout << "================= M E N U  A D M I N ================" << endl;
        cout << "1. Tambah Album" << endl;
        cout << "2. Lihat Album (Sorting berdasarkan Harga Ascending)" << endl;
        cout << "3. Lihat Album (Sorting berdasarkan Harga Descending)" << endl;
        cout << "4. Perbarui Album" << endl;
        cout << "5. Hapus Album" << endl;
        cout << "6. Cari Album" << endl;
        cout << "7. Logout" << endl;
        cout << "====================================================" << endl;
        cout << "Masukkan Pilihan: ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan pilihan yang valid: ";
        }

        switch (choice) {
            case 1:
                addAlbum();
                break;
            case 2:
                viewAlbumsSortedBySales(true); // Tampilkan daftar album yang sudah diurutkan berdasarkan penjualan secara ascending
                break;
            case 3:
                viewAlbumsSortedBySales(false); // Tampilkan daftar album yang sudah diurutkan berdasarkan penjualan secara descending
                break;
            case 4:
                updateAlbum();
                break;
            case 5:
                deleteAlbum();
                break;
            case 6:
                searchAlbum();
                break;
            case 7:
                cout << "Logging out...\n";
                return;
            default:
                cout << "Pilihan tidak ditemukan, coba lagi.\n";
        }
    }
}

void buyerMenu() {
    int choice;
    while (true) {
        cout << "================= M E N U  P E M B E L I ================" << endl;
        cout << "1. Lihat Album (Sorting berdasarkan Harga Ascending)" << endl;
        cout << "2. Lihat Album (Sorting berdasarkan Harga Descending)" << endl;
        cout << "3. Beli Album" << endl;
        cout << "4. Cari Album" << endl;
        cout << "5. Logout" << endl;
        cout << "=========================================================" << endl;
        cout << "Masukkan Pilihan: ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan pilihan yang valid: ";
        }

        switch (choice) {
            case 1:
                viewAlbumsSortedBySales(true); // Tampilkan daftar album yang sudah diurutkan berdasarkan penjualan secara ascending
                break;
            case 2:
                viewAlbumsSortedBySales(false); // Tampilkan daftar album yang sudah diurutkan berdasarkan penjualan secara descending
                break;
            case 3:
                buyAlbum();
                break;
            case 4:
                searchAlbum();
                break;
            case 5:
                cout << "Logging out.\n";
                return;
            default:
                cout << "Pilihan tidak ditemukan, coba lagi.\n";
        }
    }
}

int main() {
    // Inisialisasi album
    albumsOriginal.push_back({1, "DREAMSCAPE", "NCT DREAM", "2024-03-25", "SMOOTHIE", 30000});
    albumsOriginal.push_back({2, "FACT CHECK", "NCT 127", "2023-10-06", "FACT CHECK", 25000});
    albumsOriginal.push_back({3, "KICK BACK", "WAYV", "2021-03-10", "KICK BACK", 20000});
    albumsOriginal.push_back({4, "REBOOT", "TREASURE", "2023-07-28", "BONA BONA", 30000});
    albumsOriginal.push_back({5, "DRAMA", "AESPA", "2023-11-10", "DRAMA", 25000});
    albumsOriginal.push_back({6, "GET UP", "NEW JEANS", "2023-07-21", "SUPER SHY", 25000});
    albumsOriginal.push_back({7, "WHAT IS LOVE?", "TWICE", "2018-04-09", "WHAT IS LOVE", 10000});
    albumsOriginal.push_back({8, "BORN PINK", "BLACKPINK", "2022-09-16", "PINK VENOM", 20000});
    nextAlbumId = 9;  // Pastikan ID album berikutnya dimulai dengan benar

    // Sort albumsOriginal untuk binary search
    initializeSortedAlbumsOriginal();

    int choice;
    User loggedInUser;

    while (true) {
        cout << "================= W E L C O M E ================" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "================================================" << endl;

        cout << "Masukkan Pilihan: ";
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Masukkan pilihan yang valid (Masukkan angka 1-3): ";
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(loggedInUser)) {
                    if (loggedInUser.isAdmin) {
                        adminMenu();
                    } else {
                        buyerMenu();
                    }
                }
                break;
            case 3:
                cout << "Exiting program...\n";
                return 0;
            default:
                cout << "Pilihan tidak ditemukan, coba lagi.\n";
        }
    }

    return 0;
}
