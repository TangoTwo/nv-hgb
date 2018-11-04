#ifndef UE04_TEAM_H
#define UE04_TEAM_H

#include "list.h"

/**
 * @param metrics The array of applications.
 * @param applications The number of applications.
 * @return The list of possible compositions for
 *          teams of 4.
 */
list *get_all_team_compositions(double *metrics, int applications);

/**
 * @param all_teams The list of teams to consider.
 * @param metrics The metrics for all applications.
 * @param applications The number of applications.
 * @param average The average time of all metrics.
 * @return The best possible combination of teams
 *          with a minimum standard deviation to
 *          the average metric.
 */
list *get_best_combination_of_teams(list_handle *all_teams,
                                    const double *metrics,
                                    int applications,
                                    double average);

/**
 * @param metrics The data set.
 * @param applications The number of metrics
 *                      to use for calculation.
 * @return Returns the average of the supplied
 *          list of metrics.
 */
double get_average(const double *metrics, int applications);

#endif //!UE04_TEAM_H
