/****************************************************************************

 ****************************************************************************
 *** OVERVIEW
 ****************************************************************************

 cybermon_qwriter. Part of new queue implementation to address frequent cybermon
 crash caused due to limitation of lua threading.
 Creates args for different protocols and put in to q_entry to add in to a queue

****************************************************************************/

#include <cybermon_qwriter.h>
#include <cybermon_qargs.h>

#include <iostream>
#include <iomanip>
#include <map>

#include <boost/program_options.hpp>

#include <cybermon/engine.h>
#include <cybermon/monitor.h>
#include <cybermon/etsi_li.h>
#include <cybermon/packet_capture.h>
#include <cybermon/context.h>
#include <cybermon/cybermon-lua.h>

using namespace cybermon;

cybermon_qwriter::cybermon_qwriter(const std::string& path,
				   std::queue<q_entry*>& cybermonq,
				   threads::mutex& cqwrlock) :
    cqueue(cybermonq), lock(cqwrlock) {
}

// Connection-orientated.
void cybermon_qwriter::connection_up(const context_ptr cp,
				     const timeval& tv) {
    try {
	qargs* args = new connection_args(cp, tv);
	q_entry* qentry = new q_entry(qargs::connection_up, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::connection_down(const context_ptr cp,
				       const timeval& tv) {
    try {
	qargs* args = new connection_args(cp, tv);
	q_entry* qentry = new q_entry(qargs::connection_down, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Trigger
void cybermon_qwriter::trigger_up(const std::string& liid,
				  const tcpip::address& a,
				  const timeval& tv) {
    try {
	std::string addr;
	a.to_string(addr);
	qargs* args = new trigger_up_args(liid, addr, tv);
	q_entry* qentry = new q_entry(qargs::trigger_up, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::trigger_down(const std::string& liid,
				    const timeval& tv) {
    try {
	qargs* args = new trigger_down_args(liid, tv);
	q_entry* qentry = new q_entry(qargs::trigger_down, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::unrecognised_stream(const context_ptr cp,
					   pdu_iter s,
					   pdu_iter e,
					   const timeval& tv) {
    try {
	qargs* args = new unrecognised_stream_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::unrecognised_stream, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// Connection-less
void cybermon_qwriter::unrecognised_datagram(const context_ptr cp,
					     pdu_iter s, pdu_iter e,
					     const timeval& tv) {
    try {
	qargs* args = new unrecognised_datagram_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::unrecognised_datagram, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::icmp(const context_ptr cp, unsigned int type,
			    unsigned int code, pdu_iter s, pdu_iter e,
			    const timeval& tv) {
    try {
	qargs* args = new icmp_args(cp, type, code, s, e, tv);
	q_entry* qentry = new q_entry(qargs::icmp, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::imap(const context_ptr cp, pdu_iter s, pdu_iter e,
			    const timeval& tv)
{
    try {
	qargs* args = new imap_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::imap, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::imap_ssl(const context_ptr cp,
				pdu_iter s, pdu_iter e,
				const timeval& tv)
{
    try {
	qargs* args = new imap_ssl_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::imap_ssl, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::pop3(const context_ptr cp, pdu_iter s, pdu_iter e,
			    const timeval& tv)
{
    try {
	qargs* args = new pop3_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::pop3, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::pop3_ssl(const context_ptr cp, pdu_iter s, pdu_iter e,
				const timeval& tv)
{
    try {
	qargs* args = new pop3_ssl_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::pop3_ssl, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::rtp(const context_ptr cp, pdu_iter s, pdu_iter e,
			   const timeval& tv) {
    try {
	qargs* args = new rtp_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::rtp, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::rtp_ssl(const context_ptr cp, pdu_iter s, pdu_iter e,
			       const timeval& tv) {
    try {
	qargs* args = new rtp_ssl_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::rtp_ssl, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::smtp_auth(const context_ptr cp, pdu_iter s, pdu_iter e,
				 const timeval& tv) {
    try {
	qargs* args = new smtp_auth_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::smtp_auth, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::sip_ssl(const context_ptr cp, pdu_iter s, pdu_iter e,
			       const timeval& tv) {
    try {
	qargs* args = new sip_ssl_args(cp, s, e, tv);
	q_entry* qentry = new q_entry(qargs::sip_ssl, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}
void cybermon_qwriter::sip_request(const context_ptr cp,
				   const std::string& method,
				   const std::string& from,
				   const std::string& to,
				   pdu_iter s, pdu_iter e,
				   const timeval& tv) {
    try {
	qargs* args = new sip_request_args(cp, method, from, to, s, e,
					   tv);
	q_entry* qentry = new q_entry(qargs::sip_request, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::sip_response(const context_ptr cp,
				    unsigned int code,
				    const std::string& status,
				    const std::string& from,
				    const std::string& to,
				    pdu_iter s, pdu_iter e,
				    const timeval& tv) {
    try {
	qargs* args = new sip_response_args(cp, code, status, from, to,
					    s, e, tv);
	q_entry* qentry = new q_entry(qargs::sip_response, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// HTTP
void cybermon_qwriter::http_request(const context_ptr cp,
				    const std::string& method,
				    const std::string& url,
				    const observer::http_hdr_t& hdr,
				    pdu_iter s, pdu_iter e,
				    const timeval& tv) {
    try {
	qargs* args = new http_request_args(cp, method, url, hdr,
					    s, e, tv);
	q_entry* qentry = new q_entry(qargs::http_request, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::http_response(const context_ptr cp,
				     unsigned int code,
				     const std::string& status,
				     const observer::http_hdr_t& hdr,
				     const std::string& url,
				     pdu_iter s, pdu_iter e,
				     const timeval& tv) {
    try {
	qargs* args = new http_response_args(cp, code, status, hdr, url,
					     s, e, tv);
	q_entry* qentry = new q_entry(qargs::http_response, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// SMTP
void cybermon_qwriter::smtp_command(const context_ptr cp,
				    const std::string& command,
				    const timeval& tv) {
    try {
	qargs* args = new smtp_command_args(cp, command, tv);
	q_entry* qentry = new q_entry(qargs::smtp_command, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::smtp_response(const context_ptr cp, int status,
				     const std::list<std::string>& text,
				     const timeval& tv) {
    try {
	qargs* args = new smtp_response_args(cp, status, text, tv);
	q_entry* qentry = new q_entry(qargs::smtp_response, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::smtp_data(const context_ptr cp,
				 const std::string& from,
				 const std::list<std::string>& to,
				 pdu_iter s, pdu_iter e,
				 const timeval& tv)
{
    try {
	qargs* args = new smtp_data_args(cp, from, to, s, e, tv);
	q_entry* qentry = new q_entry(qargs::smtp_data, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// FTP
void cybermon_qwriter::ftp_command(const context_ptr cp,
				   const std::string& command,
				   const timeval& tv)
{
    try {
	qargs* args = new ftp_command_args(cp, command, tv);
	q_entry* qentry = new q_entry(qargs::ftp_command, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::ftp_response(const context_ptr cp, int status,
				    const std::list<std::string>& responses,
				    const timeval& tv) {
    try {
	qargs* args = new ftp_response_args(cp, status, responses, tv);
	q_entry* qentry = new q_entry(qargs::ftp_response, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// DNS
void cybermon_qwriter::dns_message(const context_ptr cp,
				   const dns_header hdr,
				   const std::list<dns_query> queries,
				   const std::list<dns_rr> answers,
				   const std::list<dns_rr> authorities,
				   const std::list<dns_rr> additional,
				   const timeval& tv)
{
    try {

	qargs* args = new dns_message_args(cp, hdr, queries, answers,
					   authorities, additional, tv);
	q_entry* qentry = new q_entry(qargs::dns_message, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

// NTP
void cybermon_qwriter::ntp_timestamp_message(const context_ptr cp,
					     const ntp_timestamp& ts,
					     const timeval& tv)
{
    try {
	qargs* args = new ntp_timestamp_message_args(cp, ts, tv);
	q_entry* qentry = new q_entry(qargs::ntp_timestamp_message, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::ntp_control_message(const context_ptr cp,
					   const ntp_control& ctrl,
					   const timeval& tv)
{
    try {
	qargs* args = new ntp_control_message_args(cp, ctrl, tv);
	q_entry* qentry = new q_entry(qargs::ntp_control_message, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}

void cybermon_qwriter::ntp_private_message(const context_ptr cp,
					   const ntp_private& priv,
					   const timeval& tv)
{
    try {
	qargs* args = new ntp_private_message_args(cp, priv, tv);
	q_entry* qentry = new q_entry(qargs::ntp_private_message, args);
	push(qentry);
    } catch (std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }

}
//to signal cybermon_qreader to stop
void cybermon_qwriter::close() {
    q_entry* qentry = NULL;
    push(qentry);
}

