#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


/*
 * Assessment 3 for PHY2027
 * Author: 153071
 * Date: 10/12/2023
*/

/*
 This code will simulate a random lattice walk in 2 dimesnions in accordance witht he diffusion equation. The results will then be written to a file
 The index contains code related to the questions a,b,c asked to test the functionality of the code 
 */


//error if memory alloocation fails
#define ERROR -1
//Initalise constants of the diffusion equation in a struct prototype
typedef struct constants{
    int steps;          // Number of steps in the walk
    double delta_t;      // Time interval
    int delta_x;    // Step size x
    int delta_y;   // Step size y
    double diffusion_constant; //diffusion constant
} constants;

// Function to perform a one-dimensional random walk
int random_walk(int steps, int delta_x, int delta_y, double delta_t, double diffusion_constant);

// Function to generate the direction of the movement given probabilities
int random_step(double probability_move, double probability_remain);

int main() {
    // Seed the random number generator
    srand((unsigned int)time(NULL)); 
    //call struct in main function and define as ic- initial conditions
    constants ic = {1000,1.0,1,1,0.5};
    //check constants passed
    if ((ic.delta_t <= 0)||(ic.diffusion_constant <= 0)){
        return EXIT_FAILURE;
    } 
    // Perform the random walk with initial conditions
    random_walk(ic.steps, ic.delta_x,ic.delta_y, ic.delta_t, ic.diffusion_constant);
    return 0;
}

//define structure walker that can operate for x and y dimensions
typedef struct walker{
    int position;   //coordinate in x or y
    double probability_move;    
    double probability_remain;
} walker;

// Function to perform a one-dimensional random walk
int random_walk(int steps, int delta_x, int delta_y, double delta_t, double diffusion_constant) {
    // Allocate memory for position_history array
    int (*position_history)[2] = malloc(steps * sizeof(int[2]));
    // Check for allocation failure
    if (position_history == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return ERROR;
    }
    //provide walker x and y with positions and probabilities
    walker X = {0, diffusion_constant * delta_t / (delta_x * delta_x), (1 - 2 * (diffusion_constant * delta_t / (delta_x * delta_x)))};
    walker Y = {0, diffusion_constant * delta_t / (delta_y * delta_y), (1 - 2 * (diffusion_constant * delta_y / (delta_y * delta_y)))};
    // Check if the probability is acceptable with 2 * the probability, as both forwards and backwards are required
    if ((2 * X.probability_move > 1 || X.probability_move < 0) || (2 * Y.probability_move > 1 || Y.probability_move < 0)) {
        fprintf(stderr, "Error: Invalid probability\n");
        free(position_history);
        return ERROR;
    }
    //check if all probabilities sum to 1
    if (((2*X.probability_move + X.probability_remain)!= 1 )||((2*Y.probability_move + Y.probability_remain)!= 1)){
    printf("Error: Probabilities do not sum to 1\n");
    return ERROR;
    }
    //print statment that is good for showing user the results without looking at file, could be removed or reduced
    printf("p move x %f, the p stay %f\np move y %f, the p stay %f\n", 2 * X.probability_move, X.probability_remain, 2 * Y.probability_move, Y.probability_remain);

    // Loop through steps to perform random walk
    for (int i = 0; i < steps; i++) {
        // call random step for x and y and store associated positions to array
        int direction_x = random_step(X.probability_move, X.probability_remain);
        int direction_y = random_step(Y.probability_move, Y.probability_remain);
        X.position += direction_x * delta_x;
        Y.position += direction_y * delta_y;
        position_history[i][0] = X.position;
        position_history[i][1] = Y.position;
        printf("Step %d: Position x = %d Position y = %d\n", i + 1, X.position, Y.position);
    }
    // export the array to python
    FILE *file = fopen("position_history.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        free(position_history);
        return ERROR;
    }
    // iterate through array and write coordinates to text file
    for (int i = 0; i < steps; i++) {
        fprintf(file, "%d %d\n", position_history[i][0], position_history[i][1]);
    }
    fclose(file);
    // free the allocated memory
    free(position_history);
    //set pointer to NULL
    position_history = NULL;
    return 0;
}


// Function to generate the direction of the movement given probabilities
int random_step(double probability_move, double probability_remain) {
    // Generate a random value between 0 and 1
    double random_value = (double)rand() / RAND_MAX;
    // Determine the direction based on probabilities
    if (random_value < probability_move) {
        return 1;  
    } else if (random_value < (2 * probability_move)) {
        return -1; 
    } else {
        return 0; 
    }
}




//Index

//code checks have been remved and random step is not shown
//a,b vary only in array dimension and number of objects that exist, c varies more 
//implementing multiple walks

//b
/*
int main() {
    srand((unsigned int)time(NULL)); // Seed the random number generator

    constants ic = {100, 1.0, 1.0, 1.0,0.01};
    int num_walks = 10000;
    // defining array that will have final displacements
    double position_history[num_walks][2];

    // Perform the random walk
    for (int i = 0; i < num_walks; i++) {
        double displacement_x = random_walk(ic.steps, ic.delta_x, ic.delta_t, ic.diffusion_constant);
        double displacement_y = random_walk(ic.steps, ic.delta_y, ic.delta_t, ic.diffusion_constant);
        //printf("the x displacment is %f the y displacment is %f\n",displacement_x,displacement_y);
        position_history[i][0] = displacement_x;
        position_history[i][1] = displacement_y;
    }
    FILE *file = fopen("position_history_2d_analyse.txt", "w");
    for (int i = 0; i < num_walks; i++) {
        fprintf(file, "%f %f\n", position_history[i][0], position_history[i][1]);
    }
    fclose(file);
    return 0;
}

typedef struct walker {
    int position;
    double probability_move;
    double probability_remain;
} walker;

// Function to perform a one-dimensional random walk
int random_walk(int steps, int delta_x, double delta_t, double diffusion_constant) {
    // Allocate memory for position_history array
    int *position_history = malloc(steps * sizeof(int));
    // Check for allocation failure removed for ease
    // define walker x
    walker X = {0, diffusion_constant * delta_t / (delta_x * delta_x), (1 - 2 * (diffusion_constant * delta_t / (delta_x * delta_x)))};
    //checks removed for clarity of reading
    }
    // Loop through steps to perform random walk
    for (int i = 0; i < steps; i++) {
        // Return direction and pass in probability twice; this is so that forwards and backward steps could be different
        int direction_x = random_step(X.probability_move, X.probability_remain);
        X.position += direction_x * delta_x;
    }

    free(position_history);
    return X.position;
}

*/

//c
/*
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define ERROR -1
//Initalise constants of the diffusion equation in a struct prototype
typedef struct constants{
    int steps;          // Number of steps in the walk
    double delta_t;      // Time interval
    int delta_x;    // Step size x
    int delta_y;   // Step size y
    double diffusion_constant; //diffusion constant
} constants;

// Function to perform a one-dimensional random walk
int random_walk(int steps, int delta_x, double delta_t, double diffusion_constant);

// Function to generate the direction of the movement given probabilities
int random_step(double probability_move_right, double probability_move_left, double probability_remain);

int main() {
    // Seed the random number generator
    srand((unsigned int)time(NULL)); 
    constants ic = {100, 1.0, 1.0, 1.0,0.5};
    int num_walks = 10000;
    // defining array that will have final displacements
    double position_history[num_walks];

    // Perform the random walk
    for (int i = 0; i < num_walks; i++) {
        double displacement_x = random_walk(ic.steps, ic.delta_x, ic.delta_t, ic.diffusion_constant);
        //double displacement_y = random_walk(ic.steps, ic.delta_y, ic.delta_t, ic.diffusion_constant);
    // Perform the random walk
        //random_walk(ic.steps, ic.delta_x,ic.delta_y, ic.delta_t, ic.diffusion_constant);
        position_history[i] = displacement_x;
        //position_history[i][1] = displacement_y;
    }
    FILE *file = fopen("position_history_weighted_1d.txt", "w");
    
    for (int i = 0; i < num_walks; i++) {
        fprintf(file, "%f\n", position_history[i]);
    }
    fclose(file);
    return 0;
}

//define structure walker that can operate for x and y dimensions
typedef struct walker{
    int position;
    double probability_move_right;
    double probability_move_left;
    double probability_remain;
} walker;

// Function to perform a one-dimensional random walk
int random_walk(int steps, int delta_x, double delta_t, double diffusion_constant) {
    // Allocate memory for position_history array
    int (*position_history)[2] = malloc(steps * sizeof(int[2]));
    // Check for allocation failure
    //provide walker x and y with positions and probabilities
    double weight = 0.9;
    walker X = {0, weight, 1-weight, 0 };
    // Loop through steps to perform random walk
    for (int i = 0; i < steps; i++) {
        // Return direction and pass in probability twice; this is so that forwards and backward steps could be different
        int direction_x = random_step(X.probability_move_right,X.probability_move_left, X.probability_remain);
        X.position += direction_x * delta_x;
    }
    free(position_history);
    return X.position;
}
*/
