#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

#include "starhole_common.cpp"

// Represents the area in which the particles move
static DirUpdate*   area;

// Holds the split probabilities for Carter particles in cells
static double*      splitProb;

// Holds final counts of particles in cells
static int* outArea;

// Configuration parameters
static int radius;
static int sim_steps;

// Initializes needed data structures
void initialize(int radius) {
    int rows = 2*radius+1;
    int cols = 2*radius+1;
    
    outArea = (int*)calloc(rows*cols,sizeof(int));
    splitProb = (double*)malloc(sizeof(double)*rows*cols);
    area = (DirUpdate*)malloc(sizeof(DirUpdate)*rows*cols);
    
    for(int i=-radius;i<radius+1;i++) {
        for(int j=-radius;j<radius+1;j++) {
            int offset = toOffset(i,j,radius);
            // pack split prob -- i,j
            splitProb[offset] = computeSplitProb(i,j,radius);
            // pack dir update -- i,j
            computeDirProb(i,j,radius,&(area[offset]));
        }
    }
}

// Returns the total number of particles descending from this call
// and increments the count at the right location
int walker(long int seed, int x, int y, int stepsremaining) {
    struct drand48_data seedbuf;
    srand48_r(seed, &seedbuf);
    int particles = 1;
    for( ; stepsremaining>0 ; stepsremaining-- ) {
        
        // Does the Carter particle split? If so, start the walk for the new one
        if(doesSplit(&seedbuf, splitProb, x, y, radius)) {
            //printf("spliting!\n");
            long int newseed;
            lrand48_r(&seedbuf, &newseed);
            particles += walker(seed + newseed, x, y, stepsremaining-1);
        }
        
        // Make the particle walk?
        updateLocation(&seedbuf, area, &x, &y, radius);
    }
    
    // record the final location
    outArea[toOffset(x,y,radius)] += 1;
    
    return particles;
}

int main(int argc, char** argv) {
    if(argc<6 || ((argc-4)%2 != 0)) {
        printf("Usage: %s <steps> <radius> <amount> <x1> <y1> ... <xN> <yN>\n",argv[0]);
        return 1;
    }

    printf("Attempting to setup initial state...\n");
    
    char* endptr;
    endptr = argv[1];
    sim_steps = (unsigned)strtol(argv[1],&endptr,10);
    if(*endptr != NULL || sim_steps < 0) {
        printf("Steps must be a positive integer\n");
        return 1;
    }
    endptr=argv[2];
    radius = (unsigned int)strtol(argv[2],&endptr,10);
    if(*endptr != NULL || radius>5000 || radius < 1) {
        printf("Radius must be a positive integer less than 5000\n");
        return 1;
    }
    
    endptr = argv[3];
    int amount = (int)strtol(argv[3],&endptr,10);
    if(*endptr != NULL || amount>10000 || amount < 1) {
        printf("Amount must be a positive integer less than 10000\n");
        return 1;
    }

    int coordPairs = (argc-4)/2;    
    int coords[coordPairs*2];
    for(int i=0; i<argc-4; i+=2) {
        endptr = argv[i+4];
        coords[i] = (int)strtol(argv[i+4],&endptr,10);
        if(*endptr != NULL ) {
            printf("Coordinate must fit in radius.");
            return 1;
        }
        endptr = argv[i+5];
        coords[i+1] = (int)strtol(argv[i+5],&endptr,10);
        if(*endptr != NULL || dist(coords[i],coords[i+1])>radius) {
            printf("Coordinate must fit in radius.");
            return 1;
        }
    }
    
    // Initialize simulation params
    initialize(radius);
    
    
    // Start initial walks
    printf("Starting the walks...\n");
    int totParticles = 0;
    for(int i=0;i<coordPairs*2;i+=2) {
        for(int j=0;j<amount;j++) {
            totParticles += walker(i+j, coords[i], coords[i+1], sim_steps);
        }
    }
    printf("Walks complete... finished with %d particles\n",totParticles);
   
    // Generate the output
    int rows = radius*2+1;
    int cols = radius*2+1;
 
    // find the cell with the most
    int max=0;
    for(int i=0;i<rows*cols; i++) {
        if(outArea[i]>max) {
            max=outArea[i];
        }
    }
    // Generate the output image
    printf("Mat out(%d, %d, %d)\n",radius*2+1, radius*2+1,CV_8UC3);
    Mat out(rows, cols, CV_8UC3);
    for(int i=0;i<radius*2+1;i++) {
        for(int j=0;j<radius*2+1;j++) {
            const size_t offset = i + (j*(radius*2+1));
            // paint the ring
            double ring=0.0;
            if(dist(i-radius,j-radius)<=radius) {
               ring = 1.0;
            }
            double intensity = sqrt((double)outArea[offset]/(double)max);

            if(intensity>0) {
            out.at<Vec3b>(i, j) = Vec3b(floor(intensity * 255.0),
                                        floor(intensity * 255.0),
                                        floor(intensity * 255.0));
            } else {
            out.at<Vec3b>(i, j) = Vec3b(floor(0 * 255.0),
                                        floor(0 * 255.0),
                                        floor(ring * 128.0));
            }
        }
    }
    imwrite("out.jpg", out);

    free(outArea);
    free(splitProb);
    free(area);
}
