#ifndef COMM_HUB
#define COMM_HUB

#include <vector>
#include <functional>
#include <map>
#include <queue>
#include <Comm/Bridge.hpp>

namespace Comm{
  template<class T>
  struct HubBinding;

  template<class T>
  using HandlerFunction = std::function<void(std::vector<T>)>;

  template<class T>
  class Hub;
}

/**
 * Sends data...
 */
template<class T>
class Comm::Hub{
public:
  /** An enum type.
   *  The documentation block cannot be put after the enum!
   */
  Hub(Comm::Bridge<T>* bridge);
  virtual void emit(T name, std::vector<T> message); /**< enum value 1 */
  virtual Comm::HubBinding<T> on(T name, Comm::HandlerFunction<T> handler); /**< enum value 1 */
  virtual void poll(); /**< enum value 1 */
  virtual void remove(Comm::HubBinding<T> binding); /**< enum value 1 */

protected:
  Bridge<T>* bridge; /**< enum value 1 */
  std::map<T, std::map<unsigned int, Comm::HandlerFunction<T>>> handlers; /**< enum value 1 */
  std::map<T, unsigned int> nextIDs; /**< enum value 1 */
};

template<class T>
struct Comm::HubBinding{
  T name;
  unsigned int id;
};

#include "../../src/Comm/Hub.ipp"

#endif
