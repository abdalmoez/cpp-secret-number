# cpp-secret-number
Cpp secret number is a Server/Client websocket game. The player must detect the

The server waits for clients and once connected, choose a random number for them that they must find.
The server is a console application.
The client is a gui application.
### secret_number_server 
#### arguments

    Usage: secret_number_server [options]
    Secret number game server
    
    Options:
      -?, -h, --help                   Displays this help.
      -l, --limit <limit>              Max numbers of tries (-1 for unlimited)
      -b, --bounds <bounds>            Bounds of the secret number x,y
      -v, --verbose                    Verbose mode. Prints out more information.
      -m, --max-players <max-players>  Max number of players to support (default:
                                       1000).
      -p, --port <port>                The port the server should use. You will
                                       need to Port Forward in order for players to
                                       join your server from outside your LAN.
                                       (default: 4242)
#### Ranking system
The winner player can be ranked only with other players that solved the game with the same bounds. Top players are the ones who has less time.

### secret_number_client
Login:
[![N|Solid](https://github.com/abdalmoez/cpp-secret-number/blob/master/ss.jpg?raw=true)]()
Player can chose use bots to find the secret numbers or play by him self.
#### Bots
There is 3 levels of bots that player can use Newbie, Medium and Master. Each of them use different search method to find The secret number.

