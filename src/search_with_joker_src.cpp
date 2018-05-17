#include "../hdr/search_with_joker_hdr.h"

std::vector<int> search_with_joker(std::string text, std::string jokerPattern, char joker)
{
	strToAlphabet(text, joker);
	strToAlphabet(jokerPattern, joker);

	std::vector<std::string> patterns;
	std::vector<size_t> lengths;
	getParseStrApart(patterns, lengths, jokerPattern);

	BohrTree_J bohrTree_J;
	for (size_t i = 0; i<patterns.size(); i++)
		bohrTree_J.addStrToBohr(patterns[i], i);

	return bohrTree_J.findIn(text, patterns, lengths, jokerPattern.size());
}


void strToAlphabet(std::string &str, char joker)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == joker)
		{
			str[i] = UNDEF;
			continue;
		}

		switch (str[i])
		{
		case 'A': 
		 	str[i] = A;
		 	break;
		case 'C': 
		 	str[i] = C;			
		 	break;
		case 'G': 
		 	str[i] = G;			
		 	break;
		case 'T': 
		 	str[i] = T;			
		 	break;
		case 'N': 
		 	str[i] = N;			
		 	break;
		default:  
		 	break;
		}
	}
}


void getParseStrApart(std::vector<std::string> &patterns, 
					  std::vector<size_t> &lengths, 
					  std::string &jokerPattern)
{
	std::string tmp;

	for (size_t i = 0; i < jokerPattern.size(); i++)
	{
		if (jokerPattern[i] == UNDEF)
		{
			if (tmp.size() > 0)
			{
				patterns.push_back(tmp);
				lengths.push_back(i);
				tmp.clear();
			}
			continue;
		}
		else
			tmp.push_back(jokerPattern[i]);

		if ((i == (jokerPattern.size() - 1)) && tmp.size())
		{
			patterns.push_back(tmp);
			lengths.push_back(i + 1);
		}
	}
}


Bohr_vertex_J::Bohr_vertex_J(char symbol, int parrent_link) : parrent_link(parrent_link), symbol(symbol), is_end_of_pattern(false)
{
	memset(this->next_vertices, 255, sizeof(this->next_vertices) + sizeof(this->moves)+(sizeof(int)*2));
}


BohrTree_J::BohrTree_J()
{
	bohr.push_back(Bohr_vertex_J(UNDEF));
}


void BohrTree_J::addStrToBohr(const std::string &pattern, int numberOfPattern)
{
	int vertexNum = 0;

	for (size_t i = 0; i < pattern.size(); i++)
	{
		if (bohr[vertexNum].next_vertices[pattern[i]] == -1)
		{
			bohr.push_back(Bohr_vertex_J(pattern[i], vertexNum));
			bohr[vertexNum].next_vertices[pattern[i]] = bohr.size() - 1;
		}
		vertexNum = bohr[vertexNum].next_vertices[pattern[i]];

	}

	bohr[vertexNum].pattern_number.push_back(numberOfPattern);
	bohr[vertexNum].is_end_of_pattern = true;

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
	if (bohr[vertexNum].moves[symbol] == -1)
	{
		if (bohr[vertexNum].next_vertices[symbol] != -1)
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
			if (bohr[tmpLink].is_end_of_pattern == true)
				bohr[vertexNum].correct_suffix_link = tmpLink;
			else
				bohr[vertexNum].correct_suffix_link = getCorrectSuffixLink(tmpLink);
		}
	}
	return bohr[vertexNum].correct_suffix_link;
}


void BohrTree_J::check(int vertexNum, int i, 
		   std::vector<std::string> &patterns, 
		   std::vector<int> &results,
		   std::vector<int> &background, 
		   std::vector<size_t> &lengths, 
		   size_t jokerLength)
{
	for (int nextVertex = vertexNum; nextVertex != 0; nextVertex = getCorrectSuffixLink(nextVertex))
	{
		if (bohr[nextVertex].is_end_of_pattern)
		{	
			for (int m = 0; m < bohr[nextVertex].pattern_number.size(); m++)
			{
				if((int)(i-lengths[bohr[nextVertex].pattern_number[m]]) >= 0)
					if ((++background[i-lengths[bohr[nextVertex].pattern_number[m]]]) == patterns.size())
					{
						if(background.size()-(i - lengths[bohr[nextVertex].pattern_number[m]]) >= jokerLength)
							results.push_back(i - lengths[bohr[nextVertex].pattern_number[m]] + 1);
					}
			}
		}
	}
}


std::vector<int> BohrTree_J::findIn(const std::string &text, 
							  std::vector<std::string> &patterns,
							  std::vector<size_t> &lengths, 
							  size_t jokerLength)
{
	std::vector<int> results;
	std::vector<int> background(text.size(), 0);
	int vertexNum = 0;
	
	for (size_t i = 0; i < text.size(); i++)
	{
		vertexNum = getAutoMove(vertexNum, text[i]);
		check(vertexNum, i + 1, patterns, results, background, lengths, jokerLength);
	}

	return results;
}