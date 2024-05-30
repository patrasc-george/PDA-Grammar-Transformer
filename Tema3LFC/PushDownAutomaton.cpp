#include <algorithm>
#include <fstream>
#include <sstream>

#include "PushDownAutomaton.h"

void PushDownAutomaton::ReadAutomaton()
{
	std::ifstream g("Automat.txt");
	std::string aux, word;
	std::vector<std::string> splitLine;
	std::getline(g, Q);
	std::getline(g, Q);
	std::getline(g, Sigma);
	std::getline(g, Gamma);
	std::getline(g, Delta);
	std::getline(g, S);
	//std::getline(g, aux);
	std::getline(g, Z);
	std::getline(g, F);
	std::getline(g, aux);
	std::istringstream split(aux);
	while (std::getline(split, word, ' '))
		splitLine.push_back(word);
	Tranzitie.resize(splitLine.size() / 2);
	for (int i = 0; i < splitLine.size() / 2; i++)
	{
		Tranzitie[i].first = splitLine[i * 2];
		if (splitLine[i * 2 + 1] != "_")
			Tranzitie[i].second = splitLine[i * 2 + 1];
	}
	g.close();
}

bool PushDownAutomaton::VerifyAutomaton()
{
	Sigma.push_back('_');
	Gamma.push_back('_');
	if (Q.find(S) == std::string::npos)
		return false;
	if (Gamma.find(Z) == std::string::npos)
		return false;
	for (auto it : F)
		if (Q.find(it) == std::string::npos && it != '_')
			return false;
	for (auto i : Tranzitie)
	{
		if (Q.find(i.first[0]) == std::string::npos || Sigma.find(i.first[1]) == std::string::npos || Gamma.find(i.first[2]) == std::string::npos)
			return false;
		for (auto j : i.second)
			if (Q.find(j) == std::string::npos && Gamma.find(j) == std::string::npos)
				return false;
	}
	Sigma.pop_back();
	Gamma.pop_back();
	return true;
}

void PushDownAutomaton::PrintAutomaton() const
{
	int k;
	std::cout << "M = ({";
	for (int i = 0; i < Q.size(); i++)
	{
		std::cout << Q[i];
		if (i < Q.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}, {";
	}
	for (int i = 0; i < Sigma.size(); i++)
	{
		std::cout << Sigma[i];
		if (i < Sigma.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}, " << Delta << ", " << S << ", " << Z << ", {";
	}
	for (int i = 0; i < F.size(); i++)
	{
		std::cout << F[i];
		if (i < F.size() - 1)
			std::cout << ", ";
		else
			std::cout << "})" << std::endl;
	}
	for (auto it : Tranzitie)
		if (it.second.size() > 0)
		{
			std::cout << Delta << "(" << it.first[0] << ", " << it.first[1] << ", " << it.first[2] << ") = {";
			for (int j = 0; j < it.second.size(); j = j + k)
			{
				std::cout << "(" << it.second[j] << ", ";
				k = j + 1;
				while (k < it.second.size() && Q.find(it.second[k]) == std::string::npos)
				{
					std::cout << it.second[k];
					k++;
				}
				if (k < it.second.size() - 1)
					std::cout << "), ";
				else
					std::cout << ")}" << std::endl;
			}
		}
}

bool PushDownAutomaton::IsDeterministic()
{
	Sigma.push_back('_');
	Gamma.push_back('_');
	int nrStari;
	for (auto it : Tranzitie)
		if (it.second.size())
		{
			nrStari = 0;
			if (Q.find(it.first[0]) == std::string::npos || Sigma.find(it.first[1]) == std::string::npos || Gamma.find(it.first[2]) == std::string::npos)
				return false;
			for (auto i : it.second)
				if (Q.find(i) != std::string::npos)
					nrStari++;
			if (nrStari > 1)
				return false;
		}
	for (int i = 0; i < Tranzitie.size(); i++)
	{
		if (Tranzitie[i].second.size() && Tranzitie[i].first[1] == '_')
		{
			int k = i + 1;
			while (Tranzitie[i].first[0] == Tranzitie[k].first[0])
			{
				if (Tranzitie[k].second.size() && Tranzitie[i].first[2] == Tranzitie[k].first[2])
					return false;
				k++;
			}
		}
	}
	Sigma.pop_back();
	Gamma.pop_back();
	return true;
}