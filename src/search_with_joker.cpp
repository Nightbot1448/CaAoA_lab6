#include "../hdr/search_with_joker_hdr.h"

int main()
{
	setlocale(LC_ALL, "");
	std::wstring text;
	std::wstring joker;
	wchar_t jokerSymb;
	std::wcin >> text >> joker >> jokerSymb;

	std::vector <int> result = search_with_joker(text, joker, jokerSymb);

	for (size_t i = 0; i < result.size(); i++)
		std::wcout << result[i] << std::endl;
	
	return 0;
}