part1
# set the path to the file correctly!!
system hermite load "spline2.txt"

system hermite getArcLength 1
system hermite set point 18 0 0 0
system hermite set tangent 0 5 5 0
system hermite getArcLength 1             #answer ~50 


