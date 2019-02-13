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
    map<string, int> countMap;		// party : voters
    map<string, double> gerryMap;	// party : ratio
	map<string, int> districtWinner;	// district : winning party 
	vector<string> districtVotes;	// holds instances of each vote, districts seperated by "/n"

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
                } catch (const out_of_range& oor) { // if not present,
                    countMap[ch] = 1;               // adds ch and starts counting
                }
				districtVotes.push_back(ch);
                ++totalVotes;
            }
			// insert max districtVotes (winning party) into districtWinner
			// clear districtVotes
			for (string party : districtVotes) {
				string district = districtName;
				int c = count(districtVotes.begin(), districtVotes.end(), party);
				// assign the int to the party
				districtWinner.insert_or_assign(district, c);
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
