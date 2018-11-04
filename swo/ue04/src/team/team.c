#include "team.h"
#include <stdbool.h>
#include <math.h>
#include <float.h>
#include <stddef.h>
#include <stdio.h>

/**
 * @param existing_teams The list of existing teams.
 * @param team The new team.
 * @return True if the new team does not contain
 *          any members that belong to any of the
 *          existing teams.
 */
static bool is_team_composition_available(list_handle *existing_teams, list_node *team);

/**
 * @param teams The teams who's average metric to compare to
 *              the overall average.
 * @param metrics The metrics of all members of the specified teams.
 * @param average The overall average to use for comparison.
 * @return The standard deviation given the average
 *          metric and a list of teams.
 */
static double get_standard_deviation(list_handle *teams, const double *metrics, double average);

/**
 * Finds all possible compositions for
 * teams of 4 using the supplied application
 * metrics.
 * @param team_list The list which will contain all teams.
 * @param metrics The array of applications.
 * @param applications The number of applications.
 * @param team_as_arr The currently processed team composition.
 * @param curr_team_pos The currently processed team position.
 */
static void get_all_team_compositions_helper(list_handle *team_list,
                                             double *metrics,
                                             int applications,
                                             int *team_as_arr,
                                             int curr_team_pos);

/**
 *
 * @param all_teams The list of teams to consider.
 * @param new_teams The currently processed new team combination.
 * @param optimal_teams The stored optimum.
 * @param metrics The metrics for all applications.
 * @param applications The number of applications.
 * @param average_time The average time of all metrics.
 */
static void get_best_combination_of_teams_helper(list *all_teams,
                                                 list *new_teams,
                                                 list *optimal_teams,
                                                 const double *metrics,
                                                 int applications,
                                                 double average_time);

double get_average(const double *metrics, int applications)
{
    double sum = 0;
    for (int i = 0; i < applications; ++i) {
        sum += metrics[i];
    }
    return sum / applications;
}

list *get_all_team_compositions(double *metrics, int applications)
{
    list *plist = NULL;
    if (applications > 3) {
        plist = create_list();
        get_all_team_compositions_helper(plist, metrics, applications, (int[TEAM_SIZE]) {0}, 0);
    } else {
        printf("Cannot create teams: A minimum of 4 applications is required to form a team!\n");
    }
    return plist;
}

list *get_best_combination_of_teams(list_handle *all_teams, const double *metrics, int applications, double average)
{
    list *optimal_teams = NULL;
    if (applications > 3 && all_teams != NULL) {
        list *new_teams = create_list();
        optimal_teams = create_list();
        get_best_combination_of_teams_helper(all_teams, new_teams, optimal_teams, metrics, applications, average);
        delete_list(new_teams);
    } else if (applications <= 3) {
        printf("Cannot find best team combination: A minimum of 4 applications is required to form a team!\n");
    } else {
        // maybe supplied a valid number of applications but an invalid list of teams
        printf("Cannot find best team combination: Source set is empty.\n");
    }
    return optimal_teams;
}

// ---- PRIVATE FUNCTIONS ---- //

void get_all_team_compositions_helper(list_handle *team_list,
                                      double *metrics,
                                      int applications,
                                      int *team_as_arr,
                                      int curr_team_pos)
{
    for (int current_application = 0; current_application < applications; ++current_application) {
        if (metrics[current_application] > 0) {

            // a value is accepted for a partial solution
            // by removing its metric from the array (i. e.
            // setting it to 0) and adding the index of the
            // metric to the team array.
            double current_application_metric = metrics[current_application];
            metrics[current_application] -= current_application_metric;
            team_as_arr[curr_team_pos] = current_application;

            // teams of 4 -> 3 is last index!
            if (curr_team_pos == 3) {
                add_team_to_list(team_list, team_as_arr);
            } else {
                // we only need to change curr_team_pos since accepted applications (fixed values)
                // are denoted by their metric being 0!
                get_all_team_compositions_helper(team_list, metrics, applications, team_as_arr, curr_team_pos + 1);
            }

            // the backtracking part: add the metric back to the application for future uses
            metrics[current_application] += current_application_metric;
        }
    }
}

void get_best_combination_of_teams_helper(list *all_teams,
                                          list *new_teams,
                                          list *optimal_teams,
                                          const double *metrics,
                                          int applications,
                                          double average_time)
{
    list_node *pteam = all_teams->first;
    // for all remaining teams
    while (pteam != NULL) {
        if (is_team_composition_available(new_teams, pteam)) {
            add_team_to_list(new_teams, pteam->team_composition);

            if (new_teams->size * 4 + 4 > applications) { // if we can not form another team of 4
                double stddev_new = get_standard_deviation(new_teams, metrics, average_time);
                double stddev_optimal = get_standard_deviation(optimal_teams, metrics, average_time);
                if (stddev_new < stddev_optimal) { // if we found a new optimum
                    clear_list(optimal_teams);
                    clone_list(optimal_teams, new_teams);
                }
            } else {
                // if we still have enough applications to form
                // another team (>= 4 applic.), create a new handle
                // starting from team i + 1. That way the next recursion
                // level will not see the current meaning it won't be
                // considered as additional team composition.
                list *new_all_teams = get_list_handle(all_teams, pteam->next);
                get_best_combination_of_teams_helper(
                        new_all_teams, new_teams, optimal_teams, metrics, applications, average_time);
                delete_list_handle(new_all_teams);
            }

            // backtracking by removing the most recently added
            // team from the list.
            pop_list(new_teams);
        }
        pteam = pteam->next;
    }
}

double get_standard_deviation(list_handle *teams, const double *metrics, double average)
{
    if (teams == NULL || teams->size == 0) {
        // since this functions is only used for finding an optimum
        // invalid calls return the max double, meaning that the
        // check for a new optimum will succeed. For more details,
        // see get_best_combination_of_teams_helper
        return DBL_MAX;
    }
    double average_deviation_sum = 0;
    list_node *pnode = teams->first;
    while (pnode != NULL) {
        double sum = 0;
        for (int i = 0; i < 4; ++i) {
            // team_composition holds indices to the metrics array
            sum += metrics[pnode->team_composition[i]];
        }
        average_deviation_sum += fabs(average - (sum / 4));
        pnode = pnode->next;
    }
    return average_deviation_sum / teams->size;
}

bool is_team_composition_available(list_handle *existing_teams, list_node *team)
{
    bool available = true;
    // for each new team member
    for (int i = 0; i < 4; ++i) {
        int curr_dude = team->team_composition[i];
        // check if he is already in any of the other teams
        list_node *n = existing_teams->first;
        while (n != NULL && available) {
            for (int member = 0; member < 4; ++member) {
                available = available && curr_dude != n->team_composition[member];
            }
            n = n->next;
        }
    }
    return available;
}
