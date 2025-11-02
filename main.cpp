#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <cstdint>
#include <vector>

///
// Simulation Parameters
///
int L;                         // Lattice size (L x L)
double T;                      // Temperature
double J = 1.0;                // Coupling constant
unsigned long long seed = 100; // Random value seed

unsigned long long randomU64() {
  seed ^= (seed << 21);
  seed ^= (seed >> 35);
  seed ^= (seed << 4);
  return seed;
}

double randomDouble() {
  unsigned long long next = randomU64();
  next >>= (64 - 26);
  unsigned long long next2 = randomU64();
  next2 >>= (64 - 26);
  return ((next << 27) + next2) / (double)(1LL << 53);
}

/**
 * Represents an entry in the lattice.
 *
 * This class has some nice features,
 * such as the ability to determine neighbor indecies during construction,
 * and caching the energy at each entry.
 */
class Entry {
public:
  /// Spin value for this entry, either 1 or -1
  /// TODO: Make this a bool instead?
  int8_t val = 0;

  /// Index of this entry in the lattice (0 to L*L-1)
  int index = 0;

  /// Index of right neighbor
  int right = 0;

  /// Index of left neighbor
  int left = 0;

  /// Index of up neighbor
  int up = 0;

  /// Index of down neighbor
  int down = 0;

  /// Energy of this entry, cached for performance
  double energy = 0.;

  /// Entries MUST know what they are apart of!
  Entry() : val((randomDouble() < 0.5) ? -1 : 1) {};

  /**
   * @brief Converts row and column into absolute index
   *
   * @param row Row of entry
   * @param col Column of entry
   * @return int Absolute index
   */
  static int res_index(int row, int col) { return (row * L) + col; }

  void set_index(int i, int j) {

    // int up = lattice[(i - 1 + L) % L][j];
    // int down = lattice[(i + 1) % L][j];
    // int left = lattice[i][(j - 1 + L) % L];
    // int right = lattice[i][(j + 1) % L];

    // Set the index value

    index = res_index(i, j);

    // Set all of the neighbor values

    up = res_index((i - 1 + L) % L, j);
    down = res_index((i + 1) % L, j);
    right = res_index(i, (j - 1 + L) % L);
    left = res_index(i, (j + 1) % L);
  }
};

class Lattice {
public:
  /// Array of all values in this lattice
  std::vector<Entry> vals;

  /// Total energy in this lattice
  double energyv = 0;

  Lattice() = default;

  void init() {
    // Define the size of the lattice,
    // and also include reference to us

    vals.resize(L * L);

    // Iterate over all rows and columns

    for (int i = 0; i < L; ++i) {

      for (int j = 0; j < L; ++j) {

        // Set this index to be the current row and column
        vals.at(Entry::res_index(i, j)).set_index(i, j);
      }
    }

    // Now, compute total energy

    total_energy();
  }

  /**
   * @brief Computes the new energy at a specific index
   *
   * We compute the energy at this index,
   * and save the energy value with the entry.
   *
   * @param ind Index to compute
   */
  void energy(int ind) {

    auto &ent = vals.at(ind);

    ent.energy = 0.5 * -J * ent.val *
                 (vals.at(ent.up).val + vals.at(ent.down).val +
                  vals.at(ent.left).val + vals.at(ent.right).val);
  }

  /**
   * @brief Computes the total energy in this lattice
   *
   */
  void total_energy() {

    // Iterate over all values in the lattice

    for (int i = 0; i < vals.size(); ++i) {

      // Compute the energy at this value

      energy(i);

      energyv += vals.at(i).energy;
    }
  }

  void update_energy(int ind) {

    // Take the energy for the updated from the total

    energyv -= vals.at(ind).energy;

    // Compute the new energy at the position

    energy(ind);

    // Add the energy to the total

    energyv += vals.at(ind).energy;
  }
};

Lattice lattice;

float tdiff(struct timeval *start, struct timeval *end) {
  return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec);
}

void initializeLattice() {

  // Call the init method on the lattice

  lattice.init();

  // lattice = (int **)malloc(sizeof(int *) * L);
  // for (int i = 0; i < L; i++) {
  //   lattice[i] = (int *)malloc(sizeof(int) * L);
  //   for (int j = 0; j < L; j++) {
  //     lattice[i][j] = (randomDouble() < 0.5) ? -1 : 1;
  //   }
  // }
}

double calculateTotalEnergy() {
  double energy = 0.0;

  // Iterate over all values in the lattice

  for (int i = 0; i < lattice.vals.size(); ++i) {

    // Compute the energy at this value

    lattice.energy(i);

    energy += lattice.vals.at(i).energy;
  }

  // for (int i = 0; i < L; i++) {
  //   for (int j = 0; j < L; j++) {
  //     int spin = lattice[i][j];

  //     int up = lattice[(i - 1 + L) % L][j];
  //     int down = lattice[(i + 1) % L][j];
  //     int left = lattice[i][(j - 1 + L) % L];
  //     int right = lattice[i][(j + 1) % L];

  //     energy += -J * spin * (up + down + left + right);
  //   }
  // }
  // return 0.5 * energy;

  return energy;
}

double calculateMagnetization() {
  double mag = 0.0;

  for (int i = 0; i < lattice.vals.size(); ++i) {

    mag += lattice.vals.at(i).val;
  }

  // for (int i = 0; i < L; i++) {
  //   for (int j = 0; j < L; j++) {
  //     mag += lattice[i][j];
  //   }
  // }
  return mag / (L * L);
}

void metropolisHastingsStep() {

  int i = (int)(randomDouble() * L * L);

  double E_before = lattice.energyv;
  lattice.vals.at(i).val *= -1;

  // Update the energy at the new position

  lattice.update_energy(i);

  double E_after = lattice.energyv;
  double dE = E_after - E_before;

  if (dE <= 0.0) {
    return;
  }

  double prob = exp(-dE / T);
  if (randomDouble() >= prob) {

    // Update the value at this position

    lattice.vals.at(i).val *= -1;

    // Update the energy at this position

    lattice.update_energy(i);
  }
}

void saveLatticeImage(const char *png_filename) {
  char ppm_filename[256];
  snprintf(ppm_filename, sizeof(ppm_filename), "temp_%s.ppm", png_filename);

  FILE *f = fopen(ppm_filename, "wb");
  if (!f) {
    printf("Error: Could not create temporary file %s\n", ppm_filename);
    return;
  }

  fprintf(f, "P6\n");
  fprintf(f, "%d %d\n", L, L);
  fprintf(f, "255\n");

  for (int i = 0; i < L * L; ++i) {
    unsigned char r, g, b;
    if (lattice.vals.at(i).val == 1) {
      r = 255;
      g = 255;
      b = 255;
    } else {
      r = 0;
      g = 50;
      b = 200;
    }
    fwrite(&r, 1, 1, f);
    fwrite(&g, 1, 1, f);
    fwrite(&b, 1, 1, f);
  }

  fclose(f);

  char cmd[512];
  snprintf(cmd, sizeof(cmd), "convert %s %s 2>/dev/null", ppm_filename,
           png_filename);
  int result = system(cmd);

  if (result == 0) {
    printf("Saved visualization to %s\n", png_filename);
    remove(ppm_filename);
  } else {
    rename(ppm_filename, png_filename);
    printf("Saved visualization to %s (install ImageMagick for PNG)\n",
           png_filename);
  }
}

void sanityCheck(double energy, double mag_per_spin, const char *stage) {
  double energy_per_spin = energy / (L * L);
  double Tc = 2.0 * J / log(1.0 + sqrt(2.0));

  printf("Sanity check [%s]:\n", stage);

  // 1. Energy per spin
  if (energy_per_spin < -2.0 * J - 0.01 || energy_per_spin > 2.0 * J + 0.01) {
    printf("  [ERROR] Energy per spin (%.4f) outside expected bounds "
           "[%.2f, %.2f]\n",
           energy_per_spin, -2.0 * J, 2.0 * J);
  } else {
    printf("  [OK] Energy per spin = %.4f (within bounds [%.2f, %.2f])\n",
           energy_per_spin, -2.0 * J, 2.0 * J);
  }

  // 2. Magnetization per spin
  if (fabs(mag_per_spin) > 1.01) {
    printf("  [ERROR] Magnetization per spin (%.4f) outside physical bounds "
           "[-1, 1]\n",
           mag_per_spin);
  } else {
    printf("  [OK] Magnetization per spin = %.4f (within bounds [-1, 1])\n",
           mag_per_spin);
  }

  printf("\n");
}

void freeLattice() {
  // for (int i = 0; i < L; i++) {
  //   free(lattice[i]);
  // }
  // free(lattice);

  // We use a C++ vector, which simplifies memory management
  // so we simply do nothing
}

int main(int argc, const char **argv) {
  if (argc < 4) {
    printf("Usage: %s <lattice_size> <temperature> <steps>\n", argv[0]);
    printf("Example: %s 100 2.269 10000000\n", argv[0]);
    printf("\n2D Ising Model\n");
    printf("Critical temperature: Tc = 2J/ln(1+√2) ≈ 2.26918531421\n");
    return 1;
  }

  L = atoi(argv[1]);
  T = atof(argv[2]);
  int steps = atoi(argv[3]);

  printf("2D Ising Model\n");
  printf("=================================================\n");
  printf("Lattice size: %d x %d (%d spins)\n", L, L, L * L);
  printf("Temperature: T = %.4f (Tc ≈ 2.269)\n", T);
  printf("Coupling constant: J = %.2f\n", J);
  printf("Number of Metropolis-Hastings steps: %d\n", steps);
  printf("=================================================\n\n");

  initializeLattice();

  double initial_energy = lattice.energyv;
  double initial_mag = calculateMagnetization();
  printf("Initial energy: %.4f\n", initial_energy);
  printf("Initial magnetization: %.4f\n\n", initial_mag);

  sanityCheck(initial_energy, initial_mag, "Initial state");

  saveLatticeImage("initial_state.png");

  struct timeval start, end;
  gettimeofday(&start, NULL);

  for (int step = 0; step < steps; step++) {
    metropolisHastingsStep();
  }

  gettimeofday(&end, NULL);

  double final_energy = lattice.energyv;
  double final_mag = calculateMagnetization();

  printf("\nFinal energy: %.4f\n", final_energy);
  printf("Final magnetization: %.4f\n\n", final_mag);

  sanityCheck(final_energy, final_mag, "Final state");

  printf("Total time: %0.6f seconds\n\n", tdiff(&start, &end));

  saveLatticeImage("final_state.png");

  freeLattice();
  return 0;
}
