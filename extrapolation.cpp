#include "extrapolation.h"
#include <math.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#ifdef __APPLE__ || defined(__MACOSX)
#include <OpenCL/opencl.hpp>
#else
#include <CL/cl.hpp>
#endif

Data exstrapolation(Data data)
{
    QFile inputFile("/home/echuraev/Uni/modern_computer_technology/draw_results/draftsman/extrapolation.cl");
    QString sourceStr = "";
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        sourceStr = in.readAll();
        inputFile.close();
    }

    int timeCenter = data.time.length()/2 + ((data.time.length()%2) ? 1 : 0);
    const int LIST_SIZE = data.time.length();
    double *population = new double [LIST_SIZE];
    double *time = new double [LIST_SIZE];
    int *timeCenterCL = new int [1];
    *timeCenterCL = timeCenter;
    for(int i = 0; i < LIST_SIZE; ++i)
    {
        population[i] = data.population.at(i);
        time[i] = data.time.at(i);
    }

    // Load the kernel source code into the array charSourceStr
    char * charSourceStr = new char [sourceStr.length()];
    strcpy(charSourceStr, sourceStr.toUtf8().constData());
    size_t sourceSize = strlen(charSourceStr);

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector
    cl_mem populationMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,
                                             LIST_SIZE * sizeof(double), NULL, &ret);
    cl_mem timeMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY,
                                       LIST_SIZE * sizeof(double), NULL, &ret);
    cl_mem timeCenterMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                             sizeof(int), NULL, &ret);
    cl_mem yTMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     LIST_SIZE * sizeof(double), NULL, &ret);
    cl_mem t2MemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     LIST_SIZE * sizeof(double), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, populationMemObj, CL_TRUE, 0,
                               LIST_SIZE * sizeof(double), population, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, timeMemObj, CL_TRUE, 0,
                               LIST_SIZE * sizeof(double), time, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, timeCenterMemObj, CL_TRUE, 0,
                               sizeof(int), timeCenterCL, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&charSourceStr,
                                                   (const size_t *)&sourceSize, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&populationMemObj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&timeMemObj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&timeCenterMemObj);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&yTMemObj);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&t2MemObj);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = LIST_SIZE; // Process the entire lists
    size_t local_item_size = 1; // Divide work items into groups of 1
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
                                 &global_item_size, &local_item_size,
                                 0, NULL, NULL);

    // Read the memory buffer on the device to the local variable
    double *yT = new double [LIST_SIZE];
    ret = clEnqueueReadBuffer(command_queue, yTMemObj, CL_TRUE, 0,
                              LIST_SIZE * sizeof(double), yT, 0, NULL, NULL);
    double *t2 = new double [LIST_SIZE];
    ret = clEnqueueReadBuffer(command_queue, t2MemObj, CL_TRUE, 0,
                              LIST_SIZE * sizeof(double), t2, 0, NULL, NULL);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(populationMemObj);
    ret = clReleaseMemObject(timeMemObj);
    ret = clReleaseMemObject(yTMemObj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    delete population;
    delete time;
    delete timeCenterCL;

    double sumY = 0;
    double sumYT = 0;
    double sumT2 = 0;

    for (int i (0); i < LIST_SIZE; ++i)
    {
        sumY += data.population.at(i);
        //qDebug() << yT[i] << "\t" << t2[i];
        sumYT += yT[i];
        sumT2 += t2[i];
    }
    delete yT;
    delete t2;
    double a0 = sumY/data.time.length();
    double a1 = sumYT/sumT2;

    Data ret_data;
    double timeStep = data.time.at(1) - data.time.at(0);
    for (int i (0); i < data.time.length() + timeCenter; ++i)
    {
        ret_data.time.append(i*timeStep);
        ret_data.population.append(a0 + a1*(i - timeCenter));
    }

    return ret_data;
}
