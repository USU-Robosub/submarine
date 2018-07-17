#include <Calibration/Magnetic.hpp>
#include <iostream>

Calibration::Magnetic::Magnetic(){
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
  for(int i = 0; i < (ANGULAR_RESOLUTION - 1); i++){
    float proportion = static_cast<float>(i + 1) / ANGULAR_RESOLUTION;
    float angleInRad = PI * proportion;
    this->tangentBins[i] = static_cast<int16_t>(
      100.0f * tan(RAD_DOWN + angleInRad)
    );
    //std::cout << "[" << this->tangentBins[i] << "]";
  }
  //std::cout << std::endl;
}

void Calibration::Magnetic::initMesh(){
  for(int phi = 0; phi < MESH_PHI_RESOLUTION; phi++){
    for(int theta = 0; theta < MESH_THETA_RESOLUTION; theta++){
      this->sampleIndex[phi][theta] = 0;
      this->sampleCount[phi][theta] = 0;
    }
  }
}

void Calibration::Magnetic::addSample(Calibration::Magnetic::Sample sample){
  if(sample.x == 0) return; // tha math does not work, tan becomes infinite
  //std::cout << "[" << sample.x << ", " << sample.y << "]";
  Calibration::Magnetic::Bin bin = this->getBinForSample(sample);
  //std::cout << " [" << (int)bin.theta << ", " << (int)bin.phi << "]" << std::endl;
  bool binIsFull = this->sampleCount[bin.phi][bin.theta] >= MAX_SAMPLES_PER_BIN;
  if(binIsFull){
    this->removeOldestSampleFromBin(bin);
  }
  this->insertSample(sample, bin);
}

Calibration::Magnetic::Bin Calibration::Magnetic::getBinForSample(Calibration::Magnetic::Sample sample){
  Calibration::Magnetic::Bin bin;
  int16_t tanPhi = static_cast<int16_t>(
    100.0f * (static_cast<float>(sample.z) / static_cast<float>(sample.x))
  );
  int16_t tanTheta = static_cast<int16_t>(
    100.0f * (static_cast<float>(sample.y) / static_cast<float>(sample.x))
  );
  //std::cout << " [" << tanTheta << ", " << tanPhi << "]" << std::endl;
  bin.phi = bin.theta = 0;
  while ((bin.phi < ANGULAR_RESOLUTION - 1) && (tanPhi >= this->tangentBins[bin.phi])){
    bin.phi++;
  }
  while ((bin.theta < ANGULAR_RESOLUTION - 1) && (tanTheta >= this->tangentBins[bin.theta])){
    bin.theta++;
  }
  if(sample.x < 0){
    bin.theta += MESH_PHI_RESOLUTION; // allow theta to completely circle the sphere
  }
  return bin;
}

void Calibration::Magnetic::insertSample(Calibration::Magnetic::Sample sample, Calibration::Magnetic::Bin bin){
  int8_t index = (this->sampleIndex[bin.phi][bin.theta] + this->sampleCount[bin.phi][bin.theta]) % MAX_SAMPLES_PER_BIN;

  this->sampleMesh[X][index][bin.phi][bin.theta] = sample.x;
  this->sampleMesh[Y][index][bin.phi][bin.theta] = sample.y;
  this->sampleMesh[Z][index][bin.phi][bin.theta] = sample.z;

  this->sampleCount[bin.phi][bin.theta]++;
}

void Calibration::Magnetic::removeOldestSampleFromBin(Calibration::Magnetic::Bin bin){
  // roll buffer forward one item
  this->sampleIndex[bin.phi][bin.theta] = (this->sampleIndex[bin.phi][bin.theta] + 1) % MAX_SAMPLES_PER_BIN;
  this->sampleCount[bin.phi][bin.theta]--;
}

int32_t(*Calibration::Magnetic::samples())[VECTOR3][MAX_SAMPLES_PER_BIN][MESH_PHI_RESOLUTION][MESH_THETA_RESOLUTION]{
  return &this->sampleMesh;
}

Calibration::Magnetic::Model Calibration::Magnetic::generateModel(){

}
