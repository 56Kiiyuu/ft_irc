#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>

class Message
{
	private:
		std::string					_raw;		//ligne origine
		std::string					_command;	//cmd en MAJ
		std::vector<std::string>	_params;	//liste de parametres
	public:
		Message();
		Message(const std::string& rawline);
		~Message();

		const std::string&			getRaw() const;
		const std::string&			getCommand() const;
		const std::vector<std::string>&	getParams() const;

		void						parse();
		void						printDebug() const;
};

#endif
