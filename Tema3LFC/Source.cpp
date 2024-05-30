#include <iostream>

#include "Gramatica.h"
#include "PushDownAutomaton.h"

int main()
{
	Grammar g;
	PushDownAutomaton p;
	p.ReadAutomaton();
	int x = 0;
	std::string word;
	if (g.VerifyGrammar() && g.IsIDC() && p.VerifyAutomaton())
		while (x != 5)
		{
			std::cout << "1. Afisarea gramaticii." << std::endl;
			std::cout << "2. Generarea unui numar n de cuvinte in gramatica." << std::endl;
			std::cout << "3. Afisarea gramaticii simplificate." << std::endl;
			std::cout << "4. Afisarea automatului." << std::endl;
			std::cout << "5. Exit" << std::endl;
			std::cin >> x;
			switch (x)
			{
			case 1:
				g.PrintGrammar();
				break;
			case 2:
				std::cout << "Numar de cuvinte: ";
				std::cin >> x;
				for (int i = 0; i < x; i++)
					g.GenerateWord();
				break;
			case 3:
				g.SimplifyGrammar();
				g.PrintGrammar();
				break;
			case 4:
				p.PrintAutomaton();
				break;
			}
			std::cout << std::endl;
		}
	return 0;
}