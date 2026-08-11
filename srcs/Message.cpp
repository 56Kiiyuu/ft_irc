#include "Message.hpp"
#include <sstream>
#include <iostream>

Message::Message(const std::string& rawLine) : _raw(rawLine)
{
	parse();
}

Message::~Message()
{
}

void	Message::parse()
{
	std::string line = _raw;
	//clear \r, \n, spaces
	while (!line.empty() && (line[line.size() - 1] == '\n' ||
							 line[line.size() - 1] == '\r' ||
							 line[line.size() - 1] == ' '  ||
							 line[line.size() - 1] == '\t'))
		line.erase(line.size() - 1);

	if (line.empty())
		return;

	if (line[0] == ':')
	{
		size_t spacePos = line.find(' ');
		if (spacePos != std::string::npos)
			line = line.substr(spacePos + 1);
		else
			return; // Ligne invalide si uniquement un préfixe
	}

	size_t colonPos = line.find(" :");
	std::string trailing = "";
	bool hasTrailing = false;
	if (colonPos != std::string::npos)
	{
		trailing = line.substr(colonPos + 2);
		line = line.substr(0, colonPos);
		hasTrailing = true;
	}

	std::stringstream ss(line);
	std::string token;
	if (ss >> token)
	{
		_command = token;
		for (size_t i = 0; i < _command.size(); ++i)
			_command[i] = static_cast<char>(std::toupper(_command[i]));
	}
	while (ss >> token)
	{
		_params.push_back(token);
	}
	if (hasTrailing)
	{
		_params.push_back(trailing);
	}
}

const std::string&	Message::getCommand() const {
	return _command;
}

const std::vector<std::string>&	Message::getParams() const {
	return _params;
}

/*
void Message::printDebug() const {
	std::cout << "--- Message Debug ---" << std::endl;
	std::cout << "Brut : " << _raw << std::endl;
	std::cout << "Cmd  : " << _command << std::endl;
	std::cout << "Args : ";
	for (size_t i = 0; i < _params.size(); ++i) {
		std::cout << "[" << _params[i] << "] ";
	}
	std::cout << "\n---------------------" << std::endl;
}
*/
