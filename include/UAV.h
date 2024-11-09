/**
 * The UAV registers with the TA to obtain system parameters and its own key.
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return Returns 1 if registration is successful; otherwise, returns -1
 */
int registerUAV(int argc, char *argv[]);

/**
 * The UAV starts a server to sign message for caller. *
 * @param argc  Number of command-line arguments
 * @param argv   Command-line arguments
 * @return  Returns 1 if the server starts successfully; otherwise, returns -1
 */
int startUAVServer(int argc, char *argv[]);