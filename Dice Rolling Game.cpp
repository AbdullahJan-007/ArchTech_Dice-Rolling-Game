#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
#include <cctype>

using namespace std;

void centerText(const string& text, int width = 80) {
    int padding = (width - text.length()) / 2;
    if (padding > 0) cout << string(padding, ' ');
    cout << text;
}

string getCenteredInput(const string& prompt) {
    centerText(prompt);
    string input;
    cout << "\n";
    centerText("> ");
    getline(cin, input);
    return input;
}


class Dice {
protected:
    int sides;
    int current_value;

public:
    Dice(int sides = 6) : sides(sides), current_value(1) {
        if (sides <= 0) throw "Dice must have at least one side";
    }

    virtual ~Dice() {}

    virtual void roll() {
        current_value = rand() % sides + 1;
    }

    int show() const { return current_value; }
    int side_count() const { return sides; }
};

class SixSidedDice : public Dice {
public:
    SixSidedDice() : Dice(6) {}
};


class DiceGame {
private:
    vector<unique_ptr<Dice>> dice_set;
    int total_rolls;

public:
    DiceGame() : total_rolls(0) {}

    void prepare_dice(int quantity) {
        if (quantity <= 0) throw "You need at least one dice";
        
        dice_set.clear();
        for (int i = 0; i < quantity; i++) {
            dice_set.push_back(make_unique<SixSidedDice>());
        }
    }

    void roll_dice() {
        total_rolls++;
        centerText("=== Roll #" + to_string(total_rolls) + " ===\n\n");
        
        string result = "Results: ";
        for (const auto& dice : dice_set) {
            dice->roll();
            result += "[" + to_string(dice->show()) + "] ";
        }
        centerText(result + "\n\n");
    }

    int roll_count() const {
        return total_rolls;
    }

    void reset_counter() {
        total_rolls = 0;
    }

    int dice_count() const {
        return dice_set.size();
    }
};


void display_header() {
    cout << "\n\n";
    centerText("*********************************\n");
    centerText("*       DICE ROLLER GAME       *\n");
    centerText("*********************************\n\n");
}

void display_footer(int total_rolls) {
    cout << "\n";
    centerText("*********************************\n");
    centerText("*   You made " + to_string(total_rolls) + " rolls!      *\n");
    centerText("*                               *\n");
    centerText("*   Thank you for playing!     *\n");
    centerText("*********************************\n\n");
}

void display_menu() {
    centerText("1. Roll Dice\n");
    centerText("2. Change Dice Count\n");
    centerText("3. Exit\n\n");
}

int main() {
    srand(time(nullptr));
    DiceGame game;
    int dice_quantity = 1;  

    display_header();

    try {
        game.prepare_dice(dice_quantity);
    } catch (const char* error) {
        centerText("ERROR: " + string(error) + "\n");
        return 1;
    }

    while (true) {
        display_menu();
        string choice = getCenteredInput("Enter your choice (1-3):");

        if (choice == "1") {
            game.roll_dice();
        }
        else if (choice == "2") {
            while (true) {
                string input = getCenteredInput("How many dice would you like to roll (1-10)?");
                try {
                    dice_quantity = stoi(input);
                    if (dice_quantity < 1 || dice_quantity > 10) {
                        centerText("! Please enter a number between 1 and 10.\n\n");
                    } else {
                        game.prepare_dice(dice_quantity);
                        break;
                    }
                } catch (...) {
                    centerText("! Invalid input. Please enter a number.\n\n");
                }
            }
        }
        else if (choice == "3" || choice == "exit" || choice == "quit") {
            break;
        }
        else {
            centerText("! Invalid choice. Please try again.\n\n");
        }
    }

    display_footer(game.roll_count());
    return 0;
}