#include <Calibration/Magnetic.hpp>

Calibration::Magnetic::Magnetic()
  : sampleCount(0),
    time(0){
  this->precalculateTangents();
  this->initMesh();
}

void Calibration::Magnetic::precalculateTangents(){
  // generates the positive side of each tan(angle) bin, and times
  // it by 100 to reduce the space used
  //
  //         [ a,  b,  c,  d,   e,  f,  g,  h]
  //          \/  \/  \/  \/   \/  \/  \/  \/
  // <----|[--][--][--][--]|[--][--][--][--]|----->
  //   -PI*0.5             0              PI*0.5
  //
  for(int i = 1; i < MESH_PHI_RESOLUTION; i++){
    float proportion = static_cast<float>(i) / MESH_PHI_RESOLUTION;
    float angleInRad = PI * proportion;
    this->tangentBins[i] = static_cast<int16_t>(
      100.0f * tan(RAD_DOWN + angleInRad)
    );
  }
}

void Calibration::Magnetic::initMesh(){
  for(int phi = 0; phi < MESH_PHI_RESOLUTION; phi++){
    for(int theta = 0; theta < MESH_THETA_RESOLUTION; theta++){
      this->sampleTimes[phi][theta] = EMPTY_BIN;
    }
  }
}

void Calibration::Magnetic::addSample(Calibration::Magnetic::Sample sample){
  if(sample.x == 0) return; // tna math does not work
  Calibration::Magnetic::Bin bin = this->getBinForSample(sample);
  bool atMaxSamples = this->sampleCount >= MAX_SAMPLES;
  bool binIsEmpty = this->sampleTimes[bin.phi][bin.theta] == EMPTY_BIN;
  this->insertSample(sample, bin);
  if(atMaxSamples && binIsEmpty){
    Calibration::Magnetic::Bin oldestSample = this->findOldestSample();
    this->sampleTimes[oldestSample.phi][oldestSample.theta] = EMPTY_BIN;
  }
}

Calibration::Magnetic::Bin Calibration::Magnetic::getBinForSample(Calibration::Magnetic::Sample sample){
  Calibration::Magnetic::Bin bin;
  int16_t tanPhi = static_cast<int16_t>(
    100.0f * (static_cast<float>(sample.z) / static_cast<float>(sample.x))
  );
  int16_t tanTheta = static_cast<int16_t>(
    100.0f * (static_cast<float>(sample.y) / static_cast<float>(sample.x))
  );
  bin.phi = bin.theta = 0;
  while ((bin.phi < MESH_PHI_RESOLUTION - 1) && (tanPhi >= this->tangentBins[bin.phi])){
    bin.phi++;
  }
  while ((bin.theta < MESH_PHI_RESOLUTION - 1) && (tanTheta >= this->tangentBins[bin.theta])){
    bin.theta++;
  }
  if(sample.x < 0){
    bin.theta += MESH_PHI_RESOLUTION; // mimic atan2
  }
  return bin;
}

void Calibration::Magnetic::insertSample(Calibration::Magnetic::Sample sample, Calibration::Magnetic::Bin bin){
  this->sampleMesh[X][bin.phi][bin.theta] = sample.x;
  this->sampleMesh[Y][bin.phi][bin.theta] = sample.y;
  this->sampleMesh[Z][bin.phi][bin.theta] = sample.z;
  this->sampleTimes[bin.phi][bin.theta] = this->time;
  this->time++;
}

Calibration::Magnetic::Bin Calibration::Magnetic::findOldestSample(){
  Calibration::Magnetic::Bin oldest;
  int32_t oldestTime = this->time;
  for (int phi = 0; phi < MESH_PHI_RESOLUTION; phi++){
    for (int theta = 0; theta < MESH_THETA_RESOLUTION; theta++){
      if(this->sampleTimes[phi][theta] < oldestTime){ // bin is older
        if(this->sampleTimes[phi][theta] != EMPTY_BIN){ // bin has been used
          oldest.phi = phi;
          oldest.theta = theta;
          oldestTime = this->sampleTimes[phi][theta];
        }
      }
    }
  }
  return oldest;
}

void Calibration::Magnetic::generateModelForOnlyHardIron(){

}
