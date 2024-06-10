#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <vector>

struct Event {
	const char* msg;
};

class EventTransmitter {
public:
	EventTransmitter();
	void publish();
	void receive(Event ev);
private:
	std::vector<void(*)(Event)> myCallbacks;
};

#endif // EVENT_SYSTEM_H