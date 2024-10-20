#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;
using namespace chrono;

int getRandomNumber(int min, int max) {
    return rand() % (max - min) + min;
}

// Функция для сортировки методом пузырька
void bubbleSort(vector<int>& arr) {
    int N = arr.size() - 1;   // Количество элементов
    int BOUND = N;        // Изначально BOUND равен N
    int t, temp;
    
    while (BOUND > 1) {   // Пока BOUND не уменьшится до 1
        t = 0;  // Изначально предполагаем, что обменов не будет
        
        for (int j = 0; j < BOUND - 1; ++j) {
            // Если текущий элемент больше следующего, меняем их местами
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                t = j;  // Запоминаем индекс последнего произведенного обмена
            }
        }
        
        // Если не было произведено ни одного обмена, завершаем
        if (t == 0) {
            break;
        }
        
        // Устанавливаем BOUND на позицию последнего произведенного обмена
        BOUND = t;
    }
}

// Функция для сортировки вставками
void sortS(int* arK, int N) {
    for (int j = 1; j < N; ++j) {
        int K = arK[j]; // элемент для вставки;
        int i = j - 1;  // номер последнего элемента сортированной части массива;
        while (i >= 0 && arK[i] > K) {
            arK[i + 1] = arK[i];  // выполнить сдвиг элемента вправо;
            --i;
        }
        if (++i < j) {
            arK[i] = K;  // вставить элемент;
        }
    }
}

// Функция для вывода массива в файл
void printArrayToFile(const vector<int>& arr, const string& filename) {
    ofstream outFile(filename);
    for (int num : arr) {
        outFile << num << " ";
    }
    outFile << endl;
    outFile.close();
}

// Функция для вывода времени в нужном формате
void printDuration(microseconds duration) {
    milliseconds millisec = duration_cast<milliseconds>(duration); // Получаем время в миллисекундах
    int microsec = duration.count() % 1000; // Остаток времени в микросекундах

    cout << millisec.count() 
         << setfill('0') << setw(3) << microsec
         << " microsec" << endl;
}

// Функция сравнения для qsort
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел

    vector<int> sizes = {16, 100, 500, 1000, 5000}; // Размеры массивов для теста

    // Цикл по разным размерам массивов
    for (int arraySize : sizes) {
        vector<int> arr(arraySize); // Инициализация вектора с размером arraySize
        vector<int> arrCopy(arraySize); // Копия для qsort
        vector<int> arrCopyS(arraySize); // Копия для sortS

        // Генерация случайных чисел в зависимости от размера массива
        if (arraySize < 500) {
            for (int i = 0; i < arraySize; i++) {
                arr[i] = getRandomNumber(100, 1000); // диапазон [100, 1000)
            }
        } else {
            for (int i = 0; i < arraySize; i++) {
                arr[i] = getRandomNumber(1000, 10000); // диапазон [1000, 10000)
            }
        }

        // Копируем массив для qsort и sortS
        arrCopy = arr;
        arrCopyS = arr;

        // Вывод несортированного массива в файл
        string originalFile = "f" + to_string(arraySize) + ".txt";
        printArrayToFile(arr, originalFile);

        // Замер времени сортировки методом пузырька
        high_resolution_clock::time_point startBubble = high_resolution_clock::now();
        bubbleSort(arr);
        high_resolution_clock::time_point stopBubble = high_resolution_clock::now();

        // Подсчет времени выполнения сортировки пузырьком
        microseconds durationBubble = duration_cast<microseconds>(stopBubble - startBubble);
        cout << "Bubble sort time for " << arraySize << " elements: ";
        printDuration(durationBubble);

        // Вывод отсортированного массива в файл
        string sortedFile = "f" + to_string(arraySize) + "-s" + ".txt";
        printArrayToFile(arr, sortedFile);

        // Замер времени стандартной функции qsort
        high_resolution_clock::time_point startQSort = high_resolution_clock::now();
        qsort(arrCopy.data(), arrCopy.size(), sizeof(int), compare);
        high_resolution_clock::time_point stopQSort = high_resolution_clock::now();

        // Подсчет времени выполнения qsort
        microseconds durationQSort = duration_cast<microseconds>(stopQSort - startQSort);
        cout << "qsort time for " << arraySize << " elements: ";
        printDuration(durationQSort);

        // Замер времени сортировки методом вставки (sortS)
        high_resolution_clock::time_point startSortS = high_resolution_clock::now();
        sortS(arrCopyS.data(), arrCopyS.size());
        high_resolution_clock::time_point stopSortS = high_resolution_clock::now();

        // Подсчет времени выполнения сортировки вставками
        microseconds durationSortS = duration_cast<microseconds>(stopSortS - startSortS);
        cout << "Insertion sort (sortS) time for " << arraySize << " elements: ";
        printDuration(durationSortS);

        cout << "---------------------------------------------------" << endl;
    }

    return 0;
}
