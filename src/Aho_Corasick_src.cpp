#include "../hdr/Aho_Corasick_hdr.h"

std::vector<res_pair > Aho_Corasick(std::wstring text, 
									std::vector<std::wstring> patterns, 
									size_t number_of_patterns) // typedef in Aho_Corasick_hdr
{
	BohrTree_AC bohr;
	for (size_t i = 0; i<number_of_patterns; i++)
		bohr.addStrToBohr(patterns[i], i);
	std::vector<res_pair> results = bohr.findIn(text, patterns);
	std::sort(results.begin(), results.end(), compareForSort);
	return results;
}

bool compareForSort( res_pair el1, res_pair el2)
{
	if (el1.first == el2.first)
		return el1.second < el2.second;
	else
		return el1.first < el2.first;
}

Bohr_vertex_AC::Bohr_vertex_AC(wchar_t symbol, int parrent_link) : 
	parrent_link(parrent_link), 
	symbol(symbol), 
	is_end_of_pattern(false),
	suffix_link(-1),
	correct_suffix_link(-1)
{
	next_vertices[symbol] = -1;
	moves[symbol] = -1;
}

BohrTree_AC::BohrTree_AC()
{
	bohr.push_back(Bohr_vertex_AC('$'));
}

void BohrTree_AC::addStrToBohr(const std::wstring &pattern, int numberOfPattern)
{
	int vertexNum = 0;

	for (size_t i = 0; i < pattern.size(); i++)
	{
		wchar_t symb = pattern[i];
		if (bohr[vertexNum].next_vertices.find(symb) == bohr[vertexNum].next_vertices.end() || bohr[vertexNum].next_vertices[symb] == -1)
		{
			bohr.push_back(Bohr_vertex_AC(symb, vertexNum));
			bohr[vertexNum].next_vertices[symb] = bohr.size() - 1;
		}
		
		vertexNum = bohr[vertexNum].next_vertices[symb];
	}

	bohr[vertexNum].pattern_number = numberOfPattern;
	bohr[vertexNum].is_end_of_pattern = true;
}

int BohrTree_AC::getSuffixLink(int vertexNum)
{
	if (bohr[vertexNum].suffix_link == -1)
	{
		if (vertexNum == 0 || bohr[vertexNum].parrent_link == 0)
			bohr[vertexNum].suffix_link = 0;
		else
			bohr[vertexNum].suffix_link = getAutoMove(getSuffixLink(bohr[vertexNum].parrent_link), 
													  bohr[vertexNum].symbol);
	}

	return bohr[vertexNum].suffix_link;
}

int BohrTree_AC::getAutoMove(int vertexNum, wchar_t symbol)
{
	if (bohr[vertexNum].moves.find(symbol) == bohr[vertexNum].moves.end() || bohr[vertexNum].moves[symbol] == -1)
	{
		if (bohr[vertexNum].next_vertices.find(symbol) != bohr[vertexNum].next_vertices.end() && bohr[vertexNum].next_vertices[symbol] != -1)
			bohr[vertexNum].moves[symbol] = bohr[vertexNum].next_vertices[symbol];
		else
		{
			if (vertexNum == 0)
				bohr[vertexNum].moves[symbol] = 0;
			else
				bohr[vertexNum].moves[symbol] = getAutoMove(getSuffixLink(vertexNum), symbol);
		}
	}
	return bohr[vertexNum].moves[symbol];
}

int BohrTree_AC::getCorrectSuffixLink(int vertexNum)
{
	if (bohr[vertexNum].correct_suffix_link == -1)
	{
		int tmpLink = getSuffixLink(vertexNum);
		if (tmpLink == 0)
			bohr[vertexNum].correct_suffix_link = 0;
		else
		{
			if (bohr[tmpLink].is_end_of_pattern == true)
				bohr[vertexNum].correct_suffix_link = tmpLink;
			else
				bohr[vertexNum].correct_suffix_link = getCorrectSuffixLink(tmpLink);
		}
	}
	return bohr[vertexNum].correct_suffix_link;
}

void BohrTree_AC::check(int vertexNum, int i, 
					 std::vector <std::wstring> &patterns, 
					 std::vector<res_pair > &results)
{
	for (int nextVertex = vertexNum; nextVertex != 0; nextVertex = getCorrectSuffixLink(nextVertex))
	{
		if (bohr[nextVertex].is_end_of_pattern)
		{
			res_pair tmp(i - patterns[bohr[nextVertex].pattern_number].size() + 1,
						 bohr[nextVertex].pattern_number + 1);
			results.push_back(tmp);
		}
	}
}

std::vector<res_pair > BohrTree_AC::findIn(const std::wstring &text, std::vector <std::wstring> &patterns)
{
	std::vector<res_pair > results;
	int vertexNum = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		vertexNum = getAutoMove(vertexNum, text[i]);
		check(vertexNum, i + 1, patterns, results);
	}

	return results;
}