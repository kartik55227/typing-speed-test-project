#include <bits/stdc++.h>
#include <windows.h>        // For Sleep and console color
#include <conio.h>          // For _getch()
#include <ctime>            // For time functions

using namespace std;
using namespace std::chrono; // For steady_clock, duration_cast

// Console color constants
const int COLOR_DEFAULT = 7;
const int COLOR_GREEN = 10;
const int COLOR_RED = 12;
const int COLOR_YELLOW = 14;
const int COLOR_BLUE = 9;
const int COLOR_CYAN = 11;
const int COLOR_MAGENTA = 13;

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void clearScreen() {
    system("cls");
}

void showLoading(int duration = 2) {
    auto start = steady_clock::now();
    while (duration_cast<seconds>(steady_clock::now() - start).count() < duration) {
        for (int i = 0; i < 3; i++) {
            cout << "Loading" << string(i, '.') << string(3 - i, ' ') << '\r';
            cout.flush();
            Sleep(200);  // Sleep 200 milliseconds
        }
    }
    cout << string(20, ' ') << '\r';
}

enum Difficulty { EASY, MEDIUM, HARD, EXPERT };

vector<vector<string>> sentences = {
    {
        "The quick brown fox jumps over the lazy dog.",
        "Programming is fun when you enjoy what you do.",
        "Practice makes perfect in coding and typing.",
        "C++ is a powerful programming language.",
        "Hello world is the first program for many."
    },
    {
        "Object-oriented programming helps in organizing complex code.",
        "Standard Template Library provides many useful containers.",
        "Memory management is crucial in C++ programming.",
        "Templates allow for generic programming in C++.",
        "Smart pointers help manage dynamic memory automatically."
    },
    {
        "Multithreading requires synchronization to avoid race conditions.",
        "Move semantics can significantly improve performance by avoiding copies.",
        "Template metaprogramming enables computations at compile time.",
        "The Curiously Recurring Template Pattern is a powerful technique.",
        "Perfect forwarding allows preserving argument types in templates."
    },
    {
        "SFINAE (Substitution Failure Is Not An Error) enables template constraints.",
        "Expression templates can optimize mathematical operations through lazy evaluation.",
        "Type erasure techniques like std::function provide runtime polymorphism.",
        "Policy-based design allows flexible customization of class behavior.",
        "CRTP (Curiously Recurring Template Pattern) enables static polymorphism."
    }
};

struct Score {
    string name;
    double wpm;
    double accuracy;
    Difficulty difficulty;
    time_t timestamp;
};

vector<Score> leaderboard;

void saveLeaderboard() {
    ofstream outFile("leaderboard.dat", ios::binary);
    if (outFile) {
        size_t size = leaderboard.size();
        outFile.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        for (const auto& score : leaderboard) {
            size_t nameSize = score.name.size();
            outFile.write(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
            outFile.write(score.name.c_str(), nameSize);
            outFile.write(reinterpret_cast<const char*>(&score.wpm), sizeof(double));
            outFile.write(reinterpret_cast<const char*>(&score.accuracy), sizeof(double));
            outFile.write(reinterpret_cast<const char*>(&score.difficulty), sizeof(Difficulty));
            outFile.write(reinterpret_cast<const char*>(&score.timestamp), sizeof(time_t));
        }
    }
}

void loadLeaderboard() {
    ifstream inFile("leaderboard.dat", ios::binary);
    if (inFile) {
        size_t size;
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        leaderboard.resize(size);
        for (auto& score : leaderboard) {
            size_t nameSize;
            inFile.read(reinterpret_cast<char*>(&nameSize), sizeof(size_t));
            score.name.resize(nameSize);
            inFile.read(&score.name[0], nameSize);
            inFile.read(reinterpret_cast<char*>(&score.wpm), sizeof(double));
            inFile.read(reinterpret_cast<char*>(&score.accuracy), sizeof(double));
            inFile.read(reinterpret_cast<char*>(&score.difficulty), sizeof(Difficulty));
            inFile.read(reinterpret_cast<char*>(&score.timestamp), sizeof(time_t));
        }
    }
}

void displayMenu() {
    clearScreen();
    setConsoleColor(COLOR_BLUE);
    cout << "\n\n\t\t=========================================\n";
    cout << "\t\t         TYPING SPEED TEST               \n";
    cout << "\t\t=========================================\n\n";
    
    setConsoleColor(COLOR_YELLOW);
    cout << "Main Menu:\n";
    setConsoleColor(COLOR_DEFAULT);
    cout << "1. Start Typing Test\n";
    cout << "2. View Leaderboard\n";
    cout << "3. View Statistics\n";
    cout << "4. Exit\n\n";
    cout << "Select an option: ";
}

Difficulty selectDifficulty() {
    clearScreen();
    setConsoleColor(COLOR_MAGENTA);
    cout << "Select Difficulty:\n\n";
    setConsoleColor(COLOR_DEFAULT);

    cout << "1. "; setConsoleColor(COLOR_GREEN); cout << "Easy\n"; setConsoleColor(COLOR_DEFAULT);
    cout << "2. "; setConsoleColor(COLOR_YELLOW); cout << "Medium\n"; setConsoleColor(COLOR_DEFAULT);
    cout << "3. "; setConsoleColor(COLOR_RED); cout << "Hard\n"; setConsoleColor(COLOR_DEFAULT);
    cout << "4. "; setConsoleColor(COLOR_MAGENTA); cout << "Expert\n\n"; setConsoleColor(COLOR_DEFAULT);
    cout << "Enter your choice: ";

    int choice;
    while (true) {
        cin >> choice;
        if (cin.fail() || choice < 1 || choice > 4) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            setConsoleColor(COLOR_RED);
            cout << "Invalid choice! Please enter 1-4: ";
            setConsoleColor(COLOR_DEFAULT);
        } else {
            cin.ignore();
            return static_cast<Difficulty>(choice - 1);
        }
    }
}

void displayTypingTest(const string& sentence, const string& userInput, size_t currentPos, double progress) {
    clearScreen();
    setConsoleColor(COLOR_BLUE);
    cout << "Progress: [";
    int barWidth = 50;
    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progress * 100) << "%\n\n";
    setConsoleColor(COLOR_DEFAULT);
    cout << "Type the following text:\n\n";

    for (size_t i = 0; i < sentence.size(); ++i) {
        if (i == currentPos) {
            setConsoleColor(COLOR_YELLOW); // Highlight current character
        } else if (i < userInput.size()) {
            setConsoleColor(userInput[i] == sentence[i] ? COLOR_GREEN : COLOR_RED);
        } else {
            setConsoleColor(COLOR_DEFAULT);
        }

        cout << sentence[i];
        setConsoleColor(COLOR_DEFAULT);
    }

    cout << "\n\n";
}


void runTypingTest(Difficulty difficulty) {
    srand(static_cast<unsigned int>(time(nullptr)));
    string sentence = sentences[difficulty][rand() % sentences[difficulty].size()];
    string userInput;
    auto startTime = steady_clock::now();
    size_t currentPos = 0;

    displayTypingTest(sentence, userInput, currentPos, 0.0);

    while (currentPos < sentence.size()) {
        char c = _getch(); // Get character without Enter
        if (c == '\r') continue;
        if (c == '\b') {
            if (!userInput.empty()) {
                userInput.pop_back();
                if (currentPos > 0) currentPos--;
            }
        } else if (c >= 32 && c <= 126) {
            if (currentPos < sentence.size()) {
                userInput += c;
                currentPos++;
            }
        }
        double progress = static_cast<double>(currentPos) / sentence.size();
        displayTypingTest(sentence, userInput, currentPos, progress);
    }

    auto endTime = steady_clock::now();
    double seconds = duration_cast<milliseconds>(endTime - startTime).count() / 1000.0;
    int words = sentence.size() / 5;
    double wpm = (words / seconds) * 60;

    int correctChars = 0;
    for (size_t i = 0; i < min(userInput.size(), sentence.size()); ++i) {
        if (userInput[i] == sentence[i]) correctChars++;
    }
    double accuracy = (static_cast<double>(correctChars) / sentence.size()) * 100;

    clearScreen();
    setConsoleColor(COLOR_CYAN);
    cout << "Typing Test Results\n===================\n\n";
    setConsoleColor(COLOR_DEFAULT);
    cout << "Difficulty: ";
    setConsoleColor(difficulty == EASY ? COLOR_GREEN : difficulty == MEDIUM ? COLOR_YELLOW : difficulty == HARD ? COLOR_RED : COLOR_MAGENTA);
    cout << (difficulty == EASY ? "Easy" : difficulty == MEDIUM ? "Medium" : difficulty == HARD ? "Hard" : "Expert") << "\n";
    setConsoleColor(COLOR_DEFAULT);
    cout << "Time: " << fixed << setprecision(2) << seconds << " seconds\n";
    cout << "Speed: " << fixed << setprecision(1) << wpm << " WPM\n";
    cout << "Accuracy: " << fixed << setprecision(1) << accuracy << "%\n\n";

    setConsoleColor(COLOR_YELLOW);
    cout << "Enter your name for the leaderboard: ";
    setConsoleColor(COLOR_DEFAULT);
    string name;
    getline(cin, name);

    leaderboard.push_back({name, wpm, accuracy, difficulty, time(nullptr)});
    saveLeaderboard();
    sort(leaderboard.begin(), leaderboard.end(), [](const Score& a, const Score& b) {
        return a.wpm > b.wpm;
    });

    setConsoleColor(COLOR_BLUE);
    cout << "\nSaving your score";
    showLoading(2);
    setConsoleColor(COLOR_GREEN);
    cout << "Score saved successfully!\n";
    setConsoleColor(COLOR_DEFAULT);
    cout << "Press Enter to continue...";
    cin.ignore();
}

void displayLeaderboard() {
    clearScreen();
    setConsoleColor(COLOR_CYAN);
    cout << "Leaderboard\n===========\n\n";
    setConsoleColor(COLOR_DEFAULT);

    if (leaderboard.empty()) {
        setConsoleColor(COLOR_YELLOW);
        cout << "No scores yet! Be the first to set a record.\n";
        setConsoleColor(COLOR_DEFAULT);
        cout << "Press Enter to continue...";
        cin.ignore();
        return;
    }

    setConsoleColor(COLOR_MAGENTA);
    cout << left << setw(4) << "Pos" << setw(20) << "Name" << setw(10) << "WPM" 
         << setw(12) << "Accuracy" << setw(12) << "Difficulty" << "Date\n";
    setConsoleColor(COLOR_DEFAULT);
    cout << string(60, '-') << "\n";

    for (size_t i = 0; i < min(leaderboard.size(), size_t(10)); ++i) {
        const auto& score = leaderboard[i];
        cout << left << setw(4) << i + 1;
        cout << setw(20) << (score.name.size() > 18 ? score.name.substr(0, 15) + "..." : score.name);

        setConsoleColor(score.wpm >= 80 ? COLOR_GREEN : score.wpm >= 50 ? COLOR_YELLOW : COLOR_RED);
        cout << setw(10) << fixed << setprecision(1) << score.wpm;
        setConsoleColor(COLOR_DEFAULT);

        setConsoleColor(score.accuracy >= 90 ? COLOR_GREEN : score.accuracy >= 75 ? COLOR_YELLOW : COLOR_RED);
        cout << setw(12) << fixed << setprecision(1) << score.accuracy << "%";
        setConsoleColor(COLOR_DEFAULT);

        setConsoleColor(score.difficulty == EASY ? COLOR_GREEN : score.difficulty == MEDIUM ? COLOR_YELLOW : score.difficulty == HARD ? COLOR_RED : COLOR_MAGENTA);
        cout << setw(12) << (score.difficulty == EASY ? "Easy" : score.difficulty == MEDIUM ? "Medium" : score.difficulty == HARD ? "Hard" : "Expert");
        setConsoleColor(COLOR_DEFAULT);

        string timeStr = ctime(&score.timestamp);
        timeStr.pop_back(); // remove trailing \n
        cout << timeStr << "\n";
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
}

void displayStatistics() {
    clearScreen();
    setConsoleColor(COLOR_CYAN);
    cout << "Statistics\n==========\n\n";
    setConsoleColor(COLOR_DEFAULT);

    if (leaderboard.empty()) {
        setConsoleColor(COLOR_YELLOW);
        cout << "No data available yet. Complete some typing tests first!\n";
        setConsoleColor(COLOR_DEFAULT);
        cout << "Press Enter to continue...";
        cin.ignore();
        return;
    }

    double totalWPM = 0, totalAccuracy = 0;
    int easyCount = 0, mediumCount = 0, hardCount = 0, expertCount = 0;

    for (const auto& score : leaderboard) {
        totalWPM += score.wpm;
        totalAccuracy += score.accuracy;
        switch (score.difficulty) {
            case EASY: easyCount++; break;
            case MEDIUM: mediumCount++; break;
            case HARD: hardCount++; break;
            case EXPERT: expertCount++; break;
        }
    }

    double averageWPM = totalWPM / leaderboard.size();
    double averageAccuracy = totalAccuracy / leaderboard.size();

    cout << "Total Tests: " << leaderboard.size() << "\n";
    cout << "Average WPM: " << fixed << setprecision(1) << averageWPM << "\n";
    cout << "Average Accuracy: " << fixed << setprecision(1) << averageAccuracy << "%\n";
    cout << "Easy Tests: " << easyCount << "\n";
    cout << "Medium Tests: " << mediumCount << "\n";
    cout << "Hard Tests: " << hardCount << "\n";
    cout << "Expert Tests: " << expertCount << "\n";

    cout << "\nPress Enter to continue...";
    cin.ignore();
}

int main() {
    loadLeaderboard();
    while (true) {
        displayMenu();
        int option;
        cin >> option;
        cin.ignore();
        switch (option) {
            case 1: runTypingTest(selectDifficulty()); break;
            case 2: displayLeaderboard(); break;
            case 3: displayStatistics(); break;
            case 4:
                clearScreen();
                setConsoleColor(COLOR_GREEN);
                cout << "Thank you for using Typing Master! Goodbye!\n";
                setConsoleColor(COLOR_DEFAULT);
                Sleep(2000);  // 2 seconds pause
                return 0;
            default:
                setConsoleColor(COLOR_RED);
                cout << "Invalid option! Please choose between 1 to 4.\n";
                setConsoleColor(COLOR_DEFAULT);
                Sleep(2000);
        }
    }
}
