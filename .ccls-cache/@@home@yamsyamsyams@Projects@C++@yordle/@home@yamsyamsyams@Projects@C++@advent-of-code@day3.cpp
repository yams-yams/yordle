#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int bin_to_dec(int binNum){
    int decNum = 0;
    int place = 1;
    while (binNum > 0){
        decNum += (binNum % 10) * place;
        binNum /= 10;
        place *= 2;
    }
    return decNum;
}

//returns the gamma rate in binary form
int getGammaRate(int num, int count){
    
    int digit;
    int gamma = 0;
    int place = 1;
    while (num > 0) {
        digit = num % 10;
        num = num / 10;
        if(count - digit < digit)
            gamma += place;
        place *= 10;
    }
    
    return gamma;
}

//returns the epsilon rate in binary form
int getEpsilonRate(int gamma){
    int epsilon = 0;
    int place = 1;
    while (gamma > 0){
        epsilon += place * ((gamma % 10) ^ 1);
        place *= 10;
        gamma /= 10;
    }

    return epsilon;
}


int main() {

    string line;
    int num;
    int count;

    // Read from the text file
    ifstream MyReadFile("diagnostic.txt");
    
    //add all the numbers
    while (getline (MyReadFile, line)) {
        num += stoi(line);
        count++;
    }
    
    int gamma = bin_to_dec(getGammaRate(num, count));
    int epsilon = bin_to_dec(getEpsilonRate(gamma));

    cout << "gamma rate is: " << gamma << endl;
    cout << "epsilon rate is: " << epsilon << endl;
    cout << "result is: " << gamma * epsilon << endl;

    // Close the file
    MyReadFile.close();

    return 0;

}
