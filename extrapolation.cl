__kernel void vector_add(__global double *population,
			 __global double *time,
			 __global int *timeCenter,
			 __global double *yT,
			 __global double *t2)
{

    // Get the index of the current element
    int i = get_global_id(0);

    yT[i] = population[i] * (i - *timeCenter + 1);
    printf("pop: %d, i: %d, tc: %d", population[i], i, timeCenter);
    t2[i] = (time[i] - time[*timeCenter])*(time[i] - time[*timeCenter]);
}
