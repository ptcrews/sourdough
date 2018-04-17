#include <iostream>
#include <cstdlib>
#include <cmath>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug || false )
{ }

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

  /*
  if(after_timeout) {

    this->the_window_size = this->the_window_size/2;
    cerr << "window size halved: timeout" << endl;
    if(this->the_window_size == 0) {
      this->the_window_size = 1;
    }
    this->ack_counter = 0;
  }*/


/*  if(after_timeout) {
    this->the_window_size = this->the_window_size * 3/4;
    if (this->the_window_size < 1) {
      this->the_window_size = 1;
    }
  } */


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
  if(rtt < min_rtt) {
    min_rtt = rtt;
  }
/* old method for decreasing window size (static)
  if(rtt >= min_rtt + rtt_delta) {
    this->the_window_size -= 0.1;
  }

  if(rtt >= min_rtt + 2*rtt_delta) {
    if (this->the_window_size >= 1) {
      this->the_window_size -= 0.1;
    }
  }
*/
//new method for decreasing window size (dynamic)

  char* dec_const_str = std::getenv("DEC_CONST");
  double dec_const = 0.0125;
  if (dec_const_str) {
    dec_const = atof(dec_const_str);
  }

  char* inc_const_str = std::getenv("INC_CONST");
  double inc_const = 0.5;
  if (inc_const_str) {
    inc_const = atof(inc_const_str);
  }

  if(rtt >= min_rtt + rtt_delta) {
    this->the_window_size -= (pow(rtt - min_rtt + rtt_delta, 2)/pow(rtt_delta,2)) * dec_const;
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


  if(rtt < min_rtt + rtt_delta) {
    this->the_window_size +=  (pow((min_rtt + rtt_delta - rtt), 2) / (pow(rtt_delta, 2))) * inc_const;
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
//  cerr << "window size:" << this->the_window_size << endl;
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  //return 1000; /* timeout of one second */
  return 2*min_rtt;
}
