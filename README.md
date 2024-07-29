# FullStackChattingDemo

## Description

C++17 FullStackChattingDemo, using QT6 in client and Boost-1.84.0 & jsoncpp libraries in server

## Developer Quick Start

### Platform Support

Windows, Linux, MacOS(Intel and M Serious Chip)

### Building  FullStackChattingDemo

```bash
git clone https://github.com/Liupeter01/FullStackChattingDemo
cd FullStackChattingDemo
cmake -B build
cmake --build build --parallel x
```

### Function test

Testing boost and jsoncpp library

``` c++
#include<iostream>
#include<string>
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>
#include<boost/unordered_map.hpp>
#include<boost/lexical_cast.hpp>

int main() 
{
          /*testing boost 1.84.0 library*/
          std::cout << "Enter your weight: ";
          float weight;
          std::cin >> weight;
          std::string gain = "A 10% increase raises ";
          std::string wt = boost::lexical_cast<std::string> (weight);
          gain = gain + wt + " to ";    
          weight = 1.1 * weight;
          gain = gain + boost::lexical_cast<std::string>(weight) + ".";
          std::cout << gain << std::endl;

          /*testing jsoncpp library*/
          Json::Value root;
          root["id"] = 1001;
          root["data"] = "hello world";
          std::string request = root.toStyledString();
          std::cout << "request is " << request << std::endl;

          Json::Value root2;
          Json::Reader reader;
          reader.parse(request, root2);
          std::cout << "msg id is " << root2["id"] << " msg is " << root2["data"] << std::endl;
          return 0;
}
```



