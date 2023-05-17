# ft_irc
A C++ IRC-Server following the RFC 1459 IRC protocol

## Notes
This project was developed on macOS. Compatibility with other systems has not been tested and can not be guaranteed.

## Setup

Clone the repository onto your system and run `make`. 
If everything compiled well, you should have the executable `ircserv` in your directory. Run this executable to start the program.

## How to run

```bash
./ircserv <port> <password>
```
port: the port on that the server will listen for incoming connections
password: the password that clients have to provide to register with the server

Now any IRC Client that supports the RFC 1459 IRC protocol can connect to the server and use its functionalities.

Every new channel automatically has a user named "Behaviourbot", which will kick any user who writes any word on its "bad words" list and any user who writes a message that does not include any words on the "good words" list. These lists can be accessed with the command 'bhvlist' and manipulated with the commands 'addbad', 'addgood', 'rmbad' and 'rmgood'.

## Acknowledgements
created in cooperation with:

Dejan Zivanov - https://github.com/dejanzivanov

Margarete Müller - https://github.com/Madasanya
