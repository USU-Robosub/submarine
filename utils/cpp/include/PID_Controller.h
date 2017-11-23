class PID_Controller {
public:
  PID_Controller() {}
  double p = 0, i = 0, d = 0, setPoint = 0;
  double update(double processVar);
  double update(double processVar, double deltaTime);
private:
  double integral = 0, lastError = 0;
};
