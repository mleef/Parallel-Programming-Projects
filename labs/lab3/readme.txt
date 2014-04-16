Marc Leef
CIS 410
4/16/13

TAU Profile Insight

There were a number of useful metrics that paraprof provided about the profile
data. The first surprising thing was that the amount of work done by each thread
from largest to smallest was 3,1,2, and 0, demonstrating that the distribution of work is not always as expected. The standard deviation was another useful value, providing insight into the load balancing of the program. The max-min value was another source of useful information, demonstrating how long the quickest thread had to wait relative to the slowest completing thread. Overall, this seems like a very useful tool for delving into the specifics of a parallell program's performance.
