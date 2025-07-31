#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <windows.h>
#include <cstdlib>   // для atoi()

using namespace std;

// Функция для корректного приведения к нижнему регистру (включая русские буквы)
string to_lower_win(const string& s) {
    string result;
    for (unsigned char c : s) {
        // Русские заглавные буквы в CP1251
        if (c >= 192 && c <= 223) {
            result += c + 32; // Преобразуем в строчные
        }
        // Русские строчные уже в правильном регистре
        else if (c >= 224 && c <= 255) {
            result += c;
        }
        // Латинские буквы
        else {
            result += tolower(c);
        }
    }
    return result;
}

// Разделяем файл на слова, для word1 и word2 считаем какие по порядку, возвращаем соответвствующие два списка с позициями
void index_words(ifstream& file, const string& word1, const string& word2, vector<int> *pos_word1, vector<int> *pos_word2) {
    string currentWord;
    
    // Нормализуем слова для сравнения
    string lower_word1 = to_lower_win(word1);
    string lower_word2 = to_lower_win(word2);

    char c;
    int counter = 0;
    while (file.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);
        
        // Если символ - буква (латинская или русская в CP1251)
        if (isalpha(c) || (uc >= 192 && uc <= 255)) {
            currentWord += to_lower_win(string(1, c));
        } 
        // Если встретили разделитель и текущее слово не пустое
        else if (!currentWord.empty()) {
            if (currentWord == lower_word1) {
                pos_word1->push_back(counter);
                // cout << word1 << " найдено на позиции " << counter << "\n";
            }
            if (currentWord == lower_word2) {
                pos_word2->push_back(counter);
                // cout << word2 << " найдено на позиции " << counter << "\n";
            }
            
            currentWord.clear();
            counter++;
        }
    }

    // Обработка последнего слова
    if (!currentWord.empty()) {
        if (currentWord == lower_word1) {
            pos_word1->push_back(counter);
            // cout << word1 << " найдено на позиции " << counter << "\n";
        }
        if (currentWord == lower_word2) {
            pos_word2->push_back(counter);
            // cout << word2 << " найдено на позиции " << counter << "\n";
        }
    }
}

int count_unique_pairs(vector<int>& pos_word1, vector<int>& pos_word2, int max_distance) {
    int count = 0;
    int i = 0, j = 0;
    int n = pos_word1.size(), m = pos_word2.size();
    
    while (i < n && j < m) {
        if (pos_word1[i] < pos_word2[j]) {
            // Проверяем расстояние
            if (pos_word2[j] - pos_word1[i] - 1 <= max_distance) {
                count++;  // Нашли пару (i, j)
                i++;      // Переходим к следующему word1
                j++;      // Переходим к следующему word2
            } else {
                i++;      // Расстояние слишком большое, пробуем следующий word1
            }
        } else {
            j++;          // word2 слишком рано, пробуем следующий word2
        }
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    // Устанавливаем кодировку консоли Windows
    SetConsoleOutputCP(1251);


    if (argc < 5) {
        cerr << "Использование: " << argv[0] << " файл слово1 слово2 максимальное_расстояние\n";
        SetConsoleOutputCP(866);
        return 1;
    }

    ifstream file(argv[1], ios::binary);
    if (!file) {
        cerr << "Ошибка: не удалось открыть файл" << argv[1] << "'\n";
        SetConsoleOutputCP(866);
        return 2;
    }

    // Проверка что argv[4] - число
    try {
        int max_diff = stoi(argv[4]);
        if (max_diff < 0) throw invalid_argument("must be positive");
    } catch (...) {
        cerr << "Ошибка: максимальное_расстояние должно быть целым положительным числом\n";
        SetConsoleOutputCP(866);
        return 3;
    }
    int max_diff = stoi(argv[4]);

    vector<int> pos_word1, pos_word2; // позиции слов (какие по счету слова в файле)

    index_words(file, argv[2], argv[3], &pos_word1, &pos_word2);

    int result = count_unique_pairs(pos_word1, pos_word2, max_diff);

    cout << "Количество найденных пар: " << result << "\n";

    // Устанавливаем исходную кодировку консоли Windows
    SetConsoleOutputCP(866);
    return 0;
}
