#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <numeric>
#include <vector>

int main(int argc, char **argv) {
  // Record the time at the beginning of our experiment.
  auto begin = std::chrono::steady_clock::now();

  // Experiment takes place here.
  // This for loop is just a placeholder.
  for (size_t i = 0; i < 1000000; i++) {
  }

  // Record the time at the end of our experiment.
  auto end = std::chrono::steady_clock::now();

  // Compute the duration of the experiment in seconds.
  double duration = std::chrono::duration<double>(end - begin).count();

  printf("Runtime was %lf us (microseconds).\n", 1e6 * duration);

  return 0;
}
