#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

const char* DIARY_FILE = "vault.dat";
const char* META_FILE = ".sys_config";
const int MAX_ATTEMPTS = 3;
const string AUTH_CHECK = "ENTRY_VALIDATED";

string processData(string data, string key) {
    string output = data;
    for (size_t i = 0; i < data.size(); i++) {
        output[i] = data[i] ^ key[i % key.length()];
    }
    return output;
}

int getFailedAttempts() {
    ifstream meta(META_FILE);
    int attempts = 0;
    if (meta >> attempts) return attempts;
    return 0;
}

void updateFailedAttempts(int count) {
    ofstream meta(META_FILE);
    meta << count;
}

void selfDestruct() {
    cout << "\n[!!!] LOCKOUT: WIPING DATA.\n";
    remove(DIARY_FILE);
    remove(META_FILE);
    exit(0);
}

void writeDiary(string key) {
    string content, line;
    cout << "\nEnter text (Type 'SAVE' to finish):\n";
    while (getline(cin, line) && line != "SAVE") {
        content += line + "\n";
    }
    string encrypted = processData(AUTH_CHECK + "\n" + content, key);
    ofstream file(DIARY_FILE, ios::binary);
    file << encrypted;
    file.close();
    updateFailedAttempts(0);
}

void readDiary(string key) {
    ifstream file(DIARY_FILE, ios::binary);
    if (!file.is_open()) return;
    string encrypted((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    string decrypted = processData(encrypted, key);
    if (decrypted.find(AUTH_CHECK) == 0) {
        updateFailedAttempts(0);
        cout << "\n--- OPEN ---\n" << decrypted.substr(AUTH_CHECK.length() + 1) << "\n--- END ---\n";
    } else {
        int attempts = getFailedAttempts() + 1;
        updateFailedAttempts(attempts);
        if (attempts >= MAX_ATTEMPTS) selfDestruct();
        cout << "\nINVALID (" << attempts << "/" << MAX_ATTEMPTS << ")\n";
    }
}

int main() {
    string key;
    int choice;
    if (getFailedAttempts() >= MAX_ATTEMPTS) selfDestruct();
    cout << "Key: ";
    cin >> key;
    while (true) {
        cout << "\n1.Write 2.Read 3.Exit: ";
        if (!(cin >> choice)) break;
        cin.ignore();
        if (choice == 1) writeDiary(key);
        else if (choice == 2) readDiary(key);
        else if (choice == 3) break;
    }
    return 0;
}
