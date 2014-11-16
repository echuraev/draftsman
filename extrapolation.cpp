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
        while ( !in.atEnd() )
        {
            sourceStr += in.readLine();
        }
        inputFile.close();
    }

    double sumY = 0;
    double sumYT = 0;
    double sumT2 = 0;
    int timeCenter = data.time.length()/2 + ((data.time.length()%2) ? 1 : 0);

    double * population = new double [data.population.length()];
    double * time = new double [data.time.length()];
    for (int i (0); i < data.population.length(); ++i)
    {
        population[i] = data.population.at(i);
        time[i] = data.time.at(i);
    }

    // Get platform and device information
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.size() == 0) {
        qDebug() << "Platform size 0\n";
    }
    /*cl_context_properties properties[] =
              { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
    cl::Context context(CL_DEVICE_TYPE_CPU, properties);
    cl::CommandQueue queue(context, device);
    */
    std::vector<cl::Device> devices;
    platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

    //std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    cl::Device device = devices[0];
    //qDebug() << "Device: " << device.getInfo<CL_DEVICE_NAME>();
    //For the selected device create a context
    std::vector<cl::Device> contextDevices;
    contextDevices.push_back(device);
    cl::Context context(contextDevices);

    //For the selected device create a context and command queue
    cl::CommandQueue queue(context, device);

    //Create memory buffers
    cl::Buffer populationBuf = cl::Buffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, data.population.length() * sizeof(double), population);
    cl::Buffer timeBuf = cl::Buffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, data.population.length() * sizeof(double), time);
    cl::Buffer timeCenterBuf = cl::Buffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, sizeof(int), &timeCenter);
    cl::Buffer sumYTBuf = cl::Buffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, sizeof(double), &sumYT);
    cl::Buffer sumT2Buf = cl::Buffer(context, CL_MEM_READ_WRITE|CL_MEM_COPY_HOST_PTR, sizeof(double), &sumT2);

    char * charSourceStr = new char [data.population.length() + 1];
    strcpy(charSourceStr, sourceStr.toUtf8().constData());
    strcat(charSourceStr, "\0");
    cl::Program::Sources source(1, std::make_pair(charSourceStr,strlen(charSourceStr)));
    cl::Program program = cl::Program(context, source);
    program.build(contextDevices);

    cl::Kernel kernel(program, "calcExtrapolation");

    kernel.setArg(0, populationBuf);
    kernel.setArg(1, timeBuf);
    kernel.setArg(2, timeCenterBuf);
    kernel.setArg(3, sumYTBuf);
    kernel.setArg(4, sumT2Buf);

    queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(data.population.length()), cl::NDRange(128));
    queue.finish();

    queue.enqueueReadBuffer(sumYTBuf, CL_TRUE, 0, sizeof(double), &sumYT);
    queue.enqueueReadBuffer(sumT2Buf, CL_TRUE, 0, sizeof(double), &sumT2);

    /*
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector
    cl_mem populationMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, data.population.length() * sizeof(double), NULL, &ret);
    cl_mem timeMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, data.time.length() * sizeof(double), NULL, &ret);
    cl_mem timeCenterMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int), NULL, &ret);
    cl_mem sumYTMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(double), NULL, &ret);
    cl_mem sumT2MemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(double), NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    ret = clEnqueueWriteBuffer(command_queue, populationMemObj, CL_TRUE, 0, data.population.length() * sizeof(double), population, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, timeMemObj, CL_TRUE, 0, data.time.length() * sizeof(double), time, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, timeCenterMemObj, CL_TRUE, 0, sizeof(int), &timeCenter, 0, NULL, NULL);

    int size = data.population.length() + 2;
    char * charSourceStr = new char [size-1];
    strcpy(charSourceStr, sourceStr.toUtf8().constData());
    strcat(charSourceStr, "\0");
    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&charSourceStr, (const size_t *)&size, &ret);

    // Build the program
    int code = 0;
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "calcExtrapolation", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&populationMemObj);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&timeMemObj);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&timeCenterMemObj);
    ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&sumYTMemObj);
    ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&sumT2MemObj);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = data.population.length(); // Process the entire lists
    size_t local_item_size = 64; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    double sumYT = 0;
    double sumT2 = 0;
    ret = clEnqueueReadBuffer(command_queue, sumYTMemObj, CL_TRUE, 0, sizeof(double), &sumYT, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, sumT2MemObj, CL_TRUE, 0, sizeof(double), &sumT2, 0, NULL, NULL);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(populationMemObj);
    ret = clReleaseMemObject(timeMemObj);
    ret = clReleaseMemObject(timeCenterMemObj);
    ret = clReleaseMemObject(sumYTMemObj);
    ret = clReleaseMemObject(sumT2MemObj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    */
    delete population;
    delete time;

    for (int i (0); i < data.time.length(); ++i)
    {
        sumY += data.population.at(i);
        //sumYT += data.population.at(i) * (i - timeCenter + 1);
        //sumT2 += pow((double) (data.time.at(i) - data.time.at(timeCenter)), 2.0);
    }
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
