// Michael Landes
// GaTech : GOS : Project 1
///////////////////////////

#ifndef ML_SERVER
#define ML_SERVER

/// (port number, directory to serve, number of threads) ///
int ml_server(const unsigned short int, const char*, const unsigned int);

/// ret reference to root directory ///
char* ml_server_getRootDir();

#endif
