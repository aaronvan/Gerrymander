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
	map<string, vector<string>> districtVotes; // district : votes
	map<string, string> districtWinner; 
	map<string, double> gerryMap;		// party : ratio (returned)
    
	ifstream file_in(file);
	double totalVotes = 0.0;
	double districtCounter = 0.0;
	double ratio = 0.0;
    if (file_in) {
		string line;
		string districtName;
		string partyVote;
        while (file_in >> districtName, getline(file_in, line)) {
            stringstream ss(line);
            while (ss >> partyVote) {
				try {
					++countMap[partyVote];
				}
				catch (out_of_range) {
					countMap[partyVote] = 1;
				}
				districtVotes[districtName].push_back(partyVote);
                ++totalVotes;
            }
			string winner;
			for (auto looker : districtVotes) {
				sort(districtVotes[districtName].begin(), districtVotes[districtName].end());
				int max = 0;
				for (size_t i = 0; i < districtVotes[districtName].size(); ++i)	{
					int counter = count(districtVotes[districtName].begin(), districtVotes[districtName].end(), districtVotes[districtName][i]);
					if (counter > max) {
						max = counter;
						winner = districtVotes[districtName][i];
					}
				}
			}
			districtWinner[districtName] = winner;
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