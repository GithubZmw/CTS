/**
 * @brief Registers with the TA to obtain system public parameters for subsequent threshold signature verification.
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return Returns 0 on success, -1 on failure
 */
int getParams(int argc, char *argv[]);


/**
 * @brief Verifies the threshold signature of the UAVs within the cluster.
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return Returns 0 on success, -1 on failure
 */
int verifySignature(int argc, char *argv[]);