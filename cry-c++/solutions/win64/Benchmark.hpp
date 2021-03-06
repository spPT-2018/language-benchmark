#ifndef BENCHMARK_INCLUDE_H
#define BENCHMARK_INCLUDE_H

#include <string>
#include <iostream>
#include <math.h>
#include <functional>
#include "Timer.hpp"
#include "CrySystem/ISystem.h"

static double Benchmark8(std::string msg, std::function<double(int)> &&fun, int iterations, double minTimeMs) {
	int count = 1, totalCount = 0;
	double dummy = 0.0, runningTime = 0.0, deltaTime = 0.0, deltaTimeSquared = 0.0, minTimeNs = minTimeMs * 1000000.0;
	do {
		count *= 2;
		deltaTime = 0.0;
		deltaTimeSquared = 0.0;
		for (int j = 0; j < iterations; j++) {
			Timer t;
			for (int i = 0; i < count; i++)
				dummy += fun(i);
			runningTime = t.check();
			double time = runningTime / count;
			deltaTime += time;
			deltaTimeSquared += time * time;
			totalCount += count;
		}
	} while (runningTime < minTimeNs && count < INT32_MAX / 2);
	double mean = deltaTime / iterations, standardDeviation = sqrt((deltaTimeSquared - mean * mean * iterations) / (iterations - 1));

	char buffer[300];
	sprintf(buffer, "%s,%f,%f,%d\n", msg.c_str(), mean, standardDeviation, count);

	gEnv->pLog->LogToFile(buffer);

	return dummy / totalCount;
}
#endif