*This project has been created as part of the 42 curriculum by kevlim, gchalmel*

---

## Description

The goal of **ft_irc** is to build an IRC server. The server handles multiplexing using a single `poll()` loop to manage multiple simultaneous connections without blocking.

IRC is a text-based chat protocol which has proven itself valuable and useful. It is well-suited to running on many machines in a distributed fashion. A typical setup involves multiple servers connected in a distributed network. Messages are delivered through this network and state is maintained across it for the connected clients and active channels.

### Core Features
* **Non-Blocking I/O**
* **Authentication :** `PASS`, `NICK`, and `USER` commands.
* **Channel Management :** Full implementation of user commands (`JOIN`, `PART`, `PRIVMSG`)
* **Privilege Architecture :** Operator controls (`KICK`, `INVITE`, `TOPIC`, `MODE`) allowing designated channel operators (`@`) to enforce channel rules.

---

## Instructions

Launch IRC Server :
```bash
./ircserv <port> <password> (example : ./ircserv 6667 1234)
```

Connections :

```bash
nc 127.0.0.1 6667
```
**OR**
```bash
irssi -c 127.0.0.1 -p 6667 -w 1234 -n MyNickname
```

---

## Supported Commands

* **PASS**
* **NICK**
* **USER**
* **PRIVMSG**
* **JOIN**
* **TOPIC**
* **MODE**
* **INVITE**
* **KICK**
* **PART**
* **QUIT**

---

## Ressources

[Modern IRC Client Protocol](https://modern.ircdocs.horse) : guidelines used for the network architecture, message syntax, and numeric error replies.
[Vector Docs](https://cplusplus.com/reference/vector/vector/) : Documentation describing the various methods of the `vector` class.
[String Docs](https://cplusplus.com/reference/string/string/) : Documentation describing the various methods of the `string` class.
[Tutorial Sockets C](https://www.youtube.com/watch?v=s3o5tixMFho) : A short video explaining sockets using real-world examples.
