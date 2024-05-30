#include "Gramatica.h"

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

Grammar::Grammar()
{
	std::ifstream f("Gramatica.txt");
	std::string line, word;
	std::vector<std::string> splitLine;
	std::getline(f, Vn);
	std::getline(f, Vn);
	std::getline(f, Vt);
	std::getline(f, S);
	std::getline(f, line);
	std::istringstream split(line);
	while (std::getline(split, word, ' '))
		splitLine.push_back(word);
	for (int i = 0; i < splitLine.size(); i = i + 2)
		P.push_back(std::make_pair(splitLine[i], splitLine[i + 1]));
	f.close();
}

std::string Grammar::GetVn() const
{
	return Vn;
}

std::string Grammar::GetVt() const
{
	return Vt;
}

std::string Grammar::GetS() const
{
	return S;
}

std::vector<std::pair<std::string, std::string>> Grammar::GetP() const
{
	return P;
}

bool Grammar::Intersectie() const
{
	for (auto i : Vn)
		for (auto j : Vt)
			if (i == j)
				return false;
	return true;
}

bool Grammar::Start() const
{
	return Vn.find(S) != std::string::npos;
}

bool Grammar::ProductieNeterminal() const
{
	int k = 0;
	for (auto i : P)
		for (auto j : i.first)
			if (Vn.find(j) != std::string::npos)
			{
				k++;
				break;
			}
	return k == P.size();
}

bool Grammar::ProductieStart() const
{
	for (auto it : P)
		if (it.first == S)
			return true;
	return false;
}

bool Grammar::ProductieElemente() const
{
	std::string concatenare;
	for (auto it : P)
		concatenare = concatenare + it.first + it.second;
	for (auto it : concatenare)
		if (Vn.find(it) == std::string::npos && Vt.find(it) == std::string::npos)
			return false;
	return true;
}

bool Grammar::VerifyGrammar() const
{
	if (Intersectie() && Start() && ProductieNeterminal() && ProductieStart() && ProductieElemente())
		return true;
	return false;
}

bool Grammar::IsIDC() const
{
	if (!ProductieElemente())
		return false;
	for (auto it : P)
		if (it.first.size() != 1)
			return false;
	return true;
}

void Grammar::Rename(int& i)
{
	for (int j = 0; j < P.size(); j++)
		if (P[i].second == P[j].first)
		{
			int copieJ = j;
			while (j < P.size() && P[i].second == P[j].first)
			{
				if (P[j].second.size() == 1 && Vn.find(P[j].second) != std::string::npos)
					Rename(j);
				j++;
			}
			j = copieJ;
			while (j < P.size() && P[i].second == P[j].first)
			{
				P.insert(P.begin() + i, std::make_pair(P[i].first, P[j].second));
				i++;
				j = j + 2;
			}
			P.erase(P.begin() + i);
			break;
		}
}

void Grammar::SymbolsNoGenerateWords()
{
	int k, nrNonterminal;
	for (int i = 0; i < P.size(); i = k)
	{
		k = i;
		nrNonterminal = 0;
		while (k < P.size() && P[k].first == P[i].first)
		{
			for (auto j : P[k].second)
				if (j == P[k].first[0])
					nrNonterminal++;
			k++;
		}
		char nonterminalCurrent = P[i].first[0];
		if (nrNonterminal == k - i)
		{
			for (int j = 0; j < P.size(); j++)
				for (auto l : P[j].second)
					if (l == nonterminalCurrent)
					{
						P.erase(P.begin() + j);
						if (j <= k)
							k--;
						j--;
					}
			Vn.erase(std::find(Vn.begin(), Vn.end(), nonterminalCurrent));
			SymbolsNoGenerateWords();
		}
	}
}

void Grammar::InaccessibleSymbols()
{
	int k;
	bool continueSearch = false;
	std::unordered_map<char, bool> prezenta;
	for (const auto& it : Vn)
		if (it != S[0])
			prezenta.insert(std::pair<char, bool>(it, false));
		else
			prezenta.insert(std::pair<char, bool>(it, true));
	for (const auto& i : P)
		for (const auto& j : i.second)
			if (Vn.find(j) != std::string::npos && j != i.first[0] && !prezenta.find(j)->second)
				prezenta.find(j)->second = true;
	for (auto it = prezenta.begin(); it != prezenta.end(); it++)
		if (!it->second)
		{
			continueSearch = true;
			for (int i = 0; i < P.size(); i++)
				if (P[i].first[0] == it->first)
				{
					k = i;
					while (k < P.size() && P[k].first[0] == it->first)
						k++;
					P.erase(P.begin() + i, P.begin() + k);
				}
				else
				{
					for (auto j : P[i].second)
						if (j == it->first)
						{
							P.erase(P.begin() + i);
							i--;
						}
				}
			Vn.erase(std::find(Vn.begin(), Vn.end(), it->first));
		}
	if (continueSearch)
		InaccessibleSymbols();
}

void Grammar::SimplifyGrammar()
{
	for (int i = 0; i < P.size(); i++)
	{
		if (P[i].second.size() == 1 && Vn.find(P[i].second) != std::string::npos)
			Rename(i);
	}
	SymbolsNoGenerateWords();
	InaccessibleSymbols();
}

void Grammar::PrintGrammar() const
{
	std::cout << "G = ({";
	for (int i = 0; i < Vn.size(); i++)
	{
		std::cout << Vn[i];
		if (i < Vn.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}, {";
	}
	for (int i = 0; i < Vt.size(); i++)
	{
		std::cout << Vt[i];
		if (i < Vt.size() - 1)
			std::cout << ", ";
		else
			std::cout << "}, " << S << ", P), P:" << std::endl;
	}
	for (auto it : P)
		std::cout << it.first << " -> " << it.second << std::endl;
}

int random(std::vector<int> v)
{
	srand(time(0));
	return v[rand() % v.size()];
}

bool VerificareFinal(std::string Vn, const std::string& cuvant)
{
	for (auto it : cuvant)
		if (Vn.find(it) != std::string::npos)
			return true;
	return false;
}

std::string Grammar::GenerateWord() const
{
	std::string anterior = S, copyAnterior;
	std::vector<int> arrRandom;
	int pozitie, dim, k = 0;
	std::pair<std::string, std::string> productie;
	while (VerificareFinal(Vn, anterior))
	{
		std::cout << anterior << " => ";
		for (int i = 0; i < P.size(); i++)
			if (anterior.find(P[i].first) != std::string::npos)
				arrRandom.push_back(i);
		productie = P[random(arrRandom)];
		arrRandom.clear();
		copyAnterior = anterior;
		while (copyAnterior.find(productie.first) != std::string::npos)
		{
			arrRandom.push_back(copyAnterior.find(productie.first));
			dim = copyAnterior.find(productie.first);
			for (int i = k; i <= dim; i++)
				copyAnterior[i] = '_';
			k = dim + 1;
		}
		k = 0;
		pozitie = random(arrRandom);
		arrRandom.clear();
		anterior.erase(pozitie, productie.first.size());
		anterior.insert(pozitie, productie.second);
	}
	std::cout << anterior << std::endl;
	return anterior;
}
