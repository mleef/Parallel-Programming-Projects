#ifndef STARHOLE_HELPERS
#define STARHOLE_HELPERS
// Represents the probability that a Carter particle will move to a nearby cell
// The structure is read as r < prob[i]; prob[i] is a summation of the probabilities
// of the cells checked earlier with the probability of the current cell
typedef struct _dirUpdate {
    double prob[9];
} DirUpdate;

// Helper function for writing out DirUpdate data
void dumpProb(double probdat[9]) {
    printf("%f\t%f\t%f\n",probdat[0],probdat[1],probdat[2]);
    printf("%f\t%f\t%f\n",probdat[3],probdat[4],probdat[5]);
    printf("%f\t%f\t%f\n",probdat[6],probdat[7],probdat[8]);
}


// distance from origin
double dist(int x, int y) {
    return sqrt(x*x + y*y);
}

// Maps between a coordinate plane with origin 0,0 and the linear array
int toOffset(int x, int y, int radius) {
    x = x + radius;
    y = y + radius;
    return x + (y * (2 * radius + 1));
}

// Gaussian distributions are nice
double gaussianFunc(double x, double x0, double a, double dev) {
    return a*exp(-(x-x0)*(x-x0)/(2*dev*dev));
}

// Split probabilities
double computeSplitProb(int x, int y, int radius) {
    return 0.001*(dist(x,y)/radius);
}

// Packs a DirUpdate struct for coordinates x,y on a plane of radius radius
void computeDirProb(int x, int y, int radius, DirUpdate* loc) {
    loc->prob[0] = dist(x-1,y+1);
    loc->prob[1] = dist(x,y+1);
    loc->prob[2] = dist(x+1,y+1);
    loc->prob[3] = dist(x-1,y);
    loc->prob[4] = dist(x,y);
    loc->prob[5] = dist(x+1,y);
    loc->prob[6] = dist(x-1,y-1);
    loc->prob[7] = dist(x,y-1);
    loc->prob[8] = dist(x+1,y-1);
    double acc = 0.0;
    for(int i=0;i<9;i++) {
        if(loc->prob[i] < radius) { 
            loc->prob[i] = 2*gaussianFunc(loc->prob[i],0,1,sqrt(radius)) + gaussianFunc(loc->prob[i],radius/2,1,sqrt(radius*4))+.01;
            acc+=loc->prob[i]; loc->prob[i] = acc; 
        } else { loc->prob[i] = 0.0; }
    }
    for(int i=0;i<9;i++) {
        loc->prob[i] = loc->prob[i]/acc;
    }
}

// Helper function to determine if a split occurs
int doesSplit(struct drand48_data* seedbuf, double* splitProb, int x, int y, int radius) {
    double r;
    drand48_r(seedbuf,&r);
    if(r < splitProb[toOffset(x,y,radius)]) {
        return 1;
    }
    return 0;
}

// Helper function to update a particle location
void updateLocation(struct drand48_data* seedbuf, DirUpdate* area, int* x, int *y, int radius) {
    double r;
    drand48_r(seedbuf,&r);
    DirUpdate d = area[toOffset(*x, *y, radius)];
    if(r<d.prob[0]) { *x = *x - 1; *y = *y + 1; } // ul
    else if(r<d.prob[1]) {              *y = *y + 1; } // u
    else if(r<d.prob[2]) { *x = *x + 1; *y = *y + 1; } // ur
    else if(r<d.prob[3]) { *x = *x - 1;              } // l
    else if(r<d.prob[4]) {                           } // 
    else if(r<d.prob[5]) { *x = *x + 1;              } // r
    else if(r<d.prob[6]) { *x = *x - 1; *y = *y - 1; } // ll
    else if(r<d.prob[7]) {              *y = *y - 1; } // l
    //if(d.prob[8]<r) { *x = *x + 1; *y = *y - 1; return; }
    // otherwise, lower right
    else { *x = *x + 1; *y = *y - 1; }
    
    return;
}
#endif
