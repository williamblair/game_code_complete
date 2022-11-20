#ifndef GCC4_BASE_GAME_STATE_H_INCLUDED
#define GCC4_BASE_GAME_STATE_H_INCLUDED

enum BaseGameState
{
    BGS_Invalid,
    BGS_Initializing,
    BGS_MainMenu,
    BGS_WaitingForPlayers,
    BGS_LoadingGameEnvironment,
    BGS_WaitingForPlayersToLoadEnvironment,
    BGS_SpawnPlayersActors,
    BGS_Running
};

#endif // GCC4_BASE_GAME_STATE_H_INCLUDED
