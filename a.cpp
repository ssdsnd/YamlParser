#include <iostream>
#include <fstream>
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class MyHandler : public BaseReaderHandler<UTF8<>, MyHandler> {
public:
    MyHandler(const std::string& targetPath, std::ostream& os) : targetPath_(targetPath), writer_(sb_), os_(os) {}

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

    bool StartArray() {
        path_.push_back(kArrayType);
        if (isTargetPath()) {
            handlerStack_.push(new MyHandler(targetPath_, os_));
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
        return true;
    }

    bool EndObject(SizeType memberCount) {
        path_.pop_back();
        if (isTargetPath()) {
            currentRecord_.Accept(writer_);
            currentRecord_.Clear();
        }
        return true;
    }

private:
    bool isTargetPath() const {
        if (path_.size() < targetPath_.size()) {
            return false;
        }
        for (size_t i = 0; i < targetPath_.size(); ++i) {
            if (path_[i] != targetPath_[i]) {
                return false;
            }
        }
        return true;
    }

    std::vector<int> path_;
    Document currentRecord_;
    std::string currentKey_;
    std::string targetPath_;
    StringBuffer sb_;
    Writer<StringBuffer> writer_;
    std::ostream& os_;
    std::stack<MyHandler*> handlerStack_;
};

int main() {
    // Open input and output files
    std::ifstream ifs("input.json");
    std::ofstream ofs("output.json");

    // Parse input JSON using SAX model and write filtered records to output JSON file
    MyHandler handler("path.to.array", ofs);
    Reader reader;
    char buffer[4096];
    while (ifs.read(buffer, sizeof(buffer))) {
        reader.Parse(buffer, ifs.gcount(), handler);
    }
}
