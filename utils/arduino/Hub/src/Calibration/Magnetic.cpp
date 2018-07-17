#include <Calibration/Magnetic.hpp>

/*
Notes:
  It uses a 2D bucket matrix to store the calibration samples.
*/

void Calibration::Magnetic::init(Calibration::Magnetic::Model* model, Calibration::Magnetic::Data* data){
  // init model
  for(int x = 0; x < 3; x++){
    for(int y = 0; y < 3; y++){
      model->inverseSoftIronMatrix[x][y] = (x == y ? 1 : 0);
    }
  }
  model->hardIronOffset[X] = model->hardIronOffset[Y] = model->hardIronOffset[Z] = 0.0f;
  model->geomagneticFieldMagnitude = DEFAULT_GEOMAGNETIC_FIELD;
  model->fourBsq = 4.0f * model->geomagneticFieldMagnitude * model->geomagneticFieldMagnitude;
  model->fitError = 1000.0f;
  model->validCalibration = 0;
  model->calibrationInProgress = 0;
  model->calibrationHasRun = 0;

  //init data
  data->bufferCount = 0;
  for(int x = 0; x < MAGBUFFSIZEX; x++){
    for(int y = 0; y < MAGBUFFSIZEY; y++){
      data->timeIndices[x][y] = -1;
    }
  }
  // initialize the array of (MAGBUFFSIZEX - 1) elements of 100 * tangents used for buffer indexing
	// entries cover the range 100 * tan(-PI/2 + PI/MAGBUFFSIZEX), 100 * tan(-PI/2 + 2*PI/MAGBUFFSIZEX) to
	// 100 * tan(-PI/2 + (MAGBUFFSIZEX - 1) * PI/MAGBUFFSIZEX).
	// for MAGBUFFSIZEX=12, the entries range in value from -373 to +373
  for(int x = 0; x < (MAGBUFFSIZEX - 1); x++){
    data->tangentArray[x] = static_cast<int16_t>(100.0f * tan(PI * (-0.5F + static_cast<float>(x + 1) / MAGBUFFSIZEX)));
  }
}

// TODO Note: updates bins with new value
// TODO move count to be a member of data
// TODO Creates a mesh of vectors that form a sphere like surface
void Calibration::Magnetic::updateBuffer(Calibration::Magnetic::Data* data, Components::Sensors::Magnetometer* magnetometer,
  Components::Sensors::Accelerometer* accelerometer, int count){

  if(accelerometer->x() == 0) return; // TODO filter these out?
  int16_t tanJ = 100 * static_cast<int32_t>(accelerometer->y()) / static_cast<int32_t>(accelerometer->x());
  int16_t tanK = 100 * static_cast<int32_t>(accelerometer->z()) / static_cast<int32_t>(accelerometer->x());
  // map tangent ratios to bins j and k using equal angle bins: C guarantees left to right execution of the test
	// and add an offset of MAGBUFFSIZEX bins to k to mimic atan2 on this ratio
	// j will vary from 0 to MAGBUFFSIZEX - 1 and k from 0 to 2 * MAGBUFFSIZEX - 1
  int8_t j = 0, k = 0;
  while ((j < MAGBUFFSIZEX - 1) && (tanJ >= data->tangentArray[j])){
    j++;
  }
  while ((k < MAGBUFFSIZEX - 1) && (tanK >= data->tangentArray[k])){
    k++;
  }
  if(accelerometer->x() < 0){
    k += MAGBUFFSIZEX;
  }

	if ((data->bufferCount == MAXMEASUREMENTS) && (data->timeIndices[j][k] != -1)){
    // case 1: buffer is full and this bin has a measurement: over-write without increasing number of measurements
  	// this is the most common option at run time

		// store the fast (unaveraged at typically 200Hz) integer magnetometer reading into the buffer bin j, k
		data->rawMagnetometerReadings[X][j][k] = magnetometer->x();
    data->rawMagnetometerReadings[Y][j][k] = magnetometer->y();
    data->rawMagnetometerReadings[Z][j][k] = magnetometer->z();
		data->timeIndices[j][k] = count;

	}else if ((data->bufferCount == MAXMEASUREMENTS) && (data->timeIndices[j][k] == -1)){
    // case 2: the buffer is full and this bin does not have a measurement: store and retire the oldest
  	// this is the second most common option at run time

    // store the fast (unaveraged at typically 200Hz) integer magnetometer reading into the buffer bin j, k
		data->rawMagnetometerReadings[X][j][k] = magnetometer->x();
    data->rawMagnetometerReadings[Y][j][k] = magnetometer->y();
    data->rawMagnetometerReadings[Z][j][k] = magnetometer->z();
		data->timeIndices[j][k] = count;

    // TODO Note: find min/max value over 2D matrix
    int oldestX = j, oldestY = k, oldestTime = count;
    for (int x = 0; x < MAGBUFFSIZEX; x++){
			for (int y = 0; y < MAGBUFFSIZEY; y++){
        if(data->timeIndices[x][y] < oldestTime){ // bin is older
          if(data->timeIndices[x][y] != -1){ // bin has been used
            oldestX = x;
            oldestY = y;
            oldestTime = data->timeIndices[x][y];
          }
        }
      }
    }

    data->timeIndices[oldestX][oldestY] = -1; // disable sample

  }else if((data->bufferCount < MAXMEASUREMENTS) && (data->timeIndices[j][k] == -1)){
    // case 3: buffer is not full and this bin is empty: store and increment number of measurements

    // TODO Note: this is done multiple times
    // store the fast (unaveraged at typically 200Hz) integer magnetometer reading into the buffer bin j, k
		data->rawMagnetometerReadings[X][j][k] = magnetometer->x();
    data->rawMagnetometerReadings[Y][j][k] = magnetometer->y();
    data->rawMagnetometerReadings[Z][j][k] = magnetometer->z();
		data->timeIndices[j][k] = count;

    data->bufferCount++;

  }else if((data->bufferCount < MAXMEASUREMENTS) && (data->timeIndices[j][k] != -1)){
    // case 4: buffer is not full and this bin has a measurement: over-write if close or try to slot in
	  // elsewhere if not close to the other measurements so as to create a mesh at power up
    int32_t delta = 0;
    delta += abs(static_cast<int32_t>(magnetometer->x()) - static_cast<int32_t>(data->rawMagnetometerReadings[X][j][k]));
    delta += abs(static_cast<int32_t>(magnetometer->y()) - static_cast<int32_t>(data->rawMagnetometerReadings[Y][j][k]));
    delta += abs(static_cast<int32_t>(magnetometer->z()) - static_cast<int32_t>(data->rawMagnetometerReadings[Z][j][k]));

    if(delta < MESHDELTACOUNTS){
      // to close for expanding the mesh
      // store the fast (unaveraged at typically 200Hz) integer magnetometer reading into the buffer bin j, k
      data->rawMagnetometerReadings[X][j][k] = magnetometer->x();
      data->rawMagnetometerReadings[Y][j][k] = magnetometer->y();
      data->rawMagnetometerReadings[Z][j][k] = magnetometer->z();
  		data->timeIndices[j][k] = count;
    }else{
      // TODO Note: find if true "tooClose" on 2D matrix, saving last known empty bin
      bool tooClose = false;
      int emptyBinX = 0, emptyBinY = 0;
      for(int x = 0; x < MAGBUFFSIZEX && !tooClose; x++){
        for(int y = 0; y < MAGBUFFSIZEY && !tooClose; y++){
          if(data->timeIndices[x][y] != -1){
            // TODO Note: this happens multiple times
            int32_t delta = 0;
            delta += abs(static_cast<int32_t>(magnetometer->x()) - static_cast<int32_t>(data->rawMagnetometerReadings[X][j][k]));
            delta += abs(static_cast<int32_t>(magnetometer->y()) - static_cast<int32_t>(data->rawMagnetometerReadings[Y][j][k]));
            delta += abs(static_cast<int32_t>(magnetometer->z()) - static_cast<int32_t>(data->rawMagnetometerReadings[Z][j][k]));
            if(delta < MESHDELTACOUNTS){
              tooClose = true;
            }
          }else{
            emptyBinX = x;
            emptyBinY = y;
          }
        }
      }

      if(!tooClose){
        // store the fast (unaveraged at typically 200Hz) integer magnetometer reading into the buffer bin j, k
        data->rawMagnetometerReadings[X][emptyBinX][emptyBinY] = magnetometer->x();
        data->rawMagnetometerReadings[Y][emptyBinX][emptyBinY] = magnetometer->y();
        data->rawMagnetometerReadings[Z][emptyBinX][emptyBinY] = magnetometer->z();
    		data->timeIndices[emptyBinX][emptyBinY] = count;

        data->bufferCount++;
      }
    }
  }
}

void Calibration::Magnetic::applyCalibration(Calibration::Magnetic::Model* model, Components::Sensors::Magnetometer* magnetometer){
  float tmp[3];
  float fixed[3];

  // remove hard iron offsets
  tmp[X] = magnetometer->x() - model->hardIronOffset[X];
  tmp[Y] = magnetometer->y() - model->hardIronOffset[Y];
  tmp[Z] = magnetometer->z() - model->hardIronOffset[Z];

  // remove soft iron offsets
  fixed[X] = model->inverseSoftIronMatrix[X][X] * tmp[X] + model->inverseSoftIronMatrix[X][Y] * tmp[Y] + model->inverseSoftIronMatrix[X][Z] * tmp[Z];
  fixed[Y] = model->inverseSoftIronMatrix[Y][X] * tmp[X] + model->inverseSoftIronMatrix[Y][Y] * tmp[Y] + model->inverseSoftIronMatrix[Y][Z] * tmp[Z];
  fixed[Z] = model->inverseSoftIronMatrix[Z][X] * tmp[X] + model->inverseSoftIronMatrix[Z][Y] * tmp[Y] + model->inverseSoftIronMatrix[Z][Z] * tmp[Z];

  // TODO return fixed
}

Calibration::Magnetic::Model Calibration::Magnetic::fourElementCalibration(Calibration::Magnetic::Data data){
  Calibration::Magnetic::Model model;
  // model->trialInvW = identity matrix

  // zero fSumBp4=Y^T.Y, fvecB=X^T.Y (4x1) and on and above diagonal elements of fmatA=X^T*X (4x4)
  for(int i = 0; i < 4; i++){
    model->vector4x1[i] = 0.0f;
    for(int j = i; j < 4; j++){
      model->scratch10x10MatrixA[i][j] = 0.0f;
    }
  }

  int16_t offset[3];
  offset[X] = offset[Y] = offset[Z] = 0;
  int16_t count = 0;
  float sumBp4 = 0;
  for(int j = 0; j < MAGBUFFSIZEX; j++){
    for(int k = 0; k < MAGBUFFSIZEY; k++){
      if(data->timeIndices[j][k] != -1){ // bin contains vector
        if(count == 0){ // set default vector
          offset[X] = data->rawMagnetometerReadings[X][j][k];
          offset[Y] = data->rawMagnetometerReadings[Y][j][k];
          offset[Z] = data->rawMagnetometerReadings[Z][j][k];
        }

        // store scaled and offset fBp[XYZ] in fvecA[0-2] and fBp[XYZ]^2 in fvecA[3-5]
        model->vector10x1[X] = static_cast<float>(static_cast<int32_t>(data->rawMagnetometerReadings[X][j][k] - static_cast<int32_t>(offset[X]))/* * scaling*/)
        model->vector10x1[X + 3] = model->vector10x1[X] * model->vector10x1[X];
        model->vector10x1[Y] = static_cast<float>(static_cast<int32_t>(data->rawMagnetometerReadings[Y][j][k] - static_cast<int32_t>(offset[Y]))/* * scaling*/)
        model->vector10x1[Y + 3] = model->vector10x1[Y] * model->vector10x1[Y];
        model->vector10x1[Z] = static_cast<float>(static_cast<int32_t>(data->rawMagnetometerReadings[Z][j][k] - static_cast<int32_t>(offset[Z]))/* * scaling*/)
        model->vector10x1[Z + 3] = model->vector10x1[Z] * model->vector10x1[Z];

        // calculate fBp2 = fBp[X]^2 + fBp[Y]^2 + fBp[Z]^2 (scaled uT^2)
        float Bp2 = model->vector10x1[X + 3] + model->vector10x1[Y + 3] + model->vector10x1[Z + 3];
        sumBp4 += Bp2 * Bp2;

        // now we have fBp2, accumulate fvecB[0-2] = X^T.Y =sum(fBp2.fBp[XYZ])
        model->vector4x1[X] += model->vector10x1[X] * Bp2;
        model->vector4x1[Y] += model->vector10x1[Y] * Bp2;
        model->vector4x1[Z] += model->vector10x1[Z] * Bp2;

        //accumulate fvecB[3] = X^T.Y =sum(fBp2)
        model->vector4x1[3] += Bp2;

        // accumulate on and above-diagonal terms of fmatA = X^T.X ignoring fmatA[3][3]
        model->scratch10x10MatrixA[0][0] += model->vector10x1[X + 3];
        model->scratch10x10MatrixA[0][1] += model->vector10x1[X] * model->vector10x1[Y];
        model->scratch10x10MatrixA[0][2] += model->vector10x1[X] * model->vector10x1[Z];
        model->scratch10x10MatrixA[0][3] += model->vector10x1[X];
        model->scratch10x10MatrixA[1][1] += model->vector10x1[Y + 3];
        model->scratch10x10MatrixA[1][2] += model->vector10x1[Y] * model->vector10x1[Z];
        model->scratch10x10MatrixA[1][3] += model->vector10x1[Y];
        model->scratch10x10MatrixA[2][2] += model->vector10x1[Z + 3];
        model->scratch10x10MatrixA[2][3] += model->vector10x1[Z];

        count++;
      }
    }
  }
  // set the last element of the measurement matrix to the number of buffer elements used
  model->scratch10x10MatrixA[3][3] = static_cast<float>(count);

  // store the number of measurements accumulated (defensive programming, should never be needed)
  // TODO should never be needed
  data->bufferCount = count;

  // use above diagonal elements of symmetric fmatA to set both fmatB and fmatA to X^T.X
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      model->scratch10x10MatrixB[i][j] = model->scratch10x10MatrixB[j][i] = model->scratch10x10MatrixA[j][i] = model->scratch10x10MatrixA[i][j];
    }
  }

  // calculate in situ inverse of fmatB = inv(X^T.X) (4x4) while fmatA still holds X^T.X
  float* rows[4];
  for(int i = 0; i < 4; i++){
    rows[i] = model->scratch10x10MatrixB[i];
  }
  // TODO invert matrix B

  for(int i = 0; i < 4; i++){
    model->vector10x1[i] = 0.0f;
    for(int k = 0; k < 4; k++){
      model->vector10x1[i] += model->scratch10x10MatrixB[i][k] + model->vector4x1[k];
    }
  }

  // calculate P = r^T.r = Y^T.Y - 2 * beta^T.(X^T.Y) + beta^T.(X^T.X).beta
	// = fSumBp4 - 2 * fvecA^T.fvecB + fvecA^T.fmatA.fvecA
	// first set P = Y^T.Y - 2 * beta^T.(X^T.Y) = fSumBp4 - 2 * fvecA^T.fvecB
  float error = 0.0f;
  for(int i = 0; i < 4; i++){
    error += model->vector10x1[i] * model->vector4x1[i];
  }
  error = sumBp4 - 2.0f * error;

  // set fvecB = (X^T.X).beta = fmatA.fvecA
  for(int i = 0; i < 4; i++){
    model->vector4x1[i] = 0.0f;
    for(int k = 0; k < 4; k++){
      model->vector4x1[i] += model->scratch10x10MatrixA[i][k] * model->vector10x1[k];
    }
  }

  // complete calculation of P by adding beta^T.(X^T.X).beta = fvecA^T * fvecB
  for(int i = 0; i < 4; i++){
    error += model->vector4x1[i] * model->vector10x1[i];
  }

  // compute the hard iron vector (in uT but offset and scaled by FMATRIXSCALING)
  model->trialV[X] = 0.5f * model->vector10x1[X];
  model->trialV[Y] = 0.5f * model->vector10x1[Y];
  model->trialV[Z] = 0.5f * model->vector10x1[Z];

  // compute the scaled geomagnetic field strength B (in uT but scaled by FMATRIXSCALING)
	model->trialB = sqrt(model->vector10x1[X + 1] + model->trialV[X] * model->trialV[X] + model->trialV[Y] * model->trialV[Y] + model->trialV[Z] * model->trialV[Z])

  // calculate the trial fit error (percent) normalized to number of measurements and scaled geomagnetic field strength
  model->trialFitError = sqrt(error / static_cast<float>(model->bufferCount)) * 100.0f / (2.0f * model->trialB * model->trialB);

  // correct the hard iron estimate for FMATRIXSCALING and the offsets applied (result in uT)
  model->trialV[X] = model->trialV[X] * DEFAULTB + static_cast<float>(offset[X]);/* * uTPerCount;*/

  // correct the geomagnetic field strength B to correct scaling (result in uT)
  model->trialB *= DEFAULTB;

  return model;
}
