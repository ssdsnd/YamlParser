#include "rapidjson/reader.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

class MyHandler : public BaseReaderHandler<> {
public:
    MyHandler(const std::string& path) : path(path), in_array(false), in_object(false), depth(0) {}

    bool StartArray() {
        if (depth == path.size() && path[depth - 1] == '/') {
            in_array = true;
            return true;
        }
        depth++;
        return true;
    }

    bool EndArray(SizeType size) {
        if (depth == path.size() && path[depth - 1] == '/') {
            in_array = false;
            data.push_back(arr);
            arr.clear();
        }
        depth--;
        return true;
    }

    bool StartObject() {
        if (depth == path.size() && path[depth - 1] == '/') {
            in_object = true;
            return true;
        }
        depth++;
        return true;
    }

    bool EndObject(SizeType size) {
        if (depth == path.size() && path[depth - 1] == '/') {
            in_object = false;
        }
        depth--;
        return true;
    }

    bool Key(const Ch* str, SizeType length, bool copy) {
        key = str;
        return true;
    }

    bool String(const Ch* str, SizeType length, bool copy) {
        if (in_object && depth == path.size()) {
            obj[key] = std::string(str, length);
        }
        return true;
    }

    bool Int(int i) {
        if (in_object && depth == path.size()) {
            obj[key] = i;
        }
        return true;
    }

    bool Bool(bool b) {
        if (in_object && depth == path.size()) {
            obj[key] = b;
        }
        return true;
    }

    bool Double(double d) {
        if (in_object && depth == path.size()) {
            obj[key] = d;
        }
        return true;
    }

    void clear() {
        data.clear();
        arr.clear();
    }

    std::vector<std::map<std::string, boost::variant<int, std::string, bool, double>>> getData() {
        return data;
    }

private:
    std::string path;
    bool in_array;
    bool in_object;
    int depth;
    std::string key;
    std::map<std::string, boost::variant<int, std::string, bool, double>> obj;
    std::vector<std::map<std::string, boost::variant<int, std::string, bool, double>>> arr;
    std::vector<std::map<std::string, boost::variant<int, std::string, bool, double>>> data;
};

int main() {
    // JSON input
    const char* json = "{\"users\": [{\"name\": \"Alice\", \"age\": 25, \"married\": false}, {\"name\": \"Bob\", \"age\": 30, \"married\": true}], \"groups\": [{\"name\": \"Admins\", \"members\": [\"Alice\", \"Bob\"]}, {\"name\": \"Users\", \"members\": [\"Charlie\", \"David\"]}]}";

    // Create a reader
    Reader reader
