#include "../hdr/search_with_joker_hdr.h"

std::vector<int> search_with_joker(std::string text, std::string jokerPattern, char joker)
{
	std::stringstream s_jp(jokerPattern);
	BohrTree_J bohrTree_J(text.size());
	bohrTree_J.getParseStrApart(s_jp, joker);

	bohrTree_J.findIn(text);

	std::vector<int> results;

	for(int i=0; i < text.size()-jokerPattern.size()+1; i++)
	{

		if(bohrTree_J.count[i] == bohrTree_J.patterns.size())
			results.push_back(i+1);
	}
	return results;
}


void BohrTree_J::getParseStrApart(std::stringstream &pattern_stream, char joker)
{
	std::string tmp;
	int len = 0;
	while(getline(pattern_stream, tmp, joker))
	{
		if(tmp.size()>0)
		{
			len+=tmp.size();
			lengths.push_back(len);
			addStrToBohr(tmp);
		}
		len++;
	}
}


Bohr_vertex_J::Bohr_vertex_J(char symbol, int parrent_link) :
	parrent_link(parrent_link), 
	symbol(symbol), 
	is_end_of_pattern(false),
	suffix_link(-1),
	correct_suffix_link(-1)
{
	next_vertices[symbol] = -1;
	moves[symbol] = -1;
}


BohrTree_J::BohrTree_J(size_t size)
{
	bohr.push_back(Bohr_vertex_J(0, '$'));
	count = std::vector<int>(size, 0);
}


void BohrTree_J::addStrToBohr(const std::string &pattern)
{
	int vertexNum = 0;

	for (size_t i = 0; i < pattern.size(); i++)
	{
		char symb = pattern[i];
		if (bohr[vertexNum].next_vertices.find(symb) == bohr[vertexNum].next_vertices.end() ||
			bohr[vertexNum].next_vertices[symb] == -1)
		{
			bohr.push_back(Bohr_vertex_J(symb, vertexNum));
			bohr[vertexNum].next_vertices[symb] = bohr.size() - 1;
		}
		vertexNum = bohr[vertexNum].next_vertices[symb];
	}
	bohr[vertexNum].is_end_of_pattern = true;
	patterns.push_back(pattern);
	bohr[vertexNum].pattern_number.push_back(patterns.size()-1);

}


int BohrTree_J::getSuffixLink(int vertexNum)
{
	if (bohr[vertexNum].suffix_link == -1)
	{
		if (vertexNum == 0 || bohr[vertexNum].parrent_link == 0)
			bohr[vertexNum].suffix_link = 0;
		else
			bohr[vertexNum].suffix_link = getAutoMove(getSuffixLink(bohr[vertexNum].parrent_link), bohr[vertexNum].symbol);
	}

	return bohr[vertexNum].suffix_link;
}


int BohrTree_J::getAutoMove(int vertexNum, char symbol)
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


int BohrTree_J::getCorrectSuffixLink(int vertexNum)
{
	if (bohr[vertexNum].correct_suffix_link == -1)
	{
		int tmpLink = getSuffixLink(vertexNum);
		if (tmpLink == 0)
			bohr[vertexNum].correct_suffix_link = 0;
		else
		{
			if (bohr[tmpLink].is_end_of_pattern)
				bohr[vertexNum].correct_suffix_link = tmpLink;
			else
				bohr[vertexNum].correct_suffix_link = getCorrectSuffixLink(tmpLink);
		}
	}
	return bohr[vertexNum].correct_suffix_link;
}


void BohrTree_J::check(int vertexNum, int i)
{
	for (int nextVertex = vertexNum; nextVertex != 0; nextVertex = getCorrectSuffixLink(nextVertex))
	{
		if (bohr[nextVertex].is_end_of_pattern)
		{	
			for (const auto &it : bohr[nextVertex].pattern_number)
			{
				if( static_cast<int>(i-lengths[it]) < count.size() )
					count[i-lengths[it]]++;
			}
		}
	}
}


void BohrTree_J::findIn(const std::string &text)
{
	int vertexNum = 0;
	for (size_t i = 0; i < text.size(); i++)
	{
		vertexNum = getAutoMove(vertexNum, text[i]);
		check(vertexNum, i + 1);
	}
}