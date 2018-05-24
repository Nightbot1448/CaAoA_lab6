#ifndef BohrTree_HPP
#define BohrTree_HPP

#include "includes.h"

using namespace std;

vector<int> search_with_joker(std::wstring text, std::wstring joker, wchar_t jokerSymb);

struct Bohr_Vertex
{

	Bohr_Vertex(wchar_t symb, int parrentLink = -1) : symb(symb), parrentLink(parrentLink),
		suffLink(-1), correctSuffLink(-1), isEndOfPattern(false) { }
		
	map <wchar_t, int> listOfNextVertex;
	map <wchar_t, int>	moves;

	vector <int> patternNum;
	wchar_t symb;
	int parrentLink;
	int suffLink;
	int correctSuffLink;
	bool isEndOfPattern;
};


typedef pair<int, int> Result;

class Bohr_Tree
{
public:
	Bohr_Tree(size_t text_size);	
	vector<Result> searchSubstringsByBohr(const wstring & text);
	vector<int> searchJokerByBohr(const wstring & text, const wstring & joker, wchar_t jokerSymb);
private:
	void addStrToBohr(const wstring & pattern, int numberOfPattern);
	int getSuffixLink(int vertexNum);
	int getAutoMove(int vertexNum, wchar_t symb);
	int getCorrectSuffixLink(int vertexNum);

	void SubstringsCheck(int vertexNum, int currentPos, vector <Result> & results);


	void jokerCheck(int vertexNum, int currentPos, size_t jokerLength);

	vector <size_t> jokerParse(const wstring & jokerPattern, wchar_t joker);

	vector <Bohr_Vertex> bohr;
	vector <int> results;
	vector <int> background;
	vector <size_t> lengths;
	vector <wstring> patterns;
};

#endif //BohrTree_HPP
