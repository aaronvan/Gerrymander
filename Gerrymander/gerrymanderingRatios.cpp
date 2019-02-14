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
    map<string, int> countMap;			// party : votes
    map<string, double> gerryMap;		// party : ratio
	map<string, string> districtWinner;	// district : winning party 
	vector<string> districtVotes;		// temp storage for each district's votes

    ifstream file_in(file, ios::in);
	int totalVotes = 0;
	int totalDistricts = 0;
    if (file_in) {
		string line, districtName, partyVote, topString;
        while (file_in >> districtName, getline(file_in, line)) {
            stringstream ss(line);
            while (ss >> partyVote) {
                try {
                    ++countMap.at(partyVote);              // looks for key ch
                } catch (const out_of_range& oor) { // if key not present,
                    countMap[partyVote] = 1;               // adds key and starts counting
                }
				districtVotes.push_back(partyVote);
                ++totalVotes;
            }
			// iterate over the district votes and find which party got the majority
			for (size_t i = 0; i < districtVotes.size() - 1; ++i) {
				string winner = districtVotes[i];
				int counter = 0;
				int maxCounter = 0;
				if (districtVotes[i + 1] == districtVotes[i]) {
					winner = districtVotes[i + 1];
					++counter;
				}
				else {
					if (counter > maxCounter)
						counter = maxCounter;
					topString = districtVotes[maxCounter];
					counter = 0;
				}
				districtWinner.insert_or_assign(districtName, topString);
			}
			districtVotes.clear();
		}
        ++totalDistricts;
    }
	file_in.close();

	map<string, int>::iterator iter;
    for (iter = countMap.begin(); iter != countMap.end(); ++iter) {
       double percentDistrictsPartyWonAMajority = totalDistricts / (*iter).second; //wrong
       double percentTotalVotesWonByParty = totalVotes / (*iter).second;
       double ratio = percentDistrictsPartyWonAMajority / percentTotalVotesWonByParty;
       gerryMap.insert(pair<string, double> {(*iter).first, ratio});
    }
    return gerryMap;
}

