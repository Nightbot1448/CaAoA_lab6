#ifndef SEARCH_WITH_JOKER_HPP
#define SEARCH_WITH_JOKER_HPP

#include "includes.h"

typedef std::pair<int, int> res_pair;
std::vector<int> search_with_joker(std::wstring text, std::wstring joker, wchar_t jokerSymb);

struct Bohr_Vertex
{
	Bohr_Vertex(wchar_t symb, int parrentLink = -1) : symb(symb), parrentLink(parrentLink),
		suffixLink(-1), correctSuffixLink(-1), isEndOfPattern(false) { }
	std::map<wchar_t, int> listOfNextVertex;
	std::map<wchar_t, int> moves;
	std::vector <int> patternNum;
	wchar_t symb;
	int parrentLink;
	int suffixLink;
	int correctSuffixLink;
	bool isEndOfPattern;
};



class Bohr_Tree
{
public:
	Bohr_Tree(size_t text_size);	
	std::vector<res_pair> searchSubstringsByBohr(const std::wstring & text);
	std::vector<int> searchJokerByBohr(const std::wstring & text, const std::wstring & joker, wchar_t jokerSymb);
private:
	void addStrToBohr(const std::wstring & pattern, int numberOfPattern);
	int getSuffixLink(int vertexNum);
	int getAutoMove(int vertexNum, wchar_t symb);
	int getCorrectSuffixLink(int vertexNum);

	void SubstringsCheck(int vertexNum, int currentPos, std::vector<res_pair> &results);

	void jokerCheck(int vertexNum, int currentPos, size_t jokerLength);

	std::vector<size_t> jokerParse(const std::wstring & jokerPattern, wchar_t joker);

	std::vector<Bohr_Vertex> bohr;
	std::vector<int> results;
	std::vector<int> background;
	std::vector<size_t> lengths;
	std::vector<std::wstring> patterns;
};

#endif //SEARCH_WITH_JOKER_HPP
