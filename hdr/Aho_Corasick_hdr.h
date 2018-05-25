#ifndef H_STEP1_H
#define H_STEP1_H

#include "includes.h"

typedef std::pair<int, int> res_pair;

bool compareForSort(res_pair el1, res_pair el2);

std::vector<res_pair> Aho_Corasick(std::wstring text, 
									std::vector<std::wstring> patterns,
									size_t number_of_patterns);

struct Bohr_vertex_AC
{
	 Bohr_vertex_AC(wchar_t symbol, int parrent_link = -1);

	 //data
	std::map<wchar_t,int> next_vertices;
	std::map<wchar_t,int> moves;
	int pattern_number;
	int suffix_link ;
	int correct_suffix_link;
	int parrent_link;
	bool is_end_of_pattern;
	wchar_t symbol;

};

struct BohrTree_AC
{
	BohrTree_AC(); 
	void addStrToBohr(const std::wstring &pattern, int numberOfPattern);
	int getSuffixLink(int vertexNum);
	int getAutoMove(int vertexNum, wchar_t symbol);
	int getCorrectSuffixLink(int vertexNum);
	void check(int vertexNum, int i, std::vector <std::wstring> &patterns, std::vector<res_pair > &results);
	std::vector<res_pair > findIn(const std::wstring &text, std::vector <std::wstring> &patterns);

	//data
	std::vector<Bohr_vertex_AC> bohr;
};

#endif //H_STEP1_H