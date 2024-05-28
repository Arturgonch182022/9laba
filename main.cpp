#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;

struct WeatherData {
    int day;
    int month;
    double precipitation;
    string type;
};

vector<WeatherData> readDataFromFile(const string& filename) {
    vector<WeatherData> data;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            WeatherData entry;
            iss >> entry.day >> entry.month >> entry.precipitation >> entry.type;
            data.push_back(entry);
        }
        file.close();
    }
    return data;
}
vector<WeatherData> findRainyDays(const vector<WeatherData>& data) {
    vector<WeatherData> rainyDays;
    for (const auto& entry : data) {
        if (entry.type == "дождь") {
            rainyDays.push_back(entry);
        }
    }
    return rainyDays;
}
vector<WeatherData> findLightRain(const vector<WeatherData>& data) {
    vector<WeatherData> lightRain;
    for (const auto& entry : data) {
        if (entry.precipitation < 1.5) {
            lightRain.push_back(entry);
        }
    }
    return lightRain;
}
void shakerSort(vector<WeatherData>& data, bool (*compare)(const WeatherData&, const WeatherData&)) {
    int left = 0, right = data.size() - 1;
    while (left <= right) {
        for (int i = left; i < right; ++i) {
            if (compare(data[i], data[i + 1])) {
                swap(data[i], data[i + 1]);
            }
        }
        right--;
        for (int i = right; i > left; --i) {
            if (compare(data[i - 1], data[i])) {
                swap(data[i - 1], data[i]);
            }
        }
        left++;
    }
}
void quickSort(vector<WeatherData>& data, int left, int right, bool (*compare)(const WeatherData&, const WeatherData&)) {
    int i = left, j = right;
    WeatherData pivot = data[(left + right) / 2];
    while (i <= j) {
        while (compare(data[i], pivot)) i++;
        while (compare(pivot, data[j])) j--;
        if (i <= j) {
            swap(data[i], data[j]);
            i++;
            j--;
        }
    }
    if (left < j) quickSort(data, left, j, compare);
    if (i < right) quickSort(data, i, right, compare);
}
bool compareByPrecipitation(const WeatherData& a, const WeatherData& b) {
    return a.precipitation < b.precipitation;
}

bool compareByTypeAndDate(const WeatherData& a, const WeatherData& b) {
    if (a.type == b.type) {
        if (a.month == b.month) {
            return a.day < b.day;
        }
        return a.month < b.month;
    }
    return a.type < b.type;
}
int main() {
    string filename = "3weather.txt";
    vector<WeatherData> data = readDataFromFile(filename);

    cout << "Дни с дождем:" << endl;
    vector<WeatherData> rainyDays = findRainyDays(data);
    for (const auto& entry : rainyDays) {
        cout << entry.day << "." << entry.month << " - " << entry.precipitation << " мм, " << entry.type << endl;
    }

    cout << "\nДни с осадками меньше 1.5 мм:" << endl;
    vector<WeatherData> lightRain = findLightRain(data);
    for (const auto& entry : lightRain) {
        cout << entry.day << "." << entry.month << " - " << entry.precipitation << " мм, " << entry.type << endl;
    }

    cout << "\nСортировка по возрастанию количества осадков:" << endl;
    vector<WeatherData> sortedByPrecipitation = data;
    shakerSort(sortedByPrecipitation, compareByPrecipitation);
    for (const auto& entry : sortedByPrecipitation) {
        cout << entry.day << "." << entry.month << " - " << entry.precipitation << " мм, " << entry.type << endl;
    }

    cout << "\nСортировка по возрастанию характеристики, а в рамках одной характеристики по возрастанию номера месяца, а в рамках одного месяца по возрастанию номера дня:" << std::endl;
    vector<WeatherData> sortedByTypeAndDate = data;
    quickSort(sortedByTypeAndDate, 0, sortedByTypeAndDate.size() - 1, compareByTypeAndDate);
    for (const auto& entry : sortedByTypeAndDate) {
        cout << entry.day << "." << entry.month << " - " << entry.precipitation << " мм, " << entry.type << endl;
    }

    return 0;
}