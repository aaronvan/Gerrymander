#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

map<string, double> gerrymanderingRatios(string);
void printMap(map<string, double>);

int main() {
    cout << "output should be something like: {\"D\":1.3, \"R\":0.742857}" << endl;
    printMap(gerrymanderingRatios("data.txt"));
	cout << endl;
	system("pause");
    return EXIT_SUCCESS;
}

void printMap(map<string, double> in) {
    map<string, double>::iterator dit;
    for (pair<string, double> p : in) {
      cout << '{' << p.first << ":" << p.second << "} ";
    }
}

/************************************************************************************************/

map<string, double> gerrymanderingRatios(string file) {
    map<string, int> countMap;			// party : voters
    map<string, double> gerryMap;		// party : ratio
	map<string, string> districtWinner;	// district : winning party 
	vector<string> districtVotes;		// holds a disctict's votes

    ifstream file_in(file, ios::in);
	int totalVotes = 0;
	int totalDistricts = 0;
    if (file_in) {
		string line, districtName, ch;
        while (file_in >> districtName, getline(file_in, line)) {
            stringstream ss(line);
            while (ss >> ch) {
                try {
                    ++countMap.at(ch);              // looks for key ch
                } catch (const out_of_range& oor) { // if key not present,
                    countMap[ch] = 1;               // adds key and starts counting
                }
				districtVotes.push_back(ch);
                ++totalVotes;
            }
			//sort districtVotes
			sort(districtVotes.begin(), districtVotes.end());
			// iterate through it and keep a counter that you increment when the current number is the same
			// as the previous number and reset to 0 otherwise.
			// Also keep track of what was the highest value of the counter thus far and 
			// what the current number was when that value was reached.
			for (size_t i = 0; i < districtVotes.size() - 1; ++i) {
				int counter = 0;
				int maxCounter = 0;
				string highCount;
				string winner = districtVotes[i];
				if (districtVotes[i + 1] == districtVotes[i]) {
					winner = districtVotes[i];
					highCount = districtVotes[i];
					++counter;
				}
				else {
					counter = 0;
				}
				districtWinner.insert_or_assign(districtName, winner);
			}
			districtVotes.clear();
            ++totalDistricts;
        }
        file_in.close();
    }
	// get the district winners here

	map<string, int>::iterator iter;
    for (iter = countMap.begin(); iter != countMap.end(); ++iter) {
       double percentDistrictsPartyWonAMajority = totalDistricts / (*iter).second; //wrong
       double percentTotalVotesWonByParty = totalVotes / (*iter).second;
       double ratio = percentDistrictsPartyWonAMajority / percentTotalVotesWonByParty;
       gerryMap.insert(pair<string, double> {(*iter).first, ratio});
    }
    return gerryMap;
}
