#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void process_block(ofstream& outFile, const string& block, int iter) {
    istringstream stream(block);
    string line;
    if (iter % 3 == 0) {
        string a, q;
        getline(stream, line); // a
        a = line.substr(line.find(":") + 1);
        getline(stream, line); // q
        q = line.substr(line.find(":") + 1);

        outFile << "\\item $a=" << a << ", q=" << q << "$:\n";
        outFile << "\t\\begin{itemize}\n";
    }

    while (getline(stream, line)) {
        if (line.find("DAYS") != string::npos) {
            string days = line.substr(line.find("--------------------------") + 26);
            string best, average, mediana;

            getline(stream, line); // BEST
            best = line.substr(line.find(":") + 1);

            getline(stream, line); // AVERAGE
            average = line.substr(line.find(":") + 1);

            getline(stream, line); // MEDIANA
            mediana = line.substr(line.find(":") + 1);

            outFile << "\t\t\\item " << days << " days: ";
            outFile << "$averange = " << average << "$, ";
            outFile << "$mediana = " << mediana << "$, ";
            outFile << "$best = " << best << "$;\n";
        }
    }
    if (iter % 3 == 2) outFile << "\t\\end{itemize}\n";
}

int create_tex() {
    ifstream inFile("result.txt");
    ofstream outFile("output.tex");

    if (!inFile.is_open() || !outFile.is_open()) {
        cerr << "Ошибка при открытии файла!\n";
        return 1;
    }
    outFile << "\\begin{enumerate}\n";
    string line, block;
    int iter = 0;
    while (getline(inFile, line)) {
        if (line.empty()) {
            if (!block.empty()) {
                process_block(outFile, block, iter);
                block.clear();
                iter++;
            }
        }
        else {
            block += line + "\n";
        }
    }

    outFile << "\\end{enumerate}\n";

    inFile.close();
    outFile.close();

    cout << "\nФайл успешно создан: output.tex" << std::endl;

    return 0;
}
