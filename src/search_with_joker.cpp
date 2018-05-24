#include "../hdr/search_with_joker_hdr.h"

int main()
{
	setlocale(LC_ALL, "");
	wstring text;
	wstring joker;
	wchar_t jokerSymb;
	wcin >> text >> joker >> jokerSymb;

	vector <int> result = search_with_joker(text, joker, jokerSymb);
	sort(result.begin(), result.end());

	for (size_t i = 0; i < result.size(); i++)
		wcout << result[i] << endl;
	
	return 0;
}