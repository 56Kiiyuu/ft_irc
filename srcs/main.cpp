#include "Message.hpp"

int main() {
	// Test 1 : Commande basique
	Message msg1("NsCK toto\r\n");
	msg1.printDebug();

	// Test 2 : Commande complexe avec paramètres et espaces
	Message msg2("PRIVMSG #general :Salut l'equipe, ça va ?\r\n");
	msg2.printDebug();

	// Test 3 : Commande avec des espaces superflus (le parser doit rester robuste)
	Message msg3("  USER   guest  0 *   :Mon Nom Complet   \r\n");
	msg3.printDebug();
	return 0;
}
