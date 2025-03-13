#include <iostream>
#include "meters.h"

using namespace std;

int main() {
    loadDataFromFile();
    printMeters();

    while (true) {
        string meter_id;
        cout << "Введіть ID лічильника для оновлення (або 'exit' для завершення): ";
        cin >> meter_id;

        if (meter_id == "exit") {
            break; 
        }

        if (meters.find(meter_id) != meters.end()) {
            double new_day, new_night;
            cout << "Введіть нові денні показники: ";
            cin >> new_day;
            cout << "Введіть нові нічні показники: ";
            cin >> new_night;

            updateMeter(meter_id, new_day, new_night);
            cout << "Лічильник успішно оновлено" << endl;
        } else {
            cout << "Лічильник не знайдено!" << endl;
        }
    }

    printMeters();
    saveDataToFile();
    return 0;
}
