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

#define RAD_DOWN (PI * -0.5f)

#define ANGULAR_RESOLUTION 12
#define MESH_PHI_RESOLUTION ANGULAR_RESOLUTION
#define MESH_THETA_RESOLUTION (2 * MESH_PHI_RESOLUTION)
#define MAX_SAMPLES_PER_BIN 4

// expected orientation z axis up, x axis forward

// store samples in a mesh that is based on bucket sort
// uses sphere coordinantes to place smaples into groups
// phi angle of projection on xz plane
// theta angle of projection on xy plane

class Calibration::Magnetic{
private:
  struct Bin{
    int8_t phi;
    int8_t theta;
  };

public:
  struct Model{
    int32_t hardIronOffset[VECTOR3]; // (nT)
    float scale[VECTOR3];
  };

  struct Sample{
    int32_t x; // (nT)
    int32_t y; // (nT)
    int32_t z; // (nT)
  };

  Magnetic();
  void addSample(Sample sample);
  Model generateModel();
  float getCoverage();

  int32_t(*samples())[VECTOR3][MAX_SAMPLES_PER_BIN][MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION];

private:
  void precalculateTangents();
  void initMesh();

  Bin getBinForSample(Sample sample);
  void insertSample(Sample sample, Bin bin);
  void removeOldestSampleFromBin(Bin bin);

  // 2D matrix where each entry is a circular buffer of 3D vectors
  int32_t sampleMesh[VECTOR3][MAX_SAMPLES_PER_BIN][MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION];
  int8_t sampleIndex[MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION];
  int8_t sampleCount[MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION];

  int16_t tangentBins[ANGULAR_RESOLUTION - 1];
};

#endif
