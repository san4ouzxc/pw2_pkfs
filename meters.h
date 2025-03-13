#ifndef METERS_H
#define METERS_H

#include <unordered_map>
#include <string>

extern std::unordered_map<std::string, std::unordered_map<std::string, double>> meters;

void loadDataFromFile(); 
void saveDataToFile(); 
void updateMeter(const std::string& meter_id, double new_day, double new_night); 
double calculateBill(const std::string& meter_id); 
void printMeters(); 

extern const double DAY_CORRECTION; 
extern const double NIGHT_CORRECTION; 

#endif // METERS_H
