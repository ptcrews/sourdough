#include <iostream>
#include <cstdlib>
#include <cmath>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug || false )
{
  char* seq_timeout_sep_str = std::getenv("SEQ_TIMEOUT");
  if (seq_timeout_sep_str) {
    seq_timeout_sep = strtol(seq_timeout_sep_str, NULL, 10);
    cout << "Seq Timeout: " << seq_timeout_sep << endl;
  }
  char* alpha_str = std::getenv("ALPHA");
  if (alpha_str) {
    alpha = strtol(alpha_str, NULL, 10)*0.01;
    cout << "Alpha: " << alpha << endl;
  }
  char* dec_str = std::getenv("DEC");
  if (dec_str) {
    dec_scaling = strtol(dec_str, NULL, 10)*0.01;
    cout << "Dec scaling: " << dec_scaling << endl;
  }
  char* inc_str = std::getenv("INC");
  if (inc_str) {
    inc_scaling = strtol(inc_str, NULL, 10)*0.1;
    cout << "Inc scaling: " << inc_scaling << endl;
  }
  char* timeout_str = std::getenv("TIMEOUT");
  if (timeout_str) {
    timeout_mult = strtol(timeout_str, NULL, 10);
    cout << "Timeout mult: " << timeout_mult << endl;
  }
  char* rtt_delta_str = std::getenv("RTT_DELTA");
  if (rtt_delta_str) {
    rtt_delta = strtol(rtt_delta_str, NULL, 10);
    cout << "RTT Delta: " << rtt_delta << endl;
  }
  
}

/* Get current window size, in datagrams */
unsigned int Controller::window_size()
{
  /* Default: fixed window size of 100 outstanding datagrams */
//  char* window_size_str = std::getenv("WINDOW_SIZE");
//  unsigned int the_window_size = 50;
//  if (window_size_str) {
//    the_window_size = strtol(window_size_str, NULL, 10);
//  }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << this->the_window_size << endl;
  }

  return (int)(this->the_window_size);
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp,
                                    /* in milliseconds */
				    const bool after_timeout
				    /* datagram was sent because of a timeout */ )
{
  /* Default: take no action */

  last_seq_sent = sequence_number;

  if(after_timeout && (send_timestamp - last_timeout) > seq_timeout_sep) {

    last_timeout = send_timestamp;
    this->the_window_size = this->the_window_size/2;
    //cerr << "window size halved: timeout" << endl;
    if(this->the_window_size == 0) {
      this->the_window_size = 1;
    }
    this->ack_counter = 0;
  }

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
      << " sent datagram " << sequence_number << " (timeout = " << after_timeout << ")\n";
  }
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
  /* Default: take no action */
/*
  if(sequence_number_acked > this->last_ack_rcvd) { //Received Future ack
    this->ack_counter += 1;
    if(this->ack_counter >= this->the_window_size) {
      this->ack_counter = 0;
      this->the_window_size += 1;
    }
    this->last_ack_rcvd = sequence_number_acked;
  }

  else if(sequence_number_acked <= this->last_ack_rcvd){ //Drop detected
    this->the_window_size = this->the_window_size/2;
    cerr << "window size halved: old ack" << endl;
    if(this->the_window_size == 0) {
      this->the_window_size = 1;
    }
    this->ack_counter = 0;
  }
<<<<<<< Updated upstream
*/  
  const uint64_t rtt = timestamp_ack_received - send_timestamp_acked;
  //cout << "RTT: " << rtt << "MinRTT: " << min_rtt << endl;
  this->estimated_window_size = alpha * this->the_window_size + (1-alpha)*this->estimated_window_size;
  if(rtt < min_rtt) {
    min_rtt = rtt;
  }

  if(rtt >= min_rtt + rtt_delta) {
    if (this->the_window_size > this->estimated_window_size) {
        this->the_window_size = this->estimated_window_size;
    } else {
        double adjustment = (pow(rtt - min_rtt + rtt_delta, 2) / pow(rtt_delta, 2))*dec_scaling;
	this->the_window_size -= adjustment;
    }
  }

/*
  if(rtt < min_rtt + rtt_delta) {
    this->the_window_size += 0.01;
  }
  
  if(rtt < min_rtt + rtt_delta/2) {
    this->the_window_size += 0.2;
  }


  else if(rtt < min_rtt + rtt_delta/4) {
    this->the_window_size += 0.4;
  }
*/


  // Good window size
  if(rtt < min_rtt + rtt_delta) {
    double adjustment = (pow((min_rtt + rtt_delta - rtt), 2) / (pow(rtt_delta, 2)))*inc_scaling;
    this->the_window_size = max(this->the_window_size + adjustment, this->estimated_window_size);
  }


  if(this->the_window_size < 1) { //to prevent window size of 0
    this->the_window_size = 1;
  }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
  //cerr << "window size:" << this->the_window_size << endl;
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  return timeout_mult*min_rtt;
}
