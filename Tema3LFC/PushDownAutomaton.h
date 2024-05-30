#pragma once
#include <iostream>
#include <vector>

class PushDownAutomaton
{
public:
	void ReadAutomaton();
	bool VerifyAutomaton();
	void PrintAutomaton() const;
	bool IsDeterministic();

private:
	std::string Q;
	std::string Sigma;
	std::string Gamma;
	std::string Delta;
	std::string S;
	std::string Z;
	std::string F;
	std::vector<std::pair<std::string, std::string>> Tranzitie;
};