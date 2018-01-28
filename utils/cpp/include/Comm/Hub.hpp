/**
 * \class ExampleClass
 *
 * \ingroup PackageName
 * (Note, this needs exactly one \defgroup somewhere)
 *
 * \brief Provide an example
 *
 * This class is meant as an example.  It is not useful by itself
 * rather its usefulness is only a function of how much it helps
 * the reader.  It is in a sense defined by the person who reads it
 * and otherwise does not exist in any real form.
 *
 * \note Attempts at zen rarely work.
 *
 * \author (last to touch it) $Author: bv $
 *
 * \version $Revision: 1.5 $
 *
 * \date $Date: 2005/04/14 14:16:20 $
 *
 * Contact: bv@bnl.gov
 *
 * Created on: Wed Apr 13 18:39:37 2005
 *
 * $Id: doxygen-howto.html,v 1.5 2005/04/14 14:16:20 bv Exp $
 *
 */

#ifndef COMM_HUB
#define COMM_HUB

#include <vector>
#include <functional>
#include <map>
#include <queue>
#include <Comm/Bridge.hpp>

/**
* \brief Callback function type "foo"
*
* A longer description of foo.
* \param a Description for a
* \param b Description for b
* \return Description for return value
*/
namespace Comm{
  template<class T>
  struct HubBinding;

  /**
  * \brief Callback function type "foo"
  *
  * A longer description of foo.
  * \param a Description for a
  * \param b Description for b
  * \return Description for return value
  */
  template<class T>
  using HandlerFunction = std::function<void(std::vector<T>)>;

  template<class T>
  class Hub;
}

/// Provides a standard interface to emit and listen to events.
template<class T>
class Comm::Hub{
public:
  /// Create a Hub that uses a Bridge for sending events.
  Hub(Comm::Bridge<T>* bridge);

  /** \brief Emits an event.
    * \param name an identifier for the event.
    * \param message a vector of data to send.
    *
    * Emits an event using the underlying Bridge.
    * The Hub's own listeners will not be notified.
    */
  virtual void emit(T name, std::vector<T> message);

  /** \brief Attaches a listener.
    * \param name the identifier to listen for.
    * \param handler callback of the form `void callback(vector<T>)`.
    * \return The identifier of the listener created.
    *
    * Attaches a listener to the Hub.
    * When an event with the given name is received, the handler will be called.
    */
  virtual Comm::HubBinding<T> on(T name, Comm::HandlerFunction<T> handler);

  /** \brief Process events
    *
    * Get pending events from the Bridge.
    * Then, for each event, notify the listeners associated with it.
    */
  virtual void poll();

  /** \brief Removes a listener
    * \param binding the listener's identifier
    *
    * Removes a listener from the Hub.
    */
  virtual void remove(Comm::HubBinding<T> binding);

protected:
  Bridge<T>* bridge; ///< Underlying Bridge used for communication
  std::map<T, std::map<unsigned int, Comm::HandlerFunction<T>>> handlers; ///< List of listener callbacks
  std::map<T, unsigned int> nextIDs; ///< New listener ID tracking
};

/// Identifier for listeners attached to a Hub.
template<class T>
struct Comm::HubBinding{
  T name; ///< Target event
  unsigned int id; ///< Specific ID of listener
};

#include "../../src/Comm/Hub.ipp"

#endif
