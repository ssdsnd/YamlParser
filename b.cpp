#include <iostream>
#include <fstream>
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
public:
    MyHandler(std::ostream& os) : writer_(sb_), os_(os) {}

    bool StartObject() {
        path_.push_back(kObjectType);
        if (isTargetPath()) {
            currentRecord_.Clear();
        }
        return true;
    }

    bool Key(const char* str, SizeType length, bool copy) {
        if (isTargetPath()) {
            currentKey_ = std::string(str, length);
        }
        return true;
    }

    bool String(const char* str, SizeType length, bool copy) {
        if (isTargetPath()) {
            currentRecord_.AddMember(Value(currentKey_.c_str(), currentKey_.size(), currentRecord_.GetAllocator()).Move(), Value(str, length, currentRecord_.GetAllocator()).Move(), currentRecord_.GetAllocator());
        }
        return true;
    }

    bool Int(int i) {
        if (isTargetPath()) {
            if (currentKey_ == "age" && i > 40) {
                currentRecord_.Clear();
                return true;
            }
            currentRecord_.AddMember(Value(currentKey_.c_str(), currentKey_.size(), currentRecord_.GetAllocator()).Move(), i, currentRecord_.GetAllocator());
        }
        return true;
    }

    bool StartArray() {
        path_.push_back(kArrayType);
        if (isTargetPath()) {
            handlerStack_.push(new MyHandler(os_));
            return handlerStack_.top()->StartArray();
        }
        return true;
    }

    bool EndArray(SizeType elementCount) {
        if (isTargetPath()) {
            bool success = handlerStack_.top()->EndArray(elementCount);
            delete handlerStack_.top();
            handlerStack_.pop();
            return success;
        }
        path_.pop_back();
        return true;
    }

    bool EndObject(SizeType memberCount) {
        path_.pop_back();
        if (isTargetPath()) {
            currentRecord_.Accept(writer_);
            os_ << std::endl;
            currentRecord_.Clear();
        }
        return true;
    }

private:
    bool isTargetPath() const {
        if (path_.size() != 5 || path_[0] != kObjectType || path_[1] != kStringType || path_[2] != kObjectType || path_[3] != kStringType || path_[4] != kArrayType) {
            return false;
        }
        return true;
    }

    std::vector<int> path_;
    Document currentRecord_;
    std::string currentKey_;
    StringBuffer sb_;
    Writer<StringBuffer> writer_;
    std::ostream& os_;
    std::stack<MyHandler*> handlerStack_;
};

int main() {
    // Open input and output files
    std::ifstream ifs("input.json");
    std::ofstream ofs("output.json");

    // Create parser and handler objects
    Reader reader;
    MyHandler handler(ofs);

    // Parse the input file using the SAX parser and the handler
    reader.Parse(ifs, handler);

    // Close the files
    ifs.close();
    ofs.close();

    return 0;
}
