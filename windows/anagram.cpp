// anagram.cpp : Defines the entry point for the console application.
//
#include <set>
#include <iostream>
#include <fstream> // std::ifstream
#include <string>
#include <set>
#include <algorithm>
#include <iterator>
#include <map>
#include <cstring>


using namespace std;
using std::map;


// Class to take an std::string object and store characters count in an encapsulated map;
// for each character, a character (key) , occurrence count(value) pairs
class CharCounter
{
private:
	// we make the default constructor private as we want to force a counter
	// object to be initialized at construction always with an std::string object
	// the default constructor won't be accessible outside the class
	CharCounter() {}

public:
	typedef std::map<char,int >	MAPCHARINT;	// typedef for std::map
	typedef MAPCHARINT::iterator	MAPCHARINTIT;	// typedef for std::map iterator type
	typedef MAPCHARINT::const_iterator	MAPCHARINTCONSTIT;	// typedef for std::map constant iterator type

	// constructor
	CharCounter(const string& str){
		for(string::const_iterator it=str.begin(); it!=str.end(); ++it)
		{
			MAPCHARINTIT itt;
			if((itt = countermap.find(*it)) != countermap.end()){
				itt->second++;
			}
			else{
				countermap.insert(make_pair(*it,1));
			}
		}
	}

	// we take each key value in the other counter object and we try to find the same key in
	// this object then substract the other value from this one value
	void substract(const CharCounter& ccter){
		MAPCHARINTCONSTIT itt = ccter.get().begin();
		while( itt != ccter.get().end())
		{
			MAPCHARINTIT it;
			if((it = countermap.find(itt->first)) != countermap.end()){
				it->second -= itt->second;
			}

			++itt;
		}
	}


	const MAPCHARINT& get() const{ return countermap; }
protected:
	MAPCHARINT countermap;
};


// template function implementing the union of two object type such as sets
template<class _InIt1,class _InIt2,class _OutIt>
void _Set_union(_InIt1 _First1, _InIt1 _Last1,_InIt2 _First2, _InIt2 _Last2, _OutIt& _Dest)
{
	for (; _First1 != _Last1 && _First2 != _Last2; )
	{
		if (*_First1 < *_First2){
			_Dest.insert(*_First1);
			++_First1;
		}
		else if (*_First2 < *_First1){
			_Dest.insert(*_First2);
			++_First2;
		}
		else{
			_Dest.insert(*_First1);
			++_First1;
			++_First2;
		}
	}
}


// preliminary comparison function
bool partcmp(const set<char>& gwordset, const string& compareword)
{
	// we declare two sets objects, the first one will be used as destination for union result
	// the second one cmpset is declared and filled with characters from the otehr string
	set<char> setsunion, cmpset(compareword.begin(), compareword.end());

	// we call our Set_union function to create a union of the two word character sets 
	_Set_union(gwordset.begin(), gwordset.end(),
		cmpset.begin(), cmpset.end(), setsunion);

	// if the union size is greater than the other word set, we eliminate to speed up the process
	if( setsunion.size() > cmpset.size())
		return false;

	// else , we are still in.
	return true;
}




int main(int argc, char* argv[])
{
	// we check to see if we got valid number of arguments passed into the program
	if(argc < 3){
		if(argc == 2 && strcmp(argv[1],"-h") == 0 )
		{
			printf("usage: anagram.exe file_name given_word\n");
			return 1;
		}

		printf("anagram.exe: error: too few arguments");
		printf("usage: anagram.exe file_name given_word\n");
		return -1;
	}

	// we declare string variables and set the passed in arguments in the variables
	string givenword(argv[2]), filename(argv[1]);

	// we store the length of the string once for all
	// we can avoid calling the length function thousands of times
	int nwordlen = givenword.length();
	set<string> resultset;

	// input file object
	ifstream ifs;

	// Open stream for read
	ifs.open(filename.c_str(), ifstream::in);
	if(ifs.fail())
	{
		printf("Error: failed to open file <%s>. Check if the file exist or user has permission\n", filename.c_str());
		return -1;
	}	

	// Iterators representing start and of stream
	istream_iterator<string> begin = istream_iterator<std::string>(ifs);
	istream_iterator<string> end = istream_iterator<std::string>();

	set<char> givenwordset(givenword.begin(), givenword.end());

	while(begin != end)
	{
		if(nwordlen == (*begin).length() &&
			partcmp(givenwordset, *begin))
		{
			CharCounter compc(*begin);
			CharCounter wordc(givenword);
			wordc.substract(compc);
			set<int> valuesset;
			const CharCounter::MAPCHARINT& wordvals = wordc.get();
			CharCounter::MAPCHARINTCONSTIT itt = wordvals.begin();

			while( itt != wordvals.end()){
				valuesset.insert(itt->second), ++itt;
			}

			// finally, last check
			if(valuesset.size() == 1 && *valuesset.begin() == 0)
			{
				// we got one anagram, so we storing it
				resultset.insert(*begin);
			}
		}

		++begin;
	}

	ifs.close();

	// Copies the elements in the set to `cout`
	copy(resultset.begin(), resultset.end(), ostream_iterator<string>(cout, "\n"));


	return 0;
}
