#ifndef _enum_classes_h
#define _enum_classes_h

enum client_status_t
{
      CLI_BLS = 0,
      CLI_BAN,
      CLI_SRV,
      CLI_ADV
};

enum ttt_board_status_t
{
    GOOD_MOVE = 4,
    PONE_WIN,
    PTWO_WIN,
    TTT_ODD,
    NO_WIN
};

enum ttt_board_err_t
{
  BAD_MOVE = 9,
  BAD_TURN
};

enum player_err_status_t
{
  PL_BAD_FORMAT = 11
};

enum user_err_status_t
{
  US_BAD_FORMAT = 12
};

enum user_status_t
{

  TAKEN = 13,
  BUSY,
  NONEX
};

enum game_board_status_t
{
    CORRECT_MOVE = 16,
    WIN_PONE,
    WIN_PTWO,
    WIN_ODD
};

enum game_board_err_t
{
  MOVE_BAD = 20,
  TURN_BAD
};

enum socket_status_t
{
  BAD_READ = 22
};

#endif /* _enum_classes_h */
