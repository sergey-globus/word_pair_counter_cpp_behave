#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <windows.h>
#include <cstdlib>   // ��� atoi()

using namespace std;

// ������� ��� ����������� ���������� � ������� �������� (������� ������� �����)
string to_lower_win(const string& s) {
    string result;
    for (unsigned char c : s) {
        // ������� ��������� ����� � CP1251
        if (c >= 192 && c <= 223) {
            result += c + 32; // ����������� � ��������
        }
        // ������� �������� ��� � ���������� ��������
        else if (c >= 224 && c <= 255) {
            result += c;
        }
        // ��������� �����
        else {
            result += tolower(c);
        }
    }
    return result;
}

// ��������� ���� �� �����, ��� word1 � word2 ������� ����� �� �������, ���������� ���������������� ��� ������ � ���������
void index_words(ifstream& file, const string& word1, const string& word2, vector<int> *pos_word1, vector<int> *pos_word2) {
    string currentWord;
    
    // ����������� ����� ��� ���������
    string lower_word1 = to_lower_win(word1);
    string lower_word2 = to_lower_win(word2);

    char c;
    int counter = 0;
    while (file.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);
        
        // ���� ������ - ����� (��������� ��� ������� � CP1251)
        if (isalpha(c) || (uc >= 192 && uc <= 255)) {
            currentWord += to_lower_win(string(1, c));
        } 
        // ���� ��������� ����������� � ������� ����� �� ������
        else if (!currentWord.empty()) {
            if (currentWord == lower_word1) {
                pos_word1->push_back(counter);
                // cout << word1 << " ������� �� ������� " << counter << "\n";
            }
            if (currentWord == lower_word2) {
                pos_word2->push_back(counter);
                // cout << word2 << " ������� �� ������� " << counter << "\n";
            }
            
            currentWord.clear();
            counter++;
        }
    }

    // ��������� ���������� �����
    if (!currentWord.empty()) {
        if (currentWord == lower_word1) {
            pos_word1->push_back(counter);
            // cout << word1 << " ������� �� ������� " << counter << "\n";
        }
        if (currentWord == lower_word2) {
            pos_word2->push_back(counter);
            // cout << word2 << " ������� �� ������� " << counter << "\n";
        }
    }
}

int count_unique_pairs(vector<int>& pos_word1, vector<int>& pos_word2, int max_distance) {
    int count = 0;
    int i = 0, j = 0;
    int n = pos_word1.size(), m = pos_word2.size();
    
    while (i < n && j < m) {
        if (pos_word1[i] < pos_word2[j]) {
            // ��������� ����������
            if (pos_word2[j] - pos_word1[i] - 1 <= max_distance) {
                count++;  // ����� ���� (i, j)
                i++;      // ��������� � ���������� word1
                j++;      // ��������� � ���������� word2
            } else {
                i++;      // ���������� ������� �������, ������� ��������� word1
            }
        } else {
            j++;          // word2 ������� ����, ������� ��������� word2
        }
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    // ������������� ��������� ������� Windows
    SetConsoleOutputCP(1251);


    if (argc < 5) {
        cerr << "�������������: " << argv[0] << " ���� �����1 �����2 ������������_����������\n";
        SetConsoleOutputCP(866);
        return 1;
    }

    ifstream file(argv[1], ios::binary);
    if (!file) {
        cerr << "������: �� ������� ������� ����" << argv[1] << "'\n";
        SetConsoleOutputCP(866);
        return 2;
    }

    // �������� ��� argv[4] - �����
    try {
        int max_diff = stoi(argv[4]);
        if (max_diff < 0) throw invalid_argument("must be positive");
    } catch (...) {
        cerr << "������: ������������_���������� ������ ���� ����� ������������� ������\n";
        SetConsoleOutputCP(866);
        return 3;
    }
    int max_diff = stoi(argv[4]);

    vector<int> pos_word1, pos_word2; // ������� ���� (����� �� ����� ����� � �����)

    index_words(file, argv[2], argv[3], &pos_word1, &pos_word2);

    int result = count_unique_pairs(pos_word1, pos_word2, max_diff);

    cout << "���������� ��������� ���: " << result << "\n";

    // ������������� �������� ��������� ������� Windows
    SetConsoleOutputCP(866);
    return 0;
}