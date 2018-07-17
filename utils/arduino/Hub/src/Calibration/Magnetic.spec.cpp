#include <catch2/catch.hpp>
#include <Calibration/Magnetic.hpp>

#include <iostream>
#include <iomanip>

// x * tan(theta) = y
// x * tan(phi) = z

#define DEG_TO_RAD(deg) (deg * 0.0174533f)

#define SPHERE_X(t, p) (1.0f/(sqrt(1.0f+tan(t)*tan(t)+tan(p)*tan(p))))
#define SPHERE_Y(t, p) (tan(t) * SPHERE_X(t, p))
#define SPHERE_Z(t, p) (tan(p) * SPHERE_X(t, p))

#define SPHERE_X_FULL(t, p) (((t >= DEG_TO_RAD(90.0f) || t <= DEG_TO_RAD(-90.0f)) ? -1.0f : 1.0f) * SPHERE_X(t, p))
#define SPHERE_Y_FULL(t, p) (((t >= DEG_TO_RAD(90.0f) || t <= DEG_TO_RAD(-90.0f)) ? -1.0f : 1.0f) * SPHERE_Y(t, p))
#define SPHERE_Z_FULL(t, p) (((p >= DEG_TO_RAD(90.0f) || p <= DEG_TO_RAD(-90.0f)) ? -1.0f : 1.0f) * SPHERE_Z(t, p))

#define SPHERE(t, p, r) {(int32_t)(SPHERE_X_FULL(t, p) * r), (int32_t)(SPHERE_Y_FULL(t, p) * r), (int32_t)(SPHERE_Z_FULL(t, p) * r)}
#define SPHERE_DEG(t, p, r) SPHERE(DEG_TO_RAD((t)), DEG_TO_RAD((p)), (r))

// TEST_CASE("something 2", "[something]"){
//   Calibration::Magnetic::Sample sample = SPHERE_DEG(0.0f, 0.0f);
//   // std::cout << sample.x << " " << sample.y << " " << sample.z << std::endl;
//   //
//   // sample = SPHERE_DEG(45.0f, 0.0f);
//   // std::cout << sample.x << " " << sample.y << " " << sample.z << std::endl;
//   //
//   // sample = SPHERE_DEG(-45.0f, 0.0f);
//   // std::cout << sample.x << " " << sample.y << " " << sample.z << std::endl;
//   //
//   // sample = SPHERE_DEG(45.0f, 42.0f);
//   // std::cout << sample.x << " " << sample.y << " " << sample.z << std::endl;
//
//   for(int i = -90; i <= 90; i+= 5){
//     sample = SPHERE_DEG(-90.0f - 45.0f, (float)i);
//     std::cout << sample.x << " " << sample.y << " " << sample.z << std::endl;
//   }
// }

void show(Calibration::Magnetic* magneticCalibration){
  std::cout << std::endl;
  for(int phi = 0; phi < MESH_PHI_RESOLUTION; phi++){
    for(int theta = 0; theta < MESH_THETA_RESOLUTION; theta++){
      int32_t valueX = (*(magneticCalibration->samples()))[X][0][phi][theta];
      int32_t valueY = (*(magneticCalibration->samples()))[Y][0][phi][theta];
      int32_t valueZ = (*(magneticCalibration->samples()))[Z][0][phi][theta];
      std::cout << "|" << std::setw(3) << valueX << std::setw(3) << valueY;
    }
    std::cout << std::endl;
  }
}

TEST_CASE("the mesh allows multiple samples in a bin, and when it fills up it will rotate the elements", "[something]"){
  Calibration::Magnetic magneticCalibration;

  // show(&magneticCalibration);
  //
  // for(int i = -180.0f; i <= 180.0f; i+=10.0f){
  //   std::cout << std::endl << "angle: " << i << std::endl;
  //   magneticCalibration.addSample(SPHERE_DEG((float)i, -45.0f));
  // }
  // show(&magneticCalibration);
  //
  // for(int i = -180.0f; i <= 180.0f; i+=10.0f){
  //   std::cout << std::endl << "angle: " << i << std::endl;
  //   magneticCalibration.addSample(SPHERE_DEG((float)i, 0.0f));
  // }
  // show(&magneticCalibration);
  //
  // for(int i = -180.0f; i <= 180.0f; i+=10.0f){
  //   std::cout << std::endl << "angle: " << i << std::endl;
  //   magneticCalibration.addSample(SPHERE_DEG((float)i, 45.0f));
  // }
  // show(&magneticCalibration);

  magneticCalibration.addSample(SPHERE_DEG(0.0f, 0.0f, 1.0f));
  magneticCalibration.addSample(SPHERE_DEG(0.0f, 0.0f, 2.0f));
  magneticCalibration.addSample(SPHERE_DEG(0.0f, 0.0f, 3.0f));
  magneticCalibration.addSample(SPHERE_DEG(0.0f, 0.0f, 4.0f));

  REQUIRE( (*magneticCalibration.samples())[X][0][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 1 );
  REQUIRE( (*magneticCalibration.samples())[X][1][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 2 );
  REQUIRE( (*magneticCalibration.samples())[X][2][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 3 );
  REQUIRE( (*magneticCalibration.samples())[X][3][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 4 );

  magneticCalibration.addSample(SPHERE_DEG(0.0f, 0.0f, 5.0f));

  REQUIRE( (*magneticCalibration.samples())[X][0][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 5 );
  REQUIRE( (*magneticCalibration.samples())[X][1][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 2 );
  REQUIRE( (*magneticCalibration.samples())[X][2][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 3 );
  REQUIRE( (*magneticCalibration.samples())[X][3][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 4 );

  magneticCalibration.addSample(SPHERE_DEG(0.0f, 0.0f, 6.0f));

  REQUIRE( (*magneticCalibration.samples())[X][0][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 5 );
  REQUIRE( (*magneticCalibration.samples())[X][1][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 6 );
  REQUIRE( (*magneticCalibration.samples())[X][2][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 3 );
  REQUIRE( (*magneticCalibration.samples())[X][3][MESH_PHI_RESOLUTION / 2][MESH_PHI_RESOLUTION / 2] == 4 );
}
