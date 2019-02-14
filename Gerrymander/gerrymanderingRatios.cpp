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
	int districtCounter = 0;
    if (file_in) {
		string line, districtName, partyVote, majorityVote;
        while (file_in >> districtName, getline(file_in, line)) {
            stringstream ss(line);
            while (ss >> partyVote) {
                try {
                    ++countMap.at(partyVote);           // looks for key ch
                } catch (const out_of_range& oor) {		// if key not present,
                    countMap[partyVote] = 1;            // adds key and starts counting
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
					majorityVote = districtVotes[maxCounter];
					counter = 0;
				}
				districtWinner.insert_or_assign(districtName, majorityVote);
			}
			districtVotes.clear();
			++districtCounter;
		}
    }
	file_in.close();
	
/* 
For each key in countMap, get total of that value in districtWinner
countMap {D : 12, R : 14 } 
districtWinner { DISTRICT3 : R, District1 : D, District2 : R, District4 : D, district5 : D }
 totalDistrict = 5
	for each (countMap[key] count the times it appears in districtWinner);
		calculate:
			-- percentDistrictsWon
			-- percent of votes;
			-- ratio
		insert in gerryMap
	}
}
return gerryMap */

	
	map<string, int>::iterator voteCounter; // countMap
    for (voteCounter = countMap.begin(); voteCounter != countMap.end(); ++voteCounter) {
		map<string, string>::iterator partyCounter; // districtWinner
		for (partyCounter = districtWinner.begin(); partyCounter != districtWinner.end(); ++partyCounter) {
			int districtsWon = 0;
			if ( (*voteCounter).first == (*partyCounter).second )
				++districtsWon;
			double percentOfVotes = totalVotes / (*voteCounter).second; // total votes / # of party votes
			double percentDistrictsWon = districtsWon / districtCounter; // districts won by party / # of districts
			double ratio = percentDistrictsWon / percentOfVotes;
			gerryMap.insert(pair<string, double> {(*voteCounter).first, ratio});
		}
    }
    return gerryMap;
}

