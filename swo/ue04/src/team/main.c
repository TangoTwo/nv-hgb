#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "team.h"

void separator()
{
    printf("====================\n");
}

void run_test_exactly_2_teams()
{
    double metrics[] = { 10.34, 15.6,  12.59, 11.64,
                         14.0,  17.22, 13.1,  20.42  };

    // Implementation note: we actually do not need the exact metrics here, any arbitrary
    // array would do, since the function only uses it for backtracking flags
    list *all_teams = get_all_team_compositions(metrics, 8);

    double average = get_average(metrics, 8);
    list *optimal_teams = get_best_combination_of_teams(all_teams, metrics, 8, average);
    print_list(optimal_teams);
    delete_list(all_teams);
    delete_list(optimal_teams);
}

void run_test_uneven_applications()
{
    double metrics[] = { 10.34, 15.6,  12.59, 11.64,
                         14.0,  17.22, 13.1,  20.42,
                         24.5, 12.52 };
    list *all_teams = get_all_team_compositions(metrics, 10);
    double average = get_average(metrics, 10);
    list *optimal_teams = get_best_combination_of_teams(all_teams, metrics, 10, average);
    print_list(optimal_teams);
    delete_list(all_teams);
    delete_list(optimal_teams);
}

void run_test_only_1_team()
{
    double metrics[] = { 10.34, 14.0,  12.59, 11.64,
                         15.6 };
    list *all_teams = get_all_team_compositions(metrics, 5);
    double average = get_average(metrics, 5);
    list *optimal_teams = get_best_combination_of_teams(all_teams, metrics, 5, average);
    print_list(optimal_teams);
    delete_list(all_teams);
    delete_list(optimal_teams);
}

void run_test_less_than_4_applications()
{
    double metrics[] = { 10.34, 15.6, 12.5 };
    list *all_teams = get_all_team_compositions(metrics, 3);
    double average = get_average(metrics, 3);
    list *optimal_teams = get_best_combination_of_teams(all_teams, metrics, 3, average);
    print_list(optimal_teams);
    delete_list(all_teams);
    delete_list(optimal_teams);
}

void run_test_invalid_teams_for_best_combination()
{
    double metrics[] = { 10.34, 14.0,  12.59, 11.64,
                         15.6 };
    list *optimal_teams = get_best_combination_of_teams(NULL, metrics, 5, 0);
    print_list(optimal_teams);

    // note that I am still releasing
    // hypothetical memory, since this
    // could very well happen in a real
    // use case.
    delete_list(optimal_teams);
}

int main()
{
    separator();
    run_test_exactly_2_teams();
    separator();
    run_test_uneven_applications();
    separator();
    run_test_only_1_team();
    separator();
    run_test_less_than_4_applications();
    separator();
    run_test_invalid_teams_for_best_combination();
    separator();
    return EXIT_SUCCESS;
}
