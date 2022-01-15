#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // segment的起始和结束
    size_t head = index;
    size_t rear = index + data.size();
    bool overflow = false;
    // 目标是将segment填入output中，output有容量限制
    if(_head + _output.remaining_capacity() < rear){
        overflow = true;
        rear = _head + _output.remaining_capacity();
    }
    if(head < _head){
        head = _head;
    }
    //此时[head,rear)为可以填充的部分，因为传输可能确实所以可能不连续
    for(size_t i = head;i<rear;i++){
        if(_hash.count(i)) continue; //segment中有重复发送的信息
        _hash.insert({i,data[i-index]});
        _unreassembled_bytes++; //将不重复的index记录下来,之后可以拼接,stored but not yet reassembled
    }
    //整合成连续的字符串
    string str = "";
    for(size_t i=_head;_hash.count(i);i++){
        str.push_back(_hash[i]);
        _hash.erase(i);
        _head++;
        _unreassembled_bytes--;
    }

    if(str.size()){
        _output.write(str);
    }

    if(!overflow && eof){
        //这个segment是整个stream的eof而且没有
        _eof = true;
    }
    if(_eof && empty()){
        //没有未重排的字符
        _output.end_input();
    }

}

size_t StreamReassembler::unassembled_bytes() const { 
    return _unreassembled_bytes;
 }

bool StreamReassembler::empty() const { 
    return _unreassembled_bytes == 0;
 }
