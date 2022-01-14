#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity){}

size_t ByteStream::write(const string &data) {
    size_t len = data.size();
    if(len > _capacity - buffer.size()){
        len = _capacity - buffer.size();
    }
    write_count+=len;
    for(size_t i=0;i<len;i++){
        buffer.push_back(data[i]);
    }
    DUMMY_CODE(data);
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    size_t length = len;
    if(buffer.size() < len){
        length = buffer.size();
    }
    string data;
    data.assign(buffer.begin(),buffer.begin()+length);
    return data;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    size_t length = len;
    if(buffer.size() < len){
        length = buffer.size();
    }
    // pop会增加read_count而peek不会
    read_count+=length;
    for(size_t i=0;i<length;i++){
        buffer.pop_front();
    }
 }

void ByteStream::end_input() {
    input_end = true;
}

bool ByteStream::input_ended() const { 
    return input_end;
 }

size_t ByteStream::buffer_size() const { 
    return buffer.size();
 }

bool ByteStream::buffer_empty() const { 
    return buffer.size() == 0;
 }

bool ByteStream::eof() const { 
    return buffer_empty() && input_ended();
 }

size_t ByteStream::bytes_written() const { 
    return write_count;
 }

size_t ByteStream::bytes_read() const { 
    return read_count;
 }

size_t ByteStream::remaining_capacity() const { 
    return _capacity - buffer.size(); 
}
