#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// get all the lines from the file to translate
vector<string> getText(ifstream & filename) {
	string line;
	vector<string> data;
	while (getline(filename, line)) {
		data.push_back(line);
	}
	return data;
}

map<string, string> makeMap(vector<string> line) {
	map<string, string> translate;
	string word1, word2;

	for (int i = 0; i < line.size(); i++) {
		bool found = false;
		int j = 0;
		while (j < line[i].size() && found == false) {
			if (line[i][j] == (char) "=") {
				word1 = line[i].substr(0, j + 1);
				word2 = line[i].substr(j + 1);
				found = true;
			}
			j++;
		}
		translate[word1] = word2;
	}

	return translate;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		cout << "Wrong number of files entered." << endl;
		return 1;
	}

	// first file is english -> spanish
	// second file is spanish -> polish
	// third file is file to translate
	ifstream file[3];
	for (int i = 0; i < 4; i++) {
		file[i].open(argv[i]);
		if (!file[i]) {
			cerr << "Can't open file " << i + 1;
			return 1;
		}
	}

	// put each word from file into vector
	string word;
	vector<string> data;
	while (file[2] >> word) {
		data.push_back(word);
	}

	// stores each line of file in a vector
	vector<string> engToSpan = getText(file[0]);
	vector<string> spanToPol = getText(file[1]);

	// maps translations
	map<string, string> toSpanish = makeMap(engToSpan);
	map<string, string> toPolish = makeMap(spanToPol);

	string dataSpanish;
	vector<string> dataPolish;

	for (int i = 0; i < data.size(); i++) {
		dataSpanish = "";
		// get rid of punctuation
		for (int j = 0; j < data[i].size(); j++) {
			if ((data[i][j] > (char) "A" && data[i][j] < (char) "Z") || (data[i][j] > (char)"a" && data[i][j] < (char)"z")) {
				continue;
			}
			else {
				data[i].erase(data[i].begin() + j);
			}
		}
		
		// find opcode in map
		map<string, string> ::iterator it = toSpanish.find(data[i]);

		// stores translation or question marks
		if (it != toSpanish.end()) {
			dataSpanish = it->second;
		}
		else {
			string none = "";
			for (int i = 0; i < data[i].size(); i++) {
				none += "?";
			}
			dataSpanish = none;
		}
	}

	map<string, string> ::iterator it2 = toPolish.find(dataSpanish);

	if (it2 != toPolish.end()) {
		dataPolish.push_back(it2->second);
	}
	else {
		string none = "";
		for (int i = 0; i < dataSpanish.size(); i++) {
			none += "?";
		}
		dataPolish.push_back(none);
	}

	for (int x = 0; x < dataPolish.size(); x++) {
		if (x % 80 == 0) {
			cout << endl;
		}
		cout << dataPolish[x];
	}


	return 0;
}