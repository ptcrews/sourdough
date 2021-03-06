#include <iostream>
#include <cstdlib>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug || false )
{}

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

  return this->the_window_size;
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
  if(after_timeout) {
    
    this->the_window_size = this->the_window_size/2;
    cerr << "window size halved: timeout" << endl;
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
  

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms()
{
  return 1000; /* timeout of one second */
}
