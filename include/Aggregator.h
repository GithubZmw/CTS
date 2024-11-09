/**
 * @brief The aggregator, i.e., the UAV cluster head, retrieves the public parameters (pp) and its private key (sk).
 * @param argc Number of parameters
 * @param argv List of parameters
 * @return Returns 1 if the information is successfully retrieved, otherwise returns 0
 */
int getPpAndSk(int argc, char *argv[]);

/**
 * @brief The aggregator, i.e., the UAV cluster head, retrieves the signatures of the UAVs in the cluster. *
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return  0 on success, -1 on failure
 */
int collectParSigs(int argc, char *argv[]);

/**
 * @brief Starts the Tran service, which transforms UAV signatures into signatures that can be verified by the verifier.
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return Returns 0 on success, -1 on failure
 */
int startServerTran(int argc, char *argv[]);