namespace Math{
  class DiscreteCalculus{
  public:
    static double integrate(double previousIntegral, double newValue, double deltaTime);
    static double differentiate(double previousValue, double newValue, double deltaTime);
  };
}
