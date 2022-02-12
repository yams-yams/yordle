#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

enum Color {red, yellow, green, grey};

struct key {
    char letter;
    enum Color code;
};

void evaluateGuess(string answer, string guess, enum Color code[]){
    //set code to all red
    for(int i = 0; i < 5; i++)
        code[i] = red;

    //check and mark out all greens 
    for (int i = 0; i < 5; i++){
        if (guess.at(i) == answer.at(i)){
            code[i] = green;
            guess = guess.substr(0, i) + "-" + guess.substr(i+1, 5);
            answer = answer.substr(0, i) + "-" + answer.substr(i+1, 5);
        }
    }

    //check and mark out all yellows
    for (int i = 0; i < 5; i++){
        if(code[i] != green){
            for (int j = 0; j < 5; j++){
                if (guess.at(i) == answer.at(j)){
                    code[i] = yellow;
                    guess = guess.substr(0, i) + "-" + guess.substr(i+1, 5);
                    answer = answer.substr(0, j) + "-" + answer.substr(j+1, 5);
                }
            }
        }
    }
}

bool isValid(string guess) {
    string line;
    ifstream MyReadFile("word_list.txt");

    while (getline (MyReadFile, line)) {
        if(guess.compare(line) == 0)
            return true;
    }
    MyReadFile.close();
    return false;
}

void updateKeyboard(key myKeys[], string guess, Color myCode[]){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 26; j++){
            if(guess[i] == myKeys[j].letter && myKeys[j].code != green){
                if(myKeys[j].code == grey || ((myKeys[j].code == yellow) && myCode[i] == green))
                    myKeys[j].code = myCode[i];
            }
        }
    }
}
void printKeyboard(key myKeys[]){
    printf("--------------------\n");
    for(int i = 0; i < 10; i++){
        if(myKeys[i].code == red)
            printf("\033[;31m%c \033[0m", myKeys[i].letter) ;
        else if(myKeys[i].code == yellow)
            printf("\033[;33m%c \033[0m", myKeys[i].letter);
        else if(myKeys[i].code == green)
            printf("\033[;32m%c \033[0m", myKeys[i].letter);
        else
            printf("%c ", myKeys[i].letter);
    }

    cout << endl;

    for(int i = 10; i < 19; i++){
        if(myKeys[i].code == red)
            printf("\033[;31m%c \033[0m", myKeys[i].letter) ;
        else if(myKeys[i].code == yellow)
            printf("\033[;33m%c \033[0m", myKeys[i].letter);
        else if(myKeys[i].code == green)
            printf("\033[;32m%c \033[0m", myKeys[i].letter);
        else
            printf("%c ", myKeys[i].letter);
    }

    cout << endl;

    for(int i = 19; i < 26; i++){
        if(myKeys[i].code == red)
            printf("\033[;31m%c \033[0m", myKeys[i].letter) ;
        else if(myKeys[i].code == yellow)
            printf("\033[;33m%c \033[0m", myKeys[i].letter);
        else if(myKeys[i].code == green)
            printf("\033[;32m%c \033[0m", myKeys[i].letter);
        else
            printf("%c ", myKeys[i].letter);
    }
    printf("\n--------------------");

}

string welcomeMessage = "\nWelcome to Yordle! Guess the 5-Letter Word.\n\n\033[;31mRed\033[0m indicates a letter that is not present in the word.\n\033[;33mYellow\033[0m indicates a letter that is present elsewhere in the word.\n\033[;32mGreen\033[0m indicates a correctly placed letter.\n";

int main() {
    Color myCode[5];
    key myKeys[26];
    string alphabet = "QWERTYUIOPASDFGHJKLZXCVBNM";    
    string guess;
    bool success = false;
    int guess_count = 6;

    //welcome message
    cout << endl << "-------------------------------------------" << endl;
    cout << welcomeMessage << endl;

    //obtain a correct answer
    std::srand(time(NULL));
    int random = std::rand() % 1653 + 1;
    int count = 1;
    string answer;

    ifstream MyReadFile("freq_word_list.txt");
    while(getline (MyReadFile, answer) && random != count){
        count++;
    }

    MyReadFile.close();

    for(int i = 0; i < 5; i++)
        answer.at(i) = toupper(answer.at(i));

    //initialize keyboard
    for (int i = 0; i < 26; i++){
        myKeys[i].letter = alphabet[i];
        myKeys[i].code = grey;
    }

    //game
    while(!success && guess_count > 0){

        //obtain valid guess
        cout << "You have " << guess_count << " guesses left.\nPlease enter your guess: ";
        cin >> guess; cout << endl << endl;

        while(!isValid(guess)){
            cout << "I don't know that word, please enter another: ";
            cin >> guess; cout << endl << endl;
        }

        //make guess uppercase
        for(int i = 0; i < 5; i++) {
            guess.at(i) = toupper(guess.at(i));
        }

        //evaluate guess and print encoded guess
        evaluateGuess(answer, guess, myCode);

        for(int i = 0; i < 5; i++){
            if(myCode[i] == red)
                printf("\033[;31m%c\033[0m ", guess.at(i));
            else if(myCode[i] == yellow)
                printf("\033[;33m%c\033[0m ", guess.at(i));
            else
                printf("\033[;32m%c\033[0m ", guess.at(i));
        }
        cout << endl << endl;

        updateKeyboard(myKeys, guess, myCode);
        printKeyboard(myKeys);

        cout << endl << endl;

        //finish turn
        if(guess == answer)
            success = true;
        guess_count--;
    }

    //end of game
    if(success)
        cout << "You are correct!\n\n";
    else
        cout << "Sorry, you're out of guesses. The answer was: " << answer << "\n\n";

    return 0;
}
