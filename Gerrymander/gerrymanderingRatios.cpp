#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

map<string, double> gerrymanderingRatios(const string &file);
void printMap(map<string, double>);

int main() {
    cout << endl;
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

map<string, double> gerrymanderingRatios(const string &file) {
    map<string, int> countMap;   
	map<string, string> districtWinner; 
	vector<string> districtVotes;		// temp storage for each district's votes
	map<string, double> gerryMap;		// party : ratio (returned)
    
	ifstream file_in(file, ios::in);
	double totalVotes = 0.0;
	double districtCounter = 0.0;
	double ratio = 0.0;
    if (file_in) {
		string line, districtName, partyVote, majorityVote;
        while (file_in >> districtName, getline(file_in, line)) {
            stringstream ss(line);
            while (ss >> partyVote) {
                try {
					++countMap[partyVote];
                } catch (const out_of_range& oor) {		
                   countMap[partyVote] = 1;            
                }
				districtVotes.push_back(partyVote);
                ++totalVotes;
            }
			// iterate over districtVotes, insert majority party into districtWinner
			// expected: {"A":0.75, "B":1.40625, "C":0.9375}
			//sort(districtVotes.begin(), districtVotes.end());
			sort(districtVotes.begin(), districtVotes.end());
			int max = 0;
			string winner = districtVotes[max];
			for (size_t i = 0; i < districtVotes.size(); ++i) {
				int counter = count(districtVotes.begin(), districtVotes.end(), districtVotes[i]);
				if (counter > max) {
					max = counter;
					winner = districtVotes[i];
				}
			}
			districtWinner[districtName] = winner;
			districtVotes.clear();
			++districtCounter;
		}
	}
	file_in.close();
	
	map<string, int>::iterator voteCounter = countMap.begin(); // countMap
    for (; voteCounter != countMap.end(); ++voteCounter) {
		map<string, string>::iterator partyCounter = districtWinner.begin(); // districtWinner
		int districtsWon = 0;
		for (; partyCounter != districtWinner.end(); ++partyCounter) {
			if ((*voteCounter).first == (*partyCounter).second)
				++districtsWon;
		}
		double percentOfVotes = (*voteCounter).second / totalVotes; // total votes / # of party votes
		double percentDistrictsWon = districtsWon / districtCounter; // districts won by party / # of districts
		ratio = percentDistrictsWon / percentOfVotes;
		gerryMap[(*voteCounter).first] = ratio;
    }
    return gerryMap;
}