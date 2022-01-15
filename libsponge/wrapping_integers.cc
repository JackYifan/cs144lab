#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    //absolute seqno -> seqno
    return WrappingInt32(static_cast<uint32_t>(n)+isn.raw_value());
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    //  seqno -> absolute seqno
    uint32_t _abs_seq = n.raw_value() - isn.raw_value();
    uint64_t check = (checkpoint & 0xffffffff00000000) + _abs_seq;
    uint64_t ret = check;
    // 确定前32位使得最后结果和checkpoing最近
    if(abs(int64_t(check + (1ul<<32) - checkpoint)) < abs(int64_t(check-checkpoint))){
        ret = check + (1ul<<32);
    }
    if(check >= (1ul<<32) && abs(int64_t(check - (1ul<<32) - checkpoint)) < abs(int64_t(check-checkpoint))){
        ret = check - (1ul<<32);
    }
    return ret;
}
