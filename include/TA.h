using namespace std;

/**
 * @brief Initializes the system parameters and generates the public key.
 */
void initParams();

/**
 * @brief Starts the server responsible for handling registration requests from other entities.
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return Returns 1 if the server starts successfully, otherwise returns -1
 */
int startServerTA(int argc, char *argv[]);