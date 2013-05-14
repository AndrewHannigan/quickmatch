#ifndef TIMER_H
#define TIMER_H

#define START_TIMER \
  cycles_high1 = 0; cycles_low1 = 0; cycles_high2 = 0; cycles_low2 = 0; \
  temp_cycles1 = 0, temp_cycles2 = 0, total_cycles = 0; \
  __asm__ __volatile__ ("rdtsc" : "=a"(cycles_low1), "=d"(cycles_high1));

#define STOP_TIMER \
  __asm__ __volatile__ ("rdtsc" : "=a"(cycles_low2), "=d"(cycles_high2)); \
  temp_cycles1 = ((unsigned long long) cycles_high1 << 32) | cycles_low1; \
  temp_cycles2 = ((unsigned long long) cycles_high2 << 32) | cycles_low2; \
  total_cycles = temp_cycles2 - temp_cycles1; \
  seconds = (float)  total_cycles / (MHertz*1000000);

unsigned cycles_high1, cycles_low1, cycles_high2, cycles_low2;
unsigned long long temp_cycles1, temp_cycles2;
unsigned long long total_cycles;
float seconds;
unsigned MHertz = 2400;     // My Mac is a 2.4 GHz machine, so 2400 MHz 
double cumulative_seconds;

#endif

