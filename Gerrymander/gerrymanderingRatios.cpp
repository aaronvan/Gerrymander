#include <iostream>
#include <fstream>
#include <map>
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
    map<string, int> countMap; // party : voters
    map<string, double> gerryMap; // party : ratio
	vector<string> districtWinner; // holds votes for instances of each char

    ifstream file_in(file);
	int totalVotes = 0;
	int totalDistricts = 0;
    if (file_in) {
		string line, ignoreMe, ch;
        while (file_in >> ignoreMe, getline(file_in, line)) {
            stringstream ss(line);
            while (ss >> ch) {
                try {
                    ++countMap.at(ch);              // looks for key ch
                } catch (const out_of_range& oor) { // if not present,
                    countMap[ch] = 1;               // adds ch and starts counting
                }
                ++totalVotes;
            }
			//districtWinner.push_back(ch);
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
