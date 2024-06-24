# Monte Carlo Method:
Suppose we toss darts randomly at a square with sides of 2 units in length. Inside the square we have a dartboard whose bullseye is at the origin. The radius of the dartboard is 1 unit, and therefore its area is pi square units. If the points that are hit by the darts are uniformly distributed (and we always hit the square), then the number of darts that hit inside the circle should approximately satisfy the equation:
</br> number in circle/total number of tosses = pi/4
</br> since the ratio of the area of the circle to the area of the square is pi/4. We can use this formula to estimate the value of pi with a random number generator:
</br> numberInCircle = 0;
</br> for (toss = 0; toss < number_of_tosses; toss++) {
</br> x = random double between -1 and 1;
</br> y = random double between -1 and 1;
</br> distance.squared = x*x + y*y;
</br> if (distance.squared <= 1)
</br> numberInCircle++:
</br> }
</br> PI_Estimation = 4.0*numberInCircle/ (double)number_of_tosses
### Written an MPI program to solve the problem with number of processes (2- 8 processes) -you can cahnge number of processes-, number_of_tosses = 100,000.
in attached .cpp file
