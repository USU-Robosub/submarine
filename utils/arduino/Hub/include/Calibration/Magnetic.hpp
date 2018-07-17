#ifndef CALIBRATION_MAGNETIC
#define CALIBRATION_MAGNETIC

#include <Arduino.h>
#include <Components/Sensors/Magnetometer.hpp>
#include <Components/Sensors/Accelerometer.hpp>

namespace Calibration{
  class Magnetic;
}

#define VECTOR3 3
#define X 0
#define Y 1
#define Z 2

#define MATRIX_3X3 3

#define MESH_PHI_RESOLUTION 12
#define MESH_THETA_RESOLUTION (2 * MESH_PHI_RESOLUTION)
#define MAX_SAMPLES 240 // must be less than or equal to MESH_PHI_RESOLUTION * MESH_THETA_RESOLUTION

#define DEFAULT_GEOMAGNETIC_FIELD 50.0f // (uT)

#define RAD_DOWN PI * -0.5f
#define EMPTY_BIN -1

// sensor generates -2048 - 2048 at a gain level
// use int16_t to minimize size

// this algorithm works best if the sensor outputs data oriented with the
// z axis pointing up, and the x/y axis level with the ground, x axis pointing forward

class Calibration::Magnetic{
private:
  struct Bin{
    int8_t phi;
    int8_t theta;
  };

public:
  struct Sample{
    int16_t x;
    int16_t y;
    int16_t z;
  };

  Magnetic();
  void addSample(Sample sample);
  void generateModelForOnlyHardIron();
  Sample transform(Sample sample);

private:

  void precalculateTangents();
  void initMesh();

  Bin getBinForSample(Sample sample);
  void insertSample(Sample sample, Bin bin);
  Bin findOldestSample();

  int16_t sampleMesh[VECTOR3][MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION];
  int32_t sampleTimes[MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION];
  int16_t sampleCount;
  int32_t time;

  float hardIronOffset[VECTOR3]; // (uT)
  float inverseSoftIronMatrix[MATRIX_3X3][MATRIX_3X3];
  float geomagneticFieldMagnitude; // (uT)
  float fitError; // (%)

  int16_t tangentBins[MESH_PHI_RESOLUTION - 1];
};

#endif
