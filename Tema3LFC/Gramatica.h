#pragma once
#include <iostream>
#include <vector>

class Grammar
{
public:
	Grammar();

	std::string GetVn() const;
	std::string GetVt() const;
	std::string GetS() const;
	std::vector<std::pair<std::string, std::string>> GetP() const;

	bool Intersectie() const;
	bool Start() const;
	bool ProductieNeterminal() const;
	bool ProductieStart() const;
	bool ProductieElemente() const;

	bool VerifyGrammar() const;
	bool IsIDC() const;
	void Rename(int& i);
	void SymbolsNoGenerateWords();
	void InaccessibleSymbols();
	void SimplifyGrammar();
	void PrintGrammar() const;
	std::string GenerateWord() const;

private:
	std::string Vn;
	std::string Vt;
	std::string S;
	std::vector<std::pair<std::string, std::string>> P;
};

