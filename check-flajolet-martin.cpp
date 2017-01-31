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
//to store for each hashtag -- vector <#users, #tweets, firstTimestamp, lastTimestamp>

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
	//some tweet file with each line as:
	//hashtag timestamp userId
	//here we try to count the number of distinct users....

	//printf("Working in file %s -- %d of %lu\n", fileNamesList[i].c_str(), i, fileNamesList.size());
	int linecount = 0;

	//for each line in the file.... i.e. for each tweet...
	while(getline(tweetInfoFile, line))
	{
		ss.clear();
		ss.str("");

		ss << line;
		string token;

		string twUId, twHashtags, twTimestamp;

		ss >> twHashtags >> twTimestamp >> twUId;
		updateHashtagMaps(twUId, twHashtags, twTimestamp, 0);
		setUids.insert(twUId);
	}
	tweetInfoFile.close();
	cout << "max-zeros : " << maxTrailZeros << "\n";
	cout << "set-size: " << setUids.size() << "\n";
	ofstream outFile;
    outFile.open("./hashtag_Users_Tweets_Timestamps_sample_filelist.txt");

	unordered_map<string, vector <unsigned long int> >::iterator mapStringLongVectorIterator;

	for(mapStringLongVectorIterator = hashtag_NUsers_NTweets_Timestamps_Map_Vector.begin(); mapStringLongVectorIterator != hashtag_NUsers_NTweets_Timestamps_Map_Vector.end(); mapStringLongVectorIterator++)
	{
		
		string ht = mapStringLongVectorIterator->first;

		vector<bool> tmpBoolVector = hashtag_bitVector[ht];
		//double numOfUsers = getCountWithLeadingZeros(tmpBoolVector);
		double numOfUsers = pow(2, maxTrailZeros);
		cout << "num of users: " << numOfUsers << "\n";
		vector <unsigned long> tmpLongVector = mapStringLongVectorIterator->second;

		//printf("%s\t %lu\t %lu\t %lu\t %lu\n", ht.c_str(), numOfUsers, tmpLongVector[0], tmpLongVector[1], tmpLongVector[2]);
		outFile << ht.c_str() << " " << numOfUsers << " " << tmpLongVector[0] << " " << tmpLongVector[1] << " " << tmpLongVector[2] << "\n";
	}

	outFile.close();
	return 0;
}

//
int updateHashtagMaps(string twUId, string eachHashtag, string timestamp, bool twOrRe)
{

	unsigned long intTs = boost::lexical_cast<unsigned long>(timestamp);
	unsigned long long hashValTwUId = twUidHashFunction(twUId);

	//get the number of trailing zeros in the hashbit string...
	int trailZeros = getTrailingZerosInBits(hashValTwUId);
	//cout << trailZeros << "\n";
	
	if(trailZeros > maxTrailZeros) 	maxTrailZeros = trailZeros;

	boost::trim(eachHashtag);
	try
	{
		vector<bool> tmp_bitVector = hashtag_bitVector.at(eachHashtag);
		
		hashtag_bitVector[eachHashtag][63 - trailZeros + 1] = 1;
		//tmp_bitVector[63 - trailZeros] = 1;
		//hashtag_bitVector[eachHashtag] = tmp_bitVector;


		unsigned long minTs = hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag][1];
		unsigned long maxTs = hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag][2];
		if(intTs < minTs)
		{
			hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag][1] = intTs;
		}

		if(intTs > maxTs)
		{
			hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag][2] = intTs;
		}

	}
	catch(const out_of_range &oor)
	{
		vector<bool> tmp_bitVector(64, 0);
		tmp_bitVector[63] = 1;
		hashtag_bitVector[eachHashtag] = tmp_bitVector;

		vector<unsigned long> tmp_propVector(3,0);
		tmp_propVector[1] = intTs;
		tmp_propVector[2] = intTs;
		hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag] = tmp_propVector;
	}

	if(twOrRe == 0)
	{
		hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag][0]++; 	//update count of tweet
        //cout << hashtag_NUsers_NTweets_Timestamps_Map_Vector[eachHashtag][0] << endl;
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

//get the count of leading zeros...
double getCountWithLeadingZeros(vector<bool> tmpBoolVector)
{
	unsigned int i = 0;

	for(; i < tmpBoolVector.size(); i++)
	{
		if(tmpBoolVector[i] == 0)
			continue;
		else
			break;
	}

	//double numUsers = (pow(2, i) * 1.0)/0.77351;
	double numUsers = pow(2, i);

	return numUsers;
}
