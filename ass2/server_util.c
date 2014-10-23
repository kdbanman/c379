
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

char * currtime()
{
        time_t curr;
        time(&curr);
        return asctime(gmtime(&curr));
}
