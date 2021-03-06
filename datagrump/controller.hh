#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <cstdint>

/* Congestion controller interface */

class Controller
{
private:
  bool debug_; /* Enables debugging output */
  double the_window_size = 30;
  double estimated_window_size = 30;
  uint64_t last_timeout = 0;

  // Various constants to modify
  uint64_t seq_timeout_sep = 300;
  double alpha = 0.1;
  double dec_scaling = 0.0125;
  double inc_scaling = 0.6;
  double timeout_mult = 1.5;
  double rtx_window_dec = 0.75;
  uint64_t rtt_delta = 18;

  // Estimate initial RTT value
  uint64_t min_rtt = 5000;

  unsigned int last_seq_sent = 0;
  unsigned int last_ack_rcvd = 0;
  unsigned int ack_counter = 0;
  /* Add member variables here */

public:
  /* Public interface for the congestion controller */
  /* You can change these if you prefer, but will need to change
     the call site as well (in sender.cc) */

  /* Default constructor */
  Controller( const bool debug );

  /* Get current window size, in datagrams */
  unsigned int window_size();

  /* A datagram was sent */
  void datagram_was_sent( const uint64_t sequence_number,
			  const uint64_t send_timestamp,
			  const bool after_timeout );

  /* An ack was received */
  void ack_received( const uint64_t sequence_number_acked,
		     const uint64_t send_timestamp_acked,
		     const uint64_t recv_timestamp_acked,
		     const uint64_t timestamp_ack_received );

  /* How long to wait (in milliseconds) if there are no acks
     before sending one more datagram */
  unsigned int timeout_ms();
};

#endif
