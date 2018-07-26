#include <Calibration/Magnetic.hpp>
//#include <iostream>

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
  int64_t runningAverageX = 0;
  int64_t runningAverageY = 0;
  int64_t runningAverageZ = 0;
  int32_t maxX = 0;
  int32_t minX = 0;
  int32_t maxY = 0;
  int32_t minY = 0;
  int32_t maxZ = 0;
  int32_t minZ = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t z = 0;
  unsigned int index;
  unsigned int count = 0;
  for(int phi = 0; phi < MESH_PHI_RESOLUTION; phi++){
    for(int theta = 0; theta < MESH_THETA_RESOLUTION; theta++){
      for(int i = 0; i < this->sampleCount[phi][theta]; i++){
        count++;
        index = (this->sampleIndex[phi][theta] + i) % MAX_SAMPLES_PER_BIN;
        x = this->sampleMesh[X][index][phi][theta];
        y = this->sampleMesh[Y][index][phi][theta];
        z = this->sampleMesh[Z][index][phi][theta];
        
        runningAverageX = (runningAverageX * (count - 1)  + x) / count;
        runningAverageY = (runningAverageY * (count - 1)  + y) / count;
        runningAverageZ = (runningAverageZ * (count - 1)  + z) / count;
        
        if(maxX < x){
          maxX = x;
        }else if(minX > x){
          minX = x;
        }
        if(maxY < y){
          maxY = y;
        } else if(minY > y){
          minY = y;
        }
        if(maxZ < z){
          maxZ = z;
        } else if(minZ > z){
          minZ = z;
        }
      }
    }
  }
  
  float scaleX = (1000.0f * 2.0f) / (abs(static_cast<float>(maxX - runningAverageX)) + abs(static_cast<float>(minX - runningAverageX)));
  float scaleY = (1000.0f * 2.0f) / (abs(static_cast<float>(maxY - runningAverageY)) + abs(static_cast<float>(minY - runningAverageY)));
  float scaleZ = (1000.0f * 2.0f) / (abs(static_cast<float>(maxZ - runningAverageZ)) + abs(static_cast<float>(minZ - runningAverageZ)));
  
  return {
    {static_cast<int32_t>(runningAverageX), static_cast<int32_t>(runningAverageY), static_cast<int32_t>(runningAverageZ)},
    {scaleX, scaleY, scaleZ}
  };
}

Calibration::Magnetic::Sample Calibration::Magnetic::applyCalibration(Calibration::Magnetic::Model model, Calibration::Magnetic::Sample sample){
  return {
    static_cast<int32_t>(static_cast<float>(sample.x - model.hardIronOffset[X]) * model.scale[X]),
    static_cast<int32_t>(static_cast<float>(sample.y - model.hardIronOffset[Y]) * model.scale[Y]),
    static_cast<int32_t>(static_cast<float>(sample.z - model.hardIronOffset[Z]) * model.scale[Z]),
  };
}
