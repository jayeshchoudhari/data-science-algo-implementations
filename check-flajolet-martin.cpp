/*
	This is a sample implementation of Flajolet-Martin Algo.
	Name: Jayesh Choudhari
*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <functional>		//for using hashfunction...
#include <bitset>
#include <set>

using namespace std;

int getTrailingZerosInBits(unsigned long long int);
int updateHashtagMaps(string, string, string, bool);
double getCountWithLeadingZeros(vector<bool>);
int maxTrailZeros = 0;
unordered_map<string, vector <unsigned long int> > hashtag_NUsers_NTweets_Timestamps_Map_Vector;

unordered_map<string, vector<bool> > hashtag_bitVector;
//for each hashtag get the approximate count using Flajolet-Martin

std::hash<std::string> twUidHashFunction;
//hash function

set<string> setUids;


int main()
{
	string line;
	stringstream ss;

	ifstream tweetInfoFile;
	tweetInfoFile.open("selected_votekatniss_tweets.txt");

	hashString = "votekatniss";
	
	//some tweet file with each line as:
	//hashtag timestamp userId
	//here we try to count the number of distinct users using the Flajolet Martin estimate....

	//printf("Working in file %s -- %d of %lu\n", fileNamesList[i].c_str(), i, fileNamesList.size());
	int linecount = 0;

	string twUId, twHashtags, twTimestamp;
	//for each line in the file.... i.e. for each tweet...
	while(getline(tweetInfoFile, line))
	{
		ss.clear();
		ss.str("");

		ss << line;
		string token;

		ss >> twHashtags >> twTimestamp >> twUId;
		updateHashtagMaps(twUId, twHashtags, twTimestamp, 0);
		setUids.insert(twUId);
	}
	tweetInfoFile.close();

	cout << "max-zeros : " << maxTrailZeros << "\n";
	cout << "set-size (exact number of unique users): " << setUids.size() << "\n";
	cout << "Flajolet-Martin estimate of number of unique users = " << hashtag_maxTrailZeros[hashString] << "\n";
	return 0;
}

// update max trailing zeros....
int updateHashtagMaps(string twUId, string eachHashtag, string timestamp, bool twOrRe)
{
	unsigned long intTs = boost::lexical_cast<unsigned long>(timestamp);
	unsigned long long hashValTwUId = twUidHashFunction(twUId);

	//get the number of trailing zeros in the hashbit string...
	int trailZeros = getTrailingZerosInBits(hashValTwUId);
	//cout << trailZeros << "\n";
	
	boost::trim(eachHashtag);
	try
	{
		int trailZerosTillNow = hashtag_maxTrailZeros.at(eachHashtag);
		if(trailZeros > hashtag_maxTrailZeros[eachHashtag])
		{
			hashtag_maxTrailZeros[eachHashtag] = trailZeros;
		}
	}
	catch(const out_of_range &oor)
	{
		hashtag_maxTrailZeros[eachHashtag] = 0;
	}

	return 0;
}

//get the count of trailing zeros...
int getTrailingZerosInBits(unsigned long long num)
{
	//cout << num << "\n";

	//std::bitset<sizeof(unsigned long long) * CHAR_BIT>    bits(num);
    //std::cout << bits << "\t";

   	int pos = 0;
	if(num == 0)
	{
		pos = 0;
	}
	while(num > 0)
	{
     	if(0 == (num&1))
     	{ 
     		pos++;
    		num >>= 1;
    	}
    	else
    		break;
	}
	return pos;
}