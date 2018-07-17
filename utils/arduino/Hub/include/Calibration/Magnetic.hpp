#ifndef CALIBRATION_MAGNETIC
#define CALIBRATION_MAGNETIC

#include <Arduino.h>
#include <Components/Sensors/Magnetometer.hpp>
#include <Components/Sensors/Accelerometer.hpp>

namespace Calibration{
  namespace Magnetic{
    struct Model;
    struct Data;

    void init(Model* model, Data* data);
    void updateBuffer(Data* data, Components::Sensors::Magnetometer* magnetometer,
      Components::Sensors::Accelerometer* accelerometer, int count);

    void applyCalibration(Model* model, Components::Sensors::Magnetometer* magnetometer);
    Model fourElementCalibration(Data data);
  }
}

#define MAGBUFFSIZEX 12	// x dimension in magnetometer buffer (12x24 equals 288 elements)
#define MAGBUFFSIZEY (2 * MAGBUFFSIZEX)	// y dimension in magnetometer buffer (12x24 equals 288 elements)
#define X 0
#define Y 1
#define Z 2
#define DEFAULT_GEOMAGNETIC_FIELD 50.0F // (uT)
#define MAXMEASUREMENTS 240
#define MESHDELTACOUNTS 50 // magnetic buffer mesh spacing in counts (here 5uT)

struct Calibration::Magnetic::Data{
  int16_t rawMagnetometerReadings[3][MAGBUFFSIZEX][MAGBUFFSIZEY];
  int32_t timeIndices[MAGBUFFSIZEX][MAGBUFFSIZEY];
  int16_t tangentArray[MAGBUFFSIZEX - 1]; // tangents of (100 * angle)
  int16_t bufferCount;
};

struct Calibration::Magnetic::Model{
  float hardIronOffset[3]; // V - x, y, z (uT)
  float inverseSoftIronMatrix[3][3]; // inv W
  float geomagneticFieldMagnitude; // B - (uT)
  float fourBsq; // 4 * B * B - (uT^2)
  float fitError; // %

  float trialV[3]; // trial V - x, y, z (uT)
  float trialInvW[3][3]; // trial inv W
  float trialB; // trial B - (uT)
  float trialFitError; // %

  float ellipsoidMatrix[3][3]; // A
  float inverseEllipsoidMatrix[3][3]; // inv A
  float scratch10x10MatrixA[10][10]; // scratch matrix used in calculations
  float scratch10x10MatrixB[10][10]; // scratch matrix used in calculations
  float vector10x1[10]; // scratch vector used in calculations
  float vector4x1[4]; // scratch vector used in calculations

  int calibrationInProgress; // flag
  int calibrationHasRun; // flag
  int validCalibration; // flag
};

#endif
