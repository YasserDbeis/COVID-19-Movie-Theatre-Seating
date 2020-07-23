//imports

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;    //setting namespace to be standard

// global 1D array to hold scores
int* scoreTable= NULL;

//forward declarations
bool isSafe(int xPos, int yPos, int startX, int startY, int** sol, float distSeats, float distRows, int rows, int cols);
bool solve(int rows, int cols, float distSeats, float distRows);
void printSol(int** sol, int rows, int cols, int numSol);
bool explore(int startX, int startY, int** sol, int rows, int cols, int (&moveX)[8], int (&moveY)[8], float distSeats, float distRows);
int scoreArr(int** sol, int rows, int cols, int numSol);

int main(int argc, char** argv){
    
    int rows = -1;  //taking in input, making sure rows, columns, distance betweens rows and seats are all above 0 values
    int cols = -1;
    float distSeats = -1;
    float distRows = -1;

    while(rows <= 0){
        cout << "Rows: ";
        cin >> rows;

        if(rows <= 0){
            cout << "Please enter a positive integer row value." << endl;
            continue;
        }

    }

    while(cols <= 0){
        cout << "Columns: ";
        cin >> cols;

        if(cols <= 0){
            cout << "Please enter a positive integer column value" << endl;
            continue;
        }

    }

    while(distSeats <= 0){
        
        cout << "Distance between seats in meters: ";
        cin >> distSeats;

        if(distSeats <= 0){
            cout << "Please enter a positive value distance between seats." << endl;
            continue;
        }

    }

    while(distRows <= 0){
        cout << "Distance between each row in meters: ";
        cin >> distRows;

        if(distRows <= 0){
            cout << "Please enter a positive value distance between rows." << endl;
            continue;
        }

    }

    if(rows == 1 && cols == 1){
        cout << "Stop worrying about COVID-19 and get some friends!" << endl;
        return 0;
    }


    scoreTable = new int[sizeof(int) * cols];          //allocation of memory for global scores table

    solve(rows, cols, distSeats, distRows);             //call solve function to iterate through first row and retrieve seating solution from each spot in first row

    return 0;       //execute and terminate at success
}

bool solve(int rows, int cols, float distSeats, float distRows){        //function to iterate through first row and retrieve seating solution from each spot in first row

    int moveX[8] = {0, 1, -1, 0, 1, -1, -1, 1};     //defining al possible movements (8) from any given 2D array element
    int moveY[8] = {1, 0, 0, -1, -1, 1, -1, 1};


    for(int i = 0; i < 8; i++){                     //determine the minimum distance jump for each direction given seat and row distance (altering movement vector arrays)
        if(distSeats < 1.5 && distSeats != 0){

            int mult = ceil(1.5 / distSeats);

            moveX[i] *= mult;

        }
        if(distRows < 1.5 && distRows != 0){

            int mult = ceil(1.5 / distRows);

            moveY[i] *= mult;

        }
        
    }
  
    int** sol = new int*[rows];             //dynamically allocate solution 2D array to store seating arrangements 
    for(int i = 0; i < rows; i++){
        sol[i] = new int[cols];
    }

    // float hyp = sqrt(pow(distSeats, 2) + pow(distRows, 2)); 

    for(int i = 0; i < cols; i++){

        for(int k= 0; k < rows; k++){
            for(int j = 0; j < cols; j++){
                sol[k][j] =0;
            }
        }

        sol[0][i] = 1;  //hard coding 0, 0 as start 
        // count1 = 2;

        cout << endl;
        cout << "ARRANGEMENT: " << i + 1 << endl;
        if(explore(i, 0, sol, rows, cols, moveX, moveY, distSeats, distRows) == false){

            //final touches

            if(distSeats < 1.5 && distRows < 1.5){
                
                int halfRow = ceil(1.5/distRows);
                int halfSeat = ceil(1.5/distSeats);
                
                float hyp = sqrt(pow(distRows * (halfRow/2), 2) + pow(distSeats * (halfSeat/2), 2));
                
                // cout << "1 " << halfRow << " " << halfSeat << " " << hyp << endl;

                if(halfRow % 2 == 0 && halfSeat  % 2 == 0 && hyp >= 1.5){

                    halfRow /= 2;
                    halfSeat /= 2;

                    // cout << "halfRow: " << halfRow << " halfSeat: " << halfSeat << endl;

                    for(int r = 0; r < rows; r += (halfRow * 2)){
                        for(int c = 0; c < cols; c++){
                            if(sol[r][c] == 1 && r + halfRow < rows && c + halfSeat < cols){
                                sol[r + halfRow][c + halfSeat] = 1;
                            }
                            if(sol[r][c] == 1 && r + halfRow < rows && c - halfSeat < cols){
                                sol[r + halfRow][c - halfSeat] = 1;
                            }
                        }
                        
                    }

                }
 
            }

            printSol(sol, rows, cols, i);

        }
    }

    int highestScore = 0;
    for(int m = 0; m < cols; m++){
        if(scoreTable[m] > scoreTable[highestScore]){
            highestScore = m;
        }
    }

    delete[] scoreTable;
    for(int i = 0; i < rows; i++){
        delete[] sol[i];
    }
    delete[] sol;

    cout << "SEATING ARRANGEMENT #" << highestScore + 1 << " IS MOST OPTIMAL OR TIED FOR MOST OPTIMAL" << endl;

    return true;

}

bool explore(int startX, int startY, int** sol, int rows, int cols, int (&moveX)[8], int (&moveY)[8], float distSeats, float distRows){

    for(int i = 0; i < 8; i++){
        
        // count1++;

        int xPos = startX + moveX[i];
        int yPos = startY + moveY[i];

        if (isSafe(xPos, yPos, startX, startY, sol, distSeats, distRows, rows, cols)){
            sol[yPos][xPos] = 1;


            if(explore(xPos, yPos, sol, rows, cols, moveX, moveY, distSeats, distRows)){
                return true;
            }

        }
    
    }
    return false;    

}

bool isSafe(int xPos, int yPos, int startX, int startY, int** sol, float distSeats, float distRows, int rows, int cols){
    
    if(xPos >= 0 && yPos >=0 && xPos < cols && yPos < rows && sol[yPos][xPos] == 0){ //
        float dist = sqrt( pow((xPos - startX) * distSeats, 2) + pow((yPos - startY) * distRows, 2));


        if(dist >= 1.5){
            return true;       
        }
 
    }

    return false;

}

void printSol(int** sol, int rows, int cols, int numSol){

    int score = scoreArr(sol, rows, cols, numSol);

    scoreTable[numSol] = score;

    cout << "SCORE: " << score << endl << endl;;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cout << sol[i][j] << " ";
        }
        cout << endl;
    }


    cout << endl << "------------------------" << endl;

}


int scoreArr(int** sol, int rows, int cols, int numSol){

    int score = 0;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(sol[i][j] == 1){
                score++;
            }
        }
    }

    return score;
}

