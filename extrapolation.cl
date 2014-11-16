__kernel void calcExtrapolation(__global const double *population, __global const double *time, __global const int timeCenter, __global double sumYT, __global double sumT2) {

    // Get the index of the current element to be processed
    int i = get_global_id(0);

    // Do the operation
    sumYT += population[i] * (i - timeCenter + 1);
    sumT2 += (time[i] - time[timeCenter])*(time[i] - time[timeCenter]);
}
