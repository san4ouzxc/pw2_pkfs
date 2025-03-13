#include <iostream>
#include <cassert>
#include <fstream>
#include "meters.h"

using namespace std;

void resetFile() {
    ofstream file("meters.txt");
    file << "A 0 0 100 50\n";  
    file << "B 0 0 50 30\n";  
    file.close();
}

void testUpdateExistingMeter() {
    resetFile();
    loadDataFromFile();
    updateMeter("A", 560, 330);
    assert(meters["A"]["curr_day"] == 560);
    assert(meters["A"]["curr_night"] == 330);
    cout << "[OK] testUpdateExistingMeter пройдено\n";
}

void testNewMeter() {
    resetFile();
    loadDataFromFile();
    updateMeter("C", 100, 80);  
    assert(meters["C"]["curr_day"] == 100);
    assert(meters["C"]["curr_night"] == 80);
    cout << "[OK] testNewMeter пройдено\n";
}

void testNightCorrection() {
    resetFile();
    loadDataFromFile();
    
    double old_night = meters["A"]["curr_night"]; 
    updateMeter("A", 560, 30); 

    assert(meters["A"]["curr_night"] == old_night + NIGHT_CORRECTION);
    cout << "[OK] testNightCorrection пройдено\n";
}

void testDayCorrection() {
    resetFile();
    loadDataFromFile();

    double old_day = meters["A"]["curr_day"]; 
    updateMeter("A", 40, 330); 

    assert(meters["A"]["curr_day"] == old_day + DAY_CORRECTION);
    cout << "[OK] testDayCorrection пройдено\n";
}

void testBothCorrections() {
    resetFile();
    loadDataFromFile();

    double old_day = meters["A"]["curr_day"];   
    double old_night = meters["A"]["curr_night"];

    updateMeter("A", 40, 30); 

    assert(meters["A"]["curr_day"] == old_day + DAY_CORRECTION);
    assert(meters["A"]["curr_night"] == old_night + NIGHT_CORRECTION);

    cout << "[OK] testBothCorrections пройдено\n";
}

int main() {
    testUpdateExistingMeter();
    testNewMeter();
    testNightCorrection();
    testDayCorrection();
    testBothCorrections();
    cout << "[SUCCESS] Усі тести пройдено успішно!\n";
    return 0;
}
