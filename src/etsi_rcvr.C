
/****************************************************************************

ETSI LI test receiver.  Usage:

  etsi-rcvr <portnum> | tcpdump -n -r-

****************************************************************************/

#include <cybermon/monitor.h>
#include <cybermon/etsi_li.h>
#include <cybermon/thread.h>
#include <cybermon/packet_capture.h>

class output : public monitor {
private:
    pcap_writer& p;
    threads::mutex lock;
public:
    output(pcap_writer& p) : p(p) {}
    virtual void operator()(const std::string& liid,
			    const std::string& network,
			    const std::vector<unsigned char>::iterator& s,
			    const std::vector<unsigned char>::iterator& e,
			    const timeval& tv, cybermon::direction d) {
	lock.lock();
	p.write(s, e);
	lock.unlock();
    }

    void target_up(const std::string& liid,
		   const std::string& network,
		   const tcpip::address& addr,
		   const timeval& tv) {
	std::cerr << "Target " << liid << " discovered on IP " << addr
		  << std::endl;
    }

    void target_down(const std::string& liid,
		     const std::string& network,
		     const timeval& tv) {
	std::cerr << "Target " << liid << " offline. " << std::endl;
    }

};

int main(int argc, char** argv)
{
    
    if (argc != 2) {
	std::cerr << "Usage:" << std::endl
		  << "\tetsi-rcvr <port>" << std::endl;
	exit(1);
    }

    try {

	std::istringstream buf(argv[1]);
	int port;
	buf >> port;

	pcap_writer p;

	output o(p);

	cybermon::etsi_li::receiver r(port, o);

	r.start();
	r.join();

    } catch (std::exception& e) {
	std::cerr << "Exception: " << e.what() << std::endl;
	exit(1);
    }

}

