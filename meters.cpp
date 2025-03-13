#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "meters.h"

using namespace std;

const double DAY_RATE = 1.5; 
const double NIGHT_RATE = 0.8; 
const double DAY_CORRECTION = 100; 
const double NIGHT_CORRECTION = 80; 

unordered_map<string, unordered_map<string, double>> meters;
const string FILE_NAME = "meters.txt";

void loadDataFromFile() {
    ifstream file(FILE_NAME);
    if (!file.is_open()) {
        cerr << "Увага: Не вдалося відкрити файл. Починаємо з порожніх даних." << endl;
        return;
    }

    string line;
    unordered_map<string, string> last_entries;

    while (getline(file, line)) {
        istringstream iss(line);
        string meter_id;
        double prev_day, prev_night, curr_day, curr_night;
        
        if (iss >> meter_id >> prev_day >> prev_night >> curr_day >> curr_night) {
            last_entries[meter_id] = line; 
        }
    }

    file.close();

    for (const auto& entry : last_entries) {
        istringstream iss(entry.second);
        string meter_id;
        double prev_day, prev_night, curr_day, curr_night;

        if (iss >> meter_id >> prev_day >> prev_night >> curr_day >> curr_night) {
            meters[meter_id] = {
                {"prev_day", prev_day},
                {"prev_night", prev_night},
                {"curr_day", curr_day},
                {"curr_night", curr_night}
            };
        }
    }
}

void updateMeter(const string& meter_id, double new_day, double new_night) {
    if (meters.find(meter_id) == meters.end()) {
        meters[meter_id] = {{"prev_day", 0}, {"prev_night", 0}, {"curr_day", new_day}, {"curr_night", new_night}};
    } else {
        auto& meter = meters[meter_id];

        bool applyDayCorrection = new_day < meter["curr_day"];
        bool applyNightCorrection = new_night < meter["curr_night"];

        if (applyDayCorrection || applyNightCorrection) {
            cout << "Увага: Нові показники менші за попередні!" << endl;

            if (applyDayCorrection && applyNightCorrection) {
                cout << "Якщо ви підтвердите, система додасть " << DAY_CORRECTION 
                     << " кВт·год для дня і " << NIGHT_CORRECTION << " кВт·год для ночі." << endl;
            } else if (applyDayCorrection) {
                cout << "Якщо ви підтвердите, система додасть " << DAY_CORRECTION 
                     << " кВт·год для дня." << endl;
            } else if (applyNightCorrection) {
                cout << "Якщо ви підтвердите, система додасть " << NIGHT_CORRECTION 
                     << " кВт·год для ночі." << endl;
            }

            cout << "Бажаєте продовжити? (y/n): ";
            char choice;
            cin >> choice;
            if (choice != 'y' && choice != 'Y') {
                return;
            }
        }

        if (applyDayCorrection) {
            new_day = meter["curr_day"] + DAY_CORRECTION;
        }
        if (applyNightCorrection) {
            new_night = meter["curr_night"] + NIGHT_CORRECTION;
        }

        meter["prev_day"] = meter["curr_day"];
        meter["prev_night"] = meter["curr_night"];
        meter["curr_day"] = new_day;
        meter["curr_night"] = new_night;
    }
}

double calculateBill(const string& meter_id) {
    if (meters.find(meter_id) == meters.end()) return 0;
    auto& meter = meters[meter_id];
    double used_day = meter["curr_day"] - meter["prev_day"];
    double used_night = meter["curr_night"] - meter["prev_night"];
    return used_day * DAY_RATE + used_night * NIGHT_RATE;
}

void saveDataToFile() {
    ofstream file(FILE_NAME, ios::app); 
    if (!file.is_open()) {
        cerr << "Помилка: Не вдалося зберегти дані у файл!" << endl;
        return;
    }

    for (const auto& pair : meters) {
        file << pair.first << " "
             << pair.second.at("prev_day") << " "
             << pair.second.at("prev_night") << " "
             << pair.second.at("curr_day") << " "
             << pair.second.at("curr_night") << "\n";
    }

    file.close();
}

void printMeters() {
    for (const auto& pair : meters) {
        const string& id = pair.first;
        const auto& meter = pair.second;

        double used_day = meter.at("curr_day") - meter.at("prev_day");
        double used_night = meter.at("curr_night") - meter.at("prev_night");
        double bill = calculateBill(id);

        cout << "Лічильник ID: " << id
             << " | Поточний день: " << meter.at("curr_day")
             << " | Поточна ніч: " << meter.at("curr_night") << "\n"
             << "                | Використано вдень: " << used_day
             << " | Використано вночі: " << used_night
             << " | Рахунок: $" << bill << "\n";
    }
}
