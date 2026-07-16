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

	if (!line.empty() && line[line.size() - 1] == '\n')
		line.erase(line.size() - 1);
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	while (!line.empty()
			&& (line[line.size() - 1] == ' '
			|| line[line.size() - 1] == '\t'))
		line.erase(line.size() - 1);

	if (line.empty())
		return;

	if (!line.empty() && line[0] == ':')
	{
		size_t spacePos = line.find(' ');
		if (spacePos != std::string::npos)
			line = line.substr(spacePos + 1);
	}

	size_t colonPos = line.find(" :");
	std::string trailing = "";
	if (colonPos != std::string::npos)
	{
		trailing = line.substr(colonPos + 2);
		line = line.substr(0, colonPos);
	}

	std::stringstream ss(line);
	std::string token;
	if (ss >> token)
	{
		_command = token;
		for (size_t i = 0; i < _command.size(); ++i)
			_command[i] = std::toupper(_command[i]);
	}
	while (ss >> token)
	{
		_params.push_back(token);
	}
	if (colonPos != std::string::npos)
	{
		_params.push_back(trailing);
	}
}

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
