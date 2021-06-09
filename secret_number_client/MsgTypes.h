#ifndef MSGTYPES_H
#define MSGTYPES_H

#define RC_GAME_CREATED            0x1000
#define RC_INVALID_ANSWER          0x1001
#define RC_GAMEOVER_LOSER          0x1002
#define RC_GAMEOVER_WINNER         0x1003

#define RC_TOP_PLAYERS             0x2000

#define RC_ACCEPTED_CONNECTION     0x7000
#define RC_SERVER_IS_FULL          0x7001
#define RC_INVALID_REQUEST         0x7002
#define RC_CANNOT_CREATE_GAME      0x7003

#define RC_KICKED                  0x8000
#define RC_BANNED                  0x8001

#define REQUEST_NEW_GAME           0x5000
#define REQUEST_ANSWER             0x5001
#define REQUEST_END_GAME           0x5002
#define REQUEST_QUIT               0x5003
#define REQUEST_TOP_SCORES         0x5004

#endif // MSGTYPES_H
