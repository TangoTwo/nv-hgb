#include <stdio.h>
#include <stdlib.h>

/**
 * All types of fuel the program covers.
 */
typedef enum
{
    PETROL = 1,
    DIESEL = 2
} FuelType;

/**
 * Prints a short guide for the command line
 * interface.
 */
void print_cli_guide()
{
    printf("Program usage: nova <consumption> <fuel type>\n");
    printf("\t- <consumption> = average consumption per 100km.\n");
    printf("\t- <fuel type> = the type of fuel the car model uses (1 = Diesel, 2 = Petrol).");
}

/**
 * Calculates the NOVA according to the formula
 * specified on the exercise sheet.
 * @param avg_consumption The average consumption of a particular car model.
 * @param fuel_type The type of fuel that model uses.
 */
double nova(double avg_consumption, FuelType fuel_type)
{
    int fuel_specific_coefficient = 0;
    switch (fuel_type) {
        case PETROL:
            fuel_specific_coefficient = 3;
            break;
        case DIESEL:
            fuel_specific_coefficient = 2;
            break;
        default:
            printf("Unknown fuel type: %d\n", fuel_type);
            return 0;
    }
    return (avg_consumption - fuel_specific_coefficient) * 200;
}

/*
 * argc is expected to be equal to 3
 * argv[1] is expected to be the average consumption (a positive real number)
 * argv[2] is expected to be the type of fuel for a car model (1 | 2)
 */
int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Invalid number of parameters!\n");
        print_cli_guide();
        return EXIT_FAILURE;
    }

    double avg_consumption = atof(argv[1]); // NOLINT(cert-err34-c)
    if (avg_consumption <= 0) {
        printf("The average fuel consumption / 100km should be a positive number.\n");
        return EXIT_FAILURE;
    }

    FuelType fuel_type = atoi(argv[2]); // NOLINT(cert-err34-c)
    if (fuel_type < 1 || fuel_type > 2) {
        printf("The available fuel types are: Diesel (1), Petrol (2).\n");
        print_cli_guide();
        return EXIT_FAILURE;
    }

    double result = nova(avg_consumption, fuel_type);
    printf("The Nova for the specified car is %.2f\n", result);

    return EXIT_SUCCESS;
}