#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg) {
    static size_t abs_seq = 0; // also checkpoint
    // fetch seqno from head
    abs_seq = unwrap(WrappingInt32(seg.header().seqno.raw_value()),WrappingInt32(_isn),abs_seq);
    size_t len = seg.length_in_sequence_space(); // payload().str().size() + (header().syn ? 1 : 0) + (header().fin ? 1 : 0)

    if(seg.header().syn){
        if(_syn){
            //不重复syn
            return false;
        }
        _syn = true;
        _head++;
        abs_seq = 1;
        _isn = seg.header().seqno.raw_value(); //Initial Sequence Number
        len--; // 因为len包含data+(has_SYN)+(has_FIN)，所以要-1得到data的长度
        if(len == 0){
            return true;
        }
        
    }else if(!_syn){
        return false; //没有syn请求且当前不是syn状态
    }


    if(seg.header().fin){
        if(_fin){
            return false;
        }
        _fin = true;
    }else if(seg.length_in_sequence_space() == 0){
        // 只有Header,not sym,not fin
        return _head == abs_seq;
    }else if(!seg.header().syn){
        //all data
        if(abs_seq > _head + window_size() -1 || abs_seq + len - 1 < _head){
            //不在窗口内
            return false;
        }
    }

    //接收所有帧
    _reassembler.push_substring(seg.payload().copy(),abs_seq-1,seg.header().fin);
    _head = _reassembler.get_head_index()+1; //get_head_index()是最后一个接收字符，_head是要接收的第一个字符位置
    if(_reassembler.stream_out().input_ended()) {
        _head++; //FIN占1 byte
    }
    return true;
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if(_head > 0){
        return WrappingInt32(wrap(_head,WrappingInt32(_isn)));
    }else{
        return std::nullopt;
    }
 }

size_t TCPReceiver::window_size() const { 
    return _capacity - _reassembler.stream_out().buffer_size(); //总容量-已经重新整理好的
 }
